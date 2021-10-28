#ifndef MMLMTABLENODE_CPP
#define MMLMTABLENODE_CPP

#include "MmlMtableNode.h"
#include "MmlMtrNode.h"

qreal MmlMtableNode::rowspacing() const
{
    QString value = explicitAttribute( "rowspacing" );
    if ( value.isNull() )
        return ex();
    bool ok;
    qreal spacing = interpretSpacing( value, &ok );

    if ( ok )
        return spacing;
    else
        return ex();
}

qreal MmlMtableNode::columnspacing() const
{
    QString value = explicitAttribute( "columnspacing" );
    if ( value.isNull() )
        return 0.8 * em();
    bool ok;
    qreal spacing = interpretSpacing( value, &ok );

    if ( ok )
        return spacing;
    else
        return 0.8 * em();
}

qreal MmlMtableNode::CellSizeData::colWidthSum() const
{
    qreal w = 0.0;
    for ( int i = 0; i < col_widths.count(); ++i )
        w += col_widths[i];
    return w;
}

qreal MmlMtableNode::CellSizeData::rowHeightSum() const
{
    qreal h = 0.0;
    for ( int i = 0; i < row_heights.count(); ++i )
        h += row_heights[i];
    return h;
}

void MmlMtableNode::CellSizeData::init( const MmlNode *first_row )
{
    col_widths.clear();
    row_heights.clear();

    const MmlNode *mtr = first_row;
    for ( ; mtr != 0; mtr = mtr->nextSibling() )
    {
        Q_ASSERT( mtr->nodeType() == MtrNode );

        int col_cnt = 0;
        const MmlNode *mtd = mtr->firstChild();
        for ( ; mtd != 0; mtd = mtd->nextSibling(), ++col_cnt )
        {
            Q_ASSERT( mtd->nodeType() == MtdNode );

            QRectF mtdmr = mtd->myRect();

            if ( col_cnt == col_widths.count() )
                col_widths.append( mtdmr.width() );
            else
                col_widths[col_cnt] = qMax( col_widths[col_cnt], mtdmr.width() );
        }

        row_heights.append( mtr->myRect().height() );
    }
}

void MmlMtableNode::layoutSymbol()
{
    // Obtain natural widths of columns
    m_cell_size_data.init( firstChild() );

    qreal col_spc = columnspacing();
    qreal row_spc = rowspacing();
    qreal frame_spc_hor = framespacing_hor();
    QString columnwidth_attr = explicitAttribute( "columnwidth", "auto" );

    // Is table width set by user? If so, set col_width_sum and never ever change it.
    qreal col_width_sum = m_cell_size_data.colWidthSum();
    bool width_set_by_user = false;
    QString width_str = explicitAttribute( "width", "auto" );
    if ( width_str != "auto" )
    {
        bool ok;

        qreal w = interpretSpacing( width_str, &ok );
        if ( ok )
        {
            col_width_sum = w
                            - col_spc * ( m_cell_size_data.numCols() - 1 )
                            - frame_spc_hor * 2.0;
            width_set_by_user = true;
        }
    }

    // Find out what kind of columns we are dealing with and set the widths of
    // statically sized columns.
    qreal fixed_width_sum = 0.0;          // sum of widths of statically sized set columns
    qreal auto_width_sum = 0.0;           // sum of natural widths of auto sized columns
    qreal relative_width_sum = 0.0;       // sum of natural widths of relatively sized columns
    qreal relative_fraction_sum = 0.0;    // total fraction of width taken by relatively
    // sized columns
    int i;
    for ( i = 0; i < m_cell_size_data.numCols(); ++i )
    {
        QString value = Helpers::mmlInterpretListAttr( columnwidth_attr, i, "auto" );

        // Is it an auto sized column?
        if ( value == "auto" || value == "fit" )
        {
            auto_width_sum += m_cell_size_data.col_widths[i];
            continue;
        }

        // Is it a statically sized column?
        bool ok;
        qreal w = interpretSpacing( value, &ok );
        if ( ok )
        {
            // Yup, sets its width to the user specified value
            m_cell_size_data.col_widths[i] = w;
            fixed_width_sum += w;
            continue;
        }

        // Is it a relatively sized column?
        if ( value.endsWith( "%" ) )
        {
            value.truncate( value.length() - 1 );
            qreal factor = Helpers::mmlQstringToQreal( value, &ok );
            if ( ok && !value.isEmpty() )
            {
                factor *= 0.01;
                relative_width_sum += m_cell_size_data.col_widths[i];
                relative_fraction_sum += factor;
                if ( !width_set_by_user )
                {
                    // If the table width was not set by the user, we are free to increase
                    // it so that the width of this column will be >= than its natural width
                    qreal min_col_width_sum = m_cell_size_data.col_widths[i] / factor;
                    if ( min_col_width_sum > col_width_sum )
                        col_width_sum = min_col_width_sum;
                }
                continue;
            }
            else
                qWarning() << "MmlMtableNode::layoutSymbol(): could not parse value " << value << "%%";
        }

        // Relatively sized column, but we failed to parse the factor. Treat is like an auto
        // column.
        auto_width_sum += m_cell_size_data.col_widths[i];
    }

    // Work out how much space remains for the auto olumns, after allocating
    // the statically sized and the relatively sized columns.
    qreal required_auto_width_sum = col_width_sum
                                    - relative_fraction_sum * col_width_sum
                                    - fixed_width_sum;

    if ( !width_set_by_user && required_auto_width_sum < auto_width_sum )
    {
#if 1
        if ( relative_fraction_sum < 1.0 )
            col_width_sum = ( fixed_width_sum + auto_width_sum ) / ( 1.0 - relative_fraction_sum );
        else
            col_width_sum = fixed_width_sum + auto_width_sum + relative_width_sum;
#endif
        required_auto_width_sum = auto_width_sum;
    }

    // Ratio by which we have to shring/grow all auto sized columns to make it all fit
    qreal auto_width_scale = 1.0;
    if ( auto_width_sum > 0.0 )
        auto_width_scale = required_auto_width_sum / auto_width_sum;

    // Set correct sizes for the auto sized and the relatively sized columns.
    for ( i = 0; i < m_cell_size_data.numCols(); ++i )
    {
        QString value = Helpers::mmlInterpretListAttr( columnwidth_attr, i, "auto" );

        // Is it a relatively sized column?
        if ( value.endsWith( "%" ) )
        {
            bool ok;
            qreal w = Helpers::mmlInterpretPercentSpacing( value, col_width_sum, &ok );
            if ( ok )
            {
                m_cell_size_data.col_widths[i] = w;
            }
            else
            {
                // We're treating parsing errors here as auto sized columns
                m_cell_size_data.col_widths[i] = auto_width_scale * m_cell_size_data.col_widths[i];
            }
        }
        // Is it an auto sized column?
        else if ( value == "auto" )
        {
            m_cell_size_data.col_widths[i] = auto_width_scale * m_cell_size_data.col_widths[i];
        }
    }

    m_content_width = m_cell_size_data.colWidthSum()
                      + col_spc * ( m_cell_size_data.numCols() - 1 );
    m_content_height = m_cell_size_data.rowHeightSum()
                       + row_spc * ( m_cell_size_data.numRows() - 1 );

    qreal bottom = -0.5 * m_content_height;
    MmlNode *child = firstChild();
    for ( ; child != 0; child = child->nextSibling() )
    {
        Q_ASSERT( child->nodeType() == MtrNode );
        MmlMtrNode *row = ( MmlMtrNode* ) child;

        row->layoutCells( m_cell_size_data.col_widths, col_spc );
        QRectF rmr = row->myRect();
        row->setRelOrigin( QPointF( 0.0, bottom - rmr.top() ) );
        bottom += rmr.height() + row_spc;
    }
}

QRectF MmlMtableNode::symbolRect() const
{
    qreal frame_spc_hor = framespacing_hor();
    qreal frame_spc_ver = framespacing_ver();

    return QRectF( -frame_spc_hor,
                   -0.5 * m_content_height - frame_spc_ver,
                   m_content_width + 2.0 * frame_spc_hor,
                   m_content_height + 2.0 * frame_spc_ver );
}

Mml::FrameType MmlMtableNode::frame() const
{
    QString value = explicitAttribute( "frame", "none" );
    return Helpers::mmlInterpretFrameType( value, 0, 0 );
}

Mml::FrameType MmlMtableNode::columnlines( int idx ) const
{
    QString value = explicitAttribute( "columnlines", "none" );
    return Helpers::mmlInterpretFrameType( value, idx, 0 );
}

Mml::FrameType MmlMtableNode::rowlines( int idx ) const
{
    QString value = explicitAttribute( "rowlines", "none" );
    return Helpers::mmlInterpretFrameType( value, idx, 0 );
}

void MmlMtableNode::paintSymbol(QPainter *painter, qreal x_scaling, qreal y_scaling ) const
{
    MmlNode::paintSymbol( painter, x_scaling, y_scaling );

    painter->save();

    painter->translate( devicePoint( QPointF() ) );

    QPen pen = painter->pen();

    FrameType frame_type = frame();
    if ( frame_type != FrameNone )
    {
        if ( frame_type == FrameDashed )
            pen.setStyle( Qt::DashLine );
        else
            pen.setStyle( Qt::SolidLine );
        painter->setPen( pen );
        painter->drawRect( myRect() );
    }

    qreal col_spc = columnspacing();
    qreal row_spc = rowspacing();

    qreal col_offset = 0.0;
    int i;
    for ( i = 0; i < m_cell_size_data.numCols() - 1; ++i )
    {
        FrameType frame_type = columnlines( i );
        col_offset += m_cell_size_data.col_widths[i];

        if ( frame_type != FrameNone )
        {
            if ( frame_type == FrameDashed )
                pen.setStyle( Qt::DashLine );
            else if ( frame_type == FrameSolid )
                pen.setStyle( Qt::SolidLine );

            painter->setPen( pen );
            qreal x = col_offset + 0.5 * col_spc;
            painter->drawLine( QPointF( x, -0.5 * m_content_height ),
                               QPointF( x,  0.5 * m_content_height ) );
        }
        col_offset += col_spc;
    }

    qreal row_offset = 0.0;
    for ( i = 0; i < m_cell_size_data.numRows() - 1; ++i )
    {
        FrameType frame_type = rowlines( i );
        row_offset += m_cell_size_data.row_heights[i];

        if ( frame_type != FrameNone )
        {
            if ( frame_type == FrameDashed )
                pen.setStyle( Qt::DashLine );
            else if ( frame_type == FrameSolid )
                pen.setStyle( Qt::SolidLine );

            painter->setPen( pen );
            qreal y = row_offset + 0.5 * ( row_spc - m_content_height );
            painter->drawLine( QPointF( 0, y ),
                               QPointF( m_content_width, y ) );
        }
        row_offset += row_spc;
    }

    painter->restore();
}

qreal MmlMtableNode::framespacing_ver() const
{
    if ( frame() == FrameNone )
        return 0.2 * em();

    QString value = explicitAttribute( "framespacing", "0.4em 0.5ex" );

    bool ok;
    FrameSpacing fs = Helpers::mmlInterpretFrameSpacing( value, em(), ex(), &ok );
    if ( ok )
        return fs.m_ver;
    else
        return 0.5 * ex();
}

qreal MmlMtableNode::framespacing_hor() const
{
    if ( frame() == FrameNone )
        return 0.2 * em();

    QString value = explicitAttribute( "framespacing", "0.4em 0.5ex" );

    bool ok;
    FrameSpacing fs = Helpers::mmlInterpretFrameSpacing( value, em(), ex(), &ok );
    if ( ok )
        return fs.m_hor;
    else
        return 0.4 * em();
}

#endif//MMLMTABLENODE_CPP
