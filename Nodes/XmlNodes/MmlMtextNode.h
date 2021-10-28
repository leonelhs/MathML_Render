#ifndef MMLMTEXTNODE_H
#define MMLMTEXTNODE_H

#include "../MmlTokenNode.h"
#include "../../MmlDocument.h"

class MmlMtextNode : public MmlNode
{
public:
    MmlMtextNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( MtextNode, document, attribute_map ) {}
};

#endif//MMLMTEXTNODE_H
