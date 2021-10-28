// *******************************************************************
// Static helper functions
// *******************************************************************

#include <QString>
#include "Helpers.h"

double Helpers::mmlQstringToQreal( const QString &string, bool *ok )
{
    static const int SizeOfDouble = sizeof( double );

    if ( sizeof( double ) == SizeOfDouble )
        return string.toDouble( ok );
    else
        return string.toFloat( ok );
}

QString Helpers::mmlInterpretListAttr(const QString &value_list, int idx, const QString &def )
{
    QStringList l = value_list.split( ' ' );

    if ( l.count() == 0 )
        return def;

    if ( l.count() <= idx )
        return l[l.count() - 1];
    else
        return l[idx];
}

Mml::FrameType Helpers::mmlInterpretFrameType(const QString &value_list, int idx, bool *ok )
{
    if ( ok != 0 )
        *ok = true;

    QString value = mmlInterpretListAttr( value_list, idx, "none" );

    if ( value == "none" )
        return Mml::FrameNone;
    if ( value == "solid" )
        return Mml::FrameSolid;
    if ( value == "dashed" )
        return Mml::FrameDashed;

    if ( ok != 0 )
        *ok = false;

    qWarning( "interpretFrameType(): could not parse value \"%s\"", qPrintable( value ) );
    return Mml::FrameNone;
}

double Helpers::mmlInterpretSpacing( QString value, double em, double ex, bool *ok )
{
    if ( ok != 0 )
        *ok = true;

    if ( value == "thin" )
        return 1.0;

    if ( value == "medium" )
        return 2.0;

    if ( value == "thick" )
        return 3.0;

    struct HSpacingValue
    {
        QString name;
        double factor;
    };

    static const HSpacingValue g_h_spacing_data[] =
    {
        { "veryverythinmathspace",  0.0555556 },
        { "verythinmathspace",      0.111111  },
        { "thinmathspace",          0.166667  },
        { "mediummathspace",        0.222222  },
        { "thickmathspace",         0.277778  },
        { "verythickmathspace",     0.333333  },
        { "veryverythickmathspace", 0.388889  },
        { 0,                        0.0       }
    };

    const HSpacingValue *v = g_h_spacing_data;
    for ( ; v->name != ""; ++v )
    {
        if ( value == v->name )
            return em * v->factor;
    }

    if ( value.endsWith( "em" ) )
    {
        value.truncate( value.length() - 2 );
        bool qreal_ok;
        double factor = mmlQstringToQreal( value, &qreal_ok );
        if ( qreal_ok && factor >= 0.0 )
            return em * factor;
        else
        {
            qWarning( "interpretSpacing(): could not parse \"%sem\"", qPrintable( value ) );
            if ( ok != 0 )
                *ok = false;
            return 0.0;
        }
    }

    if ( value.endsWith( "ex" ) )
    {
        value.truncate( value.length() - 2 );
        bool qreal_ok;
        double factor = mmlQstringToQreal( value, &qreal_ok );
        if ( qreal_ok && factor >= 0.0 )
            return ex * factor;
        else
        {
            qWarning( "interpretSpacing(): could not parse \"%sex\"", qPrintable( value ) );
            if ( ok != 0 )
                *ok = false;
            return 0.0;
        }
    }

    if ( value.endsWith( "cm" ) )
    {
        value.truncate( value.length() - 2 );
        bool qreal_ok;
        double factor = mmlQstringToQreal( value, &qreal_ok );
        if ( qreal_ok && factor >= 0.0 )
        {
            return factor * 10.0 * Device::width() / Device::widthMM();
        }
        else
        {
            qWarning( "interpretSpacing(): could not parse \"%scm\"", qPrintable( value ) );
            if ( ok != 0 )
                *ok = false;
            return 0.0;
        }
    }

    if ( value.endsWith( "mm" ) )
    {
        value.truncate( value.length() - 2 );
        bool qreal_ok;
        double factor = mmlQstringToQreal( value, &qreal_ok );
        if ( qreal_ok && factor >= 0.0 )
        {
            return factor * Device::width() / Device::widthMM();
        }
        else
        {
            qWarning( "interpretSpacing(): could not parse \"%smm\"", qPrintable( value ) );
            if ( ok != 0 )
                *ok = false;
            return 0.0;
        }
    }

    if ( value.endsWith( "in" ) )
    {
        value.truncate( value.length() - 2 );
        bool qreal_ok;
        double factor = mmlQstringToQreal( value, &qreal_ok );
        if ( qreal_ok && factor >= 0.0 )
        {
            return factor * 10.0 * Device::width() / (2.54 * Device::widthMM());
        }
        else
        {
            qWarning( "interpretSpacing(): could not parse \"%sin\"", qPrintable( value ) );
            if ( ok != 0 )
                *ok = false;
            return 0.0;
        }
    }

    if ( value.endsWith( "px" ) )
    {
        value.truncate( value.length() - 2 );
        bool qreal_ok;
        int i = mmlQstringToQreal( value, &qreal_ok );
        if ( qreal_ok && i >= 0 )
            return i;
        else
        {
            qWarning( "interpretSpacing(): could not parse \"%spx\"", qPrintable( value ) );
            if ( ok != 0 )
                *ok = false;
            return 0.0;
        }
    }

    bool qreal_ok;
    int i = mmlQstringToQreal( value, &qreal_ok );
    if ( qreal_ok && i >= 0 )
        return i;

    qWarning( "interpretSpacing(): could not parse \"%s\"", qPrintable( value ) );
    if ( ok != 0 )
        *ok = false;
    return 0.0;
}

Mml::FrameSpacing Helpers::mmlInterpretFrameSpacing(const QString &value_list, qreal em, qreal ex, bool *ok )
{
    Mml::FrameSpacing fs;

    QStringList l = value_list.split( ' ' );
    if ( l.count() != 2 )
    {
        qWarning( "interpretFrameSpacing: could not parse value \"%s\"", qPrintable( value_list ) );
        if ( ok != 0 )
            *ok = false;
        return Mml::FrameSpacing( 0.4 * em, 0.5 * ex );
    }

    bool hor_ok, ver_ok;
    fs.m_hor = mmlInterpretSpacing( l[0], em, ex, &hor_ok );
    fs.m_ver = mmlInterpretSpacing( l[1], em, ex, &ver_ok );

    if ( ok != 0 )
        *ok = hor_ok && ver_ok;

    return fs;
}

double Helpers::mmlInterpretPercentSpacing(QString value, double base, bool *ok )
{
    if ( !value.endsWith( "%" ) )
    {
        if ( ok != 0 )
            *ok = false;
        return 0.0;
    }

    value.truncate( value.length() - 1 );
    bool qreal_ok;
    double factor = mmlQstringToQreal( value, &qreal_ok );
    if ( qreal_ok && factor >= 0.0 )
    {
        if ( ok != 0 )
            *ok = true;
        return 0.01 * base * factor;
    }

    qWarning( "interpretPercentSpacing(): could not parse \"%s%%\"", qPrintable( value ) );
    if ( ok != 0 )
        *ok = false;
    return 0.0;
}

double Helpers::mmlInterpretPointSize( QString value, bool *ok )
{
    if ( !value.endsWith( "pt" ) )
    {
        if ( ok != 0 )
            *ok = false;
        return 0;
    }

    value.truncate( value.length() - 2 );
    bool qreal_ok;
    double pt_size = mmlQstringToQreal( value, &qreal_ok );
    if ( qreal_ok && pt_size > 0.0 )
    {
        if ( ok != 0 )
            *ok = true;
        return pt_size;
    }

    qWarning( "interpretPointSize(): could not parse \"%spt\"", qPrintable( value ) );
    if ( ok != 0 )
        *ok = false;
    return 0.0;
}

const MmlNodeSpec* Helpers::mmlFindNodeSpec( Mml::NodeType type )
{
    const MmlNodeSpec *spec = g_node_spec_data;
    for ( ; spec->type != Mml::NoNode; ++spec )
    {
        if ( type == spec->type ) return spec;
    }
    return 0;
}

const MmlNodeSpec* Helpers::mmlFindNodeSpec( const QString &tag )
{
    const MmlNodeSpec *spec = g_node_spec_data;
    for ( ; spec->type != Mml::NoNode; ++spec )
    {
        if ( tag == spec->tag ) return spec;
    }
    return 0;
}

bool Helpers::mmlCheckChildType( Mml::NodeType parent_type, Mml::NodeType child_type, QString *error_str )
{
    if ( parent_type == Mml::UnknownNode || child_type == Mml::UnknownNode )
        return true;

    const MmlNodeSpec *child_spec = mmlFindNodeSpec( child_type );
    const MmlNodeSpec *parent_spec = mmlFindNodeSpec( parent_type );

    Q_ASSERT( parent_spec != 0 );
    Q_ASSERT( child_spec != 0 );

    QString allowed_child_types( parent_spec->child_types );
    // null list means any child type is valid
    if ( allowed_child_types.isNull() )
        return true;

    QString child_type_str = QString( " " ) + child_spec->type_str + " ";
    if ( !allowed_child_types.contains( child_type_str ) )
    {
        if ( error_str != 0 )
            *error_str = QString( "illegal child " )
                         + child_spec->type_str
                         + " for parent "
                         + parent_spec->type_str;
        return false;
    }

    return true;
}

bool Helpers::mmlCheckAttributes( Mml::NodeType child_type, const MmlAttributeMap &attr, QString *error_str )
{
    const MmlNodeSpec *spec = mmlFindNodeSpec( child_type );
    Q_ASSERT( spec != 0 );

    QString allowed_attr( spec->attributes );
    // empty list means any attr is valid
    if ( allowed_attr.isEmpty() )
        return true;

    MmlAttributeMap::const_iterator it = attr.begin(), end = attr.end();
    for ( ; it != end; ++it )
    {
        QString name = it.key();

        if ( name.indexOf( ':' ) != -1 )
            continue;

        QString padded_name = " " + name + " ";
        if ( !allowed_attr.contains( padded_name ) )
        {
            if ( error_str != 0 )
                *error_str = QString( "illegal attribute " )
                             + name
                             + " in "
                             + spec->type_str;
            return false;
        }
    }

    return true;
}

int Helpers::attributeIndex( const QString &name )
{
    for ( int i = 0; i < g_oper_spec_rows; ++i )
    {
        if ( name == g_oper_spec_names[i] )
            return i;
    }
    return -1;
}

Mml::ColAlign Helpers::mmlInterpretColAlign(const QString &value_list, int colnum, bool *ok )
{
    QString value = mmlInterpretListAttr( value_list, colnum, "center" );

    if ( ok != 0 )
        *ok = true;

    if ( value == "left" )
        return Mml::ColAlignLeft;
    if ( value == "right" )
        return Mml::ColAlignRight;
    if ( value == "center" )
        return Mml::ColAlignCenter;

    if ( ok != 0 )
        *ok = false;

    qWarning( "interpretColAlign(): could not parse value \"%s\"", qPrintable( value ) );
    return Mml::ColAlignCenter;
}

Mml::RowAlign Helpers::mmlInterpretRowAlign(const QString &value_list, int rownum, bool *ok )
{
    QString value = mmlInterpretListAttr( value_list, rownum, "axis" );

    if ( ok != 0 )
        *ok = true;

    if ( value == "top" )
        return Mml::RowAlignTop;
    if ( value == "center" )
        return Mml::RowAlignCenter;
    if ( value == "bottom" )
        return Mml::RowAlignBottom;
    if ( value == "baseline" )
        return Mml::RowAlignBaseline;
    if ( value == "axis" )
        return Mml::RowAlignAxis;

    if ( ok != 0 )
        *ok = false;

    qWarning( "interpretRowAlign(): could not parse value \"%s\"", qPrintable( value ) );
    return Mml::RowAlignAxis;
}

QString Helpers::mmlDictAttribute( const QString &name, const MmlOperSpec *spec )
{
    int i = Helpers::attributeIndex( name );
    if ( i == -1 )
        return QString();
    else
        return spec->attributes[i];
}

int Helpers::mmlInterpretMathVariant( const QString &value, bool *ok )
{
    struct MathVariantValue
    {
        QString value;
        int mv;
    };

    static const MathVariantValue g_mv_data[] =
    {
        { "normal",                 Mml::NormalMV                                        },
        { "bold",                   Mml::BoldMV                                          },
        { "italic",                 Mml::ItalicMV                                        },
        { "bold-italic",            Mml::BoldMV | Mml::ItalicMV                       },
        { "double-struck",          Mml::DoubleStruckMV                                  },
        { "bold-fraktur",           Mml::BoldMV | Mml::FrakturMV                      },
        { "script",                 Mml::ScriptMV                                        },
        { "bold-script",            Mml::BoldMV | Mml::ScriptMV                       },
        { "fraktur",                Mml::FrakturMV                                       },
        { "sans-serif",             Mml::SansSerifMV                                     },
        { "bold-sans-serif",        Mml::BoldMV | Mml::SansSerifMV                    },
        { "sans-serif-italic",      Mml::SansSerifMV | Mml::ItalicMV                  },
        { "sans-serif-bold-italic", Mml::SansSerifMV | Mml::ItalicMV | Mml::BoldMV },
        { "monospace",              Mml::MonospaceMV                                     },
        { 0,                        0                                                       }
    };

    const MathVariantValue *v = g_mv_data;
    for ( ; v->value != ""; ++v )
    {
        if ( value == v->value )
        {
            if ( ok != 0 )
                *ok = true;
            return v->mv;
        }
    }

    if ( ok != 0 )
        *ok = false;

    qWarning( "interpretMathVariant(): could not parse value: \"%s\"", qPrintable( value ) );

    return Mml::NormalMV;
}

void Helpers::updateFontAttr( MmlAttributeMap &font_attr, const MmlNode *n, const QString &name, const QString &preferred_name)
{
    /*if ( font_attr.contains( preferred_name ) || font_attr.contains( name ) )
        return;
    QString value = n->explicitAttribute( name );
    if ( !value.isNull() )
        font_attr[name] = value;*/
}

MmlAttributeMap Helpers::collectFontAttributes( const MmlNode *node )
{
    MmlAttributeMap font_attr;

    for ( const MmlNode *n = node; n != 0; n = n->parent() )
    {
        if ( n == node || n->nodeType() == Mml::MstyleNode )
        {
            updateFontAttr( font_attr, n, "mathvariant" );
            updateFontAttr( font_attr, n, "mathsize" );

            // depreciated attributes
            updateFontAttr( font_attr, n, "fontsize", "mathsize" );
            updateFontAttr( font_attr, n, "fontweight", "mathvariant" );
            updateFontAttr( font_attr, n, "fontstyle", "mathvariant" );
            updateFontAttr( font_attr, n, "fontfamily", "mathvariant" );
        }
    }

    return font_attr;
}

QFont Helpers::mmlInterpretMathSize( const QString &value, QFont &fn, double em, double ex, bool *ok )
{
    if ( ok != 0 )
        *ok = true;

    if ( value == "small" )
    {
        fn.setPointSizeF( fn.pointSizeF() * 0.7 );
        return fn;
    }

    if ( value == "normal" )
        return fn;

    if ( value == "big" )
    {
        fn.setPointSizeF( fn.pointSizeF() * 1.5 );
        return fn;
    }

    bool size_ok;

    double ptsize = mmlInterpretPointSize( value, &size_ok );
    if ( size_ok )
    {
        fn.setPointSizeF( ptsize );
        return fn;
    }

    double size = mmlInterpretSpacing( value, em, ex, &size_ok );
    if ( size_ok )
    {
#if 1
        fn.setPixelSize( size );  // setPointSizeF ???
#endif
        return fn;
    }

    if ( ok != 0 )
        *ok = false;
    qWarning( "interpretMathSize(): could not parse mathsize \"%s\"", qPrintable( value ) );
    return fn;
}

QFont Helpers::mmlInterpretDepreciatedFontAttr(const MmlAttributeMap &font_attr, QFont &fn, double em, double ex )
{
    if ( font_attr.contains( "fontsize" ) )
    {
        QString value = font_attr["fontsize"];

        for ( ;; )
        {
            bool ok;
            double ptsize = mmlInterpretPointSize( value, &ok );
            if ( ok )
            {
                fn.setPointSizeF( ptsize );
                break;
            }

            ptsize = mmlInterpretPercentSpacing( value, fn.pointSize(), &ok );
            if ( ok )
            {
                fn.setPointSizeF( ptsize );
                break;
            }

            double size = mmlInterpretSpacing( value, em, ex, &ok );
            if ( ok )
            {
#if 1
                fn.setPixelSize( size );
#endif
                break;
            }

            break;
        }
    }

    if ( font_attr.contains( "fontweight" ) )
    {
        QString value = font_attr["fontweight"];
        if ( value == "normal" )
            fn.setBold( false );
        else if ( value == "bold" )
            fn.setBold( true );
        else
            qWarning( "interpretDepreciatedFontAttr(): could not parse fontweight \"%s\"", qPrintable( value ) );
    }

    if ( font_attr.contains( "fontstyle" ) )
    {
        QString value = font_attr["fontstyle"];
        if ( value == "normal" )
            fn.setItalic( false );
        else if ( value == "italic" )
            fn.setItalic( true );
        else
            qWarning( "interpretDepreciatedFontAttr(): could not parse fontstyle \"%s\"", qPrintable( value ) );
    }

    if ( font_attr.contains( "fontfamily" ) )
    {
        QString value = font_attr["fontfamily"];
        fn.setFamily( value );
    }

    return fn;
}

Mml::NodeType Helpers::domToMmlNodeType( const QDomNode &dom_node )
{
    Mml::NodeType mml_type = Mml::NoNode;

    switch ( dom_node.nodeType() )
    {
    case QDomNode::ElementNode:
    {
        QString tag = dom_node.nodeName();
        const MmlNodeSpec *spec = mmlFindNodeSpec( tag );

        // treat urecognised tags as mrow
        if ( spec == 0 )
            mml_type = Mml::UnknownNode;
        else
            mml_type = spec->type;

        break;
    }
    case QDomNode::TextNode:
        mml_type = Mml::TextNode;
        break;

    case QDomNode::DocumentNode:
        mml_type = Mml::UnknownNode;
        break;

    case QDomNode::EntityReferenceNode:
#if 0
        qWarning() << "EntityReferenceNode: name=\"" + dom_node.nodeName() + "\" value=\"" + dom_node.nodeValue() + "\"";
#endif
        break;

    case QDomNode::AttributeNode:
    case QDomNode::CDATASectionNode:
    case QDomNode::EntityNode:
    case QDomNode::ProcessingInstructionNode:
    case QDomNode::CommentNode:
    case QDomNode::DocumentTypeNode:
    case QDomNode::DocumentFragmentNode:
    case QDomNode::NotationNode:
    case QDomNode::BaseNode:
    case QDomNode::CharacterDataNode:
        break;
    }

    return mml_type;
}

QString Helpers::rectToStr( const QRectF &rect )
{
    return QString( "[(%1, %2), %3x%4]" )
           .arg( rect.x() )
           .arg( rect.y() )
           .arg( rect.width() )
           .arg( rect.height() );
}

Mml::FormType Helpers::mmlInterpretForm( const QString &value, bool *ok )
{
    if ( ok != 0 )
        *ok = true;

    if ( value == "prefix" )
        return Mml::PrefixForm;
    if ( value == "infix" )
        return Mml::InfixForm;
    if ( value == "postfix" )
        return Mml::PostfixForm;

    if ( ok != 0 )
        *ok = false;

    qWarning( "interpretForm(): could not parse value \"%s\"", qPrintable( value ) );
    return Mml::InfixForm;
}
