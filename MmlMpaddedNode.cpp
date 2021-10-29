#ifndef MMLMPADDEDNODE_CPP
#define MMLMPADDEDNODE_CPP

#include "MmlMpaddedNode.h"

qreal MmlMpaddedNode::lspace() const
{
    QString value = explicitAttribute( "lspace" );

    if ( value.isNull() )
        return 0.0;

    bool ok;
    qreal lspace = interpretSpacing( value, 0.0, &ok );

    if ( ok )
        return lspace;

    return 0.0;
}

QRectF MmlMpaddedNode::symbolRect() const
{
    return QRectF( -lspace(), -height(), lspace() + width(), height() + depth() );
}

#endif//MMLMPADDEDNODE_CPP
