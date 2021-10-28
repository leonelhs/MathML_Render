#ifndef MMLMROOTNODE_H
#define MMLMROOTNODE_H

#include "../../MmlDocument.h"
#include "../MmlRootBaseNode.h"

class MmlMrootNode : public MmlRootBaseNode
{
public:
    MmlMrootNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlRootBaseNode( MrootNode, document, attribute_map ) {}
};

#endif//MMLMROOTNODE_H
