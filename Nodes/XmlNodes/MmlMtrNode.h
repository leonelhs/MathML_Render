#ifndef MMLMTRNODE_H
#define MMLMTRNODE_H

#include "MmlMtdNode.h"
#include "../MmlTableBaseNode.h"
#include "../../MmlDocument.h"

class MmlMtrNode : public MmlTableBaseNode
{
public:
    MmlMtrNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlTableBaseNode( MtrNode, document, attribute_map ) {}
    void layoutCells( const QList<double> &col_widths, double col_spc );
};

#endif//MMLMTRNODE_H
