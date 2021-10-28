#ifndef MMLNODESPEC_H
#define MMLNODESPEC_H

#include <QString>
#include "Nodes/Mml.h"

struct MmlNodeSpec
{
    Mml::NodeType type;
    const char *tag;
    const char *type_str;
    int child_spec;
    const char *child_types;
    const char *attributes;

    enum ChildSpec
    {
        ChildAny     = -1, // any number of children allowed
        ChildIgnore  = -2, // do not build subexpression of children
        ImplicitMrow = -3  // if more than one child, build mrow
    };
};


#define MML_ATT_COMMON      " class style id xref actiontype "
#define MML_ATT_FONTSIZE    " fontsize fontweight fontstyle fontfamily color "
#define MML_ATT_MATHVARIANT " mathvariant mathsize mathcolor mathbackground "
#define MML_ATT_FONTINFO    MML_ATT_FONTSIZE MML_ATT_MATHVARIANT
#define MML_ATT_OPINFO      " form fence separator lspace rspace stretchy symmetric " \
" maxsize minsize largeop movablelimits accent "
#define MML_ATT_SIZEINFO    " width height depth "
#define MML_ATT_TABLEINFO   " align rowalign columnalign columnwidth groupalign " \
    " alignmentscope side rowspacing columnspacing rowlines " \
    " columnlines width frame framespacing equalrows " \
    " equalcolumns displaystyle "
#define MML_ATT_MFRAC       " bevelled numalign denomalign linethickness "
#define MML_ATT_MSTYLE      MML_ATT_FONTINFO MML_ATT_OPINFO \
    " scriptlevel lquote rquote linethickness displaystyle " \
    " scriptsizemultiplier scriptminsize background " \
    " veryverythinmathspace verythinmathspace thinmathspace " \
    " mediummathspace thickmathspace verythickmathspace " \
    " veryverythickmathspace open close separators " \
    " subscriptshift superscriptshift accentunder tableinfo " \
    " rowspan columnspan edge selection bevelled "
#define MML_ATT_MTABLE      " align rowalign columnalign groupalign alignmentscope " \
    " columnwidth width rowspacing columnspacing rowlines columnlines " \
    " frame framespacing equalrows equalcolumns displaystyle side " \
    " minlabelspacing "


static const MmlNodeSpec g_node_spec_data[] =
{
    //    type                    tag           type_str          child_spec                    child_types              attributes ""=none, 0=any
    //    ----------------------- ------------- ----------------- ----------------------------- ------------------------ ---------------------------------------------------------------------
    { Mml::MiNode,         "mi",         "MiNode",         MmlNodeSpec::ChildAny,     " TextNode MalignMark ", MML_ATT_COMMON MML_ATT_FONTINFO                                       },
    { Mml::MnNode,         "mn",         "MnNode",         MmlNodeSpec::ChildAny,     " TextNode MalignMark ", MML_ATT_COMMON MML_ATT_FONTINFO                                       },
    { Mml::MfracNode,      "mfrac",      "MfracNode",      2,                            0,                       MML_ATT_COMMON MML_ATT_MFRAC                                          },
    { Mml::MrowNode,       "mrow",       "MrowNode",       MmlNodeSpec::ChildAny,     0,                       MML_ATT_COMMON " display mode "                                       },
    { Mml::MsqrtNode,      "msqrt",      "MsqrtNode",      MmlNodeSpec::ImplicitMrow, 0,                       MML_ATT_COMMON                                                        },
    { Mml::MrootNode,      "mroot",      "MrootNode",      2,                            0,                       MML_ATT_COMMON                                                        },
    { Mml::MsupNode,       "msup",       "MsupNode",       2,                            0,                       MML_ATT_COMMON " subscriptshift "                                     },
    { Mml::MsubNode,       "msub",       "MsubNode",       2,                            0,                       MML_ATT_COMMON " superscriptshift "                                   },
    { Mml::MsubsupNode,    "msubsup",    "MsubsupNode",    3,                            0,                       MML_ATT_COMMON " subscriptshift superscriptshift "                    },
    { Mml::MoNode,         "mo",         "MoNode",         MmlNodeSpec::ChildAny,     " TextNode MalignMark ", MML_ATT_COMMON MML_ATT_FONTINFO MML_ATT_OPINFO                        },
    { Mml::MstyleNode,     "mstyle",     "MstyleNode",     MmlNodeSpec::ImplicitMrow, 0,                       MML_ATT_COMMON MML_ATT_MSTYLE                                         },
    { Mml::MphantomNode,   "mphantom",   "MphantomNode",   MmlNodeSpec::ImplicitMrow, 0,                       MML_ATT_COMMON                                                        },
    { Mml::MalignMarkNode, "malignmark", "MalignMarkNode", 0,                            0,                       ""                                                                    },
    { Mml::MfencedNode,    "mfenced",    "MfencedNode",    MmlNodeSpec::ChildAny,     0,                       MML_ATT_COMMON " open close separators "                              },
    { Mml::MtableNode,     "mtable",     "MtableNode",     MmlNodeSpec::ChildAny,     " MtrNode ",             MML_ATT_COMMON MML_ATT_MTABLE                                         },
    { Mml::MtrNode,        "mtr",        "MtrNode",        MmlNodeSpec::ChildAny,     " MtdNode ",             MML_ATT_COMMON " rowalign columnalign groupalign "                    },
    { Mml::MtdNode,        "mtd",        "MtdNode",        MmlNodeSpec::ImplicitMrow, 0,                       MML_ATT_COMMON " rowspan columnspan rowalign columnalign groupalign " },
    { Mml::MoverNode,      "mover",      "MoverNode",      2,                            0,                       MML_ATT_COMMON " accent align "                                       },
    { Mml::MunderNode,     "munder",     "MunderNode",     2,                            0,                       MML_ATT_COMMON " accentunder align "                                  },
    { Mml::MunderoverNode, "munderover", "MunderoverNode", 3,                            0,                       MML_ATT_COMMON " accentunder accent align "                           },
    { Mml::MerrorNode,     "merror",     "MerrorNode",     MmlNodeSpec::ImplicitMrow, 0,                       MML_ATT_COMMON                                                        },
    { Mml::MtextNode,      "mtext",      "MtextNode",      1,                            " TextNode ",            MML_ATT_COMMON " width height depth linebreak "                       },
    { Mml::MpaddedNode,    "mpadded",    "MpaddedNode",    MmlNodeSpec::ImplicitMrow, 0,                       MML_ATT_COMMON " width height depth lspace "                          },
    { Mml::MspaceNode,     "mspace",     "MspaceNode",     MmlNodeSpec::ImplicitMrow, 0,                       MML_ATT_COMMON " width height depth linebreak "                       },
    { Mml::TextNode,       0,            "TextNode",       MmlNodeSpec::ChildIgnore,  0,                       ""                                                                    },
    { Mml::UnknownNode,    0,            "UnknownNode",    MmlNodeSpec::ChildAny,     0,                       0                                                                     },
    { Mml::NoNode,         0,            0,                0,                            0,                       0                                                                     }
};

static const MmlNodeSpec *mmlFindNodeSpec( Mml::NodeType type )
{
    const MmlNodeSpec *spec = g_node_spec_data;
    for ( ; spec->type != Mml::NoNode; ++spec )
    {
        if ( type == spec->type ) return spec;
    }
    return 0;
}

static const MmlNodeSpec *mmlFindNodeSpec( const QString &tag )
{
    const MmlNodeSpec *spec = g_node_spec_data;
    for ( ; spec->type != Mml::NoNode; ++spec )
    {
        if ( tag == spec->tag ) return spec;
    }
    return 0;
}

#endif // MMLNODESPEC_H
