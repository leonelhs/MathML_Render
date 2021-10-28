#ifndef OPERSPECSEARCHRESULT_H
#define OPERSPECSEARCHRESULT_H

#include "MmlOperSpec.h"
#include "Helpers.h"

#include <QStringList>

class OperSpecSearchResult
{
public:
    OperSpecSearchResult();

    ~OperSpecSearchResult();

    const MmlOperSpec *prefix_form,
          *infix_form,
          *postfix_form;

    const MmlOperSpec *&getForm( Mml::FormType form );

    bool haveForm( const Mml::FormType &form )
    {
        return getForm( form ) != 0;
    }
    void addForm( const MmlOperSpec *spec )
    {
        getForm( spec->form ) = spec;
    }

    static const MmlOperSpec *searchOperSpecData( const QString &name );

    static OperSpecSearchResult _mmlFindOperSpec( const QStringList &name_list, Mml::FormType form );

    static const MmlOperSpec *mmlFindOperSpec( const QString &text, Mml::FormType form );
};

#endif // OPERSPECSEARCHRESULT_H
