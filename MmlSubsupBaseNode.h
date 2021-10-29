#ifndef MMLSUBSUPBASENODE_H
#define MMLSUBSUPBASENODE_H

#include "MmlNode.h"
#include "MmlDocument.h"

class MmlSubsupBaseNode : public MmlNode
{
public:
    MmlSubsupBaseNode( Mml::NodeType type, MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( type, document, attribute_map ) {}

    MmlNode *base() const;
    MmlNode *sscript() const;

    virtual int scriptlevel( const MmlNode *child = 0 ) const;
};

#endif//MMLSUBSUPBASENODE_H
