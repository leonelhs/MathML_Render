#ifndef MMLSPACINGNODE_CPP
#define MMLSPACINGNODE_CPP

#include "MmlSpacingNode.h"
#include "../Helpers.h"

double MmlSpacingNode::interpretSpacing( QString value, double base_value, bool *ok ) const
{
    if ( ok != 0 )
        *ok = false;

    value.replace( ' ', "" );

    QString sign, factor_str, pseudo_unit;
    bool percent = false;

    // extract the sign
    int idx = 0;
    if ( idx < value.length() && ( value.at( idx ) == '+' || value.at( idx ) == '-' ) )
        sign = value.at( idx++ );

    // extract the factor
    while ( idx < value.length() && ( value.at( idx ).isDigit() || value.at( idx ) == '.' ) )
        factor_str.append( value.at( idx++ ) );

    if ( factor_str == "" )
        factor_str = "1.0";

    // extract the % sign
    if ( idx < value.length() && value.at( idx ) == '%' )
    {
        percent = true;
        ++idx;
    }

    // extract the pseudo-unit
    pseudo_unit = value.mid( idx );

    bool qreal_ok;
    double factor = Helpers::mmlQstringToQreal( factor_str, &qreal_ok );
    if ( !qreal_ok || factor < 0.0 )
    {
        qWarning( "MmlSpacingNode::interpretSpacing(): could not parse \"%s\"", qPrintable( value ) );
        return 0.0;
    }

    if ( percent )
        factor *= 0.01;

    QRectF cr;
    if ( firstChild() == 0 )
        cr = QRectF( 0.0, 0.0, 0.0, 0.0 );
    else
        cr = firstChild()->myRect();

    double unit_size;

    if ( pseudo_unit.isEmpty() )
        unit_size = base_value;
    else if ( pseudo_unit == "width" )
        unit_size = cr.width();
    else if ( pseudo_unit == "height" )
        unit_size = -cr.top();
    else if ( pseudo_unit == "depth" )
        unit_size = cr.bottom();
    else
    {
        bool unit_ok;

        if (    pseudo_unit == "em" || pseudo_unit == "ex"
                || pseudo_unit == "cm" || pseudo_unit == "mm"
                || pseudo_unit == "in" || pseudo_unit == "px" )
            unit_size = MmlNode::interpretSpacing( "1" + pseudo_unit, &unit_ok );
        else
            unit_size = MmlNode::interpretSpacing( pseudo_unit, &unit_ok );

        if ( !unit_ok )
        {
            qWarning( "MmlSpacingNode::interpretSpacing(): could not parse \"%s\"", qPrintable( value ) );
            return 0.0;
        }
    }

    if ( ok != 0 )
        *ok = true;

    if ( sign.isNull() )
        return factor * unit_size;
    else if ( sign == "+" )
        return base_value + factor * unit_size;
    else // sign == "-"
        return base_value - factor * unit_size;
}

double MmlSpacingNode::width() const
{
    double child_width = 0.0;
    if ( firstChild() != 0 )
        child_width = firstChild()->myRect().width();

    QString value = explicitAttribute( "width" );
    if ( value.isNull() )
        return child_width;

    bool ok;
    double w = interpretSpacing( value, child_width, &ok );
    if ( ok )
        return w;

    return child_width;
}

double MmlSpacingNode::height() const
{
    QRectF cr;
    if ( firstChild() == 0 )
        cr = QRectF( 0.0, 0.0, 0.0, 0.0 );
    else
        cr = firstChild()->myRect();

    QString value = explicitAttribute( "height" );
    if ( value.isNull() )
        return -cr.top();

    bool ok;
    double h = interpretSpacing( value, -cr.top(), &ok );
    if ( ok )
        return h;

    return -cr.top();
}

double MmlSpacingNode::depth() const
{
    QRectF cr;
    if ( firstChild() == 0 )
        cr = QRectF( 0.0, 0.0, 0.0, 0.0 );
    else
        cr = firstChild()->myRect();

    QString value = explicitAttribute( "depth" );
    if ( value.isNull() )
        return cr.bottom();

    bool ok;
    double h = interpretSpacing( value, cr.bottom(), &ok );
    if ( ok )
        return h;

    return cr.bottom();
}

void MmlSpacingNode::layoutSymbol()
{
    if ( firstChild() == 0 )
        return;

    firstChild()->setRelOrigin( QPointF( 0.0, 0.0 ) );
}


QRectF MmlSpacingNode::symbolRect() const
{
    return QRectF( 0.0, -height(), width(), height() + depth() );
}

#endif//MMLSPACINGNODE_CPP
