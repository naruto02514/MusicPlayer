/***************************************************************
 * Name:      WXMusicPlayerApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2018-07-17
 * Copyright:  ()
 * License:
 **************************************************************/

#include "WXMusicPlayerApp.h"

//(*AppHeaders
#include "WXMusicPlayerMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(WXMusicPlayerApp);

bool WXMusicPlayerApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	WXMusicPlayerFrame* Frame = new WXMusicPlayerFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
