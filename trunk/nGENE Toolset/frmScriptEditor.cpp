/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmScriptEditor.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "StdAfx.h"
#include <vcclr.h>
#include "Engine.h"
#include "ScriptSystemLua.h"
#include "ScriptLua.h"
#include "frmScriptEditor.h"

using nGENE::Engine;
using nGENE::ScriptLua;
using nGENE::ScriptSystemLua;

namespace nGENEToolset
{
	System::Void frmScriptEditor::btnRunScript_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!m_pScript)
		{
			ScriptSystemLua* pLua = Engine::getSingleton().getLuaScripting();
			m_pScript = pLua->createScript();
		}

		String^ text = rtfScript->Text;
		pin_ptr <const wchar_t> str1 = PtrToStringChars(text);
		wstring temp(str1);
		string script(temp.begin(), temp.end());
		m_pScript->runScriptFromString(script);
	}
//----------------------------------------------------------------------
}