/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmPropertyManager.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMPROPERTYMANAGER_H_
#define __INC_FRMPROPERTYMANAGER_H_


namespace nGENEToolset
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace WeifenLuo::WinFormsUI::Docking;


	/** Properties window.
	*/
	public ref class frmPropertyManager : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		frmPropertyManager(void)
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
		~frmPropertyManager()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PropertyGrid^  prgProperties;
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
			this->prgProperties = (gcnew System::Windows::Forms::PropertyGrid());
			this->SuspendLayout();
			// 
			// prgProperties
			// 
			this->prgProperties->Location = System::Drawing::Point(0, 0);
			this->prgProperties->Name = L"prgProperties";
			this->prgProperties->Size = System::Drawing::Size(274, 389);
			this->prgProperties->TabIndex = 0;
			// 
			// frmPropertyManager
			// 
			this->AllowEndUserDocking = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(274, 389);
			this->Controls->Add(this->prgProperties);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>(((((WeifenLuo::WinFormsUI::Docking::DockAreas::Float | WeifenLuo::WinFormsUI::Docking::DockAreas::DockLeft) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockRight) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockTop) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockBottom));
			this->HideOnClose = true;
			this->Name = L"frmPropertyManager";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockRight;
			this->Text = L"Properties";
			this->TabText = L"Properties";
			this->Resize += gcnew System::EventHandler(this, &frmPropertyManager::frmPropertyManager_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion
	private:
			 System::Void frmPropertyManager_Resize(System::Object^ sender, System::EventArgs^ e)
			 {
				 prgProperties->Height = this->Height;
				 prgProperties->Width = this->Width;

				 prgProperties->Top = 0;
				 prgProperties->Left = 0;
			 }

	public: 
			[Browsable(false)]
			property System::Object^ SelectedObject
			{
				System::Object^ get()
				{
					return prgProperties->SelectedObject;
				}

				void set(System::Object^ object)
				{
					prgProperties->SelectedObject = object;
				}
			}
	};
}


#endif