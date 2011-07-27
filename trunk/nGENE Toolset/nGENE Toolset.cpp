/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		nGENE Toolset.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "frmMain.h"

using namespace nGENEToolset;


int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	System::Windows::Forms::Application::Run(gcnew frmMain());
	
	return 0;
}
