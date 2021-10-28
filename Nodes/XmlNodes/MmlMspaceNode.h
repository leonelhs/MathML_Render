#ifndef MMLMSPACENODE_H
#define MMLMSPACENODE_H

#include "../MmlNode.h"
#include "../../MmlDocument.h"

class MmlMspaceNode : public MmlSpacingNode
{
public:
    MmlMspaceNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlSpacingNode( MspaceNode, document, attribute_map ) {}
};

#endif//MMLMSPACENODE_H
