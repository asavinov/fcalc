
//*******************************************************************
//
// Borland C++ - (C) Copyright 1992 by Lobynsev Y., Loginov K.
//
// program - FCALC.C
// purpose - a Windows program to emulate a Fuzzy Calculator.
//
//*******************************************************************

#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include "fcalc.h"


// Data used in the program.

#define MAXDIGITS 9                             // defines a maxium of
												// digits in the buffer

char    DigBuffer[ MAXDIGITS + 5 ] = "0";       // a buffer of numbers

short 	nHscrollInc,
		nHscrollPos      = 0,
		BuffPointer      = 0;                   // a pointer to current
												// position in the buffer
double  FirstRegister    = 0,
		SecondRegister   = 0,
		SParameter       = 1;                   // a parameter using while
        										// computing an operation

char    CurrentOp        = ' ';                 // contains a current
												// operation

char  	szAppName[]      = "FCALC";             // an application name

BOOL    NewNumber        = FALSE,
		OperationIs      = FALSE,
        SecondRegisterIs = FALSE,
		PointIs          = FALSE;

HWND	hFcalc;                    				// hWnd of main window

FARPROC lpfnAboutDlgProc;                       // a long pointer to a
                                                // dialog procedure

HANDLE  hInst,                                  // hInstance of application
        hScroll;

                                                // pushbutton's bitmaps
HBITMAP    nbmp0,   nbmp1,   nbmp2,     nbmp3,   nbmp4,     nbmp5,
           nbmp6,   nbmp7,   nbmp8,     nbmp9,   nbmpClear, nbmpBack,
		   nbmpDiv, nbmpMul, nbmpPoint, nbmpSub, nbmpAdd,   nbmpEqu,
           pbmp0,   pbmp1,   pbmp2,     pbmp3,   pbmp4,     pbmp5,
           pbmp6,   pbmp7,   pbmp8,     pbmp9,   pbmpClear, pbmpBack,
		   pbmpDiv, pbmpMul, pbmpPoint, pbmpSub, pbmpAdd,   pbmpEqu;

HBITMAP    bmpOut;

HBITMAP    out0, out1, out2, out3, out4, out5,
           out6, out7, out8, out9, oute, outp, outm;

RECT   rcOut = {4, 4, 147, 20};


// Function prototypes.

long FAR PASCAL FcalcWndProc ( HWND, WORD, WORD, LONG );


//*******************************************************************
// WinMain - FCALC main
//
// paramaters:
//             hInstance     - The instance of this instance of this
//                             application.
//             hPrevInstance - The instance of the previous instance
//                             of this application. This will be 0
//                             if this is the first instance.
//             lpszCmdLine   - A long pointer to the command line that
//                             started this application.
//             cmdShow       - Indicates how the window is to be shown
//                             initially. ie. SW_SHOWNORMAL, SW_HIDE,
//                             SW_MIMIMIZE.
//
// returns:
//             wParam from last message.
//
//*******************************************************************
int PASCAL WinMain ( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )

	{
     MSG        msg;
     HMENU      hMenu;
     HBRUSH     hBrush;
     HBITMAP    hBitmap;
     WNDCLASS   wndclass;
     RECT       rc;

     // This is a global instance.
     hInst = hInstance;

     // Load bitmaps used in the program.

     bmpOut  = LoadBitmap ( hInstance, "OUT" );
     hBitmap = LoadBitmap ( hInstance, "BRUSH" );
     hBrush  = CreatePatternBrush ( hBitmap );

     out0 = LoadBitmap ( hInst, "OUT0" );
     out1 = LoadBitmap ( hInst, "OUT1" );
     out2 = LoadBitmap ( hInst, "OUT2" );
     out3 = LoadBitmap ( hInst, "OUT3" );
     out4 = LoadBitmap ( hInst, "OUT4" );
     out5 = LoadBitmap ( hInst, "OUT5" );
     out6 = LoadBitmap ( hInst, "OUT6" );
     out7 = LoadBitmap ( hInst, "OUT7" );
     out8 = LoadBitmap ( hInst, "OUT8" );
     out9 = LoadBitmap ( hInst, "OUT9" );
     oute = LoadBitmap ( hInst, "OUTE" );
     outm = LoadBitmap ( hInst, "OUTM" );
     outp = LoadBitmap ( hInst, "OUTP" );

     nbmp0     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_0 		 + BMP_NORMAL ));
     nbmp1     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_1 		 + BMP_NORMAL ));
     nbmp2     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_2 		 + BMP_NORMAL ));
     nbmp3     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_3 		 + BMP_NORMAL ));
     nbmp4     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_4 		 + BMP_NORMAL ));
     nbmp5     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_5 		 + BMP_NORMAL ));
     nbmp6     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_6 		 + BMP_NORMAL ));
     nbmp7     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_7 	     + BMP_NORMAL ));
     nbmp8     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_8 		 + BMP_NORMAL ));
     nbmp9     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_9         + BMP_NORMAL ));
     nbmpClear = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_CLEAR     + BMP_NORMAL ));
     nbmpBack  = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_BACKSPACE + BMP_NORMAL ));
     nbmpEqu   = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_EQUAL     + BMP_NORMAL ));
     nbmpMul   = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_MULTIPLY  + BMP_NORMAL ));
     nbmpDiv   = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_DEVIDE    + BMP_NORMAL ));
     nbmpPoint = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_POINT     + BMP_NORMAL ));
     nbmpSub   = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_MINUS     + BMP_NORMAL ));
     nbmpAdd   = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_SUM       + BMP_NORMAL ));

     pbmp0     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_0 		 + BMP_PRESSED ));
     pbmp1     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_1 		 + BMP_PRESSED ));
     pbmp2     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_2 		 + BMP_PRESSED ));
     pbmp3     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_3 		 + BMP_PRESSED ));
     pbmp4     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_4 		 + BMP_PRESSED ));
     pbmp5     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_5 		 + BMP_PRESSED ));
     pbmp6     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_6 		 + BMP_PRESSED ));
     pbmp7     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_7 	     + BMP_PRESSED ));
     pbmp8     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_8 		 + BMP_PRESSED ));
     pbmp9     = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_9         + BMP_PRESSED ));
     pbmpClear = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_CLEAR     + BMP_PRESSED ));
     pbmpBack  = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_BACKSPACE + BMP_PRESSED ));
     pbmpEqu   = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_EQUAL     + BMP_PRESSED ));
     pbmpMul   = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_MULTIPLY  + BMP_PRESSED ));
     pbmpDiv   = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_DEVIDE    + BMP_PRESSED ));
     pbmpPoint = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_POINT     + BMP_PRESSED ));
     pbmpSub   = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_MINUS     + BMP_PRESSED ));
     pbmpAdd   = LoadBitmap ( hInst, MAKEINTRESOURCE ( IDD_SUM       + BMP_PRESSED ));

     // If no previous instance, this is first.
     if ( ! hPrevInstance )
	     {

          // Define the window class for this application.
	 	  wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	   	  wndclass.lpfnWndProc   = FcalcWndProc;
     	  wndclass.cbClsExtra    = 0;
	      wndclass.cbWndExtra    = DLGWINDOWEXTRA;
	      wndclass.hInstance     = hInstance;
	      wndclass.hIcon         = LoadIcon ( hInstance, szAppName );
	      wndclass.hCursor       = LoadCursor ( hInstance, szAppName );
	      wndclass.hbrBackground = hBrush;
	      wndclass.lpszMenuName  = NULL;
	      wndclass.lpszClassName = szAppName;

          // Register the class.
     	  RegisterClass ( &wndclass ) ;
	     }

     // Create applications main window.
     hFcalc = CreateDialog ( hInstance, szAppName, 0, NULL );

     // Adjust main window and all controls size.
     GetWindowRect( hFcalc, &rc );
     MoveWindow ( hFcalc, rc.left, rc.top, 158, 186, 0 );

     MoveWindow( GetDlgItem(hFcalc,IDD_OUTPUT),      4,   4, 147, 18, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_SCROLL),      4,  26, 147, 15, 0);

     MoveWindow( GetDlgItem(hFcalc,IDD_CLEAR),       4,  46,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_BACKSPACE),  42,  46,  72, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_DEVIDE),    118,  46,  34, 21, 0);

     MoveWindow( GetDlgItem(hFcalc,IDD_7),           4,  70,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_8),          42,  70,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_9),          80,  70,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_MULTIPLY),  118,  70,  34, 21, 0);

     MoveWindow( GetDlgItem(hFcalc,IDD_4),           4,  94,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_5),          42,  94,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_6),          80,  94,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_MINUS),     118,  94,  34, 21, 0);

     MoveWindow( GetDlgItem(hFcalc,IDD_1),           4, 118,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_2),          42, 118, 34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_3),          80, 118,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_SUM),       118, 118,  34, 21, 0);

     MoveWindow( GetDlgItem(hFcalc,IDD_0),           4, 142,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_POINT),      42, 142,  34, 21, 0);
     MoveWindow( GetDlgItem(hFcalc,IDD_EQUAL),      80, 142,  72, 21, 0);


     // Add the system menu a new point "About..."
     hMenu  = GetSystemMenu ( hFcalc, FALSE );
     AppendMenu ( hMenu, MF_SEPARATOR, 0, NULL );
     AppendMenu ( hMenu, MF_STRING, IDM_ABOUT, "About..." );

     // Install the scroll range for SCROLLBAR.
     hScroll        = GetDlgItem ( hFcalc, IDD_SCROLL );
     SetScrollRange ( hScroll, SB_CTL, 0, 99, FALSE );
     SetScrollPos   ( hScroll, SB_CTL, nHscrollPos, TRUE );

     ShowWindow ( hFcalc , SW_SHOW );

     // Get and dispatch messages for this applicaton.
     while ( GetMessage ( &msg, NULL, 0, 0 ))
	     {
	      TranslateMessage ( &msg );
	      DispatchMessage ( &msg );
	     }

     DeleteObject ( hBrush );
     DeleteObject ( hBitmap );

     return msg.wParam ;
    }

//*******************************************************************
// AboutDlgProc - handles messages for ABOUTDIALOG
//
// paramaters:
//             hWnd          - The window handle for this message
//             message       - The message number
//             wParam        - The WORD parmater for this message
//             lParam        - The LONG parmater for this message
//
// returns:
//             depends on message.
//
//*******************************************************************
BOOL FAR PASCAL _export AboutDlgProc ( HWND hDlg, WORD message, WORD wParam, LONG lParam )

    {
     char 	szBuffer[5];                			// a buffer for SParameter

     switch (message)
         {
          case WM_INITDIALOG:

              // Set limits of the input text.
              SendMessage    ( GetDlgItem ( hDlg, IDD_SPARAM ), EM_LIMITTEXT, 4, 0 );

              // Send SParameter to a edit window.
              SetDlgItemText ( hDlg, IDD_SPARAM, gcvt ( SParameter, 4 , szBuffer ));
              return TRUE;

          case WM_COMMAND:
              switch (wParam)
                  {

                   // Button "OK" was pushed.
	               case IDOK:

                       // Obtain a new SParameter.
                       GetDlgItemText ( hDlg, IDD_SPARAM, szBuffer, 4 );
                       if (( szBuffer[0] == '0' ) & ( szBuffer[1] == 0 ))
	    		           SParameter = 0;
                       else
                           if (( SParameter = atof ( szBuffer )) <= 0 )                            {
                                MessageBox ( hDlg, "Incorrect S-parameter!",
					    		             "Fuzzy Calculator", MB_OK |
						    	        	 MB_ICONEXCLAMATION );
                                SetFocus ( GetDlgItem ( hDlg, IDD_SPARAM ));
                                return TRUE;
                               }

                       EndDialog ( hDlg, 0 );
			           return TRUE;

                       // Button "Cancel" was pushed.
    		           case IDCANCEL:
                           EndDialog ( hDlg, 0 );
		    	           return TRUE;
                    }
              break;
         }
     return FALSE;
    }

//*******************************************************************
// Reset - done for reset the buffer of numbers
//
// parameters:
//             none ( it ueses global variables )
// returns:
//             none
//
//*******************************************************************
void Reset ( void )

	{
     BuffPointer 			  = 0;
     DigBuffer[ BuffPointer ] = 0;
     nHscrollPos			  = 0;
     NewNumber 				  = FALSE;
     PointIs 				  = FALSE;
     SetScrollPos ( hScroll, SB_CTL, nHscrollPos, TRUE );
    }


//*******************************************************************
// ResetAll - done for reset all the variables
//
// parameters:
//             none ( it ueses global variables )
// returns:
//             none
//
//*******************************************************************
void ResetAll ( void )

	{
     Reset ();
     FirstRegister    = 0;
     SecondRegister   = 0;
     CurrentOp        = ' ';
     OperationIs      = FALSE;
     SecondRegisterIs = FALSE;
	 lstrcpy ( DigBuffer, "0" );
    }


//*******************************************************************
// AddDigitBuffer - done for adding a new digit to
//                  the buffer of numbers
//
// parameters:
//             CharDigit - contains a ASCII code of digit
// returns:
//             none
//
//*******************************************************************
void AddDigitBuffer ( char CharDigit )

    {

     // Reset the buffer if a new number should be.
     if ( NewNumber ) Reset ();

     // Set the flags of the operation and the second register.
     if ( OperationIs ) { SecondRegisterIs = TRUE; OperationIs = FALSE; }

     // Add the digit to the buffer.
     if ( BuffPointer < MAXDIGITS )
	     {
	      DigBuffer[ BuffPointer++ ] = CharDigit;
	      DigBuffer[ BuffPointer ]   = 0;
	     }
    }


//*******************************************************************
// AddDigitBuffer - done for adding a new digit to
//                  the buffer of numbers
//
// parameters:
//             none ( it ueses global variables )
// returns:
//             none
//
//*******************************************************************
void SetPoint ( void )

	{
     if ( NewNumber ) Reset ();
     if ( OperationIs ) { SecondRegisterIs = TRUE; OperationIs = FALSE; }
     if (( BuffPointer < MAXDIGITS ) & ( !PointIs ))
	     {
          if ( BuffPointer == 0 ) DigBuffer[ BuffPointer++ ] = '0';
	      DigBuffer[ BuffPointer++ ] = '.';
	      DigBuffer[ BuffPointer ]    = 0;
          PointIs                     = TRUE;
	     }
	}


//*******************************************************************
// BackSpace - done for deleting the last digit in
//                  the buffer of number
//
// parameters:
//             none ( it ueses global variables )
// returns:
//             none
//
//*******************************************************************
void BackSpace ( void )

	{
     if (( !OperationIs ) & ( BuffPointer > 0 ))
	     {
	      BuffPointer -- ;
          if ( DigBuffer[ BuffPointer ] == '.' ) PointIs = FALSE;
	      DigBuffer[ BuffPointer ] = 0;
	     }
     if ( BuffPointer == 0 ) lstrcpy ( DigBuffer, "0" );
    }


//*******************************************************************
// Logarithm - done for computing logaritm
//
// parameters:
//             s - base of logarithm
//             a - argument of logarithm
// returns:
//             a result of logarithm
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
// returns:
//             a result of fuzzy multiply
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
// returns:
//             a result of fuzzy dividing
//
//*******************************************************************
double FuzzyDiv ( double x, double y )

	{
     if ( SParameter == 1 ) return x/y;
     if ( SParameter == 0 ) return x;
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
// returns:
//             a result of fuzzy addition
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
// returns:
//             a result of fuzzy subtraction
//
//*******************************************************************
double FuzzySub ( double x, double y )

	{
     if ( SParameter == 1 ) return ( x - y ) / ( 1 - y );
     if ( SParameter == 0 ) return x;
     return ( 1 - Logarithm ( SParameter,
	     1 + ( pow ( SParameter, 1 - x ) - 1 ) *
		 ( SParameter - 1 ) / ( pow ( SParameter, 1 - y ) - 1 )));
    }


//*******************************************************************
// ControlOp - done for controlling on the operands
//
// parameters:
//             none ( it ueses global variables )
// returns:
//             TRUE  - if controlling was success
//             FALSE - if it was not
//
//*******************************************************************
BOOL ControlOp ( void )

	{
     char szFCALC[] = "Fuzzy Calculator";

	 if ((( FirstRegister  < 0 ) | ( FirstRegister  > 1 )) |
         (( SecondRegister < 0 ) | ( SecondRegister > 1 )))
         {
          MessageBox ( hFcalc, "Fuzzy numbers must be within [0,1]", szFCALC, MB_OK | MB_ICONEXCLAMATION );
          return FALSE;
         }
     else
         if (( CurrentOp == '/' ) & ( SecondRegister == 0 ))
    		 {
              MessageBox ( hFcalc, "The second operand must not be 0!", szFCALC, MB_OK | MB_ICONEXCLAMATION );
              return FALSE;
             }
	     else
	         if (( CurrentOp == '/' ) & ( FirstRegister > SecondRegister ))
		         {
                  MessageBox ( hFcalc, "The first operand must be less \nthan or equal to the second!", szFCALC, MB_OK | MB_ICONEXCLAMATION );
                  return FALSE;
                 }
             else
                 if (( CurrentOp == '-' ) & ( FirstRegister < SecondRegister ))
                     {
                      MessageBox ( hFcalc, "The first operand must be gereater \nthan or equal to the second!", szFCALC, MB_OK | MB_ICONEXCLAMATION );
                      return FALSE;
                     }
                 else return TRUE;
	}


//*******************************************************************
// CalcIt - done for computing the result of current operation
//
// parameters:
//             none ( it ueses global variables )
// returns:
//             none
//
//*******************************************************************
void CalcIt ( void )

    {
	 if ( SecondRegisterIs ) SecondRegister = atof ( DigBuffer );
     if ( !ControlOp ()) return;
     switch ( CurrentOp )
         {
          case '+': FirstRegister = FuzzySum ( FirstRegister, SecondRegister ); break;
          case '-': FirstRegister = FuzzySub ( FirstRegister, SecondRegister ); break;
          case '*': FirstRegister = FuzzyMult ( FirstRegister, SecondRegister ); break;
          case '/': FirstRegister = FuzzyDiv ( FirstRegister, SecondRegister ); break;
          case ' ': FirstRegister = atof ( DigBuffer ); break;
         }
     gcvt ( FirstRegister, MAXDIGITS , DigBuffer );
     BuffPointer      = lstrlen ( DigBuffer );
     NewNumber        = TRUE;
     OperationIs      = FALSE;
     SecondRegisterIs = FALSE;
    }


//*******************************************************************
// SetOperation - done for setting current operation
//
// parameters:
//             none ( it ueses global variables )
// returns:
//             none
//
//*******************************************************************
void SetOperation ( char Operation )

	{
     if ( SecondRegisterIs ) CalcIt ();
     FirstRegister  = atof ( DigBuffer );
     SecondRegister = FirstRegister;
     if ( !ControlOp ()) return;
     CurrentOp      = Operation;
     NewNumber      = TRUE;
     OperationIs    = TRUE;
    }


//*******************************************************************
// SetDigScroll - done for adding the current position of
//                SCROLLBAR into the buffer of numbers
//
// parameters:
//             none ( it ueses global variables )
// returns:
//             none
//
//*******************************************************************
void SetDigScroll ( void )

    {
     char   szHscrollPos[3], szHstring[3] = "0";
     double	dHscrollPos;
     int    iCount = 0;

     // Get the previous meaning of the buffer of numbers.
     if ((( !PointIs ) & ( lstrlen ( DigBuffer ) <=  MAXDIGITS - 3 )) |
         (( PointIs )  & ( lstrlen ( DigBuffer ) <=  MAXDIGITS )))
     	 {
	      dHscrollPos = ( double ) nHscrollPos;
    	  gcvt ( dHscrollPos, 2, szHscrollPos );
	   	  if ( lstrlen ( szHscrollPos ) == 1 )
             {
	          lstrcat ( szHstring, szHscrollPos );
              lstrcpy ( szHscrollPos, szHstring );
             }

          // Adding the current position of SCROLLBAR.
	      if ( NewNumber ) Reset ();
    	  if ( OperationIs ) { SecondRegisterIs = TRUE; OperationIs = FALSE; }
	      if ( BuffPointer == 0 )
	          {
		       DigBuffer[ BuffPointer++ ] = '0';
	           DigBuffer[ BuffPointer ]   = 0;
	          }
	      if ( PointIs )
	          {
	           while ( DigBuffer[ iCount ] != '.' ) iCount++; iCount++;
	           DigBuffer[ iCount++ ] = szHscrollPos[0];
	           DigBuffer[ iCount ]   = szHscrollPos[1];
	          }
	      else
    	      {
	           lstrcat     ( DigBuffer, "." );
		       lstrcat     ( DigBuffer, szHscrollPos );
		       BuffPointer = lstrlen ( DigBuffer );
    	       PointIs     = TRUE;
			  }
	     }
    }


//*******************************************************************
// SetScrollBar - done for setting accordance bitween position of
//                SCROLLBAR and the buffer of numbers
//
// parameters:
//             none ( it ueses global variables )
// returns:
//             none
//
//*******************************************************************
void SetScrollBar ( void )
    {
     char   szHscrollPos[3];
     int    iCount = 0;

     while ( DigBuffer[ iCount ] != '.' ) iCount++; iCount++;
     szHscrollPos[ 0 ] = DigBuffer[ iCount++ ];
     szHscrollPos[ 1 ] = DigBuffer[ iCount ];
     szHscrollPos[ 2 ] = 0;
     nHscrollPos       = atoi ( szHscrollPos );
     if ( DigBuffer[ iCount ] == 0 ) nHscrollPos *= 10;
	 SetScrollPos      ( hScroll, SB_CTL, nHscrollPos, TRUE );
    }


//*******************************************************************
// PushButton - done for immitating of pressing a button
//
// parameters:
//             hWnd - hWnd of main window
//			   IDD  - ID of the button to be pushed
// returns:
//             none
//
//*******************************************************************
void PushButton ( HWND hWnd, int IDD )

	{
     HWND hButton;

     hButton = GetDlgItem ( hWnd, IDD );
     SendMessage ( hButton, BM_SETSTATE, 1, 0 );
     SendMessage ( hButton, BM_SETSTATE, 0, 0 );
	 SendMessage ( hWnd, WM_COMMAND, IDD, 0 );
    }


//*******************************************************************
// DrawButton - done for drawing a button
//
// parameters:
//             hWnd - hWnd of main window
//			   IDD  - ID of the button to be pushed
// returns:
//             none
//
//*******************************************************************
void DrawButton ( LPDRAWITEMSTRUCT pDrawStruct )
	{
    HDC hdcCompat;
    HBITMAP hBmp;

    if ( pDrawStruct->CtlType != ODT_BUTTON )
        {
        return;
        }

    if ( pDrawStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT ))
        {
         if ( !(pDrawStruct->itemState & ODS_SELECTED ))

		     // Draw in normal state.
             switch ( pDrawStruct->CtlID )
                 {
                  case IDD_0         : hBmp = nbmp0;     break;
                  case IDD_1         : hBmp = nbmp1;     break;
                  case IDD_2         : hBmp = nbmp2;     break;
                  case IDD_3         : hBmp = nbmp3;     break;
                  case IDD_4         : hBmp = nbmp4;     break;
                  case IDD_5         : hBmp = nbmp5;     break;
                  case IDD_6         : hBmp = nbmp6;     break;
                  case IDD_7         : hBmp = nbmp7;     break;
                  case IDD_8         : hBmp = nbmp8;     break;
                  case IDD_9         : hBmp = nbmp9;     break;
                  case IDD_CLEAR     : hBmp = nbmpClear; break;
                  case IDD_BACKSPACE : hBmp = nbmpBack;  break;
                  case IDD_DEVIDE    : hBmp = nbmpDiv;   break;
                  case IDD_SUM       : hBmp = nbmpAdd;   break;
                  case IDD_MULTIPLY  : hBmp = nbmpMul;   break;
                  case IDD_MINUS     : hBmp = nbmpSub;   break;
                  case IDD_POINT     : hBmp = nbmpPoint; break;
                  case IDD_EQUAL     : hBmp = nbmpEqu;   break;
                 }
         else

		     // Draw in pressed state.
             switch ( pDrawStruct->CtlID )
                 {
                  case IDD_0         : hBmp = pbmp0;     break;
                  case IDD_1         : hBmp = pbmp1;     break;
                  case IDD_2         : hBmp = pbmp2;     break;
                  case IDD_3         : hBmp = pbmp3;     break;
                  case IDD_4         : hBmp = pbmp4;     break;
                  case IDD_5         : hBmp = pbmp5;     break;
                  case IDD_6         : hBmp = pbmp6;     break;
                  case IDD_7         : hBmp = pbmp7;     break;
                  case IDD_8         : hBmp = pbmp8;     break;
                  case IDD_9         : hBmp = pbmp9;     break;
                  case IDD_CLEAR     : hBmp = pbmpClear; break;
                  case IDD_BACKSPACE : hBmp = pbmpBack;  break;
                  case IDD_DEVIDE    : hBmp = pbmpDiv;   break;
                  case IDD_SUM       : hBmp = pbmpAdd;   break;
                  case IDD_MULTIPLY  : hBmp = pbmpMul;   break;
                  case IDD_MINUS     : hBmp = pbmpSub;   break;
                  case IDD_POINT     : hBmp = pbmpPoint; break;
                  case IDD_EQUAL     : hBmp = pbmpEqu;   break;
                 }
         hdcCompat = CreateCompatibleDC ( pDrawStruct->hDC );
         SelectObject ( hdcCompat, hBmp );
         BitBlt ( pDrawStruct->hDC,
                  pDrawStruct->rcItem.left, pDrawStruct->rcItem.top,
                  100, 100, hdcCompat, 0, 0, SRCCOPY );
         DeleteDC ( hdcCompat );
        }
    }


//*******************************************************************
// Paint -
//
// paramaters:
//
//
// returns:
//
//
//*******************************************************************
void Paint ( void )
    {
     HBITMAP hBmp;
     HDC hdcCompat;
     PAINTSTRUCT ps;
     HDC hdc;
     int i;

     hdc = BeginPaint ( hFcalc, &ps );

     hdcCompat = CreateCompatibleDC ( hdc );
     SelectObject ( hdcCompat, bmpOut );
     BitBlt ( hdc, 4, 4, 147, 18, hdcCompat, 0, 0, SRCCOPY );

     if ( BuffPointer == 0 )
	     {
          hBmp = out0;
          SelectObject ( hdcCompat, hBmp );
          BitBlt ( hdc, 137, 7, 10, 13, hdcCompat, 0, 0, SRCCOPY );
		 }
     else for ( i=0; i<BuffPointer; i++ )
        {
         switch ( DigBuffer[BuffPointer-i-1] )
             {
              case '0' : hBmp = out0; break;
              case '1' : hBmp = out1; break;
              case '2' : hBmp = out2; break;
              case '3' : hBmp = out3; break;
              case '4' : hBmp = out4; break;
              case '5' : hBmp = out5; break;
              case '6' : hBmp = out6; break;
              case '7' : hBmp = out7; break;
              case '8' : hBmp = out8; break;
              case '9' : hBmp = out9; break;
              case '.' : hBmp = outp; break;
              case '-' : hBmp = outm; break;
              case 'e' : hBmp = oute; break;
             }
          SelectObject ( hdcCompat, hBmp );
          BitBlt ( hdc, 137 - 10*i, 7, 10, 13, hdcCompat, 0, 0, SRCCOPY );
         }

     DeleteDC ( hdcCompat );
     EndPaint ( hFcalc, &ps );
    }


//*******************************************************************
// FcalcWndProc - handles messages for this application
//
// paramaters:
//             hWnd          - The window handle for this message
//             message       - The message number
//             wParam        - The WORD parmater for this message
//             lParam        - The LONG parmater for this message
//
// returns:
//             depends on message.
//
//*******************************************************************
long FAR PASCAL _export FcalcWndProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)

    {
     char   szBuffer[ MAXDIGITS + 5 ];          		// a helpful buffer


     switch ( message )
	     {
          case WM_PAINT:
              Paint ();
              return 0;

          case WM_SYSCOMMAND:

               // Create ABOUTDIALOG.
               if ( wParam == IDM_ABOUT )
                   {
                    lpfnAboutDlgProc = MakeProcInstance ( AboutDlgProc, hInst );
                    DialogBox ( hInst, "ABOUTDIALOG", hwnd, lpfnAboutDlgProc );
                    return 0;
                   }
               break;

          case WM_HSCROLL:

               // Service messages of SCROLLBAR.
               switch (wParam)
                   {
                    case SB_LINEUP  : nHscrollInc =  -1; break;
                    case SB_LINEDOWN: nHscrollInc =   1; break;
                    case SB_PAGEUP  : nHscrollInc = -10; break;
                    case SB_PAGEDOWN: nHscrollInc =  10; break;
                    case SB_THUMBPOSITION: nHscrollInc = LOWORD (lParam) - nHscrollPos; break;
                    default: nHscrollInc = 0;
                   }
               if ( nHscrollInc = max ( -nHscrollPos, min ( nHscrollInc, 99 - nHscrollPos )))
                   {
                    nHscrollPos    += nHscrollInc ;
                    SetScrollPos   ( hScroll, SB_CTL, nHscrollPos, TRUE ) ;
                    SetDigScroll   ();
  				    lstrcpy        ( szBuffer, DigBuffer );
			        lstrcat        ( szBuffer, " " );
                    InvalidateRect ( hwnd, &rcOut, FALSE );
                   }
               return 0;

          case WM_DRAWITEM:
              DrawButton ((LPDRAWITEMSTRUCT) lParam );
			  return 0;

	      case WM_COMMAND:

              // Service the messages of callculator's buttons.
              SetFocus ( hFcalc );
	          switch ( wParam )
		          {
		           case IDD_1		 : AddDigitBuffer ('1'); break;
		 		   case IDD_2		 : AddDigitBuffer ('2'); break;
		 		   case IDD_3		 : AddDigitBuffer ('3'); break;
		 		   case IDD_4		 : AddDigitBuffer ('4'); break;
		 	 	   case IDD_5		 : AddDigitBuffer ('5'); break;
		           case IDD_6		 : AddDigitBuffer ('6'); break;
		           case IDD_7		 : AddDigitBuffer ('7'); break;
		           case IDD_8		 : AddDigitBuffer ('8'); break;
		           case IDD_9		 : AddDigitBuffer ('9'); break;
		           case IDD_0		 : AddDigitBuffer ('0'); break;
		           case IDD_CLEAR    : ResetAll ();          break;
		           case IDD_BACKSPACE: BackSpace ();         break;
                   case IDD_SUM      : SetOperation ('+');   break;
                   case IDD_MINUS    : SetOperation ('-');   break;
                   case IDD_DEVIDE   : SetOperation ('/');   break;
                   case IDD_MULTIPLY : SetOperation ('*');   break;
                   case IDD_EQUAL    : CalcIt ();            break;
                   case IDD_POINT    : SetPoint ();          break;
		          }

              // Reflect the current number on the buffer.
              if ( PointIs ) SetScrollBar ();
  			  lstrcpy        ( szBuffer, DigBuffer );
		      lstrcat        ( szBuffer, " " );
              InvalidateRect ( hwnd, &rcOut, FALSE );
	          return 0;

	      case WM_KEYDOWN:

              // Service the messages of callculator's buttons.
              switch ( wParam )
                  {
                   case VK_BACK  : PushButton ( hwnd, IDD_BACKSPACE ); break;
                   case VK_RETURN: PushButton ( hwnd, IDD_EQUAL );     break;
                   case VK_SPACE : PushButton ( hwnd, IDD_CLEAR );     break;
                   case VK_PRIOR : SendMessage ( hwnd, WM_HSCROLL, SB_PAGEDOWN, 0 ); break;
                   case VK_NEXT  : SendMessage ( hwnd, WM_HSCROLL, SB_PAGEUP, 0 ); break;
                   case VK_RIGHT : SendMessage ( hwnd, WM_HSCROLL, SB_LINEDOWN, 0 ); break;
                   case VK_LEFT  : SendMessage ( hwnd, WM_HSCROLL, SB_LINEUP, 0 ); break;
                  }
              return 0;

	      case WM_CHAR:

              // Immitate of pressing the proper buttons.
              switch ( wParam )
                  {
                   case '1': PushButton ( hwnd, IDD_1 ); 		break;
                   case '2': PushButton ( hwnd, IDD_2 ); 		break;
                   case '3': PushButton ( hwnd, IDD_3 ); 		break;
                   case '4': PushButton ( hwnd, IDD_4 ); 		break;
                   case '5': PushButton ( hwnd, IDD_5 ); 		break;
                   case '6': PushButton ( hwnd, IDD_6 ); 		break;
                   case '7': PushButton ( hwnd, IDD_7 ); 		break;
                   case '8': PushButton ( hwnd, IDD_8 ); 		break;
                   case '9': PushButton ( hwnd, IDD_9 );		break;
                   case '0': PushButton ( hwnd, IDD_0 ); 		break;
                   case '+': PushButton ( hwnd, IDD_SUM );      break;
                   case '*': PushButton ( hwnd, IDD_MULTIPLY ); break;
                   case '-': PushButton ( hwnd, IDD_MINUS );    break;
                   case '/': PushButton ( hwnd, IDD_DEVIDE );   break;
                   case '.': PushButton ( hwnd, IDD_POINT );    break;
                  }
	          return 0;

          // Destroy window.
	      case WM_DESTROY:
	          PostQuitMessage ( 0 );
	          return 0;
	     }
     return DefWindowProc( hwnd, message, wParam, lParam );
    }
