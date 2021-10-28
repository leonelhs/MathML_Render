#ifndef MMLMOVERNODE_H
#define MMLMOVERNODE_H

#include "../MmlNode.h"
#include "../../MmlDocument.h"

class MmlMoverNode : public MmlNode
{
public:
    MmlMoverNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( MoverNode, document, attribute_map ) {}
    virtual int scriptlevel( const MmlNode *node = 0 ) const;

protected:
    virtual void layoutSymbol();
};

#endif//MMLMOVERNODE_H
