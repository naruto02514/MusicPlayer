/***************************************************************
 * Name:      WXMusicPlayerMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2018-07-17
 * Copyright:  ()
 * License:
 **************************************************************/

#include "WXMusicPlayerMain.h"
#include <wx/msgdlg.h>
#include <wx/file.h>
#include "Player.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <wx/ctrlsub.h>
#include <wx/listbox.h>
#include "Memory.h"
#include <wx/mstream.h>
#include "json.hpp"
#include <fstream>
#include <wx/wx.h>
#include <wx/sizer.h>

int shrink_w=280,shrink_h=160,shrink=0;
int form_w = 280,form_h = 420;

using json = nlohmann::json;

//Player player;
//(*InternalHeaders(WXMusicPlayerFrame)
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)


//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(WXMusicPlayerFrame)
const long WXMusicPlayerFrame::ID_BUTTON2 = wxNewId();
const long WXMusicPlayerFrame::ID_LISTBOX1 = wxNewId();
const long WXMusicPlayerFrame::ID_STATICTEXT1 = wxNewId();
const long WXMusicPlayerFrame::ID_STATICTEXT2 = wxNewId();
const long WXMusicPlayerFrame::ID_STATICBITMAP1 = wxNewId();
const long WXMusicPlayerFrame::ID_SLIDER1 = wxNewId();
const long WXMusicPlayerFrame::ID_BUTTON1 = wxNewId();
const long WXMusicPlayerFrame::ID_BUTTON3 = wxNewId();
const long WXMusicPlayerFrame::ID_BITMAPBUTTON1 = wxNewId();
const long WXMusicPlayerFrame::ID_BITMAPBUTTON2 = wxNewId();
const long WXMusicPlayerFrame::ID_BITMAPBUTTON3 = wxNewId();
const long WXMusicPlayerFrame::ID_BITMAPBUTTON4 = wxNewId();
const long WXMusicPlayerFrame::ID_CHECKBOX1 = wxNewId();
const long WXMusicPlayerFrame::ID_CHECKBOX2 = wxNewId();
const long WXMusicPlayerFrame::ID_BITMAPBUTTON5 = wxNewId();
const long WXMusicPlayerFrame::ID_BITMAPBUTTON6 = wxNewId();
const long WXMusicPlayerFrame::ID_SLIDER2 = wxNewId();
const long WXMusicPlayerFrame::ID_BITMAPBUTTON7 = wxNewId();
const long WXMusicPlayerFrame::ID_TIMER1 = wxNewId();
const long WXMusicPlayerFrame::ID_TIMER2 = wxNewId();
const long WXMusicPlayerFrame::ID_TIMER3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(WXMusicPlayerFrame,wxFrame)
    //(*EventTable(WXMusicPlayerFrame)
       //-----for frame with mouse move------//
       EVT_LEFT_DOWN(WXMusicPlayerFrame::OnLeftDown)
       EVT_LEFT_UP(WXMusicPlayerFrame::OnLeftUp)
       EVT_MOTION(WXMusicPlayerFrame::OnMouseMove)
    //*)
END_EVENT_TABLE()

void WXMusicPlayerFrame::OnLeftDown(wxMouseEvent& evt)
{
    CaptureMouse();
    wxPoint pos = ClientToScreen(evt.GetPosition());
    wxPoint origin = GetPosition();
    int dx =  pos.x - origin.x;
    int dy = pos.y - origin.y;
    m_delta = wxPoint(dx, dy);
}

void WXMusicPlayerFrame::OnLeftUp(wxMouseEvent& WXUNUSED(evt))
{
    if (HasCapture())
        ReleaseMouse();
}

void WXMusicPlayerFrame::OnMouseMove(wxMouseEvent& evt)
{
    wxPoint pt = evt.GetPosition();
    if (evt.Dragging() && evt.LeftIsDown())
    {
        wxPoint pos = ClientToScreen(pt);
        Move(wxPoint(pos.x - m_delta.x, pos.y - m_delta.y));
    }
}

WXMusicPlayerFrame::WXMusicPlayerFrame(wxWindow* parent,wxWindowID id)
{
    int form_w = 280;
    int form_h = 420;
    std::string form_c = "#ffffff";

    int art_w = 70;
    int art_h = 70;
    int art_x = 10;
    int art_y = 10;
    std::string art_c = "#ffffff";

    int play_w = 30;
    int play_h = 30;
    int play_x = 50;
    int play_y = 120;
    std::string play_i;

    int stop_w = 30;
    int stop_h = 30;
    int stop_x = 90;
    int stop_y = 120;
    std::string stop_i;

    int prev_w = 30;
    int prev_h = 30;
    int prev_x = 10;
    int prev_y = 120;
    std::string prev_i;

    int next_w = 30;
    int next_h = 30;
    int next_x = 130;
    int next_y = 120;
    std::string next_i;

    int play_list_w = 260;
    int play_list_h = 215;
    int play_list_x = 10;
    int play_list_y = 160;
    std::string play_list_c = "#ffffff";

    int add_w = 40;
    int add_h = 30;
    int add_x = 10;
    int add_y = 380;

    int delete_w = 40;
    int delete_h = 30;
    int delete_x = 50;
    int delete_y = 380;

    int clear_w = 40;
    int clear_h = 30;
    int clear_x = 90;
    int clear_y = 380;

    int volume_x = 100;
    int volume_y = 60;
    int volume_w = 100;
    int volume_h = 20;

    int duration_x = 0;
    int duration_y = 90;
    int duration_w = 280;
    int duration_h = 24;

    int repeat_x = 210;
    int repeat_y = 120;
    int repeat_h;
    int repeat_w;

    int shuffle_x = 210;
    int shuffle_y = 135;
    int shuffle_w;
    int shuffle_h;

    int artist_x = 100;
    int artist_y = 20;
    int artist_w = 170;
    int artist_h = 15;

    int songs_x = 100;
    int songs_y = 40;
    int songs_w = 170;
    int songs_h = 15;

    int close_x = 260;
    int close_y = 0;
    int close_w = 15;
    int close_h = 15;
    std::string close_i;

    int minimize_x = 250;
    int minimize_y = 0;
    int minimize_w = 15;
    int minimize_h = 15;
    std::string minimize_i;

    int repeat = 0;
    int shuffle = 0;
    int main_x = 0;
    int main_y = 0;

    int shrinkb_x =250;
    int shrinkb_y = 0;
    int shrinkb_w = 15;
    int shrinkb_h = 15;
    std::string shrinkb_i;


    std::ifstream i("skin.json");
    json j;
    if(!i.fail()){
        try{
            i >> j;
            if(!j.empty()){
                auto j_form = j.find("form");
                auto j_art = j.find("art");
                auto j_play = j.find("play");
                auto j_stop = j.find("stop");
                auto j_prev = j.find("prev");
                auto j_next = j.find("next");
                auto j_play_list = j.find("play_list");
                auto j_add = j.find("add");
                auto j_delete = j.find("delete");
                auto j_clear = j.find("clear");
                auto j_volume = j.find("volume");
                auto j_duration = j.find("duration");
                auto j_repeat = j.find("repeat");
                auto j_shuffle = j.find("shuffle");
                auto j_artist = j.find("artist");
                auto j_songs = j.find("songs");
                auto j_close = j.find("close");
                auto j_minimize = j.find("minimize");
                auto j_shrink = j.find("shrink");

                if(j_form != j.end()){
                    auto width = j.at("form").find("width");
                    auto height = j.at("form").find("height");
                    auto color = j.at("form").find("color");

                    if(width != j.at("form").end() && j.at("form").at("width").is_number())
                        form_w = j.at("form").at("width").get<int>();
                    if(height != j.at("form").end() && j.at("form").at("height").is_number())
                        form_h = j.at("form").at("height").get<int>();
                    if(color != j.at("form").end() && j.at("form").at("color").is_string())
                        form_c = j.at("form").at("color").get<std::string>();
                }
                if(j_art != j.end()){
                    auto width = j.at("art").find("width");
                    auto height = j.at("art").find("height");
                    auto x = j.at("art").find("x");
                    auto y = j.at("art").find("y");

                    if(width != j.at("art").end() && j.at("art").at("width").is_number())
                        art_w = j.at("art").at("width").get<int>();
                    if(height != j.at("art").end() && j.at("art").at("height").is_number())
                        art_h = j.at("art").at("height").get<int>();
                    if(x != j.at("art").end() && j.at("art").at("x").is_number())
                        art_x = j.at("art").at("x").get<int>();
                    if(y != j.at("art").end() && j.at("art").at("y").is_number())
                        art_y = j.at("art").at("y").get<int>();
                }
                if(j_play != j.end()){
                    auto width = j.at("play").find("width");
                    auto height = j.at("play").find("height");
                    auto x = j.at("play").find("x");
                    auto y = j.at("play").find("y");
                    auto img = j.at("play").find("img");

                    if(width != j.at("play").end() && j.at("play").at("width").is_number())
                        play_w = j.at("play").at("width").get<int>();
                    if(height != j.at("play").end() && j.at("play").at("height").is_number())
                        play_h = j.at("play").at("height").get<int>();
                    if(x != j.at("play").end() && j.at("play").at("x").is_number())
                        play_x = j.at("play").at("x").get<int>();
                    if(y != j.at("play").end() && j.at("play").at("y").is_number())
                        play_y = j.at("play").at("y").get<int>();
                    if(img != j.at("play").end() && j.at("play").at("img").is_string())
                        play_i = j.at("play").at("img").get<std::string>();
                }
                if(j_stop != j.end()){
                    auto width = j.at("stop").find("width");
                    auto height = j.at("stop").find("height");
                    auto x = j.at("stop").find("x");
                    auto y = j.at("stop").find("y");
                    auto img = j.at("stop").find("img");

                    if(width != j.at("stop").end() && j.at("stop").at("width").is_number())
                        stop_w = j.at("stop").at("width").get<int>();
                    if(height != j.at("stop").end() && j.at("stop").at("height").is_number())
                        stop_h = j.at("stop").at("height").get<int>();
                    if(x != j.at("stop").end() && j.at("stop").at("x").is_number())
                        stop_x = j.at("stop").at("x").get<int>();
                    if(y != j.at("stop").end() && j.at("stop").at("y").is_number())
                        stop_y = j.at("stop").at("y").get<int>();
                    if(img != j.at("stop").end() && j.at("stop").at("img").is_string())
                        stop_i = j.at("stop").at("img").get<std::string>();
                }
                if(j_prev != j.end()){
                    auto width = j.at("prev").find("width");
                    auto height = j.at("prev").find("height");
                    auto x = j.at("prev").find("x");
                    auto y = j.at("prev").find("y");
                    auto img = j.at("prev").find("img");

                    if(width != j.at("prev").end() && j.at("prev").at("width").is_number())
                        prev_w = j.at("prev").at("width").get<int>();
                    if(height != j.at("prev").end() && j.at("prev").at("height").is_number())
                        prev_h = j.at("prev").at("height").get<int>();
                    if(x != j.at("prev").end() && j.at("prev").at("x").is_number())
                        prev_x = j.at("prev").at("x").get<int>();
                    if(y != j.at("prev").end() && j.at("prev").at("y").is_number())
                        prev_y = j.at("prev").at("y").get<int>();
                    if(img != j.at("prev").end() && j.at("prev").at("img").is_string())
                        prev_i = j.at("prev").at("img").get<std::string>();
                }
                if(j_next != j.end()){
                    auto width = j.at("next").find("width");
                    auto height = j.at("next").find("height");
                    auto x = j.at("next").find("x");
                    auto y = j.at("next").find("y");
                    auto img = j.at("next").find("img");

                    if(width != j.at("next").end() && j.at("next").at("width").is_number())
                        next_w = j.at("next").at("width").get<int>();
                    if(height != j.at("next").end() && j.at("next").at("height").is_number())
                        next_h = j.at("next").at("height").get<int>();
                    if(x != j.at("next").end() && j.at("next").at("x").is_number())
                        next_x = j.at("next").at("x").get<int>();
                    if(y != j.at("next").end() && j.at("next").at("y").is_number())
                        next_y = j.at("next").at("y").get<int>();
                    if(img != j.at("next").end() && j.at("next").at("img").is_string())
                        next_i = j.at("next").at("img").get<std::string>();
                }
                if(j_play_list != j.end()){
                    auto width = j.at("play_list").find("width");
                    auto height = j.at("play_list").find("height");
                    auto x = j.at("play_list").find("x");
                    auto y = j.at("play_list").find("y");

                    if(width != j.at("play_list").end() && j.at("play_list").at("width").is_number())
                        play_list_w = j.at("play_list").at("width").get<int>();
                    if(height != j.at("play_list").end() && j.at("play_list").at("height").is_number())
                        play_list_h = j.at("play_list").at("height").get<int>();
                    if(x != j.at("play_list").end() && j.at("play_list").at("x").is_number())
                        play_list_x = j.at("play_list").at("x").get<int>();
                    if(y != j.at("play_list").end() && j.at("play_list").at("y").is_number())
                        play_list_y = j.at("play_list").at("y").get<int>();
                }
                if(j_delete != j.end()){
                    auto width = j.at("delete").find("width");
                    auto height = j.at("delete").find("height");
                    auto x = j.at("delete").find("x");
                    auto y = j.at("delete").find("y");

                    if(width != j.at("delete").end() && j.at("delete").at("width").is_number())
                        delete_w = j.at("delete").at("width").get<int>();
                    if(height != j.at("delete").end() && j.at("delete").at("height").is_number())
                        delete_h = j.at("delete").at("height").get<int>();
                    if(x != j.at("delete").end() && j.at("delete").at("x").is_number())
                        delete_x = j.at("delete").at("x").get<int>();
                    if(y != j.at("delete").end() && j.at("delete").at("y").is_number())
                        delete_y = j.at("delete").at("y").get<int>();
                }
                if(j_clear != j.end()){
                    auto width = j.at("clear").find("width");
                    auto height = j.at("clear").find("height");
                    auto x = j.at("clear").find("x");
                    auto y = j.at("clear").find("y");

                    if(width != j.at("clear").end() && j.at("clear").at("width").is_number())
                        clear_w = j.at("clear").at("width").get<int>();
                    if(height != j.at("clear").end() && j.at("clear").at("height").is_number())
                        clear_h = j.at("clear").at("height").get<int>();
                    if(x != j.at("clear").end() && j.at("clear").at("x").is_number())
                        clear_x = j.at("clear").at("x").get<int>();
                    if(y != j.at("clear").end() && j.at("clear").at("y").is_number())
                        clear_y = j.at("clear").at("y").get<int>();
                }
                if(j_add != j.end()){
                    auto width = j.at("add").find("width");
                    auto height = j.at("add").find("height");
                    auto x = j.at("add").find("x");
                    auto y = j.at("add").find("y");

                    if(width != j.at("add").end() && j.at("add").at("width").is_number())
                        add_w = j.at("add").at("width").get<int>();
                    if(height != j.at("add").end() && j.at("add").at("height").is_number())
                        add_h = j.at("add").at("height").get<int>();
                    if(x != j.at("add").end() && j.at("add").at("x").is_number())
                        add_x = j.at("add").at("x").get<int>();
                    if(y != j.at("add").end() && j.at("add").at("y").is_number())
                        add_y = j.at("add").at("y").get<int>();
                }
                if(j_volume != j.end()){
                    auto width = j.at("volume").find("width");
                    auto height = j.at("volume").find("height");
                    auto x = j.at("volume").find("x");
                    auto y = j.at("volume").find("y");

                    if(width != j.at("volume").end() && j.at("volume").at("width").is_number())
                        volume_w = j.at("volume").at("width").get<int>();
                    if(height != j.at("volume").end() && j.at("volume").at("height").is_number())
                        volume_h = j.at("volume").at("height").get<int>();
                    if(x != j.at("volume").end() && j.at("volume").at("x").is_number())
                        volume_x = j.at("volume").at("x").get<int>();
                    if(y != j.at("volume").end() && j.at("volume").at("y").is_number())
                        volume_y = j.at("volume").at("y").get<int>();
                }
                if(j_duration != j.end()){
                    auto width = j.at("duration").find("width");
                    auto height = j.at("duration").find("height");
                    auto x = j.at("duration").find("x");
                    auto y = j.at("duration").find("y");

                    if(width != j.at("duration").end() && j.at("duration").at("width").is_number())
                        duration_w = j.at("duration").at("width").get<int>();
                    if(height != j.at("duration").end() && j.at("duration").at("height").is_number())
                        duration_h = j.at("duration").at("height").get<int>();
                    if(x != j.at("duration").end() && j.at("duration").at("x").is_number())
                        duration_x = j.at("duration").at("x").get<int>();
                    if(y != j.at("duration").end() && j.at("duration").at("y").is_number())
                        duration_y = j.at("duration").at("y").get<int>();
                }
                if(j_repeat != j.end()){
                    auto width = j.at("repeat").find("width");
                    auto height = j.at("repeat").find("height");
                    auto x = j.at("repeat").find("x");
                    auto y = j.at("repeat").find("y");

                    if(width != j.at("repeat").end() && j.at("repeat").at("width").is_number())
                        repeat_w = j.at("repeat").at("width").get<int>();
                    if(height != j.at("repeat").end() && j.at("repeat").at("height").is_number())
                        repeat_h = j.at("repeat").at("height").get<int>();
                    if(x != j.at("repeat").end() && j.at("repeat").at("x").is_number())
                        repeat_x = j.at("repeat").at("x").get<int>();
                    if(y != j.at("repeat").end() && j.at("repeat").at("y").is_number())
                        repeat_y = j.at("repeat").at("y").get<int>();
                }
                if(j_shuffle != j.end()){
                    auto width = j.at("shuffle").find("width");
                    auto height = j.at("shuffle").find("height");
                    auto x = j.at("shuffle").find("x");
                    auto y = j.at("shuffle").find("y");

                    if(width != j.at("shuffle").end() && j.at("shuffle").at("width").is_number())
                        shuffle_w = j.at("shuffle").at("width").get<int>();
                    if(height != j.at("shuffle").end() && j.at("shuffle").at("height").is_number())
                        shuffle_h = j.at("shuffle").at("height").get<int>();
                    if(x != j.at("shuffle").end() && j.at("shuffle").at("x").is_number())
                        shuffle_x = j.at("shuffle").at("x").get<int>();
                    if(y != j.at("shuffle").end() && j.at("shuffle").at("y").is_number())
                        shuffle_y = j.at("shuffle").at("y").get<int>();
                }
                if(j_artist != j.end()){
                    auto width = j.at("artist").find("width");
                    auto height = j.at("artist").find("height");
                    auto x = j.at("artist").find("x");
                    auto y = j.at("artist").find("y");

                    if(width != j.at("artist").end() && j.at("artist").at("width").is_number())
                        artist_w = j.at("artist").at("width").get<int>();
                    if(height != j.at("artist").end() && j.at("artist").at("height").is_number())
                        artist_h = j.at("artist").at("height").get<int>();
                    if(x != j.at("artist").end() && j.at("artist").at("x").is_number())
                        artist_x = j.at("artist").at("x").get<int>();
                    if(y != j.at("artist").end() && j.at("artist").at("y").is_number())
                        artist_y = j.at("artist").at("y").get<int>();
                }
                if(j_songs != j.end()){
                    auto width = j.at("songs").find("width");
                    auto height = j.at("songs").find("height");
                    auto x = j.at("songs").find("x");
                    auto y = j.at("songs").find("y");

                    if(width != j.at("songs").end() && j.at("songs").at("width").is_number())
                        songs_w = j.at("songs").at("width").get<int>();
                    if(height != j.at("songs").end() && j.at("songs").at("height").is_number())
                        songs_h = j.at("songs").at("height").get<int>();
                    if(x != j.at("songs").end() && j.at("songs").at("x").is_number())
                        songs_x = j.at("songs").at("x").get<int>();
                    if(y != j.at("songs").end() && j.at("songs").at("y").is_number())
                        songs_y = j.at("songs").at("y").get<int>();
                }
                if(j_close != j.end()){
                    auto width = j.at("close").find("width");
                    auto height = j.at("close").find("height");
                    auto x = j.at("close").find("x");
                    auto y = j.at("close").find("y");
                    auto img = j.at("close").find("img");

                    if(width != j.at("close").end() && j.at("close").at("width").is_number())
                        close_w = j.at("close").at("width").get<int>();
                    if(height != j.at("close").end() && j.at("close").at("height").is_number())
                        close_h = j.at("close").at("height").get<int>();
                    if(x != j.at("close").end() && j.at("close").at("x").is_number())
                        close_x = j.at("close").at("x").get<int>();
                    if(y != j.at("close").end() && j.at("close").at("y").is_number())
                        close_y = j.at("close").at("y").get<int>();
                    if(img != j.at("close").end() && j.at("close").at("img").is_string())
                        close_i = j.at("close").at("img").get<std::string>();
                }
                if(j_minimize != j.end()){
                    auto width = j.at("minimize").find("width");
                    auto height = j.at("minimize").find("height");
                    auto x = j.at("minimize").find("x");
                    auto y = j.at("minimize").find("y");
                    auto img = j.at("minimize").find("img");

                    if(width != j.at("minimize").end() && j.at("minimize").at("width").is_number())
                        minimize_w = j.at("minimize").at("width").get<int>();
                    if(height != j.at("minimize").end() && j.at("minimize").at("height").is_number())
                        minimize_h = j.at("minimize").at("height").get<int>();
                    if(x != j.at("minimize").end() && j.at("minimize").at("x").is_number())
                        minimize_x = j.at("minimize").at("x").get<int>();
                    if(y != j.at("minimize").end() && j.at("minimize").at("y").is_number())
                        minimize_y = j.at("minimize").at("y").get<int>();
                    if(img != j.at("minimize").end() && j.at("minimize").at("img").is_string())
                        minimize_i = j.at("minimize").at("img").get<std::string>();
                }
                if(j_shrink != j.end()){
                    auto width = j.at("shrink").find("width");
                    auto height = j.at("shrink").find("height");
                    auto x = j.at("shrink").find("x");
                    auto y = j.at("shrink").find("y");
                    auto s_w = j.at("shrink").find("s_width");
                    auto s_h = j.at("shrink").find("s_height");
                    auto img = j.at("shrink").find("img");

                    if(width != j.at("shrink").end() && j.at("shrink").at("width").is_number())
                        shrinkb_w = j.at("shrink").at("width").get<int>();
                    if(height != j.at("shrink").end() && j.at("shrink").at("height").is_number())
                        shrinkb_h = j.at("shrink").at("height").get<int>();
                    if(x != j.at("shrink").end() && j.at("shrink").at("x").is_number())
                        shrinkb_x = j.at("shrink").at("x").get<int>();
                    if(y != j.at("shrink").end() && j.at("shrink").at("y").is_number())
                        shrinkb_y = j.at("shrink").at("y").get<int>();
                    if(img != j.at("shrink").end() && j.at("shrink").at("img").is_string())
                        shrinkb_i = j.at("shrink").at("img").get<std::string>();
                    if(s_w != j.at("shrink").end() && j.at("shrink").at("s_width").is_number())
                        shrink_w = j.at("shrink").at("s_width").get<int>();
                    if(s_h != j.at("shrink").end() && j.at("shrink").at("s_height").is_number())
                        shrink_h = j.at("shrink").at("s_height").get<int>();
                }
            }
        }
        catch(json::exception& e){
                std::cout << e.what() << std::endl;
        }
        i.close();
        std::ifstream i("settings.json");
        if(!i.fail()){
            json settings;
            try{
                i>>settings;
                repeat = settings.at("repeat").get<int>();
                shuffle = settings.at("shuffle").get<int>();
                main_x = settings.at("x").get<int>();
                main_y = settings.at("y").get<int>();
                shrink = settings.at("shrink").get<int>();
                for(auto& x : settings.at("playlist_path").items()){
                    std::stringstream geek(x.key());
                    int num;
                    geek >> num;
                    memory.append_list(settings.at("playlist_path").at(num).get<std::string>(),settings.at("playlist_name").at(num).get<std::string>());
                }
                memory.set_list_update(1);

            }
            catch(json::exception& e){
                std::cout << e.what() << std::endl;
            }
        }

    }



      wxBitmap img_play_BMP, img_stop_BMP, img_next_BMP, img_prev_BMP,img_close_BMP,img_minimize_BMP,img_shrink_BMP;
      wxBitmap img_play_BMP2, img_stop_BMP2, img_next_BMP2, img_prev_BMP2,img_close_BMP2,img_minimize_BMP2,img_shrink_BMP2;
      if(!play_i.empty()){
            wxImage img_play;
            if(img_play.LoadFile(wxString(play_i),wxBITMAP_TYPE_ANY)){
                img_play.Rescale(play_w-5,play_h-5);
                img_play_BMP=wxBitmap(img_play,-1);
            }
      }
      if(!stop_i.empty()){
            wxImage img_stop;
            if(img_stop.LoadFile(wxString(stop_i),wxBITMAP_TYPE_ANY)){
                img_stop.Rescale(stop_w-5,stop_h-5);
                img_stop_BMP=wxBitmap(img_stop,-1);
            }
      }
      if(!prev_i.empty()){
            wxImage img_prev;
            if(img_prev.LoadFile(wxString(prev_i),wxBITMAP_TYPE_ANY)){
                img_prev.Rescale(prev_w-5,prev_h-5);
                img_prev_BMP=wxBitmap(img_prev,-1);
            }
      }
      if(!next_i.empty()){
            wxImage img_next;
            if(img_next.LoadFile(wxString(next_i),wxBITMAP_TYPE_ANY)){
                img_next.Rescale(next_w-5,next_h-5);
                img_next_BMP=wxBitmap(img_next,-1);
            }
      }
      if(!minimize_i.empty()){
            wxImage img_minimize;
            if(img_minimize.LoadFile(wxString(minimize_i),wxBITMAP_TYPE_ANY)){
                img_minimize.Rescale(minimize_w-2,minimize_h-2);
                img_minimize_BMP=wxBitmap(img_minimize,-1);
            }
      }
      if(!close_i.empty()){
            wxImage img_close;
            if(img_close.LoadFile(wxString(close_i),wxBITMAP_TYPE_ANY)){
                img_close.Rescale(close_w-2,close_h-2);
                img_close_BMP=wxBitmap(img_close,-1);
            }
      }
      if(!shrinkb_i.empty()){
            wxImage img_shrink;
            if(img_shrink.LoadFile(wxString(shrinkb_i),wxBITMAP_TYPE_ANY)){
                img_shrink.Rescale(shrinkb_w-2,shrinkb_h-2);
                img_shrink_BMP=wxBitmap(img_shrink,-1);
            }
      }

      Create(parent, wxID_ANY, _("WxMusic"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTRANSPARENT_WINDOW, _T("wxID_ANY"));
      btn_add = new wxButton(this, ID_BUTTON2, _("Add"), wxPoint(add_x,add_y), wxSize(add_w,add_h), 0, wxDefaultValidator, _T("ID_BUTTON2"));
      lb_playlist = new wxListBox(this, ID_LISTBOX1, wxPoint(play_list_x,play_list_y), wxSize(play_list_w,play_list_h), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
      st_artist = new wxStaticText(this, ID_STATICTEXT1, wxEmptyString, wxPoint(artist_x,artist_y), wxSize(artist_w,artist_h), 0, _T("ID_STATICTEXT1"));
      st_songname = new wxStaticText(this, ID_STATICTEXT2, wxEmptyString, wxPoint(songs_x,songs_y), wxSize(songs_w,songs_h), 0, _T("ID_STATICTEXT2"));
      sb_cover = new wxStaticBitmap(this, ID_STATICBITMAP1, wxNullBitmap, wxPoint(art_x,art_y), wxSize(art_w,art_h), wxSIMPLE_BORDER, _T("ID_STATICBITMAP1"));
      sl_volume = new wxSlider(this, ID_SLIDER1, 128, 0, 128, wxPoint(volume_x,volume_y), wxSize(volume_w,volume_h), 0, wxDefaultValidator, _T("ID_SLIDER1"));
      btn_clear = new wxButton(this, ID_BUTTON1, _("Clear"), wxPoint(clear_x,clear_y), wxSize(clear_w,clear_h), 0, wxDefaultValidator, _T("ID_BUTTON1"));
      btn_delete = new wxButton(this, ID_BUTTON3, _("Delete"), wxPoint(delete_x,delete_y), wxSize(delete_w,delete_h), 0, wxDefaultValidator, _T("ID_BUTTON3"));
      btn_play = new wxBitmapButton(this, ID_BITMAPBUTTON1, img_play_BMP, wxPoint(play_x,play_y), wxSize(play_w,play_h), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
      btn_prev = new wxBitmapButton(this, ID_BITMAPBUTTON2, img_prev_BMP, wxPoint(prev_x,prev_y), wxSize(prev_w,prev_h), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
      btn_stop = new wxBitmapButton(this, ID_BITMAPBUTTON3, img_stop_BMP, wxPoint(stop_x,stop_y), wxSize(stop_w,stop_h), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON3"));
      btn_next = new wxBitmapButton(this, ID_BITMAPBUTTON4, img_next_BMP, wxPoint(next_x,next_y), wxSize(next_w,next_h), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON4"));
      cb_repeat = new wxCheckBox(this, ID_CHECKBOX1, _("Repeat"), wxPoint(repeat_x,repeat_y), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
      cb_shuffle = new wxCheckBox(this, ID_CHECKBOX2, _("Shuffle"), wxPoint(shuffle_x,shuffle_y), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
      btn_close = new wxBitmapButton(this, ID_BITMAPBUTTON5, img_close_BMP, wxPoint(close_x,close_y), wxSize(close_w,close_h), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON5"));
      btn_minimize = new wxBitmapButton(this, ID_BITMAPBUTTON6, img_minimize_BMP, wxPoint(minimize_x,minimize_y), wxSize(minimize_w,minimize_h), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON6"));
      sl_duration = new wxSlider(this, ID_SLIDER2, 0, 0, 100, wxPoint(duration_x,duration_y), wxSize(duration_w,duration_h), 0, wxDefaultValidator, _T("ID_SLIDER2"));
      btn_shrink = new wxBitmapButton(this, ID_BITMAPBUTTON7, img_shrink_BMP, wxPoint(shrinkb_x,shrinkb_y), wxSize(shrinkb_w,shrinkb_h), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON7"));
      FileDialog1 = new wxFileDialog(this, _("Open Audio Files"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_DEFAULT_STYLE|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
      Timer1.SetOwner(this, ID_TIMER1);
      Timer1.Start(250, false);
      Timer2.SetOwner(this, ID_TIMER2);
      Timer2.Start(10, false);
      t_timeline.SetOwner(this, ID_TIMER3);
      t_timeline.Start(100, false);

      Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onbtn_addClick);
      Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onlb_playlistDClick);
      Connect(ID_SLIDER1,wxEVT_SCROLL_TOP|wxEVT_SCROLL_BOTTOM|wxEVT_SCROLL_LINEUP|wxEVT_SCROLL_LINEDOWN|wxEVT_SCROLL_PAGEUP|wxEVT_SCROLL_PAGEDOWN|wxEVT_SCROLL_THUMBTRACK|wxEVT_SCROLL_THUMBRELEASE|wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onsl_volumeCmdScroll);
      Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onbtn_clearClick);
      Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onbtn_deleteClick);
      Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onbtn_playClick);
      Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onbtn_prevClick);
      Connect(ID_BITMAPBUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onbtn_stopClick);
      Connect(ID_BITMAPBUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onbtn_nextClick);
      Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Oncb_repeatClick);
      Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Oncb_shuffleClick);
      Connect(ID_BITMAPBUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onbtn_closeClick);
      Connect(ID_BITMAPBUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onbtn_minimizeClick);
      Connect(ID_SLIDER2,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&WXMusicPlayerFrame::Onsl_durationCmdScroll);
      Connect(ID_SLIDER2,wxEVT_SCROLL_THUMBRELEASE,(wxObjectEventFunction)&WXMusicPlayerFrame::Onsl_durationCmdScrollChanged);
      Connect(ID_BITMAPBUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMusicPlayerFrame::Onbtn_shrinkClick);
      Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&WXMusicPlayerFrame::OnTimer1Trigger);
      Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&WXMusicPlayerFrame::OnTimer2Trigger);
      Connect(ID_TIMER3,wxEVT_TIMER,(wxObjectEventFunction)&WXMusicPlayerFrame::Ont_timelineTrigger);
      Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMusicPlayerFrame::OnClose);
      //(*Initialize(WXMusicPlayerFrame)
      //*)
      if(shrink==1){
        SetClientSize(wxSize(shrink_w,shrink_h));
      }
      else{
        SetClientSize(wxSize(form_w,form_h));
      }
      SetBackgroundColour(wxString(form_c));
      if(main_x == 0 && main_y==0)
        CentreOnScreen();
      if(repeat == 0 )
        cb_repeat->SetValue(false);
      else{
        cb_repeat->SetValue(true);
        memory.set_repeat(1);
      }
      if(shuffle == 0 )
        cb_shuffle->SetValue(false);
      else{
        cb_shuffle->SetValue(1);
        memory.set_shuffle(1);
      }
      btn_next->SetBackgroundColour(wxString(form_c));
      btn_play->SetBackgroundColour(wxString(form_c));
      btn_prev->SetBackgroundColour(wxString(form_c));
      btn_shrink->SetBackgroundColour(wxString(form_c));
      btn_stop->SetBackgroundColour(wxString(form_c));
      btn_minimize->SetBackgroundColour(wxString(form_c));
      btn_add->SetBackgroundColour(wxString(form_c));
      btn_clear->SetBackgroundColour(wxString(form_c));
      btn_delete->SetBackgroundColour(wxString(form_c));
      btn_close->SetBackgroundColour(wxString(form_c));

}

WXMusicPlayerFrame::~WXMusicPlayerFrame()
{
    std::ofstream file("settings.json");
    json settings_file;
    if(cb_repeat->GetValue())
        settings_file["repeat"] = 1;
    else
        settings_file["repeat"] = 0;
    if(cb_shuffle->GetValue())
        settings_file["shuffle"] = 1;
    else
        settings_file["shuffle"] = 0;
    wxPoint origin = GetPosition();
    settings_file["x"] = origin.x;
    settings_file["y"] = origin.y;
    settings_file["playlist_path"]=nullptr;
    settings_file["playlist_name"]=nullptr;
    settings_file["shrink"]=shrink;
    std::vector <std::string> playlist_path = memory.get_play_list_path();
    std::vector <std::string> playlist_name = memory.get_play_list();
    for(int i=0;i<playlist_path.size();i++){
        settings_file["playlist_path"]+=playlist_path[i];
        settings_file["playlist_name"]+=playlist_name[i];
    }
    file << settings_file << std::endl;
    file.close();
    //(*Destroy(WXMusicPlayerFrame)
    //*)

}

void WXMusicPlayerFrame::OnQuit(wxCommandEvent& event)
{
    //player.Stop();
    event.Skip();
}

void WXMusicPlayerFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}


void WXMusicPlayerFrame::Onbtn_addClick(wxCommandEvent& event)
{
    if(FileDialog1->ShowModal() == wxID_OK){
        wxArrayString FilePaths;
        wxArrayString Filenames;
        FileDialog1->GetPaths(FilePaths);
        FileDialog1->GetFilenames(Filenames);
        for(int i=0;i<FilePaths.GetCount();i++){
            memory.append_list(std::string(FilePaths[i].mb_str()),std::string(Filenames[i].mb_str()));
            //printf("%d: %s\n",i,FilePaths[i].mb_str());
        }
        memory.set_list_update(1);
    }


    // FileName points to selected file
}

void WXMusicPlayerFrame::OnTimer1Trigger(wxTimerEvent& event)
{
    if(memory.get_update()==1){
        std::string stlstring = memory.get_name();
        wxString name(stlstring.c_str(), wxConvUTF8);
        std::string stlstring2 = memory.get_title();
        wxString title(stlstring2.c_str(), wxConvUTF8);
        st_artist->SetLabel(name);
        st_songname->SetLabel(title);
        std::vector <unsigned char> tmp_img(memory.get_img());
        if(!tmp_img.empty()){
            wxMemoryInputStream jpegStream(&tmp_img[0], tmp_img.size());
            wxImage jpegImage;
            jpegImage.LoadFile(jpegStream, wxBITMAP_TYPE_ANY);
            sb_cover->SetBitmap(wxBitmap(jpegImage.Scale(70,70,wxIMAGE_QUALITY_NORMAL)));
        }
        else{
            wxBitmap myBmp;
            sb_cover->SetBitmap(myBmp);
        }
        memory.set_update(0);
    }
    else{
        wxSize song,artist;
        song = st_songname->GetClientSize();
        artist = st_artist->GetClientSize();
        wxString getsong = st_songname->GetLabelText();
        wxString getartist = st_artist->GetLabelText();

        if(getsong.size() > (song.GetWidth()/6)){

            getsong+=getsong[0];
            getsong.erase(0,1);
            st_songname->SetLabelText(getsong);
        }
        if(getartist.size() > (artist.GetWidth()/6)){
            getartist+=getartist[0];
            getartist.erase(0,1);
            st_artist->SetLabelText(getartist);
        }
    }
}

void WXMusicPlayerFrame::OnTimer2Trigger(wxTimerEvent& event)
{

    if(memory.get_list_update()==1){
        lb_playlist->Clear();
        std::vector <std::string> tmp_name = memory.get_play_list();
        for(int i=0;i<tmp_name.size();i++){
            lb_playlist->Append(tmp_name[i].c_str());
        }
        memory.set_list_update(0);
    }
}

void WXMusicPlayerFrame::OnClose(wxCloseEvent& event)
{
    //player.Stop();
    event.Skip();
}


void WXMusicPlayerFrame::Onbtn_playClick(wxCommandEvent& event)
{
    if(lb_playlist->GetCount()>0){
        memory.set_play_index(0);
        memory.shuffle_play_list();
        int play_index = memory.get_shuffle_play_number();
        std::string path = memory.get_play_list_path(play_index);
        player.set_path(path);
        player.Play();
    }
    else{
        int signal = player.get_play_signal();
        if(signal>0)
            player.Play();
    }

}

void WXMusicPlayerFrame::Onbtn_prevClick(wxCommandEvent& event)
{
    if(lb_playlist->GetCount()>0)
        player.Prev();
}

void WXMusicPlayerFrame::Onbtn_stopClick(wxCommandEvent& event)
{
    player.Stop();
}

void WXMusicPlayerFrame::Onbtn_nextClick(wxCommandEvent& event)
{
    if(lb_playlist->GetCount()>0)
        player.Next();
}


void WXMusicPlayerFrame::Oncb_repeatClick(wxCommandEvent& event)
{
    if(cb_repeat->GetValue()){
        memory.set_repeat(1);
    }
    else{
        memory.set_repeat(0);
    }
}

void WXMusicPlayerFrame::Oncb_shuffleClick(wxCommandEvent& event)
{
    if(cb_shuffle->GetValue()){
        memory.set_shuffle(1);
    }
    else{
        memory.set_shuffle(0);
    }
    memory.shuffle_play_list();
}


void WXMusicPlayerFrame::Onbtn_minimizeClick(wxCommandEvent& event)
{
    wxTopLevelWindow::Iconize();
}

void WXMusicPlayerFrame::Onbtn_closeClick(wxCommandEvent& event)
{
    Close();
}

void WXMusicPlayerFrame::Onsl_durationCmdScroll(wxScrollEvent& event)
{
    t_timeline.Stop();
}

void WXMusicPlayerFrame::Ont_timelineTrigger(wxTimerEvent& event)
{
    sl_duration->SetMax(memory.get_duration_total());
    sl_duration->SetValue(memory.get_duration_now());
}

void WXMusicPlayerFrame::Onsl_durationCmdScrollChanged(wxScrollEvent& event)
{
    memory.set_jump(sl_duration->GetValue()*10);
    memory.set_jump_signal(1);
    t_timeline.Start();
}

void WXMusicPlayerFrame::Onbtn_shrinkClick(wxCommandEvent& event)
{
    if(shrink==0){
        shrink=1;
        SetClientSize(wxSize(shrink_w,shrink_h));
    }
    else{
        shrink=0;
        SetClientSize(wxSize(form_w,form_h));
    }
}

void WXMusicPlayerFrame::Onlb_playlistDClick(wxCommandEvent& event)
{
    if(memory.get_shuffle()==1){
        memory.shuffle_play_list(lb_playlist->GetSelection());
        memory.set_play_index(0);
    }
    else{
        memory.shuffle_play_list(lb_playlist->GetSelection());
        memory.set_play_index(lb_playlist->GetSelection());
    }
    player.Play_force();
}

void WXMusicPlayerFrame::Onsl_volumeCmdScroll(wxScrollEvent& event)
{
    player.set_volume(sl_volume->GetValue());
}

void WXMusicPlayerFrame::Onbtn_clearClick(wxCommandEvent& event)
{
    if(lb_playlist->GetCount()>0){
        memory.clear_list();
        memory.set_list_update(1);
    }
}

void WXMusicPlayerFrame::Onbtn_deleteClick(wxCommandEvent& event)
{
    if(lb_playlist->GetCount()>0 || lb_playlist->GetSelection()>0){
        memory.delete_list(lb_playlist->GetSelection());
        memory.set_list_update(1);
    }
}
