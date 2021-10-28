
#include "MmlTextNode.h"
#include "../../MmlDocument.h"

MmlTextNode::MmlTextNode( const QString &text, MmlDocument *document ) : MmlNode( TextNode, document, MmlAttributeMap() )
{
    m_text = text;
    // Trim whitespace from ends, but keep nbsp and thinsp
    m_text.remove( QRegularExpression( "^[^\\S\\x00a0\\x2009]+" ) );
    m_text.remove( QRegularExpression( "[^\\S\\x00a0\\x2009]+$" ) );
}

QString MmlTextNode::toStr() const
{
    return MmlNode::toStr() + " text=\"" + m_text + "\"";
}

bool MmlTextNode::isInvisibleOperator() const
{
    return    m_text == QString( QChar( 0x61, 0x20 ) )  // &ApplyFunction;
              || m_text == QString( QChar( 0x62, 0x20 ) )  // &InvisibleTimes;
              || m_text == QString( QChar( 0x63, 0x20 ) )  // &InvisibleComma;
              || m_text == QString( QChar( 0x64, 0x20 ) ); // Invisible addition
}

void MmlTextNode::paintSymbol(QPainter *painter, double x_scaling, double y_scaling ) const
{
    MmlNode::paintSymbol( painter, x_scaling, y_scaling );

    if ( isInvisibleOperator() )
        return;

    painter->save();

    QPointF d_pos = devicePoint( QPointF() );
    QPointF s_pos = symbolRect().topLeft();

    painter->translate( d_pos + s_pos );
    painter->scale( x_scaling, y_scaling );
    painter->setFont( font() );

    painter->drawText( QPointF( 0.0, basePos() ) - s_pos, m_text );

    painter->restore();
}

QRectF MmlTextNode::symbolRect() const
{
    QRectF br = isInvisibleOperator() ? QRectF() : QFontMetricsF( font() ).tightBoundingRect( m_text );
    br.translate( 0.0, basePos() );

    return br;
}
