#ifndef MMLMSUBNODE_H
#define MMLMSUBNODE_H

#include "../../MmlDocument.h"
#include "../MmlSubsupBaseNode.h"

class MmlMsubNode : public MmlSubsupBaseNode
{
public:
    MmlMsubNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlSubsupBaseNode( MsubNode, document, attribute_map ) {}

protected:
    virtual void layoutSymbol();
};


#endif//MMLMSUBNODE_H
