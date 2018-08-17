/***************************************************************
 * Name:      WXMusicPlayerMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2018-07-17
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef WXMUSICPLAYERMAIN_H
#define WXMUSICPLAYERMAIN_H

//(*Headers(WXMusicPlayerFrame)
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/slider.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/timer.h>
//*)



class WXMusicPlayerFrame: public wxFrame
{
    public:

        WXMusicPlayerFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~WXMusicPlayerFrame();

    private:
        //Frame mouse move//
        wxPoint m_delta;
        void OnLeftDown(wxMouseEvent& evt);
        void OnLeftUp(wxMouseEvent& WXUNUSED(evt));
        void OnMouseMove(wxMouseEvent& evt);
        //----------------//

        //(*Handlers(WXMusicPlayerFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnButton3Click(wxCommandEvent& event);
        void OnButton4Click(wxCommandEvent& event);
        void OnTextCtrl1Text(wxCommandEvent& event);
        void OnButton4Click1(wxCommandEvent& event);
        void OnButton3Click1(wxCommandEvent& event);
        void OnListBox1Select(wxCommandEvent& event);
        void OnListBox1Select1(wxCommandEvent& event);
        void OnTimer1Trigger(wxTimerEvent& event);
        void OnListBox1Select2(wxCommandEvent& event);
        void OnTimer2Trigger(wxTimerEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnButton6Click(wxCommandEvent& event);
        void OnButton5Click(wxCommandEvent& event);
        void OnSlider1CmdScroll(wxScrollEvent& event);
        void OnButton7Click(wxCommandEvent& event);
        void OnButton8Click(wxCommandEvent& event);
        void OnListBox1Select3(wxCommandEvent& event);
        void Onbtn_playClick(wxCommandEvent& event);
        void Onbtn_prevClick(wxCommandEvent& event);
        void Onbtn_stopClick(wxCommandEvent& event);
        void Onbtn_nextClick(wxCommandEvent& event);
        void OnRadioButton3Select(wxCommandEvent& event);
        void OnRadioButton1Select(wxCommandEvent& event);
        void Onrb_repeatSelect(wxCommandEvent& event);
        void Onrb_shuffleSelect(wxCommandEvent& event);
        void Oncb_repeatClick(wxCommandEvent& event);
        void Oncb_shuffleClick(wxCommandEvent& event);
        void OnBitmapButton1Click(wxCommandEvent& event);
        void Onbtn_minimizeClick(wxCommandEvent& event);
        void Onbtn_closeClick(wxCommandEvent& event);
        void Onsl_durationCmdScroll(wxScrollEvent& event);
        void Ont_timelineTrigger(wxTimerEvent& event);
        void Onsl_durationCmdScrollChanged(wxScrollEvent& event);
        void Onbtn_shrinkClick(wxCommandEvent& event);
        void Onbtn_addClick(wxCommandEvent& event);
        void Onlb_playlistDClick(wxCommandEvent& event);
        void Onsl_volumeCmdScroll(wxScrollEvent& event);
        void Onbtn_clearClick(wxCommandEvent& event);
        void Onbtn_deleteClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(WXMusicPlayerFrame)
        static const long ID_BUTTON2;
        static const long ID_LISTBOX1;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICBITMAP1;
        static const long ID_SLIDER1;
        static const long ID_BUTTON1;
        static const long ID_BUTTON3;
        static const long ID_BITMAPBUTTON1;
        static const long ID_BITMAPBUTTON2;
        static const long ID_BITMAPBUTTON3;
        static const long ID_BITMAPBUTTON4;
        static const long ID_CHECKBOX1;
        static const long ID_CHECKBOX2;
        static const long ID_BITMAPBUTTON5;
        static const long ID_BITMAPBUTTON6;
        static const long ID_SLIDER2;
        static const long ID_BITMAPBUTTON7;
        static const long ID_TIMER1;
        static const long ID_TIMER2;
        static const long ID_TIMER3;
        //*)

        //(*Declarations(WXMusicPlayerFrame)
        wxBitmapButton* btn_close;
        wxBitmapButton* btn_minimize;
        wxBitmapButton* btn_next;
        wxBitmapButton* btn_play;
        wxBitmapButton* btn_prev;
        wxBitmapButton* btn_shrink;
        wxBitmapButton* btn_stop;
        wxButton* btn_add;
        wxButton* btn_clear;
        wxButton* btn_delete;
        wxCheckBox* cb_repeat;
        wxCheckBox* cb_shuffle;
        wxFileDialog* FileDialog1;
        wxListBox* lb_playlist;
        wxSlider* sl_duration;
        wxSlider* sl_volume;
        wxStaticBitmap* sb_cover;
        wxStaticText* st_artist;
        wxStaticText* st_songname;
        wxTimer Timer1;
        wxTimer Timer2;
        wxTimer t_timeline;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // WXMUSICPLAYERMAIN_H
