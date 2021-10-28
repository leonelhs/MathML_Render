// *******************************************************************
// MmlDocument
// *******************************************************************

#include "Helpers.h"
#include "MmlDocument.h"
#include "MmlEntityTable.h"

#include "Nodes/XmlNodes/MmlMfracNode.h"
#include "Nodes/XmlNodes/MmlMiNode.h"
#include "Nodes/XmlNodes/MmlMnNode.h"
#include "Nodes/XmlNodes/MmlTextNode.h"
#include "Nodes/XmlNodes/MmlMrowNode.h"
#include "Nodes/XmlNodes/MmlMsqrtNode.h"
#include "Nodes/XmlNodes/MmlMrootNode.h"
#include "Nodes/XmlNodes/MmlMsupNode.h"
#include "Nodes/XmlNodes/MmlMsubNode.h"
#include "Nodes/XmlNodes/MmlMsubsupNode.h"
#include "Nodes/XmlNodes/MmlMoNode.h"
#include "Nodes/XmlNodes/MmlMstyleNode.h"
#include "Nodes/XmlNodes/MmlTextNode.h"
#include "Nodes/XmlNodes/MmlMphantomNode.h"
#include "Nodes/XmlNodes/MmlMfencedNode.h"
#include "Nodes/XmlNodes/MmlMtableNode.h"
#include "Nodes/XmlNodes/MmlMtrNode.h"
#include "Nodes/XmlNodes/MmlMtdNode.h"
#include "Nodes/XmlNodes/MmlMoverNode.h"
#include "Nodes/XmlNodes/MmlMunderNode.h"
#include "Nodes/XmlNodes/MmlMunderoverNode.h"
#include "Nodes/XmlNodes/MmlMalignMarkNode.h"
#include "Nodes/XmlNodes/MmlMerrorNode.h"
#include "Nodes/XmlNodes/MmlMtextNode.h"
#include "Nodes/XmlNodes/MmlMpaddedNode.h"
#include "Nodes/XmlNodes/MmlMspaceNode.h"
#include "Nodes/XmlNodes/MmlUnknownNode.h"


#include <QtDebug>

MmlDocument::MmlDocument()
{
    m_root_node = 0;

    m_normal_font_name = "Times New Roman";

    m_fraktur_font_name = "Fraktur";
    m_sans_serif_font_name = "Luxi Sans";
    m_script_font_name = "Urw Chancery L";
    m_monospace_font_name = "Luxi Mono";
    m_doublestruck_font_name = "Doublestruck";
    m_base_font_point_size = 16;
    m_foreground_color = Qt::black;
    m_background_color = Qt::white;

#ifdef MML_TEST
    m_draw_frames = false;
#endif
}

MmlDocument::~MmlDocument()
{
    clear();
}

void MmlDocument::clear()
{
    delete m_root_node;
    m_root_node = 0;
}

void MmlDocument::dump() const
{
    if ( m_root_node == 0 )
        return;

    QString indent;
    _dump( m_root_node, indent );
}

void MmlDocument::_dump(const MmlNode *node, const QString &indent ) const
{
    if ( node == 0 ) return;

    qWarning() << indent + node->toStr();

    const MmlNode *child = node->firstChild();
    for ( ; child != 0; child = child->nextSibling() )
        _dump( child, indent + "  " );
}

void MmlDocument::insertOperator( MmlNode *node, const QString &text )
{
    MmlNode *text_node = createNode( TextNode, MmlAttributeMap(), text, 0 );
    MmlNode *mo_node = createNode( MoNode, MmlAttributeMap(), QString(), 0 );

    bool ok = insertChild( node, mo_node, 0 );
    Q_ASSERT( ok );
    ok = insertChild( mo_node, text_node, 0 );
    Q_ASSERT( ok );
    Q_UNUSED( ok );
}

bool MmlDocument::insertChild( MmlNode *parent, MmlNode *new_node, QString *errorMsg )
{
    if ( new_node == 0 )
        return true;

    Q_ASSERT( new_node->parent() == 0
              && new_node->nextSibling() == 0
              && new_node->previousSibling() == 0 );

    if ( parent != 0 )
    {
        if ( !Helpers::mmlCheckChildType( parent->nodeType(), new_node->nodeType(), errorMsg ) )
            return false;
    }

    if ( parent == 0 )
    {
        if ( m_root_node == 0 )
        {
            m_root_node = new_node;
        }
        else
        {
            MmlNode *n = m_root_node->lastSibling();
            n->m_next_sibling = new_node;
            new_node->m_previous_sibling = n;
        }
    }
    else
    {
        new_node->m_parent = parent;
        if ( parent->hasChildNodes() )
        {
            MmlNode *n = parent->firstChild()->lastSibling();
            n->m_next_sibling = new_node;
            new_node->m_previous_sibling = n;
        }
        else
        {
            parent->m_first_child = new_node;
        }
    }

    return true;
}

bool MmlDocument::setContent(const QString &text, QString *errorMsg, int *errorLine, int *errorColumn )
{
    clear();

    MMLEntityTable EntityTable;

    QString prefix = "<?xml version=\"1.0\"?>\n";
    prefix.append(EntityTable.entities());

    QDomDocument dom;
    if ( !dom.setContent( prefix + text, false, errorMsg, errorLine, errorColumn ) )
        return false;

    // we don't have access to line info from now on
    if ( errorLine != 0 ) *errorLine = -1;
    if ( errorColumn != 0 ) *errorColumn = -1;

    bool ok;
    MmlNode *root_node = domToMml( dom, &ok, errorMsg );
    if ( !ok )
        return false;

    if ( root_node == 0 )
    {
        if ( errorMsg != 0 )
            *errorMsg = "empty document";
        return false;
    }

    insertChild( 0, root_node, 0 );
    layout();

    return true;
}

void MmlDocument::layout()
{
    if ( m_root_node == 0 )
        return;

    m_root_node->layout();
    m_root_node->stretch();
}

MmlNode *MmlDocument::createNode( Mml::NodeType type, const MmlAttributeMap &mml_attr, const QString &mml_value, QString *errorMsg )
{
    Q_ASSERT( type != NoNode );

    MmlNode *mml_node = 0;

    if ( !Helpers::mmlCheckAttributes( type, mml_attr, errorMsg ) )
        return 0;

    switch ( type )
    {
    case MiNode:
        mml_node = new MmlMiNode( this, mml_attr );
        break;
    case MnNode:
        mml_node = new MmlMnNode( this, mml_attr );
        break;
    case MfracNode:
        mml_node = new MmlMfracNode( this, mml_attr );
        break;
    case MrowNode:
        mml_node = new MmlMrowNode( this, mml_attr );
        break;
    case MsqrtNode:
        mml_node = new MmlMsqrtNode( this, mml_attr );
        break;
    case MrootNode:
        mml_node = new MmlMrootNode( this, mml_attr );
        break;
    case MsupNode:
        mml_node = new MmlMsupNode( this, mml_attr );
        break;
    case MsubNode:
        mml_node = new MmlMsubNode( this, mml_attr );
        break;
    case MsubsupNode:
        mml_node = new MmlMsubsupNode( this, mml_attr );
        break;
    case MoNode:
        mml_node = new MmlMoNode( this, mml_attr );
        break;
    case MstyleNode:
        mml_node = new MmlMstyleNode( this, mml_attr );
        break;
    case TextNode:
        mml_node = new MmlTextNode( mml_value, this );
        break;
    case MphantomNode:
        mml_node = new MmlMphantomNode( this, mml_attr );
        break;
    case MfencedNode:
        mml_node = new MmlMfencedNode( this, mml_attr );
        break;
    case MtableNode:
        mml_node = new MmlMtableNode( this, mml_attr );
        break;
    case MtrNode:
        mml_node = new MmlMtrNode( this, mml_attr );
        break;
    case MtdNode:
        mml_node = new MmlMtdNode( this, mml_attr );
        break;
    case MoverNode:
        mml_node = new MmlMoverNode( this, mml_attr );
        break;
    case MunderNode:
        mml_node = new MmlMunderNode( this, mml_attr );
        break;
    case MunderoverNode:
        mml_node = new MmlMunderoverNode( this, mml_attr );
        break;
    case MalignMarkNode:
        mml_node = new MmlMalignMarkNode( this );
        break;
    case MerrorNode:
        mml_node = new MmlMerrorNode( this, mml_attr );
        break;
    case MtextNode:
        mml_node = new MmlMtextNode( this, mml_attr );
        break;
    case MpaddedNode:
        mml_node = new MmlMpaddedNode( this, mml_attr );
        break;
    case MspaceNode:
        mml_node = new MmlMspaceNode( this, mml_attr );
        break;
    case UnknownNode:
        mml_node = new MmlUnknownNode( this, mml_attr );
        break;
    case NoNode:
        mml_node = 0;
        break;
    }

    return mml_node;
}

MmlNode *MmlDocument::domToMml( const QDomNode &dom_node, bool *ok, QString *errorMsg )
{
    // create the node

    Q_ASSERT( ok != 0 );

    NodeType mml_type = Helpers::domToMmlNodeType( dom_node );

    if ( mml_type == NoNode )
    {
        *ok = true;
        return 0;
    }

    QDomNamedNodeMap dom_attr = dom_node.attributes();
    MmlAttributeMap mml_attr;
    for ( int i = 0; i < dom_attr.length(); ++i )
    {
        QDomNode attr_node = dom_attr.item( i );
        Q_ASSERT( !attr_node.nodeName().isNull() );
        Q_ASSERT( !attr_node.nodeValue().isNull() );
        mml_attr[attr_node.nodeName()] = attr_node.nodeValue();
    }

    QString mml_value;
    if ( mml_type == TextNode )
        mml_value = dom_node.nodeValue();
    MmlNode *mml_node = createNode( mml_type, mml_attr, mml_value, errorMsg );
    if ( mml_node == 0 )
    {
        *ok = false;
        return 0;
    }

    // create the node's children according to the child_spec

    const MmlNodeSpec *spec = mmlFindNodeSpec( mml_type );
    QDomNodeList dom_child_list = dom_node.childNodes();
    int child_cnt = dom_child_list.count();
    MmlNode *mml_child = 0;

    QString separator_list;
    if ( mml_type == MfencedNode )
        separator_list = mml_node->explicitAttribute( "separators", "," );

    switch ( spec->child_spec )
    {
    case MmlNodeSpec::ChildIgnore:
        break;

    case MmlNodeSpec::ImplicitMrow:

        if ( child_cnt > 0 )
        {
            mml_child = createImplicitMrowNode( dom_node, ok, errorMsg );
            if ( !*ok )
            {
                delete mml_node;
                return 0;
            }

            if ( !insertChild( mml_node, mml_child, errorMsg ) )
            {
                delete mml_node;
                delete mml_child;
                *ok = false;
                return 0;
            }
        }

        break;

    default:
        // exact ammount of children specified - check...
        if ( spec->child_spec != child_cnt )
        {
            if ( errorMsg != 0 )
                *errorMsg = QString( "element " )
                            + spec->tag
                            + " requires exactly "
                            + QString::number( spec->child_spec )
                            + " arguments, got "
                            + QString::number( child_cnt );
            delete mml_node;
            *ok = false;
            return 0;
        }

    // ...and continue just as in ChildAny

    case MmlNodeSpec::ChildAny:
        if ( mml_type == MfencedNode )
            insertOperator( mml_node, mml_node->explicitAttribute( "open", "(" ) );

        for ( int i = 0; i < child_cnt; ++i )
        {
            QDomNode dom_child = dom_child_list.item( i );

            MmlNode *mml_child = domToMml( dom_child, ok, errorMsg );
            if ( !*ok )
            {
                delete mml_node;
                return 0;
            }

            if ( mml_type == MtableNode && mml_child->nodeType() != MtrNode )
            {
                MmlNode *mtr_node = createNode( MtrNode, MmlAttributeMap(), QString(), 0 );
                insertChild( mml_node, mtr_node, 0 );
                if ( !insertChild( mtr_node, mml_child, errorMsg ) )
                {
                    delete mml_node;
                    delete mml_child;
                    *ok = false;
                    return 0;
                }
            }
            else if ( mml_type == MtrNode && mml_child->nodeType() != MtdNode )
            {
                MmlNode *mtd_node = createNode( MtdNode, MmlAttributeMap(), QString(), 0 );
                insertChild( mml_node, mtd_node, 0 );
                if ( !insertChild( mtd_node, mml_child, errorMsg ) )
                {
                    delete mml_node;
                    delete mml_child;
                    *ok = false;
                    return 0;
                }
            }
            else
            {
                if ( !insertChild( mml_node, mml_child, errorMsg ) )
                {
                    delete mml_node;
                    delete mml_child;
                    *ok = false;
                    return 0;
                }
            }

            if ( i < child_cnt - 1 && mml_type == MfencedNode && !separator_list.isEmpty() )
            {
                QChar separator;
                if ( i >= separator_list.length() )
                    separator = separator_list.at( separator_list.length() - 1 );
                else
                    separator = separator_list[i];
                insertOperator( mml_node, QString( separator ) );
            }
        }

        if ( mml_type == MfencedNode )
            insertOperator( mml_node, mml_node->explicitAttribute( "close", ")" ) );

        break;
    }

    *ok = true;
    return mml_node;
}

MmlNode *MmlDocument::createImplicitMrowNode( const QDomNode &dom_node,
        bool *ok,
        QString *errorMsg )
{
    QDomNodeList dom_child_list = dom_node.childNodes();
    int child_cnt = dom_child_list.count();

    if ( child_cnt == 0 )
    {
        *ok = true;
        return 0;
    }

    if ( child_cnt == 1 )
        return domToMml( dom_child_list.item( 0 ), ok, errorMsg );

    MmlNode *mml_node = createNode( MrowNode, MmlAttributeMap(),
                                    QString(), errorMsg );
    Q_ASSERT( mml_node != 0 ); // there is no reason in heaven or hell for this to fail

    for ( int i = 0; i < child_cnt; ++i )
    {
        QDomNode dom_child = dom_child_list.item( i );

        MmlNode *mml_child = domToMml( dom_child, ok, errorMsg );
        if ( !*ok )
        {
            delete mml_node;
            return 0;
        }

        if ( !insertChild( mml_node, mml_child, errorMsg ) )
        {
            delete mml_node;
            delete mml_child;
            *ok = false;
            return 0;
        }
    }

    return mml_node;
}

void MmlDocument::paint( QPainter *painter, const QPointF &pos )
{
    if ( m_root_node == 0 )
        return;

    m_root_node->setRelOrigin( pos - m_root_node->myRect().topLeft() );
    m_root_node->paint( painter, 1.0, 1.0 );
}

QSizeF MmlDocument::size() const
{
    if ( m_root_node == 0 )
        return QSizeF( 0.0, 0.0 );
    return m_root_node->deviceRect().size();
}

QString MmlDocument::fontName( MmlFont::Font type ) const
{
    switch ( type )
    {
    case MmlFont::Font::NormalFont:
        return m_normal_font_name;
    case MmlFont::Font::FrakturFont:
        return m_fraktur_font_name;
    case MmlFont::Font::SansSerifFont:
        return m_sans_serif_font_name;
    case MmlFont::Font::ScriptFont:
        return m_script_font_name;
    case MmlFont::Font::MonospaceFont:
        return m_monospace_font_name;
    case MmlFont::Font::DoublestruckFont:
        return m_doublestruck_font_name;
    };

    return QString();
}

void MmlDocument::setFontName( MmlFont::Font type, const QString &name )
{
    switch ( type )
    {
    case MmlFont::Font::NormalFont:
        m_normal_font_name = name;
        break;
    case MmlFont::Font::FrakturFont:
        m_fraktur_font_name = name;
        break;
    case MmlFont::Font::SansSerifFont:
        m_sans_serif_font_name = name;
        break;
    case MmlFont::Font::ScriptFont:
        m_script_font_name = name;
        break;
    case MmlFont::Font::MonospaceFont:
        m_monospace_font_name = name;
        break;
    case MmlFont::Font::DoublestruckFont:
        m_doublestruck_font_name = name;
        break;
    };
}
