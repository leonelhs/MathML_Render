#ifndef MMLMUNDEROVERNODE_H
#define MMLMUNDEROVERNODE_H

#include "../MmlTokenNode.h"
#include "../../MmlDocument.h"

class MmlMunderoverNode : public MmlNode
{
public:

    MmlMunderoverNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( MunderoverNode, document, attribute_map ) {}

    virtual int scriptlevel( const MmlNode *node = 0 ) const;

protected:
    virtual void layoutSymbol();
};

#endif//MMLMUNDEROVERNODE_H
