//*******************************************************************
// Logarithm - done for computing logaritm
//
// parameters:
//             s - base of logarithm
//             a - argument of logarithm
//
//*******************************************************************
double Logarithm ( double s, double a )
    { return log ( a ) / log ( s ); }


//*******************************************************************
// FuzzyMult - done for computing result of fuzzy multiply
//
// parameters:
//             x - first number
//             y - second number
//
//*******************************************************************
double FuzzyMult ( double x, double y )

	{
     if ( SParameter == 1 ) return x*y;
     if ( SParameter == 0 ) return x < y ? x : y;
     return Logarithm ( SParameter,
	     1 + ( pow ( SParameter, x ) - 1 ) *
		 ( pow ( SParameter, y ) - 1 ) / ( SParameter -1 ));
    }


//*******************************************************************
// FuzzyDiv - done for computing result of fuzzy dividing
//
// parameters:
//             x - first number
//             y - second number
//
//*******************************************************************
double FuzzyDiv ( double x, double y )

	{
//     if ( SParameter == 1 ) return x*y;
//     if ( SParameter == 0 ) return x < y ? x : y;
     return Logarithm ( SParameter,
	     1 + ( pow ( SParameter, x ) - 1 ) *
		 ( SParameter - 1 ) / ( pow( SParameter, y ) - 1 ));
    }

//*******************************************************************
// FuzzySum - done for computing result of fuzzy addition
//
// parameters:
//             x - first number
//             y - second number
//
//*******************************************************************
double FuzzySum ( double x, double y )

	{
     if ( SParameter == 1 ) return x + y - x * y;
     if ( SParameter == 0 ) return x > y ? x : y;
     return ( x + y - Logarithm ( SParameter,
	     1 + ( pow ( SParameter, x ) - 1 ) *
		 ( pow ( SParameter, y ) - 1 ) / ( SParameter - 1 )));
    }


//*******************************************************************
// FuzzySub - done for computing result of fuzzy subtraction
//
// parameters:
//             x - first number
//             y - second number
//
//*******************************************************************
double FuzzySub ( double x, double y )

	{
//     if ( SParameter == 1 ) return x + y - x * y;
//     if ( SParameter == 0 ) return x > y ? x : y;
     return ( 1 - Logarithm ( SParameter,
	     1 + ( pow ( SParameter, 1 - x ) - 1 ) *
		 ( SParameter - 1 ) / ( pow ( SParameter, 1 - y ) - 1 )));
    }

