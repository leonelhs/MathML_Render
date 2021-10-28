#ifndef MMLNODE_H
#define MMLNODE_H

#include "Mml.h"
#include "../MmlDocument.h"

#include <QRect>
#include <QRectF>
#include <QMap>
#include <QString>
#include <QPointF>
#include <QFont>
#include <QColor>
#include <QPainter>

class MmlDocument;

class MmlNode : public Mml
{
    friend class MmlDocument;

public:

    MmlNode( Mml::NodeType type, MmlDocument *document, const MmlAttributeMap &attribute_map );

    virtual ~MmlNode();

    // Mml stuff
    Mml::NodeType nodeType() const
    {
        return m_node_type;
    }

    virtual QString toStr() const;

    void setRelOrigin( const QPointF &rel_origin );

    void stretchTo( const QRectF &rect );
    QPointF devicePoint( const QPointF &p ) const;

    QRectF myRect() const
    {
        return m_my_rect;
    }
    virtual void setMyRect( const QRectF &rect )
    {
        m_my_rect = rect;
    }
    void updateMyRect();

    QRectF parentRect() const;
    virtual QRectF deviceRect() const;

    virtual void stretch();
    virtual void layout();
    virtual void paint( QPainter *painter, qreal x_scaling, qreal y_scaling );

    qreal basePos() const;

    qreal em() const;
    qreal ex() const;

    QString explicitAttribute( const QString &name, const QString &def = QString() ) const;
    QString inheritAttributeFromMrow( const QString &name, const QString &def = QString() ) const;

    virtual QFont font() const;
    virtual QColor color() const;
    virtual QColor background() const;
    virtual int scriptlevel( const MmlNode *child = 0 ) const;


    // Node stuff
    MmlNode *parent() const
    {
        return m_parent;
    }
    MmlNode *firstChild() const
    {
        return m_first_child;
    }
    MmlNode *nextSibling() const
    {
        return m_next_sibling;
    }
    MmlNode *previousSibling() const
    {
        return m_previous_sibling;
    }
    MmlNode *lastSibling() const;
    MmlNode *firstSibling() const;
    bool isLastSibling() const
    {
        return m_next_sibling == 0;
    }
    bool isFirstSibling() const
    {
        return m_previous_sibling == 0;
    }
    bool hasChildNodes() const
    {
        return m_first_child != 0;
    }

    //protected:
    virtual void layoutSymbol();
    virtual void paintSymbol( QPainter *painter, qreal, qreal ) const;
    virtual QRectF symbolRect() const
    {
        return QRectF( 0.0, 0.0, 0.0, 0.0 );
    }

    MmlNode *parentWithExplicitAttribute( const QString &name, NodeType type = NoNode );
    qreal interpretSpacing( const QString &value, bool *ok ) const;

    qreal lineWidth() const;

    //private:
    MmlAttributeMap m_attribute_map;
    bool m_stretched;
    QRectF m_my_rect, m_parent_rect;
    QPointF m_rel_origin;

    Mml::NodeType m_node_type;
    MmlDocument *m_document;

    MmlNode *m_parent,
            *m_first_child,
            *m_next_sibling,
            *m_previous_sibling;
};

#endif // MMLNODE_H
