/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SettingsWindowWin32.cpp
Version:	0.10
---------------------------------------------------------------------------
*/

#define NOMINMAX
#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400
#endif
#include <windows.h>

#include "PrecompiledHeaders.h"
#include "Resource.h"
#include "SettingsWindowWin32.h"
#include "FuncSplit.h"
#include "RendererEnumerator.h"


namespace nGENE
{
namespace Application
{
	bool DisplayModesComparator(SDisplayMode* _mode1, SDisplayMode* _mode2)
	{
		if(_mode1->width == _mode2->width)
			return (_mode1->height < _mode2->height);
		else
			return (_mode1->width < _mode2->width);
	}


	SettingsWindowWin32::SettingsWindowWin32()
	{
	}
//----------------------------------------------------------------------
	SettingsWindowWin32::~SettingsWindowWin32()
	{
	}
//----------------------------------------------------------------------
	bool SettingsWindowWin32::createWindow()
	{
		// Retrieve display modes
		DEVMODE mode;
		dword counter = 0;
		while(EnumDisplaySettings(NULL, counter++, &mode))
		{
			SDisplayMode displayMode;
			displayMode.width  = mode.dmPelsWidth;
			displayMode.height = mode.dmPelsHeight;

			stringstream buffer;
			buffer << displayMode.width << "x" << displayMode.height;
			m_DisplayModes[buffer.str()] = displayMode;
		}

		HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle(NULL));
		int i = DialogBox(hInstance, MAKEINTRESOURCE(IDD_DLG_CONFIG), NULL, DlgProc);

		// Cancel clicked, so we have to exit
		if(!i)
			return false;

		// Error occured, so log it
		if(i == -1)
        {
            int winError = GetLastError();
            wchar_t* errDesc = new wchar_t[255];

            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM |
						  FORMAT_MESSAGE_IGNORE_INSERTS,
						  NULL,
						  winError,
						  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						  static_cast<LPWSTR>(errDesc),
						  255,
						  NULL);
			
			Log::log(LET_ERROR, L"Application", __WFILE__, __WFUNCTION__, __LINE__,
				errDesc);

			return false;
        }

		return true;
	}
//----------------------------------------------------------------------
	void SettingsWindowWin32::showWindow(bool value)
	{
	}
//----------------------------------------------------------------------
	BOOL SettingsWindowWin32::DlgProc(HWND hDlg, UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		HWND hwndItem;
		int sel;
		char data[10];
		HashTable <string, SDisplayMode>::iterator iter;
		HashTable <string, SDisplayMode> table = SettingsWindowWin32::getSingleton().getDisplayModes();
		vector <SDisplayMode*> modes;
		for(iter = table.begin(); iter != table.end(); ++iter)
			modes.push_back(&iter->second);
		std::sort(modes.begin(), modes.end(), DisplayModesComparator);

		switch(nMsg)
		{
		case WM_INITDIALOG:
			{
				// Initialize combo box
				hwndItem = GetDlgItem(hDlg, IDC_CBO_RENDERSYSTEM);
				vector <wstring> vRenderers = RendererEnumerator::getSingleton().getNamesList();
				for(uint i = 0; i < vRenderers.size(); ++i)
				{
					string stName(vRenderers[i].begin(), vRenderers[i].end());
					SendMessage(hwndItem, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(stName.c_str()));
				}
				SendMessage(hwndItem, CB_SETCURSEL, static_cast<WPARAM>(0), 0);

				// Initialize combo box
				hwndItem = GetDlgItem(hDlg, IDC_CBO_SOUNDSYSTEM);
				SendMessage(hwndItem, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>("FMOD"));
				SendMessage(hwndItem, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>("No sound"));
				SendMessage(hwndItem, CB_SETCURSEL, static_cast<WPARAM>(0), 0);

				// Initialize list box
				hwndItem = GetDlgItem(hDlg, IDC_LST_MODE);
				for(uint i = 0; i < modes.size(); ++i)
				{
					char line[12];
					sprintf(line, "%dx%d", modes[i]->width, modes[i]->height);
					SendMessage(hwndItem, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(line));
				}
				SendMessage(hwndItem, LB_SELECTSTRING, 0, reinterpret_cast<LPARAM>("800x600"));

				// Set default display mode to windowed
				hwndItem = GetDlgItem(hDlg, IDC_CHB_WINDOWED);
				SendMessage(hwndItem, BM_SETCHECK, static_cast<WPARAM>(TRUE), 0);

				// Set default number of target windows to 1
				hwndItem = GetDlgItem(hDlg, IDC_TXT_WNDS);
				SendMessage(hwndItem, WM_SETTEXT, 0, reinterpret_cast<LPARAM>("1"));

				// Enable VSync by default
				hwndItem = GetDlgItem(hDlg, IDC_CHB_VSYNC);
				SendMessage(hwndItem, BM_SETCHECK, static_cast<WPARAM>(TRUE), 0);


				// Set startup size and position of the window
				int x, y, screenWidth, screenHeight;
				RECT rcDlg;
				GetWindowRect(hDlg, &rcDlg);
				screenWidth	 = GetSystemMetrics(SM_CXFULLSCREEN);
				screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

				x = (screenWidth >> 1) - ((rcDlg.right - rcDlg.left) >> 1);
				y = (screenHeight>> 1) - ((rcDlg.bottom - rcDlg.top) >> 1);

				MoveWindow(hDlg, x, y, (rcDlg.right - rcDlg.left),
					(rcDlg.bottom - rcDlg.top), TRUE);
			}
			

			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDC_CBO_RENDERSYSTEM:
				hwndItem = GetDlgItem(hDlg, IDC_CBO_RENDERSYSTEM);
				if(HIWORD(wParam) == CBN_SELCHANGE)
				{
					sel = SendMessage(hwndItem, CB_GETCURSEL, 0, 0);
					if(sel != -1)
					{
						char temp[16];
						SendMessage(hwndItem, CB_GETLBTEXT, static_cast<WORD>(sel), LPARAM(temp));
						string selected = temp;
						wstring stRenderer(selected.begin(), selected.end());

						SettingsWindowWin32::getSingleton().getEngineInfo().renderer = stRenderer;
					}
				}

				return TRUE;

			case IDC_CBO_SOUNDSYSTEM:
				hwndItem = GetDlgItem(hDlg, IDC_CBO_SOUNDSYSTEM);
				sel = SendMessage(hwndItem, CB_GETCOUNT, 0, 0);

				if(HIWORD(wParam) == CBN_SELCHANGE)
				{
					sel = SendMessage(hwndItem, CB_GETCURSEL, 0, 0);
					if(sel != -1)
					{
						// Set rendering API
						if(sel == 0)
							SettingsWindowWin32::getSingleton().getEngineInfo().soundLibrary = L"FMOD";
						else if(sel == 1)
							SettingsWindowWin32::getSingleton().getEngineInfo().soundLibrary = L"";
					}                    
				}
				return TRUE;
			
			case IDC_LST_MODE:
				hwndItem = GetDlgItem(hDlg, IDC_LST_MODE);
				if(HIWORD(wParam) == LBN_SELCHANGE)
				{
					sel = SendMessage(hwndItem, LB_GETCURSEL, 0, 0);
					if(sel != -1)
					{
						char temp[10];
						SendMessage(hwndItem, LB_GETTEXT, static_cast<WORD>(sel), LPARAM(temp));
						string selected = temp;

						vector <string> vecValues;
						FuncSplit <string> tokenizer;
						tokenizer(selected, vecValues, "x");

						SettingsWindowWin32::getSingleton().getEngineInfo().DisplayMode.width = atoi(vecValues[0].c_str());
						SettingsWindowWin32::getSingleton().getEngineInfo().DisplayMode.height = atoi(vecValues[1].c_str());
					}
				}

				return TRUE;
			
			case IDC_CHB_WINDOWED:
				if(IsDlgButtonChecked(hDlg, IDC_CHB_WINDOWED))
				{
					hwndItem = GetDlgItem(hDlg, IDC_TXT_WNDS);
					SendMessage(hwndItem, WM_ENABLE, static_cast<WPARAM>(TRUE), 0);
					SettingsWindowWin32::getSingleton().getEngineInfo().DisplayMode.windowed = true;
				}
				else
				{
					hwndItem = GetDlgItem(hDlg, IDC_TXT_WNDS);
					SendMessage(hwndItem, WM_ENABLE, static_cast<WPARAM>(FALSE), 0);
					SettingsWindowWin32::getSingleton().getEngineInfo().DisplayMode.windowed = false;
				}

				return TRUE;

			case IDC_CHB_VSYNC:
				if(IsDlgButtonChecked(hDlg, IDC_CHB_VSYNC))
				{
					SettingsWindowWin32::getSingleton().getEngineInfo().DisplayMode.vsync = true;
				}
				else
				{
					SettingsWindowWin32::getSingleton().getEngineInfo().DisplayMode.vsync = false;
				}

				return TRUE;
			
			case ID_OK:
				hwndItem = GetDlgItem(hDlg, IDC_TXT_WNDS);
				SendMessage(hwndItem, WM_GETTEXT, 10, reinterpret_cast<LPARAM>(data));
				SettingsWindowWin32::getSingleton().getEngineInfo().renderWindowsNum = atoi(data);

				EndDialog(hDlg, 1);
				return TRUE;

			case ID_CANCEL: 
				EndDialog(hDlg, 0);
				return FALSE;
			}
		}

		return FALSE;
	}
//----------------------------------------------------------------------
}
}