#ifndef MMLMTABLENODE_H
#define MMLMTABLENODE_H

#include "Helpers.h"
#include "MmlDocument.h"
#include "MmlTableBaseNode.h"

#include <QDebug>

class MmlMtableNode : public MmlTableBaseNode
{
public:
    MmlMtableNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlTableBaseNode( MtableNode, document, attribute_map ) {}

    double rowspacing() const;
    double columnspacing() const;
    double framespacing_hor() const;
    double framespacing_ver() const;
    FrameType frame() const;
    FrameType columnlines( int idx ) const;
    FrameType rowlines( int idx ) const;

protected:
    virtual void layoutSymbol();
    virtual QRectF symbolRect() const;
    virtual void paintSymbol( QPainter *painter, double x_scaling, double y_scaling ) const;

private:
    struct CellSizeData
    {
        void init( const MmlNode *first_row );
        QList<double> col_widths, row_heights;
        int numCols() const
        {
            return col_widths.count();
        }
        int numRows() const
        {
            return row_heights.count();
        }
        double colWidthSum() const;
        double rowHeightSum() const;
    };

    CellSizeData m_cell_size_data;
    double m_content_width, m_content_height;
};

#endif//MMLMTABLENODE_H
