#ifndef MMLMFRACNODE_H
#define MMLMFRACNODE_H

#include "MmlNode.h"
#include "MmlDocument.h"

class MmlMfracNode : public MmlNode
{
public:
    MmlMfracNode( MmlDocument *document, const MmlAttributeMap &attribute_map )
        : MmlNode( MfracNode, document, attribute_map ) {}

    MmlNode *numerator() const;
    MmlNode *denominator() const;

protected:
    virtual void layoutSymbol();
    virtual void paintSymbol( QPainter *painter, double x_scaling, double y_scaling ) const;
    virtual QRectF symbolRect() const;

private:
    double lineThickness() const;
};

#endif // MMLMFRACNODE_H
