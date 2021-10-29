#ifndef MMLMPHANTOMNODE_H
#define MMLMPHANTOMNODE_H

#include "MmlNode.h"
#include "MmlDocument.h"

class MmlMphantomNode : public MmlNode
{
public:
    MmlMphantomNode( MmlDocument *document, const MmlAttributeMap &attribute_map ): MmlNode( MphantomNode, document, attribute_map ) {}

    virtual void paint( QPainter *, double, double ) {}
};

#endif//MMLMPHANTOMNODE_H
