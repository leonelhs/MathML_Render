#ifndef MMLROOTBASENODE_H
#define MMLROOTBASENODE_H

#include "MmlNode.h"
#include "MmlDocument.h"

class MmlRootBaseNode : public MmlNode
{
public:
    MmlRootBaseNode( Mml::NodeType type, MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlNode( type, document, attribute_map ) {}

    MmlNode *base() const;
    MmlNode *index() const;

    virtual int scriptlevel( const MmlNode *child = 0 ) const;

protected:
    virtual void layoutSymbol();
    virtual void paintSymbol( QPainter *painter, double x_scaling, double y_scaling ) const;
    virtual QRectF symbolRect() const;

private:
    QRectF baseRect() const;
    QRectF radicalRect() const;
    double radicalMargin() const;
    double radicalLineWidth() const;
};

#endif//MMLROOTBASENODE_H
