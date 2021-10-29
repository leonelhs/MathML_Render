#ifndef MMLMPADDEDNODE_H
#define MMLMPADDEDNODE_H

#include "MmlDocument.h"
#include "MmlSpacingNode.h"

class MmlMpaddedNode : public MmlSpacingNode
{
public:
    MmlMpaddedNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlSpacingNode( MpaddedNode, document, attribute_map ) {}

public:
    virtual double lspace() const;

protected:
    virtual QRectF symbolRect() const;
};

#endif//MMLMPADDEDNODE_H
