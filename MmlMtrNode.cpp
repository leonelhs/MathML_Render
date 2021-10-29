#ifndef MMLMTRNODE_CPP
#define MMLMTRNODE_CPP

#include "MmlMtrNode.h"

void MmlMtrNode::layoutCells( const QList<qreal> &col_widths, qreal col_spc )
{
    const QRectF mr = myRect();

    MmlNode *child = firstChild();
    qreal col_offset = 0.0;
    int colnum = 0;
    for ( ; child != 0; child = child->nextSibling(), ++colnum )
    {
        Q_ASSERT( child->nodeType() == MtdNode );
        MmlMtdNode *mtd = ( MmlMtdNode* ) child;

        QRectF rect = QRectF( 0.0, mr.top(), col_widths[colnum], mr.height() );
        mtd->setMyRect( rect );
        mtd->setRelOrigin( QPointF( col_offset, 0.0 ) );
        col_offset += col_widths[colnum] + col_spc;
    }

    updateMyRect();
}

#endif//MMLMTRNODE_CPP
