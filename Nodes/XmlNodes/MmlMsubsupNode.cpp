#ifndef MMLMSUBSUPNODE_CPP
#define MMLMSUBSUPNODE_CPP

#include "MmlMsubsupNode.h"

static const char * g_subsup_spacing = "veryverythinmathspace";

MmlNode *MmlMsubsupNode::base() const
{
    Q_ASSERT( firstChild() != 0 );
    return firstChild();
}

MmlNode *MmlMsubsupNode::subscript() const
{
    MmlNode *sub = base()->nextSibling();
    Q_ASSERT( sub != 0 );
    return sub;
}

MmlNode *MmlMsubsupNode::superscript() const
{
    MmlNode *sup = subscript()->nextSibling();
    Q_ASSERT( sup != 0 );
    return sup;
}

void MmlMsubsupNode::layoutSymbol()
{
    MmlNode *b = base();
    MmlNode *sub = subscript();
    MmlNode *sup = superscript();
    qreal subsup_spacing = interpretSpacing( g_subsup_spacing, 0 );
    qreal sub_threshold = b->myRect().top() + 0.5 * ( b->myRect().height() + subsup_spacing );
    qreal sup_threshold = sub_threshold - subsup_spacing;
    qreal sub_shift = 0.0;
    qreal sup_shift = 0.0;

    if ( b->myRect().bottom() + sub->myRect().top() < sub_threshold )
        sub_shift = sub_threshold - ( b->myRect().bottom() + sub->myRect().top() );

    if ( b->myRect().top() + sup->myRect().bottom() > sup_threshold )
        sup_shift = sup_threshold - ( b->myRect().top() + sup->myRect().bottom() );

    b->setRelOrigin( QPointF( -b->myRect().width(), 0.0 ) );
    sub->setRelOrigin( QPointF( subsup_spacing, b->myRect().bottom() + sub_shift ) );
    sup->setRelOrigin( QPointF( subsup_spacing, b->myRect().top() + sup_shift ) );
}

int MmlMsubsupNode::scriptlevel( const MmlNode *child ) const
{
    int sl = MmlNode::scriptlevel();

    MmlNode *sub = subscript();
    MmlNode *sup = superscript();

    if ( child != 0 && ( child == sup || child == sub ) )
        return sl + 1;
    else
        return sl;
}

#endif//MMLMSUBSUPNODE_CPP
