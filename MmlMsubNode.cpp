#ifndef MMLMSUBNODE_CPP
#define MMLMSUBNODE_CPP

#include "MmlMsubNode.h"

static const char *  g_subsup_spacing         = "veryverythinmathspace";

void MmlMsubNode::layoutSymbol()
{
    MmlNode *b = base();
    MmlNode *s = sscript();
    qreal subsup_spacing = interpretSpacing( g_subsup_spacing, 0 );
    qreal threshold = b->myRect().top() + 0.5 * ( b->myRect().height() + subsup_spacing );
    qreal sub_shift = 0.0;

    if ( b->myRect().bottom() + s->myRect().top() < threshold )
        sub_shift = threshold - ( b->myRect().bottom() + s->myRect().top() );

    b->setRelOrigin( QPointF( -b->myRect().width(), 0.0 ) );
    s->setRelOrigin( QPointF( subsup_spacing, b->myRect().bottom() + sub_shift ) );
}


#endif//MMLMSUBNODE_CPP
