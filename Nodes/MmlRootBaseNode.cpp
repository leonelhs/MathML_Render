#ifndef MMLROOTBASENODE_CPP
#define MMLROOTBASENODE_CPP

#include "MmlRootBaseNode.h"

static const ushort g_radical               = ( 0x22 << 8 ) | 0x1B;
static const int g_radical_points_size      = 11;
static const double g_mroot_base_margin     = 0.1;
static const double g_mroot_base_line       = 0.5;

static const QPointF g_radical_points[] = { QPointF( 0.0,         0.344439758 ),
                                            QPointF( 0.217181096, 0.419051636 ),
                                            QPointF( 0.557377049, 0.102829829 ),
                                            QPointF( 0.942686988, 1.048864253 ),
                                            QPointF( 1.0,         1.048864253 ),
                                            QPointF( 1.0,         1.0         ),
                                            QPointF( 1.0,         1.0         ),
                                            QPointF( 0.594230277, 0.0         ),
                                            QPointF( 0.516457480, 0.0         ),
                                            QPointF( 0.135213883, 0.352172079 ),
                                            QPointF( 0.024654201, 0.316221808 )
                                          };

int MmlRootBaseNode::scriptlevel( const MmlNode *child ) const
{
    int sl = MmlNode::scriptlevel();

    MmlNode *i = index();
    if ( child != 0 && child == i )
        return sl + 1;
    else
        return sl;
}

QRectF MmlRootBaseNode::baseRect() const
{
    MmlNode *b = base();
    if ( b == 0 )
        return QRectF( 0.0, 0.0, 1.0, 1.0 );
    else
        return b->myRect();
}

QRectF MmlRootBaseNode::radicalRect() const
{
    return QFontMetricsF( font() ).boundingRect( QChar( g_radical ) );
}

double MmlRootBaseNode::radicalMargin() const
{
    return g_mroot_base_margin * baseRect().height();
}

double MmlRootBaseNode::radicalLineWidth() const
{
    return g_mroot_base_line * lineWidth();
}

QRectF MmlRootBaseNode::symbolRect() const
{
    QRectF base_rect = baseRect();
    double radical_margin = radicalMargin();
    double radical_width = radicalRect().width();
    int radical_line_width = qCeil( radicalLineWidth() );

    return QRectF( -radical_width, base_rect.top() - radical_margin - radical_line_width,
                   radical_width + base_rect.width() + radical_margin, base_rect.height() + 2.0 * radical_margin + radical_line_width );
}

void MmlRootBaseNode::layoutSymbol()
{
    MmlNode *b = base();
    if ( b != 0 )
        b->setRelOrigin( QPointF( 0.0, 0.0 ) );

    MmlNode *i = index();
    if ( i != 0 )
    {
        QRectF i_rect = i->myRect();
        i->setRelOrigin( QPointF( -0.33 * radicalRect().width() - i_rect.width(),
                                  -1.1 * i_rect.bottom() ) );
    }
}

void MmlRootBaseNode::paintSymbol(
    QPainter *painter, double x_scaling, double y_scaling ) const
{
    MmlNode::paintSymbol( painter, x_scaling, y_scaling );

    painter->save();

    QRectF s_rect = symbolRect();
    s_rect.moveTopLeft( devicePoint( s_rect.topLeft() ) );

    QRectF radical_rect = QFontMetricsF( font() ).boundingRect( QChar( g_radical ) );

    QRectF rect = s_rect;
    rect.adjust(  0.0, qCeil( radicalLineWidth() ),
                  -(rect.width() - radical_rect.width() ), 0.0 );

    painter->translate( rect.bottomLeft() );

    QPointF radical_points[ g_radical_points_size ];

    for ( int i = 0; i < g_radical_points_size; ++i )
    {
        radical_points[ i ].setX( radical_rect.width() * g_radical_points[ i ].x() );
        radical_points[ i ].setY( -rect.height() * g_radical_points[ i ].y() );
    }

    double x2 = radical_points[ 2 ].x();
    double y2 = radical_points[ 2 ].y();
    double x3 = radical_points[ 3 ].x();
    double y3 = radical_points[ 3 ].y();

    radical_points[ 4 ].setX( s_rect.width() );
    radical_points[ 5 ].setX( s_rect.width() );

    radical_points[ 3 ].setY( -s_rect.height() );
    radical_points[ 4 ].setY( -s_rect.height() );

    double new_y3 = radical_points[ 3 ].y();

    radical_points[ 3 ].setX( x2 + ( x3 - x2 ) * ( new_y3 - y2 ) / ( y3 - y2 ) );

    QBrush brush = painter->brush();
    brush.setColor( painter->pen().color() );
    brush.setStyle( Qt::SolidPattern );
    painter->setBrush( brush );

    painter->setRenderHint( QPainter::Antialiasing, true );

    painter->drawPolygon( radical_points, g_radical_points_size );

    painter->restore();
}

MmlNode *MmlRootBaseNode::base() const
{
    return firstChild();
}

MmlNode *MmlRootBaseNode::index() const
{
    MmlNode *b = base();
    if ( b == 0 )
        return 0;
    return b->nextSibling();
}

#endif//MMLROOTBASENODE_CPP
