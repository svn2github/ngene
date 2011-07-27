/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmAbout.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMABOUT_H_
#define __INC_FRMABOUT_H_


namespace nGENEToolset
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for frmAbout
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmAbout : public System::Windows::Forms::Form
	{
	public:
		frmAbout(void)
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
		~frmAbout()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  cmdOK;
	protected: 
	private: System::Windows::Forms::PictureBox^  pctLogo;
	private: System::Windows::Forms::Label^  label1;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmAbout::typeid));
			this->cmdOK = (gcnew System::Windows::Forms::Button());
			this->pctLogo = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pctLogo))->BeginInit();
			this->SuspendLayout();
			// 
			// cmdOK
			// 
			this->cmdOK->Location = System::Drawing::Point(470, 253);
			this->cmdOK->Name = L"cmdOK";
			this->cmdOK->Size = System::Drawing::Size(75, 23);
			this->cmdOK->TabIndex = 0;
			this->cmdOK->Text = L"OK";
			this->cmdOK->UseVisualStyleBackColor = true;
			this->cmdOK->Click += gcnew System::EventHandler(this, &frmAbout::cmdOK_Click);
			// 
			// pctLogo
			// 
			this->pctLogo->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pctLogo.Image")));
			this->pctLogo->InitialImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pctLogo.InitialImage")));
			this->pctLogo->Location = System::Drawing::Point(12, 2);
			this->pctLogo->Name = L"pctLogo";
			this->pctLogo->Size = System::Drawing::Size(533, 245);
			this->pctLogo->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pctLogo->TabIndex = 1;
			this->pctLogo->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 263);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(63, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Version: 0.1";
			// 
			// frmAbout
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(557, 287);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pctLogo);
			this->Controls->Add(this->cmdOK);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"frmAbout";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"About nGENE Tech Editor";
			this->TopMost = true;
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pctLogo))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void cmdOK_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->Hide();
			 }
	};
}


#endif