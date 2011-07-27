/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmMaterialLib.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMMATERIALLIB_H_
#define __INC_FRMMATERIALLIB_H_


#include "frmPropertyManager.h"
#include "frmFXs.h"
#include "MaterialWrapper.h"


namespace nGENEToolset
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace WeifenLuo::WinFormsUI::Docking;


	ref class MaterialWrapper;

	/// <summary>
	/// Summary for frmMaterialLib
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmMaterialLib : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		frmMaterialLib(frmPropertyManager^ propertyManager, frmFXs^ FX)
		{
			InitializeComponent();
			
			frmProperty = propertyManager;
			frmFX = FX;

			m_AllMaterials = gcnew Hashtable();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmMaterialLib()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TreeView^  trvMaterials;
	private: frmPropertyManager^ frmProperty;
	private: frmFXs^ frmFX;
	private: Hashtable^ m_AllMaterials;
	private: System::Windows::Forms::ContextMenuStrip^  cmnMaterial;
	private: System::Windows::Forms::ToolStripMenuItem^  addNewMaterialToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  applyToTheSelectedObjectToolStripMenuItem;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Label^  lblLibrary;
	private: System::Windows::Forms::ComboBox^  cmbLibrary;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuLoadLibrary;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuApplyToScene;
	private: System::ComponentModel::IContainer^  components;
	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->trvMaterials = (gcnew System::Windows::Forms::TreeView());
			this->cmnMaterial = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->addNewMaterialToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->applyToTheSelectedObjectToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mnuLoadLibrary = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->cmbLibrary = (gcnew System::Windows::Forms::ComboBox());
			this->lblLibrary = (gcnew System::Windows::Forms::Label());
			this->mnuApplyToScene = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cmnMaterial->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// trvMaterials
			// 
			this->tableLayoutPanel1->SetColumnSpan(this->trvMaterials, 2);
			this->trvMaterials->ContextMenuStrip = this->cmnMaterial;
			this->trvMaterials->Dock = System::Windows::Forms::DockStyle::Fill;
			this->trvMaterials->Location = System::Drawing::Point(3, 30);
			this->trvMaterials->Name = L"trvMaterials";
			this->trvMaterials->Size = System::Drawing::Size(278, 430);
			this->trvMaterials->TabIndex = 0;
			this->trvMaterials->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &frmMaterialLib::trvMaterials_AfterSelect);
			// 
			// cmnMaterial
			// 
			this->cmnMaterial->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->addNewMaterialToolStripMenuItem, 
				this->applyToTheSelectedObjectToolStripMenuItem, this->mnuApplyToScene, this->toolStripSeparator1, this->mnuLoadLibrary});
			this->cmnMaterial->Name = L"cmnMaterial";
			this->cmnMaterial->Size = System::Drawing::Size(222, 120);
			// 
			// addNewMaterialToolStripMenuItem
			// 
			this->addNewMaterialToolStripMenuItem->Name = L"addNewMaterialToolStripMenuItem";
			this->addNewMaterialToolStripMenuItem->Size = System::Drawing::Size(221, 22);
			this->addNewMaterialToolStripMenuItem->Text = L"Add new material";
			// 
			// applyToTheSelectedObjectToolStripMenuItem
			// 
			this->applyToTheSelectedObjectToolStripMenuItem->Name = L"applyToTheSelectedObjectToolStripMenuItem";
			this->applyToTheSelectedObjectToolStripMenuItem->Size = System::Drawing::Size(221, 22);
			this->applyToTheSelectedObjectToolStripMenuItem->Text = L"Apply to the selected object";
			this->applyToTheSelectedObjectToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMaterialLib::applyToTheSelectedObjectToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(218, 6);
			// 
			// mnuLoadLibrary
			// 
			this->mnuLoadLibrary->Name = L"mnuLoadLibrary";
			this->mnuLoadLibrary->Size = System::Drawing::Size(221, 22);
			this->mnuLoadLibrary->Text = L"Load library";
			this->mnuLoadLibrary->Click += gcnew System::EventHandler(this, &frmMaterialLib::mnuLoadLibrary_Click);
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				28.52113F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				71.47887F)));
			this->tableLayoutPanel1->Controls->Add(this->trvMaterials, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->cmbLibrary, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->lblLibrary, 0, 0);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(284, 463);
			this->tableLayoutPanel1->TabIndex = 1;
			// 
			// cmbLibrary
			// 
			this->cmbLibrary->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbLibrary->FormattingEnabled = true;
			this->cmbLibrary->Location = System::Drawing::Point(84, 3);
			this->cmbLibrary->Name = L"cmbLibrary";
			this->cmbLibrary->Size = System::Drawing::Size(197, 21);
			this->cmbLibrary->Sorted = true;
			this->cmbLibrary->TabIndex = 2;
			this->cmbLibrary->SelectedIndexChanged += gcnew System::EventHandler(this, &frmMaterialLib::cmbLibrary_SelectedIndexChanged);
			// 
			// lblLibrary
			// 
			this->lblLibrary->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->lblLibrary->AutoSize = true;
			this->lblLibrary->Location = System::Drawing::Point(3, 7);
			this->lblLibrary->Name = L"lblLibrary";
			this->lblLibrary->Size = System::Drawing::Size(38, 13);
			this->lblLibrary->TabIndex = 1;
			this->lblLibrary->Text = L"Library";
			// 
			// mnuApplyToScene
			// 
			this->mnuApplyToScene->Name = L"mnuApplyToScene";
			this->mnuApplyToScene->Size = System::Drawing::Size(221, 22);
			this->mnuApplyToScene->Text = L"Apply to scene";
			this->mnuApplyToScene->Click += gcnew System::EventHandler(this, &frmMaterialLib::mnuApplyToScene_Click);
			// 
			// frmMaterialLib
			// 
			this->AllowEndUserDocking = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 463);
			this->Controls->Add(this->tableLayoutPanel1);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>((((WeifenLuo::WinFormsUI::Docking::DockAreas::Float
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockLeft) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockRight) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::Document));
			this->HideOnClose = true;
			this->Name = L"frmMaterialLib";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockLeft;
			this->Text = L"Materials";
			this->TabText = L"Materials";
			this->cmnMaterial->ResumeLayout(false);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	public: System::Void addMaterial(MaterialWrapper^ _material, Hashtable^ _matlib);

	private: System::Void trvMaterials_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
			 {
				 Hashtable^ lib = (Hashtable^)m_AllMaterials[cmbLibrary->SelectedItem->ToString()];
				 MaterialWrapper^ material = (MaterialWrapper^)lib[e->Node->Name];
				 frmProperty->SelectedObject = material;

				 if(material->PostProcess)
				 {
					 applyToTheSelectedObjectToolStripMenuItem->Enabled = false;
					 mnuApplyToScene->Enabled = true;
				 }
				 else
				 {
					 applyToTheSelectedObjectToolStripMenuItem->Enabled = true;
					 mnuApplyToScene->Enabled = false;
				 }
			 }
	private: System::Void applyToTheSelectedObjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);

	public: System::Void addLibrary(const wstring& _name);

private: System::Void mnuLoadLibrary_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void cmbLibrary_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 trvMaterials->Nodes->Clear();
			 trvMaterials->BeginUpdate();
				Hashtable^ lib = (Hashtable^)m_AllMaterials[cmbLibrary->SelectedItem->ToString()];
				for each(MaterialWrapper^ m in lib->Values)
				{
					trvMaterials->Nodes->Add(m->Key, m->Name);
				}
				trvMaterials->Sort();
			 trvMaterials->EndUpdate();
		 }
private: System::Void mnuApplyToScene_Click(System::Object^  sender, System::EventArgs^  e);
};
}


#endif