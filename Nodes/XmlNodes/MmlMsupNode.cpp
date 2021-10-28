#ifndef MMLMSUPNODE_CPP
#define MMLMSUPNODE_CPP

#include "MmlMsupNode.h"

static const char *  g_subsup_spacing  = "veryverythinmathspace";

void MmlMsupNode::layoutSymbol()
{
    MmlNode *b = base();
    MmlNode *s = sscript();
    qreal subsup_spacing = interpretSpacing( g_subsup_spacing, 0 );
    qreal threshold = b->myRect().top() + 0.5 * ( b->myRect().height() - subsup_spacing );
    qreal shift = 0.0;

    if ( b->myRect().top() + s->myRect().bottom() > threshold )
        shift = threshold - ( b->myRect().top() + s->myRect().bottom() );

    b->setRelOrigin( QPointF( -b->myRect().width(), 0.0 ) );
    s->setRelOrigin( QPointF( subsup_spacing, b->myRect().top() + shift ) );
}

#endif//MMLMSUPNODE_CPP
