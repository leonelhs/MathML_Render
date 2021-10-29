#ifndef MMLMOVERNODE_CPP
#define MMLMOVERNODE_CPP

#include "MmlMoverNode.h"

static const qreal   g_mfrac_spacing = 0.05;

void MmlMoverNode::layoutSymbol()
{
    MmlNode *base = firstChild();
    Q_ASSERT( base != 0 );
    MmlNode *over = base->nextSibling();
    Q_ASSERT( over != 0 );

    QRectF base_rect = base->myRect();
    QRectF over_rect = over->myRect();

    qreal spacing = explicitAttribute( "accent" ) == "true" ? 0.0 : g_mfrac_spacing * ( over_rect.height() + base_rect.height() );
    QString align_value = explicitAttribute( "align" );
    qreal over_rel_factor = align_value == "left" ? 1.0 : align_value == "right" ? 0.0 : 0.5;

    base->setRelOrigin( QPointF( -0.5 * base_rect.width(), 0.0 ) );
    over->setRelOrigin( QPointF( -over_rel_factor * over_rect.width(),
                                 base_rect.top() - spacing - over_rect.bottom() ) );
}

int MmlMoverNode::scriptlevel( const MmlNode *node ) const
{
    MmlNode *base = firstChild();
    Q_ASSERT( base != 0 );
    MmlNode *over = base->nextSibling();
    Q_ASSERT( over != 0 );

    int sl = MmlNode::scriptlevel();
    if ( node != 0 && node == over )
        return sl + 1;
    else
        return sl;
}

#endif//MMLMOVERNODE_CPP
