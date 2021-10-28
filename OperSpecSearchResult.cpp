#include "OperSpecSearchResult.h"

OperSpecSearchResult::OperSpecSearchResult()
{
    prefix_form = infix_form = postfix_form = 0;
}

OperSpecSearchResult::~OperSpecSearchResult()
{

}

const MmlOperSpec *&OperSpecSearchResult::getForm( Mml::FormType form )
{
    switch ( form )
    {
    case Mml::PrefixForm:
        return prefix_form;
    case Mml::InfixForm:
        return infix_form;
    case Mml::PostfixForm:
        return postfix_form;
    }
    return postfix_form; // just to avoid warning
}

/*
    Searches g_oper_spec_data and returns any instance of operator name. There may
    be more instances, but since the list is sorted, they will be next to each other.
*/
const MmlOperSpec *OperSpecSearchResult::searchOperSpecData( const QString &name )
{
    // binary search
    // establish invariant g_oper_spec_data[begin].name < name < g_oper_spec_data[end].name

    int cmp = name.compare( g_oper_spec_data[0].name );
    if ( cmp < 0 )
        return 0;

    if ( cmp == 0 )
        return g_oper_spec_data;

    int begin = 0;
    int end = g_oper_spec_count;

    // invariant holds
    while ( end - begin > 1 )
    {
        int mid = 0.5 * ( begin + end );

        const MmlOperSpec *spec = g_oper_spec_data + mid;
        int cmp = name.compare( spec->name );
        if ( cmp < 0 )
            end = mid;
        else if ( cmp > 0 )
            begin = mid;
        else
            return spec;
    }

    return 0;
}

/*
    This searches g_oper_spec_data until at least one name in name_list is found with FormType form,
    or until name_list is exhausted. The idea here is that if we don't find the operator in the
    specified form, we still want to use some other available form of that operator.
*/
OperSpecSearchResult OperSpecSearchResult::_mmlFindOperSpec( const QStringList &name_list, Mml::FormType form )
{
    OperSpecSearchResult result;

    const MmlOperSpec *firstSpec = 0;

    QStringList::const_iterator it = name_list.begin();
    for ( ; it != name_list.end(); ++it )
    {
        const QString &name = *it;

        const MmlOperSpec *spec = searchOperSpecData( name );

        if ( spec == 0 )
            continue;

        // backtrack to the first instance of name
        while ( spec > g_oper_spec_data && ( spec - 1 )->name.compare( name ) == 0 )
            --spec;

        // Keep track of the first intance, if we haven't already done so

        if ( !firstSpec )
            firstSpec = spec;

        // iterate over instances of name until the instances are exhausted or until we
        // find an instance in the specified form.
        do
        {
            result.addForm( spec++ );
            if ( result.haveForm( form ) )
                break;
        }
        while ( spec->name.compare( name ) == 0 );

        if ( result.haveForm( form ) )
            break;
    }

    // Check whether we have found an instance in the specified form for one of
    // the different names in the given list. If not, and if there is more than
    // one name in the given list, then use our first instance, if any.

    if ( !result.haveForm( form ) && name_list.count() > 1 && firstSpec )
    {
        const QString &name = firstSpec->name;

        do
        {
            result.addForm( firstSpec++ );
            if ( result.haveForm( form ) )
                break;
        }
        while ( firstSpec->name.compare( name ) == 0 );
    }

    return result;
}

/*
text is a string between <mo> and </mo>. It can be a character ('+'), an
entity reference ("&infin;") or a character reference ("&#x0221E"). Our
job is to find an operator spec in the operator dictionary (g_oper_spec_data)
that matches text. Things are further complicated by the fact, that many
operators come in several forms (prefix, infix, postfix).

If available, this function returns an operator spec matching text in the specified
form. If such operator is not available, returns an operator spec that matches
text, but of some other form in the preference order specified by the MathML spec.
If that's not available either, returns the default operator spec.
*/

const MmlOperSpec *OperSpecSearchResult::mmlFindOperSpec( const QString &text, Mml::FormType form )
{
    QStringList name_list;
    name_list.append( text );

    // First, just try to find text in the operator dictionary.
    OperSpecSearchResult result = _mmlFindOperSpec( name_list, form );

    if ( !result.haveForm( form ) )
    {
        // Try to find other names for the operator represented by text.

        const MMLEntityTable::Spec *ent = 0;
        for ( ;; )
        {
            ent = MMLEntityTable::search( text, ent );
            if ( ent == 0 )
                break;
            name_list.append( '&' + QString( ent->name ) + ';' );
            ++ent;
        }

        result = _mmlFindOperSpec( name_list, form );
    }

    const MmlOperSpec *spec = result.getForm( form );
    if ( spec != 0 )
        return spec;

    spec = result.getForm( Mml::InfixForm );
    if ( spec != 0 )
        return spec;

    spec = result.getForm( Mml::PostfixForm );
    if ( spec != 0 )
        return spec;

    spec = result.getForm( Mml::PrefixForm );
    if ( spec != 0 )
        return spec;

    return &g_oper_spec_defaults;
}
