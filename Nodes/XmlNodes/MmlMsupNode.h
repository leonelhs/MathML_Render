#ifndef MMLMSUPNODE_H
#define MMLMSUPNODE_H

#include "../../MmlDocument.h"
#include "../MmlSubsupBaseNode.h"

class MmlMsupNode : public MmlSubsupBaseNode
{
public:
    MmlMsupNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlSubsupBaseNode( MsupNode, document, attribute_map ) {}

protected:
    virtual void layoutSymbol();
};

#endif//MMLMSUPNODE_H
