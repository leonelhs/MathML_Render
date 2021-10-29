#ifndef MMLMALIGNMARKNODE_H
#define MMLMALIGNMARKNODE_H

#include "MmlNode.h"
#include "MmlDocument.h"

class MmlMalignMarkNode : public MmlNode
{
public:
    MmlMalignMarkNode( MmlDocument *document ) : MmlNode( MalignMarkNode, document, MmlAttributeMap() ) {}
};

#endif//MMLMALIGNMARKNODE_H
