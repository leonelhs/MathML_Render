#ifndef MMLMERRORNODE_H
#define MMLMERRORNODE_H

#include "../MmlNode.h"
#include "../../MmlDocument.h"

class MmlMerrorNode : public MmlNode
{
public:
    MmlMerrorNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( MerrorNode, document, attribute_map ) {}
};

#endif//MMLMERRORNODE_H
