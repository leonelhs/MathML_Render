#ifndef MMLMSQRTNODE_H
#define MMLMSQRTNODE_H

#include "../../MmlDocument.h"
#include "../MmlRootBaseNode.h"

class MmlMsqrtNode : public MmlRootBaseNode
{
public:
    MmlMsqrtNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlRootBaseNode( MsqrtNode, document, attribute_map ) {}

};

#endif//MMLMSQRTNODE_H
