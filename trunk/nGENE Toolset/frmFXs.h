/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmFXs.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRM_FXS_H_
#define __INC_FRM_FXS_H_

#include "MaterialWrapper.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace WeifenLuo::WinFormsUI::Docking;


namespace nGENEToolset
{

	/// <summary>
	/// Summary for frmFXs
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmFXs : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		frmFXs(void)
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
		~frmFXs()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  lstEffects;
	private: System::Windows::Forms::ToolStripContainer^  toolStripContainer1;
	private: System::Windows::Forms::ToolStrip^  tsbMove;
	private: System::Windows::Forms::ToolStripButton^  btnUp;


	private: System::Windows::Forms::ToolStripButton^  btnDown;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripButton^  btnRemove;




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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmFXs::typeid));
			this->lstEffects = (gcnew System::Windows::Forms::ListBox());
			this->toolStripContainer1 = (gcnew System::Windows::Forms::ToolStripContainer());
			this->tsbMove = (gcnew System::Windows::Forms::ToolStrip());
			this->btnUp = (gcnew System::Windows::Forms::ToolStripButton());
			this->btnDown = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->btnRemove = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripContainer1->ContentPanel->SuspendLayout();
			this->toolStripContainer1->RightToolStripPanel->SuspendLayout();
			this->toolStripContainer1->SuspendLayout();
			this->tsbMove->SuspendLayout();
			this->SuspendLayout();
			// 
			// lstEffects
			// 
			this->lstEffects->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lstEffects->FormattingEnabled = true;
			this->lstEffects->Location = System::Drawing::Point(0, 0);
			this->lstEffects->Name = L"lstEffects";
			this->lstEffects->Size = System::Drawing::Size(247, 264);
			this->lstEffects->TabIndex = 0;
			this->lstEffects->SelectedIndexChanged += gcnew System::EventHandler(this, &frmFXs::lstEffects_SelectedIndexChanged);
			// 
			// toolStripContainer1
			// 
			this->toolStripContainer1->BottomToolStripPanelVisible = false;
			// 
			// toolStripContainer1.ContentPanel
			// 
			this->toolStripContainer1->ContentPanel->Controls->Add(this->lstEffects);
			this->toolStripContainer1->ContentPanel->Size = System::Drawing::Size(247, 264);
			this->toolStripContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->toolStripContainer1->LeftToolStripPanelVisible = false;
			this->toolStripContainer1->Location = System::Drawing::Point(0, 0);
			this->toolStripContainer1->Name = L"toolStripContainer1";
			// 
			// toolStripContainer1.RightToolStripPanel
			// 
			this->toolStripContainer1->RightToolStripPanel->Controls->Add(this->tsbMove);
			this->toolStripContainer1->Size = System::Drawing::Size(284, 264);
			this->toolStripContainer1->TabIndex = 1;
			this->toolStripContainer1->Text = L"toolStripContainer1";
			this->toolStripContainer1->TopToolStripPanelVisible = false;
			// 
			// tsbMove
			// 
			this->tsbMove->Dock = System::Windows::Forms::DockStyle::None;
			this->tsbMove->ImageScalingSize = System::Drawing::Size(32, 32);
			this->tsbMove->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->btnUp, this->btnDown, 
				this->toolStripSeparator1, this->btnRemove});
			this->tsbMove->Location = System::Drawing::Point(0, 10);
			this->tsbMove->Name = L"tsbMove";
			this->tsbMove->Size = System::Drawing::Size(37, 153);
			this->tsbMove->TabIndex = 0;
			// 
			// btnUp
			// 
			this->btnUp->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->btnUp->Enabled = false;
			this->btnUp->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnUp.Image")));
			this->btnUp->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnUp->Name = L"btnUp";
			this->btnUp->Size = System::Drawing::Size(35, 36);
			this->btnUp->Text = L"toolStripButton1";
			this->btnUp->ToolTipText = L"Move effect up";
			this->btnUp->Click += gcnew System::EventHandler(this, &frmFXs::btnUp_Click);
			// 
			// btnDown
			// 
			this->btnDown->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->btnDown->Enabled = false;
			this->btnDown->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnDown.Image")));
			this->btnDown->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnDown->Name = L"btnDown";
			this->btnDown->Size = System::Drawing::Size(35, 36);
			this->btnDown->Text = L"toolStripButton2";
			this->btnDown->ToolTipText = L"Move effect down";
			this->btnDown->Click += gcnew System::EventHandler(this, &frmFXs::btnDown_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(35, 6);
			// 
			// btnRemove
			// 
			this->btnRemove->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->btnRemove->Enabled = false;
			this->btnRemove->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnRemove.Image")));
			this->btnRemove->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnRemove->Name = L"btnRemove";
			this->btnRemove->Size = System::Drawing::Size(35, 36);
			this->btnRemove->Text = L"toolStripButton1";
			this->btnRemove->ToolTipText = L"Remove selected effect";
			this->btnRemove->Click += gcnew System::EventHandler(this, &frmFXs::btnRemove_Click);
			// 
			// frmFXs
			// 
			this->AllowEndUserDocking = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 264);
			this->Controls->Add(this->toolStripContainer1);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>(((((WeifenLuo::WinFormsUI::Docking::DockAreas::Float | WeifenLuo::WinFormsUI::Docking::DockAreas::DockLeft) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockRight) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockTop) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockBottom));
			this->HideOnClose = true;
			this->Name = L"frmFXs";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockLeft;
			this->Text = L"Scene Effects";
			this->toolStripContainer1->ContentPanel->ResumeLayout(false);
			this->toolStripContainer1->RightToolStripPanel->ResumeLayout(false);
			this->toolStripContainer1->RightToolStripPanel->PerformLayout();
			this->toolStripContainer1->ResumeLayout(false);
			this->toolStripContainer1->PerformLayout();
			this->tsbMove->ResumeLayout(false);
			this->tsbMove->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

		public: System::Void addEffect(FXWrapper^ fx)
				{
					for(int i = 0; i < lstEffects->Items->Count; ++i)
					{
						FXWrapper^ mat = (FXWrapper^)lstEffects->Items[i];
						mat->Priority += 1;
					}
					
					this->lstEffects->Items->Insert(0, fx);

					((nGENE::PostStage*)nGENE::Renderer::getSingleton().getRenderStage(L"PostProcess"))->sortMaterials();
				}
	private: System::Void lstEffects_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(lstEffects->SelectedIndex >= 0)
				 {
					 if(lstEffects->SelectedIndex < lstEffects->Items->Count - 1)
						 btnDown->Enabled = true;
					 else
						 btnDown->Enabled = false;

					 if(lstEffects->SelectedIndex > 0)
						 btnUp->Enabled = true;
					 else
						 btnUp->Enabled = false;

					 btnRemove->Enabled = true;
				 }
			 }
private: System::Void btnUp_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 int index = lstEffects->SelectedIndex;
			 if(index != -1)
			 {
				Object^ obj = lstEffects->SelectedItem;
				lstEffects->Items->RemoveAt(index);
				lstEffects->Items->Insert(index - 1, obj);
				lstEffects->SelectedItem = obj;

				FXWrapper^ mat = (FXWrapper^)obj;
				if(mat->Priority > 0)
					mat->Priority -= 1;

				mat = (FXWrapper^)lstEffects->Items[index];
				mat->Priority += 1;

				((nGENE::PostStage*)nGENE::Renderer::getSingleton().getRenderStage(L"PostProcess"))->sortMaterials();
			 }
		 }
private: System::Void btnDown_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 int index = lstEffects->SelectedIndex;
			 if(index != -1)
			 {
				Object^ obj = lstEffects->SelectedItem;
				lstEffects->Items->RemoveAt(index);
				lstEffects->Items->Insert(index + 1, obj);
				lstEffects->SelectedItem = obj;

				FXWrapper^ mat = (FXWrapper^)obj;
				mat->Priority += 1;

				mat = (FXWrapper^)lstEffects->Items[index];
				if(mat->Priority > 0)
					mat->Priority -= 1;

				((nGENE::PostStage*)nGENE::Renderer::getSingleton().getRenderStage(L"PostProcess"))->sortMaterials();
			 }
		 }
private: System::Void btnRemove_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 int index = lstEffects->SelectedIndex;
			 for(int i = index + 1; i < lstEffects->Items->Count; ++i)
			 {
				 FXWrapper^ mat = (FXWrapper^)lstEffects->Items[i];
				 mat->Priority -= 1;
			 }

			 FXWrapper^ mat = (FXWrapper^)lstEffects->Items[index];
			 ((nGENE::PostStage*)nGENE::Renderer::getSingleton().getRenderStage(L"PostProcess"))->removeFromRender(mat->getMaterial());

			 lstEffects->Items->RemoveAt(index);

			 ((nGENE::PostStage*)nGENE::Renderer::getSingleton().getRenderStage(L"PostProcess"))->sortMaterials();

			 if(!lstEffects->Items->Count)
				 btnRemove->Enabled = false;
			 else
				 lstEffects->SelectedIndex = 0;
		 }

public: System::Void cleanup()
		{
			if(lstEffects->Items->Count > 0)
			{
				for(int i = 0; i < lstEffects->Items->Count; ++i)
				{
					 FXWrapper^ mat = (FXWrapper^)lstEffects->Items[i];
					 ((nGENE::PostStage*)nGENE::Renderer::getSingleton().getRenderStage(L"PostProcess"))->removeFromRender(mat->getMaterial());
				}

				lstEffects->Items->Clear();
			}
		}
};
}


#endif