#ifndef MMLTEXTNODE_H
#define MMLTEXTNODE_H

#include "MmlTokenNode.h"
#include "MmlDocument.h"

#include <QRegularExpression>

class MmlTextNode : public MmlNode
{
public:
    MmlTextNode( const QString &text, MmlDocument *document );

    virtual QString toStr() const;
    QString text() const
    {
        return m_text;
    }

    // TextNodes are not xml elements, so they can't have attributes of
    // their own. Everything is taken from the parent.
    virtual QFont font() const
    {
        return parent()->font();
    }
    virtual int scriptlevel( const MmlNode* = 0 ) const
    {
        return parent()->scriptlevel( this );
    }
    virtual QColor color() const
    {
        return parent()->color();
    }
    virtual QColor background() const
    {
        return parent()->background();
    }

protected:
    virtual void paintSymbol( QPainter *painter, double x_scaling, double y_scaling ) const;
    virtual QRectF symbolRect() const;

    QString m_text;

private:
    bool isInvisibleOperator() const;
};

#endif//MMLTEXTNODE_H
