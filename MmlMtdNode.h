#ifndef MMLMTDNODE_H
#define MMLMTDNODE_H

#include "Mml.h"
#include "MmlTableBaseNode.h"

#include "Helpers.h"
#include "MmlDocument.h"

#include <QDebug>

class MmlMtdNode : public MmlTableBaseNode
{
public:
    MmlMtdNode( MmlDocument *document, const MmlAttributeMap &attribute_map ) : MmlTableBaseNode( Mml::NodeType::MtdNode, document, attribute_map )
    {
        m_scriptlevel_adjust = 0;
    }
    virtual void setMyRect( const QRectF &rect );

    Mml::ColAlign columnalign();
    Mml::RowAlign rowalign();
    int colNum() const;
    int rowNum() const;
    virtual int scriptlevel( const MmlNode *child = 0 ) const;

private:
    int m_scriptlevel_adjust; // added or subtracted to scriptlevel to
    // make contents fit the cell
};

#endif//MMLMTDNODE_H
