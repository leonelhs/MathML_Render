#ifndef MMLMFENCEDNODE_H
#define MMLMFENCEDNODE_H

#include "../MmlNode.h"
#include "../../MmlDocument.h"

class MmlMfencedNode : public MmlNode
{
public:
    MmlMfencedNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( MfencedNode, document, attribute_map ) {}
};

#endif//MMLMFENCEDNODE_H
