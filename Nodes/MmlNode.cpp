#ifndef MMLNODE_CPP
#define MMLNODE_CPP

#include "Helpers.h"
#include "MmlNode.h"
#include "XmlNodes/MmlMiNode.h"

#include <QDebug>

MmlNode::MmlNode( Mml::NodeType type, MmlDocument *document, const MmlAttributeMap &attribute_map )
{
    m_parent = 0;
    m_first_child = 0;
    m_next_sibling = 0;
    m_previous_sibling = 0;

    m_node_type = type;
    m_document = document;
    m_attribute_map = attribute_map;

    m_my_rect = m_parent_rect = QRectF( 0.0, 0.0, 0.0, 0.0 );
    m_rel_origin = QPointF( 0.0, 0.0 );
    m_stretched = false;
}

MmlNode::~MmlNode()
{
    MmlNode *n = m_first_child;
    while ( n != 0 )
    {
        MmlNode *tmp = n->nextSibling();
        delete n;
        n = tmp;
    }
}

QColor MmlNode::background() const
{
    QString value_str = inheritAttributeFromMrow( "mathbackground" );
    if ( value_str.isNull() )
        value_str = inheritAttributeFromMrow( "background" );
    if ( value_str.isNull() )
        return QColor();

    return QColor( value_str );
}

QPointF MmlNode::devicePoint( const QPointF &pos ) const
{
    QRectF dr = deviceRect();

    if ( m_stretched )
    {
        return dr.topLeft() + QPointF( ( pos.x() - m_my_rect.left() ) * dr.width() / m_my_rect.width(),
                                       ( pos.y() - m_my_rect.top() ) * dr.height() / m_my_rect.height() );
    }
    else
    {
        return dr.topLeft() + pos - m_my_rect.topLeft();
    }
}

void MmlNode::paintSymbol( QPainter *painter, double, double ) const
{

#ifdef MML_TEST
    QRectF d_rect = deviceRect();
    if ( m_document->drawFrames() && d_rect.isValid() )
    {
        painter->save();

        painter->setPen( QPen( Qt::red, 0 ) );

        painter->drawRect( d_rect );

        QPen pen = painter->pen();
        pen.setStyle( Qt::DotLine );
        painter->setPen( pen );

        const QPointF d_pos = devicePoint( QPointF() );

        painter->drawLine( QPointF( d_rect.left(), d_pos.y() ),
                           QPointF( d_rect.right(), d_pos.y() ) );

        painter->restore();
    }
#else
    Q_UNUSED( painter )
#endif
}

int MmlNode::scriptlevel( const MmlNode * ) const
{
    int parent_sl;
    if ( m_parent == 0 )
        parent_sl = 0;
    else
        parent_sl = m_parent->scriptlevel( this );

    QString expl_sl_str = explicitAttribute( "scriptlevel" );
    if ( expl_sl_str.isNull() )
        return parent_sl;

    if ( expl_sl_str.startsWith( "+" ) || expl_sl_str.startsWith( "-" ) )
    {
        bool ok;
        int expl_sl = expl_sl_str.toInt( &ok );
        if ( ok )
        {
            return parent_sl + expl_sl;
        }
        else
        {
            qWarning() << "MmlNode::scriptlevel(): bad value " + expl_sl_str;
            return parent_sl;
        }
    }

    bool ok;
    int expl_sl = expl_sl_str.toInt( &ok );
    if ( ok )
        return expl_sl;


    if ( expl_sl_str == "+" )
        return parent_sl + 1;
    else if ( expl_sl_str == "-" )
        return parent_sl - 1;
    else
    {
        qWarning() << "MmlNode::scriptlevel(): could not parse value: \"" + expl_sl_str + "\"";
        return parent_sl;
    }
}

QString MmlNode::explicitAttribute( const QString &name, const QString &def ) const
{
    MmlAttributeMap::const_iterator it = m_attribute_map.find( name );
    if ( it != m_attribute_map.end() )
        return *it;
    return def;
}

QFont MmlNode::font() const
{
    QFont fn( m_document->fontName( MmlFont::NormalFont ) );
    fn.setPointSizeF( m_document->baseFontPointSize() );

    qreal ps = fn.pointSizeF();
    int sl = scriptlevel();
    if ( sl >= 0 )
    {
        for ( int i = 0; i < sl; ++i )
            ps = ps * g_script_size_multiplier;
    }
    else
    {
        for ( int i = 0; i > sl; --i )
            ps = ps / g_script_size_multiplier;
    }
    if ( ps < g_min_font_point_size )
        ps = g_min_font_point_size;
    fn.setPointSize( ps );

    const qreal em = QFontMetricsF( fn ).boundingRect( 'm' ).width();
    const qreal ex = QFontMetricsF( fn ).boundingRect( 'x' ).height();

    MmlAttributeMap font_attr = Helpers::collectFontAttributes( this );

    if ( font_attr.contains( "mathvariant" ) )
    {
        QString value = font_attr["mathvariant"];

        bool ok;
        int mv = Helpers::mmlInterpretMathVariant( value, &ok );

        if ( ok )
        {
            if ( mv & ScriptMV )
                fn.setFamily( m_document->fontName( MmlFont::ScriptFont ) );

            if ( mv & FrakturMV )
                fn.setFamily( m_document->fontName( MmlFont::FrakturFont ) );

            if ( mv & SansSerifMV )
                fn.setFamily( m_document->fontName( MmlFont::SansSerifFont ) );

            if ( mv & MonospaceMV )
                fn.setFamily( m_document->fontName( MmlFont::MonospaceFont ) );

            if ( mv & DoubleStruckMV )
                fn.setFamily( m_document->fontName( MmlFont::DoublestruckFont ) );

            if ( mv & BoldMV )
                fn.setBold( true );

            if ( mv & ItalicMV )
                fn.setItalic( true );
        }
    }

    if ( font_attr.contains( "mathsize" ) )
    {
        QString value = font_attr["mathsize"];
        fn = Helpers::mmlInterpretMathSize( value, fn, em, ex, 0 );
    }

    fn = Helpers::mmlInterpretDepreciatedFontAttr( font_attr, fn, em, ex );

    if ( m_node_type == MiNode && !font_attr.contains( "mathvariant" ) && !font_attr.contains( "fontstyle" ) )
    {
        const MmlMiNode *mi_node = ( const MmlMiNode* ) this;
        if ( mi_node->text().length() == 1 )
            fn.setItalic( true );
    }

    if ( m_node_type == MoNode )
    {
        fn.setItalic( false );
        fn.setBold( false );
    }

    return fn;
}

QColor MmlNode::color() const
{
    // If we are child of <merror> return red
    const MmlNode *p = this;
    for ( ; p != 0; p = p->parent() )
    {
        if ( p->nodeType() == MerrorNode )
            return QColor( "red" );
    }

    QString value_str = inheritAttributeFromMrow( "mathcolor" );
    if ( value_str.isNull() )
        value_str = inheritAttributeFromMrow( "color" );
    if ( value_str.isNull() )
        return QColor();

    return QColor( value_str );
}

QString MmlNode::toStr() const
{
    const MmlNodeSpec *spec = mmlFindNodeSpec( m_node_type );
    Q_ASSERT( spec != 0 );

    return QString( "%1 %2 mr=%3 pr=%4 dr=%5 ro=(%7, %8) str=%9" )
           .arg( spec->type_str )
           .arg( ( quintptr )this, 0, 16 )
           .arg( Helpers::rectToStr( m_my_rect ) )
           .arg( Helpers::rectToStr( parentRect() ) )
           .arg( Helpers::rectToStr( deviceRect() ) )
           .arg( m_rel_origin.x() )
           .arg( m_rel_origin.y() )
           .arg( ( int )m_stretched );
}

double MmlNode::basePos() const
{
    QFontMetricsF fm( font() );
    return fm.strikeOutPos();
}

QRectF MmlNode::deviceRect() const
{
    if ( m_parent == 0 )
        return QRectF( m_rel_origin + m_my_rect.topLeft(), m_my_rect.size() );

    QRectF pdr = m_parent->deviceRect();
    QRectF pr = parentRect();
    QRectF pmr = m_parent->myRect();

    double scale_w = 0.0;
    if ( pmr.width() != 0.0 )
        scale_w = pdr.width() / pmr.width();
    double scale_h = 0.0;
    if ( pmr.height() != 0.0 )
        scale_h = pdr.height() / pmr.height();

    return QRectF( pdr.left() + ( pr.left() - pmr.left() ) * scale_w,
                   pdr.top()  + ( pr.top() - pmr.top() ) * scale_h,
                   pr.width() * scale_w,
                   pr.height() * scale_h );
}

QRectF MmlNode::parentRect() const
{
    if ( m_stretched )
        return m_parent_rect;

    return QRectF( m_rel_origin + m_my_rect.topLeft(), m_my_rect.size() );
}

QString MmlNode::inheritAttributeFromMrow( const QString &name, const QString &def ) const
{
    const MmlNode *p = this;
    for ( ; p != 0; p = p->parent() )
    {
        if ( p == this || p->nodeType() == MstyleNode )
        {
            QString value = p->explicitAttribute( name );
            if ( !value.isNull() )
                return value;
        }
    }

    return def;
}

void MmlNode::setRelOrigin( const QPointF &rel_origin )
{
    m_rel_origin = rel_origin + QPointF( -m_my_rect.left(), 0.0 );
    m_stretched = false;
}

void MmlNode::paint(QPainter *painter, double x_scaling, double y_scaling )
{
    if ( !m_my_rect.isValid() )
        return;

    painter->save();

    QRectF d_rect = deviceRect();

    if ( m_stretched )
    {
        x_scaling *= d_rect.width() / m_my_rect.width();
        y_scaling *= d_rect.height() / m_my_rect.height();
    }

    if ( m_node_type != UnknownNode )
    {
        const QColor bg = background();
        if ( bg.isValid() )
            painter->fillRect( d_rect, bg );
        else
            painter->fillRect( d_rect, m_document->backgroundColor() );

        const QColor fg = color();
        if ( fg.isValid() )
            painter->setPen( QPen( fg, 1 ) );
        else
            painter->setPen( QPen( m_document->foregroundColor(), 1 ) );
    }

    MmlNode *child = m_first_child;
    for ( ; child != 0; child = child->nextSibling() )
        child->paint( painter, x_scaling, y_scaling );

    if ( m_node_type != UnknownNode )
        paintSymbol( painter, x_scaling, y_scaling );

    painter->restore();
}

void MmlNode::layout()
{
    m_parent_rect = QRectF( 0.0, 0.0, 0.0, 0.0 );
    m_stretched = false;
    m_rel_origin = QPointF( 0.0, 0.0 );

    MmlNode *child = m_first_child;
    for ( ; child != 0; child = child->nextSibling() )
        child->layout();

    layoutSymbol();

    updateMyRect();

    if ( m_parent == 0 )
        m_rel_origin = QPointF( 0.0, 0.0 );
}

void MmlNode::stretch()
{
    MmlNode *child = m_first_child;
    for ( ; child != 0; child = child->nextSibling() )
        child->stretch();
}

MmlNode *MmlNode::lastSibling() const
{
    const MmlNode *n = this;
    while ( !n->isLastSibling() )
        n = n->nextSibling();
    return const_cast<MmlNode*>( n );
}

void MmlNode::layoutSymbol()
{
    // default behaves like an mrow

    // now lay them out in a neat row, aligning their origins to my origin
    double w = 0.0;
    MmlNode *child = m_first_child;
    for ( ; child != 0; child = child->nextSibling() )
    {
        child->setRelOrigin( QPointF( w, 0.0 ) );
        w += child->parentRect().width() + 1.0;
    }
}

void MmlNode::updateMyRect()
{
    m_my_rect = symbolRect();
    MmlNode *child = m_first_child;
    for ( ; child != 0; child = child->nextSibling() )
        m_my_rect |= child->parentRect();
}

void MmlNode::stretchTo( const QRectF &rect )
{
    m_parent_rect = rect;
    m_stretched = true;
}

qreal MmlNode::em() const
{
    return QFontMetricsF( font() ).boundingRect( 'm' ).width();
}

qreal MmlNode::ex() const
{
    return QFontMetricsF( font() ).boundingRect( 'x' ).height();
}

qreal MmlNode::lineWidth() const
{
    return qMax( 1.0, QFontMetricsF( font() ).lineWidth() );
}

qreal MmlNode::interpretSpacing( const QString &value, bool *ok ) const
{
    return Helpers::mmlInterpretSpacing( value, em(), ex(), ok );
}

MmlNode *MmlNode::firstSibling() const
{
    const MmlNode *n = this;
    while ( !n->isFirstSibling() )
        n = n->previousSibling();
    return const_cast<MmlNode*>( n );
}

#endif //MMLNODE_CPP
