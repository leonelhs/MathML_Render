#ifndef MMLSPACINGNODE_H
#define MMLSPACINGNODE_H

#include "MmlNode.h"
#include "../MmlDocument.h"


class MmlSpacingNode : public MmlNode
{
public:
    MmlSpacingNode( const Mml::NodeType &node_type, MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( node_type, document, attribute_map ) {}

public:
    double width() const;
    double height() const;
    double depth() const;

protected:
    virtual void layoutSymbol();
    virtual QRectF symbolRect() const;

    double interpretSpacing( QString value, double base_value, bool *ok ) const;
};

#endif//MMLSPACINGNODE_H
