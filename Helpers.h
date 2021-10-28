#ifndef HELPERS_H
#define HELPERS_H

#include "Nodes/Mml.h"
#include "Nodes/MmlNode.h"
#include "MmlNodeSpec.h"
#include "MmlOperSpec.h"
#include "MmlEntityTable.h"
#include "Device.h"

#include <QDomDocument>

class Helpers
{

public:

    static double mmlQstringToQreal( const QString &string, bool *ok );

    static QString mmlInterpretListAttr(const QString &value_list, int idx, const QString &def );

    static Mml::FrameType mmlInterpretFrameType(const QString &value_list, int idx, bool *ok );

    static double mmlInterpretSpacing( QString value, double em, double ex, bool *ok );

    static Mml::FrameSpacing mmlInterpretFrameSpacing(const QString &value_list, qreal em, qreal ex, bool *ok );

    static double mmlInterpretPercentSpacing(QString value, double base, bool *ok );

    static double mmlInterpretPointSize( QString value, bool *ok );

    static const MmlNodeSpec *mmlFindNodeSpec( Mml::NodeType type );

    static const MmlNodeSpec *mmlFindNodeSpec( const QString &tag );

    static bool mmlCheckChildType( Mml::NodeType parent_type, Mml::NodeType child_type, QString *error_str );

    static bool mmlCheckAttributes( Mml::NodeType child_type, const MmlAttributeMap &attr, QString *error_str );

    static int attributeIndex( const QString &name );

    static QString mmlDictAttribute( const QString &name, const MmlOperSpec *spec );

    static int mmlInterpretMathVariant( const QString &value, bool *ok );

    static MmlAttributeMap collectFontAttributes( const MmlNode *node );

    static QFont mmlInterpretMathSize( const QString &value, QFont &fn, double em, double ex, bool *ok );

    static void updateFontAttr( MmlAttributeMap &font_attr, const MmlNode *n, const QString &name, const QString &preferred_name = QString() );

    static QFont mmlInterpretDepreciatedFontAttr(const MmlAttributeMap &font_attr, QFont &fn, double em, double ex );

    static Mml::NodeType domToMmlNodeType( const QDomNode &dom_node );

    static QString rectToStr( const QRectF &rect );

    static Mml::ColAlign mmlInterpretColAlign(const QString &value_list, int colnum, bool *ok );

    static Mml::RowAlign mmlInterpretRowAlign(const QString &value_list, int rownum, bool *ok );

    static Mml::FormType mmlInterpretForm( const QString &value, bool *ok );
};


#endif // HELPERS_H
