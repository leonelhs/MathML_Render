#ifndef _MMLENTITYTABLE_H_
#define _MMLENTITYTABLE_H_

#include <qstring.h>


class MMLEntityTable
{
public:
    struct Spec
    {
        const char *name;
        const char *value;
    };

    QString entities() const;
    static const Spec *search( const QString &value, const Spec *from = 0 );
};

#endif





