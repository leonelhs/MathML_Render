#ifndef MMLTABLEBASENODE_H
#define MMLTABLEBASENODE_H

#include "MmlNode.h"
#include "../MmlDocument.h"

class MmlTableBaseNode : public MmlNode
{
public:
    MmlTableBaseNode( Mml::NodeType type, MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( type, document, attribute_map ) {}
};
#endif//MMLTABLEBASENODE_H
