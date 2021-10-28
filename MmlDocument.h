#ifndef MMLDOCUMENT_H
#define MMLDOCUMENT_H

#include "Nodes/Mml.h"
#include "Nodes/MmlNode.h"
#include "MmlFont.h"

#include <QDomDocument>
#include <QMap>
#include <QColor>
#include <QSizeF>
#include <QString>
#include <QRect>
#include <QPainter>

class MmlNode;

class MmlDocument : public Mml
{
public:
    MmlDocument();

    ~MmlDocument();

    void clear();

    bool setContent( const QString &text, QString *errorMsg = 0, int *errorLine = 0, int *errorColumn = 0 );

    void paint( QPainter *painter, const QPointF &pos );

    void dump() const;

    QSizeF size() const;

    void layout();

    QString fontName( MmlFont::Font type ) const;

    void setFontName( MmlFont::Font type, const QString &name );

    double baseFontPointSize() const
    {
        return m_base_font_point_size;
    }
    void setBaseFontPointSize( double size )
    {
        m_base_font_point_size = size;
    }

    QColor foregroundColor() const
    {
        return m_foreground_color;
    }
    void setForegroundColor( const QColor &color )
    {
        m_foreground_color = color;
    }

    QColor backgroundColor() const
    {
        return m_background_color;
    }
    void setBackgroundColor( const QColor &color )
    {
        m_background_color = color;
    }


private:
    void _dump( const MmlNode *node, const QString &indent ) const;

    bool insertChild( MmlNode *parent, MmlNode *new_node, QString *errorMsg );

    void insertOperator( MmlNode *node, const QString &text );

    MmlNode *domToMml( const QDomNode &dom_node, bool *ok, QString *errorMsg );

    MmlNode *createNode( Mml::NodeType type, const MmlAttributeMap &mml_attr, const QString &mml_value, QString *errorMsg );

    MmlNode *createImplicitMrowNode( const QDomNode &dom_node, bool *ok, QString *errorMsg );

    MmlNode *m_root_node;

    QString m_normal_font_name;
    QString m_fraktur_font_name;
    QString m_sans_serif_font_name;
    QString m_script_font_name;
    QString m_monospace_font_name;
    QString m_doublestruck_font_name;
    double m_base_font_point_size;
    QColor m_foreground_color;
    QColor m_background_color;

};
#endif // MMLDOCUMENT_H
