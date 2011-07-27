/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmMaterialEditor.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMMATERIALEDITOR_H_
#define __INC_FRMMATERIALEDITOR_H_


namespace nGENEToolset {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Windows::Forms;

	using namespace WeifenLuo::WinFormsUI::Docking;

	/// <summary>
	/// Summary for frmMaterialEditor
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmMaterialEditor : WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		frmMaterialEditor(void)
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
		~frmMaterialEditor()
		{
			if (components)
			{
				delete components;
			}
		}


	private: System::Windows::Forms::ToolTip^  tipFunctions;
	private: System::Windows::Forms::ContextMenuStrip^  cmnuFunctions;


	private: System::Windows::Forms::ToolStripMenuItem^  createNewFunctionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  removeFunctionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadFunctionsSetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuCreateCategory;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuRemoveCategory;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  viewCodeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveFunctionsSetToolStripMenuItem;
	private: System::Windows::Forms::ImageList^  imgList;
	private: System::Windows::Forms::TreeView^  trvFunctions;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  tsbBuildMaterial;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
	private: nGENEMaterialEditorControl::nGENEMaterialEditorControlControl^  nGENEMaterialEditorControlControl1;
	private: nGENEMaterialEditorControl::MaterialFunction^  materialFunction1;










	private: System::ComponentModel::IContainer^  components;
	protected: 

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmMaterialEditor::typeid));
			this->cmnuFunctions = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->mnuCreateCategory = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuRemoveCategory = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->createNewFunctionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->removeFunctionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->viewCodeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->loadFunctionsSetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveFunctionsSetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imgList = (gcnew System::Windows::Forms::ImageList(this->components));
			this->tipFunctions = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->trvFunctions = (gcnew System::Windows::Forms::TreeView());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->tsbBuildMaterial = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->nGENEMaterialEditorControlControl1 = (gcnew nGENEMaterialEditorControl::nGENEMaterialEditorControlControl());
			this->materialFunction1 = (gcnew nGENEMaterialEditorControl::MaterialFunction());
			this->cmnuFunctions->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->nGENEMaterialEditorControlControl1->SuspendLayout();
			this->SuspendLayout();
			// 
			// cmnuFunctions
			// 
			this->cmnuFunctions->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->mnuCreateCategory, 
				this->mnuRemoveCategory, this->toolStripSeparator1, this->createNewFunctionToolStripMenuItem, this->removeFunctionToolStripMenuItem, 
				this->toolStripSeparator3, this->viewCodeToolStripMenuItem, this->toolStripSeparator2, this->loadFunctionsSetToolStripMenuItem, 
				this->saveFunctionsSetToolStripMenuItem});
			this->cmnuFunctions->Name = L"cmnuFunctions";
			this->cmnuFunctions->Size = System::Drawing::Size(183, 176);
			this->cmnuFunctions->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &frmMaterialEditor::contextMenuStrip1_Opening);
			// 
			// mnuCreateCategory
			// 
			this->mnuCreateCategory->Name = L"mnuCreateCategory";
			this->mnuCreateCategory->Size = System::Drawing::Size(182, 22);
			this->mnuCreateCategory->Text = L"Create new category";
			this->mnuCreateCategory->Click += gcnew System::EventHandler(this, &frmMaterialEditor::mnuCreateCategory_Click);
			// 
			// mnuRemoveCategory
			// 
			this->mnuRemoveCategory->Name = L"mnuRemoveCategory";
			this->mnuRemoveCategory->Size = System::Drawing::Size(182, 22);
			this->mnuRemoveCategory->Text = L"Remove category";
			this->mnuRemoveCategory->Click += gcnew System::EventHandler(this, &frmMaterialEditor::mnuRemoveCategory_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(179, 6);
			// 
			// createNewFunctionToolStripMenuItem
			// 
			this->createNewFunctionToolStripMenuItem->Name = L"createNewFunctionToolStripMenuItem";
			this->createNewFunctionToolStripMenuItem->Size = System::Drawing::Size(182, 22);
			this->createNewFunctionToolStripMenuItem->Text = L"Create new function";
			// 
			// removeFunctionToolStripMenuItem
			// 
			this->removeFunctionToolStripMenuItem->Name = L"removeFunctionToolStripMenuItem";
			this->removeFunctionToolStripMenuItem->Size = System::Drawing::Size(182, 22);
			this->removeFunctionToolStripMenuItem->Text = L"Remove function";
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(179, 6);
			// 
			// viewCodeToolStripMenuItem
			// 
			this->viewCodeToolStripMenuItem->Name = L"viewCodeToolStripMenuItem";
			this->viewCodeToolStripMenuItem->Size = System::Drawing::Size(182, 22);
			this->viewCodeToolStripMenuItem->Text = L"View code";
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(179, 6);
			// 
			// loadFunctionsSetToolStripMenuItem
			// 
			this->loadFunctionsSetToolStripMenuItem->Name = L"loadFunctionsSetToolStripMenuItem";
			this->loadFunctionsSetToolStripMenuItem->Size = System::Drawing::Size(182, 22);
			this->loadFunctionsSetToolStripMenuItem->Text = L"Load functions set";
			// 
			// saveFunctionsSetToolStripMenuItem
			// 
			this->saveFunctionsSetToolStripMenuItem->Name = L"saveFunctionsSetToolStripMenuItem";
			this->saveFunctionsSetToolStripMenuItem->Size = System::Drawing::Size(182, 22);
			this->saveFunctionsSetToolStripMenuItem->Text = L"Save functions set";
			// 
			// imgList
			// 
			this->imgList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imgList.ImageStream")));
			this->imgList->TransparentColor = System::Drawing::Color::Transparent;
			this->imgList->Images->SetKeyName(0, L"folder-closed_16.png");
			this->imgList->Images->SetKeyName(1, L"folder-open_16.png");
			// 
			// tipFunctions
			// 
			this->tipFunctions->IsBalloon = true;
			// 
			// trvFunctions
			// 
			this->trvFunctions->ContextMenuStrip = this->cmnuFunctions;
			this->trvFunctions->Dock = System::Windows::Forms::DockStyle::Fill;
			this->trvFunctions->ImageIndex = 0;
			this->trvFunctions->ImageList = this->imgList;
			this->trvFunctions->LabelEdit = true;
			this->trvFunctions->Location = System::Drawing::Point(0, 0);
			this->trvFunctions->Margin = System::Windows::Forms::Padding(2);
			this->trvFunctions->Name = L"trvFunctions";
			this->trvFunctions->SelectedImageIndex = 1;
			this->trvFunctions->Size = System::Drawing::Size(237, 571);
			this->trvFunctions->TabIndex = 0;
			this->trvFunctions->AfterLabelEdit += gcnew System::Windows::Forms::NodeLabelEditEventHandler(this, &frmMaterialEditor::trvFunctions_AfterLabelEdit);
			this->trvFunctions->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &frmMaterialEditor::trvFunctions_AfterSelect);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->AllowDrop = true;
			this->splitContainer1->Panel1->Controls->Add(this->nGENEMaterialEditorControlControl1);
			this->splitContainer1->Panel1->Controls->Add(this->toolStrip1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->trvFunctions);
			this->splitContainer1->Size = System::Drawing::Size(779, 571);
			this->splitContainer1->SplitterDistance = 539;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 0;
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->tsbBuildMaterial, 
				this->toolStripSeparator4});
			this->toolStrip1->Location = System::Drawing::Point(0, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(539, 25);
			this->toolStrip1->TabIndex = 0;
			this->toolStrip1->Text = L"tlsMaterialEditor";
			// 
			// tsbBuildMaterial
			// 
			this->tsbBuildMaterial->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->tsbBuildMaterial->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbBuildMaterial.Image")));
			this->tsbBuildMaterial->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbBuildMaterial->Name = L"tsbBuildMaterial";
			this->tsbBuildMaterial->Size = System::Drawing::Size(38, 22);
			this->tsbBuildMaterial->Text = L"Build";
			this->tsbBuildMaterial->ToolTipText = L"Builds material";
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(6, 25);
			// 
			// nGENEMaterialEditorControlControl1
			// 
			this->nGENEMaterialEditorControlControl1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->nGENEMaterialEditorControlControl1->Controls->Add(this->materialFunction1);
			this->nGENEMaterialEditorControlControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->nGENEMaterialEditorControlControl1->Functions = (cli::safe_cast<System::Collections::ArrayList^  >(resources->GetObject(L"nGENEMaterialEditorControlControl1.Functions")));
			this->nGENEMaterialEditorControlControl1->Location = System::Drawing::Point(0, 25);
			this->nGENEMaterialEditorControlControl1->Name = L"nGENEMaterialEditorControlControl1";
			this->nGENEMaterialEditorControlControl1->Size = System::Drawing::Size(539, 546);
			this->nGENEMaterialEditorControlControl1->TabIndex = 1;
			// 
			// materialFunction1
			// 
			this->materialFunction1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->materialFunction1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->materialFunction1->Code = nullptr;
			this->materialFunction1->FunctionName = L"MaterialFunction";
			this->materialFunction1->InputsNum = 0;
			this->materialFunction1->Location = System::Drawing::Point(178, 187);
			this->materialFunction1->Name = L"materialFunction1";
			this->materialFunction1->Size = System::Drawing::Size(185, 143);
			this->materialFunction1->TabIndex = 0;
			this->materialFunction1->Texture = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"materialFunction1.Texture")));
			this->materialFunction1->Type = nGENEMaterialEditorControl::FunctionType::Texture;
			// 
			// frmMaterialEditor
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(779, 571);
			this->Controls->Add(this->splitContainer1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"frmMaterialEditor";
			this->TabText = L"Material Editor";
			this->Text = L"Material Editor";
			this->Load += gcnew System::EventHandler(this, &frmMaterialEditor::frmMaterialEditor_Load);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &frmMaterialEditor::frmMaterialEditor_Closing);
			this->cmnuFunctions->ResumeLayout(false);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->nGENEMaterialEditorControlControl1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void frmMaterialEditor_Load(System::Object^  sender, System::EventArgs^  e)
			 {
				this->tipFunctions->SetToolTip(trvFunctions, "List of available functions");

				loadFunctionsCategories();
			 }

private: System::Void frmMaterialEditor_Closing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
		 {
			 try
			 {
				 StreamWriter^ writer = gcnew StreamWriter("categories.txt");
				 
				 System::Collections::IEnumerator^ allNodes =
					 (safe_cast<System::Collections::IEnumerable^>(trvFunctions->Nodes)->GetEnumerator());
				 try
				 {
					 while(allNodes->MoveNext())
					 {
						 TreeNode^ pNode = safe_cast<TreeNode^>(allNodes->Current);
						 writer->WriteLine(pNode->Text);
					 }
				 }
				 finally
				 {
					 if(writer)
						 delete (IDisposable^)writer;
					 /// @todo Uncomment this.
					 //IDisposable^ pDisposable = static_cast<System::IDisposable^>(allNodes);
					 //if(pDisposable != nullptr)
					 //		pDisposable->Dispose();
				 }
			 }
			 catch(Exception^ exception)
			 {
				 MessageBox::Show("Writing to file failed:\n" + exception->Message);
			 }
		 }
private: System::Void contextMenuStrip1_Opening(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
		 {
		 }
private: System::Void trvFunctions_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		 {
		 }

private: System::Void loadFunctionsCategories()
		 {
			 try
			 {
				 StreamReader^ reader = gcnew StreamReader("categories.txt");
				 try
				 {
					String^ line;

					trvFunctions->BeginUpdate();

					while(line)
					{
						line = reader->ReadLine();
						trvFunctions->Nodes->Add(line);
					}

					trvFunctions->EndUpdate();
				 }
				 finally
				 {
					 if(reader)
						 delete (IDisposable^)reader;
				 }
			 }
			 catch(Exception^ exception)
			 {
				 MessageBox::Show("The file could not be read:\n" + exception->Message);
			 }
		 }
private: System::Void mnuCreateCategory_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 trvFunctions->BeginUpdate();

			 trvFunctions->Nodes->Add("New category");
			 trvFunctions->Nodes[trvFunctions->Nodes->Count - 1]->BeginEdit();

			 trvFunctions->EndUpdate();
		 }

		 System::Void trvFunctions_AfterLabelEdit(System::Object^ sender, System::Windows::Forms::NodeLabelEditEventArgs^ e)
		 {
			 if(e->Label != nullptr)
			 {
				 if(e->Label->Length > 0)
				 {
					 array <Char>^ notAllowed = {'@','.',',','!','?','&','*'};
					 if(e->Label->IndexOfAny(notAllowed) == -1)
					 {
						 e->Node->EndEdit(false);
					 }
					 else
					 {
						 e->CancelEdit = true;
						 MessageBox::Show("Invalid tree node label.\n",
							"The invalid characters are: '@','.',',','!','?','&','*'");
						 e->Node->BeginEdit();
					 }
				 }
				 else
				 {
					 e->CancelEdit = true;
					 MessageBox::Show("Category name cannot be blank");
					 e->Node->BeginEdit();
				 }
			 }
		 }
private: System::Void mnuRemoveCategory_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(trvFunctions->SelectedNode != nullptr)
			 {
				 System::Windows::Forms::DialogResult result;
				 result = MessageBox::Show("Are you sure to delete this category?", "Prompt",
					 System::Windows::Forms::MessageBoxButtons::YesNo,
					 System::Windows::Forms::MessageBoxIcon::Question,
					 System::Windows::Forms::MessageBoxDefaultButton::Button1);
				 if(result == System::Windows::Forms::DialogResult::Yes)
						trvFunctions->Nodes->Remove(trvFunctions->SelectedNode);
			 }
		 }
};
}


#endif
