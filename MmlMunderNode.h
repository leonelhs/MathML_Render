#ifndef MMLMUNDERNODE_H
#define MMLMUNDERNODE_H

#include "MmlTokenNode.h"
#include "MmlDocument.h"

class MmlMunderNode : public MmlNode
{
public:
    MmlMunderNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( MunderNode, document, attribute_map ) {}
    virtual int scriptlevel( const MmlNode *node = 0 ) const;

protected:
    virtual void layoutSymbol();
};

#endif//MMLMUNDERNODE_H
