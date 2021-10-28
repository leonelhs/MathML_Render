#ifndef MMLMUNDEROVERNODE_CPP
#define MMLMUNDEROVERNODE_CPP

#include "MmlMunderoverNode.h"

static const double  g_mfrac_spacing  = 0.05;

void MmlMunderoverNode::layoutSymbol()
{
    MmlNode *base = firstChild();
    Q_ASSERT( base != 0 );
    MmlNode *under = base->nextSibling();
    Q_ASSERT( under != 0 );
    MmlNode *over = under->nextSibling();
    Q_ASSERT( over != 0 );

    QRectF base_rect = base->myRect();
    QRectF under_rect = under->myRect();
    QRectF over_rect = over->myRect();

    qreal over_spacing = explicitAttribute( "accent" ) == "true" ? 0.0 : g_mfrac_spacing * ( base_rect.height() + under_rect.height() + over_rect.height() );
    qreal under_spacing = explicitAttribute( "accentunder" ) == "true" ? 0.0 : g_mfrac_spacing * ( base_rect.height() + under_rect.height() + over_rect.height() );
    QString align_value = explicitAttribute( "align" );
    qreal underover_rel_factor = align_value == "left" ? 1.0 : align_value == "right" ? 0.0 : 0.5;

    base->setRelOrigin( QPointF( -0.5 * base_rect.width(), 0.0 ) );
    under->setRelOrigin( QPointF( -underover_rel_factor * under_rect.width(), base_rect.bottom() + under_spacing - under_rect.top() ) );
    over->setRelOrigin( QPointF( -underover_rel_factor * over_rect.width(), base_rect.top() - over_spacing - under_rect.bottom() ) );
}

int MmlMunderoverNode::scriptlevel( const MmlNode *node ) const
{
    MmlNode *base = firstChild();
    Q_ASSERT( base != 0 );
    MmlNode *under = base->nextSibling();
    Q_ASSERT( under != 0 );
    MmlNode *over = under->nextSibling();
    Q_ASSERT( over != 0 );

    int sl = MmlNode::scriptlevel();
    if ( node != 0 && ( node == under || node == over ) )
        return sl + 1;
    else
        return sl;
}

#endif//MMLMUNDEROVERNODE_CPP
