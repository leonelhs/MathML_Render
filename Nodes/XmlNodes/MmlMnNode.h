#ifndef MMLMNNODE_H
#define MMLMNNODE_H

#include "../MmlTokenNode.h"
#include "../../MmlDocument.h"

class MmlMnNode : public MmlTokenNode
{
public:
    MmlMnNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlTokenNode( MnNode, document, attribute_map ) {}
};

#endif // MMLMNNODE_H
