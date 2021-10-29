#ifndef MMLSUBSUPBASENODE_CPP
#define MMLSUBSUPBASENODE_CPP

#include "MmlSubsupBaseNode.h"

MmlNode *MmlSubsupBaseNode::base() const
{
    Q_ASSERT( firstChild() != 0 );
    return firstChild();
}

MmlNode *MmlSubsupBaseNode::sscript() const
{
    MmlNode *s = base()->nextSibling();
    Q_ASSERT( s != 0 );
    return s;
}

int MmlSubsupBaseNode::scriptlevel( const MmlNode *child ) const
{
    int sl = MmlNode::scriptlevel();

    MmlNode *s = sscript();
    if ( child != 0 && child == s )
        return sl + 1;
    else
        return sl;
}


#endif//MMLSUBSUPBASENODE_CPP
