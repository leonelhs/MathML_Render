#ifndef MMLUNKNOWNNODE_H
#define MMLUNKNOWNNODE_H

#include "MmlTokenNode.h"
#include "MmlDocument.h"

class MmlUnknownNode : public MmlNode
{
public:
    MmlUnknownNode( MmlDocument *document, const MmlAttributeMap &attribute_map ): MmlNode( UnknownNode, document, attribute_map ) {}
};

#endif//MMLUNKNOWNNODE_H
