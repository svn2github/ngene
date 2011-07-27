/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmToolbox.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMTOOLBOX_H_
#define __INC_FRMTOOLBOX_H_


namespace nGENEToolset {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace WeifenLuo::WinFormsUI::Docking;

	/// <summary>
	/// Summary for frmToolbox
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmToolbox : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		frmToolbox(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmToolbox()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// frmToolbox
			// 
			this->AllowEndUserDocking = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(219, 364);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>(((WeifenLuo::WinFormsUI::Docking::DockAreas::Float
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockLeft) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockRight));
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"frmToolbox";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockLeft;
			this->Text = L"Toolbox";
			this->TabText = L"Toolbox";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}


#endif