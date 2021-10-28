
#include "XmlNodes/MmlTextNode.h"
#include "MmlTokenNode.h"

QString MmlTokenNode::text() const
{
    QString result;

    const MmlNode *child = firstChild();
    for ( ; child != 0; child = child->nextSibling() )
    {
        if ( child->nodeType() != TextNode ) continue;
        if ( !result.isEmpty() )
            result += ' ';
        result += static_cast<const MmlTextNode *>( child )->text();
    }

    return result;
}
