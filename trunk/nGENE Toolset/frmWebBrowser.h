/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmWebBrowser.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMWEBBROWSER_H_
#define __INC_FRMWEBBROWSER_H_


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace nGENEToolset {

	/// <summary>
	/// Summary for frmWebBrowser
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmWebBrowser : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		frmWebBrowser(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmWebBrowser()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::WebBrowser^  webBrowser;
	protected: 
		virtual void OnPaint(PaintEventArgs^ e) override
		{
			DockContent::OnPaint(e);
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
			this->webBrowser = (gcnew System::Windows::Forms::WebBrowser());
			this->SuspendLayout();
			// 
			// webBrowser
			// 
			this->webBrowser->Dock = System::Windows::Forms::DockStyle::Fill;
			this->webBrowser->Location = System::Drawing::Point(0, 0);
			this->webBrowser->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->webBrowser->MinimumSize = System::Drawing::Size(15, 16);
			this->webBrowser->Name = L"webBrowser";
			this->webBrowser->Size = System::Drawing::Size(715, 489);
			this->webBrowser->TabIndex = 0;
			this->webBrowser->Url = (gcnew System::Uri(L"http://ngene.wikidot.com", System::UriKind::Absolute));
			// 
			// frmWebBrowser
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(715, 489);
			this->Controls->Add(this->webBrowser);
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"frmWebBrowser";
			this->Text = L"Web Browser";
			this->TabText = L"Web Browser";
			this->Load += gcnew System::EventHandler(this, &frmWebBrowser::frmWebBrowser_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void frmWebBrowser_Load(System::Object^  sender, System::EventArgs^  e) {
			 }

    public: System::Void setURL(String^ url)
			{
				this->webBrowser->Url = (gcnew System::Uri(url, System::UriKind::Absolute));
			}
	};
}


#endif