/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmOutput.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMOUTPUT_H_
#define __INC_FRMOUTPUT_H_


#include "Logger.h"


namespace nGENEToolset
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace WeifenLuo::WinFormsUI::Docking;

	using System::String;

	public delegate void DelegateAddMsg(String^ str, int type);


	/// <summary>
	/// Summary for frmOutput
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmOutput : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	private:
		ToolsetLogManaged^ logger;
		DelegateAddMsg^ addMsgDelegate;

	public:
		frmOutput(void)
		{
			InitializeComponent();

			addMsgDelegate = gcnew DelegateAddMsg(this, &frmOutput::addMsg);

			logger = gcnew ToolsetLogManaged();
			logger->log += gcnew LogEventHandler(this, &frmOutput::frmOutput_Log);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmOutput()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  txtLog;
	protected: 

	protected: 





























	protected: 















	protected: 

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
			this->txtLog = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// txtLog
			// 
			this->txtLog->Dock = System::Windows::Forms::DockStyle::Fill;
			this->txtLog->Location = System::Drawing::Point(0, 0);
			this->txtLog->Multiline = true;
			this->txtLog->Name = L"txtLog";
			this->txtLog->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtLog->Size = System::Drawing::Size(721, 209);
			this->txtLog->TabIndex = 0;
			// 
			// frmOutput
			// 
			this->AllowEndUserDocking = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(721, 209);
			this->Controls->Add(this->txtLog);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>(((WeifenLuo::WinFormsUI::Docking::DockAreas::Float
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockTop) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockBottom));
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"frmOutput";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockBottom;
			this->Text = L"Output";
			this->TabText = L"Output";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		System::Void frmOutput_Log(String^ str, int type)
		{
			addMsgDelegate(str, type);
		}

	public:
		System::Void addMsg(String^ str, int type)
		{
			try
			{
				this->txtLog->Text += str;
				this->txtLog->Select(this->txtLog->Text->Length, 1);
				this->txtLog->ScrollToCaret();
			}
			catch (Exception^ e)
			{
				System::Console::WriteLine(e->Message);
			}
		}
};
}


#endif