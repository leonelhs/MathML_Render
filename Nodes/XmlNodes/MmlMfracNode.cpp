#ifndef MMLMFRACNODE_CPP
#define MMLMFRACNODE_CPP

#include "MmlMfracNode.h"

static const double g_mfrac_spacing = 0.05;

static bool zeroLineThickness( const QString &s )
{
    if ( s.length() == 0 || !s[0].isDigit() )
        return false;

    for ( int i = 0; i < s.length(); ++i )
    {
        QChar c = s.at( i );
        if ( c.isDigit() && c != '0' )
            return false;
    }
    return true;
}

MmlNode *MmlMfracNode::numerator() const
{
    Q_ASSERT( firstChild() != 0 );
    return firstChild();
}

MmlNode *MmlMfracNode::denominator() const
{
    MmlNode *node = numerator()->nextSibling();
    Q_ASSERT( node != 0 );
    return node;
}

QRectF MmlMfracNode::symbolRect() const
{
    QRectF num_rect = numerator()->myRect();
    QRectF denom_rect = denominator()->myRect();
    qreal spacing = g_mfrac_spacing * ( num_rect.height() + denom_rect.height() );
    qreal my_width = qMax( num_rect.width(), denom_rect.width() ) + 2.0 * spacing;
    int line_thickness = qCeil( lineThickness() );

    return QRectF( -0.5 * ( my_width + line_thickness ), -0.5 * line_thickness,
                   my_width + line_thickness, line_thickness );
}

void MmlMfracNode::layoutSymbol()
{
    MmlNode *num = numerator();
    MmlNode *denom = denominator();

    QRectF num_rect = num->myRect();
    QRectF denom_rect = denom->myRect();

    qreal spacing = g_mfrac_spacing * ( num_rect.height() + denom_rect.height() );
    int line_thickness = qCeil( lineThickness() );

    num->setRelOrigin( QPointF( -0.5 * num_rect.width(), - spacing - num_rect.bottom() - 0.5 * line_thickness ) );
    denom->setRelOrigin( QPointF( -0.5 * denom_rect.width(), spacing - denom_rect.top() + 0.5 * line_thickness ) );
}

qreal MmlMfracNode::lineThickness() const
{
    QString linethickness_str = inheritAttributeFromMrow( "linethickness", QString::number( 0.75 * lineWidth () ) );

    /* InterpretSpacing returns a qreal, which might be 0 even if the thickness
       is > 0, though very very small. That's ok, because we can set it to 1.
       However, we have to run this check if the line thickness really is zero */
    if ( !zeroLineThickness( linethickness_str ) )
    {
        bool ok;
        qreal line_thickness = interpretSpacing( linethickness_str, &ok );
        if ( !ok || !line_thickness )
            line_thickness = 1.0;

        return line_thickness;
    }
    else
    {
        return 0.0;
    }
}

void MmlMfracNode::paintSymbol(QPainter *painter, qreal x_scaling, qreal y_scaling ) const
{
    MmlNode::paintSymbol( painter, x_scaling, y_scaling );

    int line_thickness = qCeil( lineThickness() );

    if ( line_thickness != 0.0 )
    {
        painter->save();

        QPen pen = painter->pen();
        pen.setWidthF( line_thickness );
        painter->setPen( pen );

        QRectF s_rect = symbolRect();
        s_rect.moveTopLeft( devicePoint( s_rect.topLeft() ) );

        painter->drawLine( QPointF( s_rect.left() + 0.5 * line_thickness, s_rect.center().y() ),
                           QPointF( s_rect.right() - 0.5 * line_thickness, s_rect.center().y() ) );

        painter->restore();
    }
}

#endif//MMLMFRACNODE_CPP
