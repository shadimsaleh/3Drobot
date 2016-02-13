// stdafx.h : Includedatei f�r Standardsystem-Includedateien,
// oder projektspezifische Includedateien, die h�ufig benutzt, aber
// in unregelm��igen Abst�nden ge�ndert werden.
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Selten verwendete Teile der Windows-Header nicht einbinden
// Windows-Headerdateien:
#include <windows.h>
// C RunTime-Headerdateien
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <fstream> 
#include <iostream>

// TODO: Verweisen Sie hier auf zus�tzliche Header, die Ihr Programm erfordert
//#include <string.h>
#include <string>
#include <map>


using namespace std;
//OpenCv Headers
#include <opencv\cv.h> 
#include <opencv\cxcore.h> 
#include <opencv\highgui.h> 
#include <opencv\cvaux.h>
//headers for CommonDialog (File Dialog) and Header for commctrl (e.g.required for sliders)


#include "Commdlg.h"
#include <Commctrl.h> 
#include "File_Dialog_DEF.h"
#include "File_String_OP_DEF.h"
#include "OpenCv_Video_DEF.h"




