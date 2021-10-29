#ifndef MMLMSTYLENODE_H
#define MMLMSTYLENODE_H

#include "MmlNode.h"
#include "MmlDocument.h"

class MmlMstyleNode : public MmlNode
{
public:
    MmlMstyleNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( MstyleNode, document, attribute_map ) {}
};

#endif//MMLMSTYLENODE_H
