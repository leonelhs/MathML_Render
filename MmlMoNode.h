#ifndef MMLMONODE_H
#define MMLMONODE_H

#include "MmlTokenNode.h"

#include "Helpers.h"
#include "MmlDocument.h"
#include "MmlOperSpec.h"
#include "OperSpecSearchResult.h"

#include <QDebug>

class MmlMoNode : public MmlTokenNode
{
public:
    MmlMoNode( MmlDocument *document, const MmlAttributeMap &attribute_map );

    QString dictionaryAttribute( const QString &name ) const;
    virtual void stretch();
    virtual double lspace() const;
    virtual double rspace() const;

    virtual QString toStr() const;

protected:
    virtual void layoutSymbol();
    virtual QRectF symbolRect() const;

    virtual Mml::FormType form() const;

private:
    const MmlOperSpec *m_oper_spec;
    bool unaryMinus() const;
};

#endif//MMLMONODE_H
