#ifndef MMLMTDNODE_CPP
#define MMLMTDNODE_CPP

#include "MmlMtdNode.h"

int MmlMtdNode::scriptlevel( const MmlNode *child ) const
{
    int sl = MmlNode::scriptlevel();
    if ( child != 0 && child == firstChild() )
        return sl + m_scriptlevel_adjust;
    else
        return sl;
}

void MmlMtdNode::setMyRect( const QRectF &rect )
{
    MmlNode::setMyRect( rect );
    MmlNode *child = firstChild();
    if ( child == 0 )
        return;

    if ( rect.width() < child->myRect().width() )
    {
        while ( rect.width() < child->myRect().width()
                && child->font().pointSize() > g_min_font_point_size )
        {
            qWarning() << "MmlMtdNode::setMyRect(): rect.width()=" << rect.width()
                       << ", child->myRect().width=" << child->myRect().width()
                       << " sl=" << m_scriptlevel_adjust;

            ++m_scriptlevel_adjust;
            child->layout();
        }

        qWarning() << "MmlMtdNode::setMyRect(): rect.width()=" << rect.width()
                   << ", child->myRect().width=" << child->myRect().width()
                   << " sl=" << m_scriptlevel_adjust;
    }

    QRectF mr = myRect();
    QRectF cmr = child->myRect();

    QPointF child_rel_origin;

    switch ( columnalign() )
    {
    case ColAlignLeft:
        child_rel_origin.setX( 0.0 );
        break;
    case ColAlignCenter:
        child_rel_origin.setX( mr.left() + 0.5 * ( mr.width() - cmr.width() ) );
        break;
    case ColAlignRight:
        child_rel_origin.setX( mr.right() - cmr.width() );
        break;
    }

    switch ( rowalign() )
    {
    case RowAlignTop:
        child_rel_origin.setY( mr.top() - cmr.top() );
        break;
    case RowAlignCenter:
    case RowAlignBaseline:
        child_rel_origin.setY( mr.top() - cmr.top() + 0.5 * ( mr.height() - cmr.height() ) );
        break;
    case RowAlignBottom:
        child_rel_origin.setY( mr.bottom() - cmr.bottom() );
        break;
    case RowAlignAxis:
        child_rel_origin.setY( 0.0 );
        break;
    }

    child->setRelOrigin( child_rel_origin );
}

int MmlMtdNode::colNum() const
{
    MmlNode *syb = previousSibling();

    int i = 0;
    for ( ; syb != 0; syb = syb->previousSibling() )
        ++i;

    return i;
}

int MmlMtdNode::rowNum() const
{
    MmlNode *row = parent()->previousSibling();

    int i = 0;
    for ( ; row != 0; row = row->previousSibling() )
        ++i;

    return i;
}

MmlMtdNode::ColAlign MmlMtdNode::columnalign()
{
    QString val = explicitAttribute( "columnalign" );
    if ( !val.isNull() )
        return Helpers::mmlInterpretColAlign( val, 0, 0 );

    MmlNode *node = parent(); // <mtr>
    if ( node == 0 )
        return ColAlignCenter;

    int colnum = colNum();
    val = node->explicitAttribute( "columnalign" );
    if ( !val.isNull() )
        return Helpers::mmlInterpretColAlign( val, colnum, 0 );

    node = node->parent(); // <mtable>
    if ( node == 0 )
        return ColAlignCenter;

    val = node->explicitAttribute( "columnalign" );
    if ( !val.isNull() )
        return Helpers::mmlInterpretColAlign( val, colnum, 0 );

    return ColAlignCenter;
}

MmlMtdNode::RowAlign MmlMtdNode::rowalign()
{
    QString val = explicitAttribute( "rowalign" );
    if ( !val.isNull() )
        return Helpers::mmlInterpretRowAlign( val, 0, 0 );

    MmlNode *node = parent(); // <mtr>
    if ( node == 0 )
        return RowAlignAxis;

    int rownum = rowNum();
    val = node->explicitAttribute( "rowalign" );
    if ( !val.isNull() )
        return Helpers::mmlInterpretRowAlign( val, rownum, 0 );

    node = node->parent(); // <mtable>
    if ( node == 0 )
        return RowAlignAxis;

    val = node->explicitAttribute( "rowalign" );
    if ( !val.isNull() )
        return Helpers::mmlInterpretRowAlign( val, rownum, 0 );

    return RowAlignAxis;
}

#endif//MMLMTDNODE_CPP
