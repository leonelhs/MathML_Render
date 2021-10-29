#ifndef MMLMINODE_H
#define MMLMINODE_H

#include "Mml.h"
#include "MmlTokenNode.h"
#include "MmlDocument.h"

class MmlMiNode : public MmlTokenNode
{
public:
    MmlMiNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlTokenNode( Mml::MiNode, document, attribute_map ) {}
};

#endif // MMLMINODE_H
