#ifndef MMLMONODE_CPP
#define MMLMONODE_CPP

#include "MmlMoNode.h"

MmlMoNode::MmlMoNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlTokenNode( MoNode, document, attribute_map )
{
    m_oper_spec = 0;
}

QString MmlMoNode::toStr() const
{
    return MmlNode::toStr() + QString( " form=%1" ).arg( ( int )form() );
}

void MmlMoNode::layoutSymbol()
{
    if ( firstChild() == 0 )
        return;

    firstChild()->setRelOrigin( QPointF( 0.0, 0.0 ) );

    if ( m_oper_spec == 0 )
        m_oper_spec = OperSpecSearchResult::mmlFindOperSpec( !text().compare( "−" )?"-":text(), form() );
}

QString MmlMoNode::dictionaryAttribute( const QString &name ) const
{
    const MmlNode *p = this;
    for ( ; p != 0; p = p->parent() )
    {
        if ( p == this || p->nodeType() == MstyleNode )
        {
            QString expl_attr = p->explicitAttribute( name );
            if ( !expl_attr.isNull() )
                return expl_attr;
        }
    }

    return Helpers::mmlDictAttribute( name, m_oper_spec );
}

bool MmlMoNode::unaryMinus() const
{
    return !text().compare( "−" )
           && previousSibling() != 0
           && previousSibling()->nodeType() == MoNode
           && ( !( ( MmlMoNode* ) previousSibling() )->text().compare( "=" )
                || !( ( MmlMoNode* ) previousSibling() )->text().compare( "(" )
                || !( ( MmlMoNode* ) previousSibling() )->text().compare( "|" )
                || !( ( MmlMoNode* ) previousSibling() )->text().compare( "⌊" )
                || !( ( MmlMoNode* ) previousSibling() )->text().compare( "⌈" )
                || !( ( MmlMoNode* ) previousSibling() )->text().compare( "," ) );
}

Mml::FormType MmlMoNode::form() const
{
    QString value_str = inheritAttributeFromMrow( "form" );
    if ( !value_str.isNull() )
    {
        bool ok;
        FormType value = Helpers::mmlInterpretForm( value_str, &ok );
        if ( ok )
            return value;
        else
            qWarning() << "Could not convert " << value_str << " to form";
    }

    // Default heuristic.
    if ( firstSibling() == ( MmlNode* )this && lastSibling() != ( MmlNode* )this )
        return PrefixForm;
    else if ( lastSibling() == ( MmlNode* )this && firstSibling() != ( MmlNode* )this )
        return PostfixForm;
    else if ( unaryMinus() )
        return PrefixForm;
    else
        return InfixForm;
}

void MmlMoNode::stretch()
{
    if ( parent() == 0 )
        return;

    if ( m_oper_spec == 0 )
        return;

    if ( m_oper_spec->stretch_dir == MmlOperSpec::HStretch
            && parent()->nodeType() == MrowNode
            && ( previousSibling() != 0 || nextSibling() != 0) )
        return;

    QRectF pmr = parent()->myRect();
    QRectF pr = parentRect();

    switch ( m_oper_spec->stretch_dir )
    {
    case MmlOperSpec::VStretch:
        stretchTo( QRectF( pr.left(), pmr.top(), pr.width(), pmr.height() ) );
        break;
    case MmlOperSpec::HStretch:
        stretchTo( QRectF( pmr.left(), pr.top(), pmr.width(), pr.height() ) );
        break;
    case MmlOperSpec::HVStretch:
        stretchTo( pmr );
        break;
    case MmlOperSpec::NoStretch:
        break;
    }
}

qreal MmlMoNode::lspace() const
{
    Q_ASSERT( m_oper_spec != 0 );
    if ( parent() == 0
            || ( parent()->nodeType() != MrowNode
                 && parent()->nodeType() != MfencedNode
                 && parent()->nodeType() != UnknownNode )
            || previousSibling() == 0
            || ( previousSibling() == 0 && nextSibling() == 0 )
            || ( previousSibling() != 0
                 && previousSibling()->nodeType() == MoNode
                 && !( ( MmlMoNode* ) previousSibling() )->text().compare( text() ) )
            || unaryMinus()
            || ( !text().compare( "|" )
                 && previousSibling() != 0
                 && previousSibling()->nodeType() == MoNode
                 && ( !( ( MmlMoNode* ) previousSibling() )->text().compare( "=" )
                      || !( ( MmlMoNode* ) previousSibling() )->text().compare( "−" )
                      || !( ( MmlMoNode* ) previousSibling() )->text().compare( "," ) ) ) )
        return 0.0;
    else
        return interpretSpacing( dictionaryAttribute( "lspace" ), 0 );
}

qreal MmlMoNode::rspace() const
{
    Q_ASSERT( m_oper_spec != 0 );
    if ( parent() == 0
            || ( parent()->nodeType() != MrowNode
                 && parent()->nodeType() != MfencedNode
                 && parent()->nodeType() != UnknownNode )
            || nextSibling() == 0
            || ( previousSibling() == 0 && nextSibling() == 0 ) )
        return 0.0;
    else
        return interpretSpacing( dictionaryAttribute( "rspace" ), 0 );
}

QRectF MmlMoNode::symbolRect() const
{
    if ( firstChild() == 0 )
        return QRectF( 0.0, 0.0, 0.0, 0.0 );

    QRectF cmr = firstChild()->myRect();

    return QRectF( -lspace(), cmr.top(),
                   cmr.width() + lspace() + rspace(), cmr.height() );
}


#endif//MMLMONODE_CPP
