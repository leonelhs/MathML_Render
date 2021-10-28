#ifndef MMLMROWNODE_H
#define MMLMROWNODE_H

#include "../MmlNode.h"
#include "../../MmlDocument.h"

class MmlMrowNode : public MmlNode
{
public:
    MmlMrowNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( MrowNode, document, attribute_map ) {}
};

#endif//MMLMROWNODE_H
