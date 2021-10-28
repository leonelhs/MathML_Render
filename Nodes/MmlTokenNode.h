#ifndef MMLTOKENNODE_H
#define MMLTOKENNODE_H

#include "Mml.h"
#include "MmlNode.h"
#include "../MmlDocument.h"

class MmlDocument;

class MmlTokenNode : public MmlNode
{
public:
    MmlTokenNode( Mml::NodeType type, MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( type, document, attribute_map ) {}

    QString text() const;
};


#endif // MMLTOKENNODE_H
