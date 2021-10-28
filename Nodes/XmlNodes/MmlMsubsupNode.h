#ifndef MMLMSUBSUPNODE_H
#define MMLMSUBSUPNODE_H

#include "../MmlNode.h"
#include "../../MmlDocument.h"

class MmlMsubsupNode : public MmlNode
{
public:
    MmlMsubsupNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( MsubsupNode, document, attribute_map ) {}

    MmlNode *base() const;
    MmlNode *superscript() const;
    MmlNode *subscript() const;

    virtual int scriptlevel( const MmlNode *child = 0 ) const;

protected:
    virtual void layoutSymbol();
};

#endif//MMLMSUBSUPNODE_H
