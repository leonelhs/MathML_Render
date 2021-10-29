#ifndef MMLMUNDERNODE_CPP
#define MMLMUNDERNODE_CPP

#include "MmlMunderNode.h"

static const double g_mfrac_spacing = 0.05;

void MmlMunderNode::layoutSymbol()
{
    MmlNode *base = firstChild();
    Q_ASSERT( base != 0 );
    MmlNode *under = base->nextSibling();
    Q_ASSERT( under != 0 );

    QRectF base_rect = base->myRect();
    QRectF under_rect = under->myRect();

    qreal spacing = explicitAttribute( "accentunder" ) == "true" ? 0.0 : g_mfrac_spacing * ( under_rect.height() + base_rect.height() );
    QString align_value = explicitAttribute( "align" );
    qreal under_rel_factor = align_value == "left" ? 1.0 : align_value == "right" ? 0.0 : 0.5;

    base->setRelOrigin( QPointF( -0.5 * base_rect.width(), 0.0 ) );
    under->setRelOrigin( QPointF( -under_rel_factor * under_rect.width(), base_rect.bottom() + spacing - under_rect.top() ) );
}

int MmlMunderNode::scriptlevel( const MmlNode *node ) const
{
    MmlNode *base = firstChild();
    Q_ASSERT( base != 0 );
    MmlNode *under = base->nextSibling();
    Q_ASSERT( under != 0 );

    int sl = MmlNode::scriptlevel();
    if ( node != 0 && node == under )
        return sl + 1;
    else
        return sl;
}

#endif//MMLMUNDERNODE_CPP
