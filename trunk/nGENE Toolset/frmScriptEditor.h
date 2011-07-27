/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScriptEditor.h
Version:	0.02
---------------------------------------------------------------------------
*/


#pragma once
#ifndef __INC_FRM_SCRIPT_EDITOR_H_
#define __INC_FRM_SCRIPT_EDITOR_H_

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Text::RegularExpressions;

using namespace WeifenLuo::WinFormsUI::Docking;

using nGENE::ScriptLua;


namespace nGENEToolset
{

	/// <summary>
	/// Summary for frmScriptEditor
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmScriptEditor : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	private:
		System::Collections::ArrayList^ keywords;
	private: System::Windows::Forms::ToolStripContainer^  toolStripContainer1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripButton^  btnClean;
			 ScriptLua* m_pScript;

	public:
		frmScriptEditor(void)
		{
			InitializeComponent();
			
			// Add all keywords
			keywords = gcnew System::Collections::ArrayList();
			keywords->Add("while");
			keywords->Add("until");
			keywords->Add("then");
			keywords->Add("return");
			keywords->Add("repeat");
			keywords->Add("or");
			keywords->Add("not");
			keywords->Add("nil");
			keywords->Add("do");
			keywords->Add("end");
			keywords->Add("function");
			keywords->Add("local");
			keywords->Add("if");
			keywords->Add("for");
			keywords->Add("elseif");
			keywords->Add("else");
			keywords->Add("break");
			keywords->Add("and");

			m_pScript = NULL;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmScriptEditor()
		{
			if (components)
			{
				delete components;
			}

			NGENE_DELETE(m_pScript);
		}
	private: System::Windows::Forms::RichTextBox^  rtfScript;
	private: System::Windows::Forms::ToolStrip^  tsbScript;
	private: System::Windows::Forms::ToolStripButton^  btnRunScript;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmScriptEditor::typeid));
			this->rtfScript = (gcnew System::Windows::Forms::RichTextBox());
			this->tsbScript = (gcnew System::Windows::Forms::ToolStrip());
			this->btnRunScript = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->btnClean = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripContainer1 = (gcnew System::Windows::Forms::ToolStripContainer());
			this->tsbScript->SuspendLayout();
			this->toolStripContainer1->ContentPanel->SuspendLayout();
			this->toolStripContainer1->TopToolStripPanel->SuspendLayout();
			this->toolStripContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// rtfScript
			// 
			this->rtfScript->AllowDrop = true;
			this->rtfScript->Dock = System::Windows::Forms::DockStyle::Fill;
			this->rtfScript->Location = System::Drawing::Point(0, 0);
			this->rtfScript->Name = L"rtfScript";
			this->rtfScript->ShowSelectionMargin = true;
			this->rtfScript->Size = System::Drawing::Size(626, 391);
			this->rtfScript->TabIndex = 1;
			this->rtfScript->Text = L"";
			this->rtfScript->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &frmScriptEditor::rtfScript_DragDrop);
			this->rtfScript->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &frmScriptEditor::rtfScript_DragEnter);
			this->rtfScript->TextChanged += gcnew System::EventHandler(this, &frmScriptEditor::rtfScript_TextChanged);
			// 
			// tsbScript
			// 
			this->tsbScript->Dock = System::Windows::Forms::DockStyle::None;
			this->tsbScript->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->btnRunScript, this->toolStripSeparator1, 
				this->btnClean});
			this->tsbScript->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Flow;
			this->tsbScript->Location = System::Drawing::Point(3, 0);
			this->tsbScript->Name = L"tsbScript";
			this->tsbScript->Size = System::Drawing::Size(53, 23);
			this->tsbScript->TabIndex = 2;
			this->tsbScript->Text = L"toolStrip1";
			// 
			// btnRunScript
			// 
			this->btnRunScript->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->btnRunScript->Enabled = false;
			this->btnRunScript->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnRunScript.Image")));
			this->btnRunScript->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnRunScript->Name = L"btnRunScript";
			this->btnRunScript->Size = System::Drawing::Size(23, 20);
			this->btnRunScript->Text = L"toolStripButton1";
			this->btnRunScript->ToolTipText = L"Run this script";
			this->btnRunScript->Click += gcnew System::EventHandler(this, &frmScriptEditor::btnRunScript_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(6, 23);
			// 
			// btnClean
			// 
			this->btnClean->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->btnClean->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnClean.Image")));
			this->btnClean->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnClean->Name = L"btnClean";
			this->btnClean->Size = System::Drawing::Size(23, 20);
			this->btnClean->Text = L"toolStripButton1";
			this->btnClean->ToolTipText = L"Clear all code";
			this->btnClean->Click += gcnew System::EventHandler(this, &frmScriptEditor::btnClean_Click);
			// 
			// toolStripContainer1
			// 
			this->toolStripContainer1->BottomToolStripPanelVisible = false;
			// 
			// toolStripContainer1.ContentPanel
			// 
			this->toolStripContainer1->ContentPanel->Controls->Add(this->rtfScript);
			this->toolStripContainer1->ContentPanel->Size = System::Drawing::Size(626, 391);
			this->toolStripContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->toolStripContainer1->LeftToolStripPanelVisible = false;
			this->toolStripContainer1->Location = System::Drawing::Point(0, 0);
			this->toolStripContainer1->Name = L"toolStripContainer1";
			this->toolStripContainer1->RightToolStripPanelVisible = false;
			this->toolStripContainer1->Size = System::Drawing::Size(626, 414);
			this->toolStripContainer1->TabIndex = 3;
			this->toolStripContainer1->Text = L"toolStripContainer1";
			// 
			// toolStripContainer1.TopToolStripPanel
			// 
			this->toolStripContainer1->TopToolStripPanel->Controls->Add(this->tsbScript);
			// 
			// frmScriptEditor
			// 
			this->AllowEndUserDocking = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(626, 414);
			this->Controls->Add(this->toolStripContainer1);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>((WeifenLuo::WinFormsUI::Docking::DockAreas::Float | WeifenLuo::WinFormsUI::Docking::DockAreas::Document));
			this->HideOnClose = true;
			this->Name = L"frmScriptEditor";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::Document;
			//this->TabText = L"Script Editor";
			this->Text = L"Script Editor";
			this->Load += gcnew System::EventHandler(this, &frmScriptEditor::frmScriptEditor_Load);
			this->tsbScript->ResumeLayout(false);
			this->tsbScript->PerformLayout();
			this->toolStripContainer1->ContentPanel->ResumeLayout(false);
			this->toolStripContainer1->TopToolStripPanel->ResumeLayout(false);
			this->toolStripContainer1->TopToolStripPanel->PerformLayout();
			this->toolStripContainer1->ResumeLayout(false);
			this->toolStripContainer1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void frmScriptEditor_Load(System::Object^  sender, System::EventArgs^  e) {
			 }

			 private: System::Void rtfScript_DragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
			 {
				 if(e->Data->GetDataPresent(DataFormats::FileDrop))
					 e->Effect = DragDropEffects::All;
				 else
					 e->Effect = DragDropEffects::None;
			 }

	private: System::Void rtfScript_DragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
			 {
				 if(e->Data->GetDataPresent(DataFormats::FileDrop))
				 {
					 array<String^>^ files = (array<String^>^)(e->Data->GetData(DataFormats::FileDrop));
					 if(files->Length > 1)
					 {
						 MessageBox::Show("Drop one file at a time",
										  "Error",
										  System::Windows::Forms::MessageBoxButtons::OK,
										  System::Windows::Forms::MessageBoxIcon::Error);
					 }
					 else
					 {
						 try
						 {
							 System::IO::FileInfo file(files[0]);
							 if(file.Extension->ToLower() == ".lua" || file.Extension->ToLower() == ".rb")
							 {
								 rtfScript->Text = "";
								 System::IO::StreamReader^ reader = gcnew System::IO::StreamReader(files[0]);
								 String^ text = reader->ReadToEnd();

								Regex^ r = gcnew Regex("\\n");
								cli::array<String^>^ lines = r->Split(text);
								for(int k = 0; k < lines->Length; ++k)
								{
									 String^ tmpLine = lines[k];
									 String^ line = nullptr;
									 String^ comment = nullptr;
									 int index = tmpLine->IndexOf("--");
									 if(index >= 0)
									 {
										 line = tmpLine->Substring(0, index);
										 comment = tmpLine->Substring(index);
									 }
									 else
										 line = tmpLine;

									 Regex^ r = gcnew Regex("([ \\n\\r\\t{}();])");
									 cli::array<String^>^ tokens = r->Split(line);
									 for(int j = 0; j < tokens->Length; ++j)
									 {
										 String^ token = tokens[j];
										 token->Trim();

										 // Set the token's default color and font.
										 rtfScript->SelectionColor = Color::Black;
										 rtfScript->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Regular);
										 

										 for(int i = 0; i < keywords->Count; i++)
										 {
											 if ((String^)keywords[i] == token->ToLower())
											 {
												 // Apply alternative color and font to highlight keyword.
												 rtfScript->SelectionColor = Color::Blue;
												 rtfScript->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Bold);

												 break;
											 }
										 }
										 rtfScript->SelectedText = token;
									 }

									 if(comment != nullptr)
									 {
										 rtfScript->SelectionColor = Color::Green;
										 rtfScript->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Italic);
										 rtfScript->SelectedText = comment;
									 }
								}

								rtfScript->Refresh();

								btnRunScript->Enabled = true;
							 }
							 else
							 {
								 MessageBox::Show("Unsupported file extension.",
												  "Error",
												  System::Windows::Forms::MessageBoxButtons::OK,
												  System::Windows::Forms::MessageBoxIcon::Error);
								 return;
							 }							 
						 } catch(System::Exception^ exc) {
							 MessageBox::Show(exc->Message);
							 return;
						 }
					 }
				 }
			 }
	
	private: System::Void rtfScript_TextChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 btnRunScript->Enabled = true;

				 //FreezeDraw();

				 int start = 0;
				 int end = 0;

				 for(start = rtfScript->SelectionStart - 1; start > 0; start--)
				 {
					 if(rtfScript->Text[start] == '\n')
					 {
						 start++;
						 break;
					 }
				 }
				 if(start == -1)
					 return;

				 // Calculate the end position of the current line.
				 for(end = rtfScript->SelectionStart; end < rtfScript->Text->Length; end++)
				 {
					 if(rtfScript->Text[end] == '\n')
						 break;
				 }
				 // Extract the current line that is being edited.
				 String^ tmpLine = rtfScript->Text->Substring(start, end - start);

				 String^ line = nullptr;
				 String^ comment = nullptr;
				 int indexComment = tmpLine->IndexOf("--");
				 if(indexComment >= 0)
				 {
					 line = tmpLine->Substring(0, indexComment);
					 comment = tmpLine->Substring(indexComment);
				 }
				 else
					 line = tmpLine;

				 // Backup the users current selection point.
				 int selectionStart = rtfScript->SelectionStart;
				 int selectionLength = rtfScript->SelectionLength;
				 // Split the line into tokens.
				 Regex^ r = gcnew Regex("([ \\r\\n\\t{}();])");
				 cli::array<String^>^ tokens = r->Split(line);
				 int index = start;
				 for(int j = 0; j < tokens->Length; ++j)
				 {
					 String^ token = tokens[j];

					 // Set the token's default color and font.
					 rtfScript->SelectionStart = index;
					 rtfScript->SelectionLength = token->Length;
					 rtfScript->SelectionColor = Color::Black;
					 rtfScript->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Regular);
					 
					 // Check whether the token is a keyword.
					 for(int i = 0; i < keywords->Count; i++)
					 {
						 if((String^)keywords[i] == token->ToLower())
						 {
							 // Apply alternative color and font to highlight keyword.
							 rtfScript->SelectionColor = Color::Blue;
							 rtfScript->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Bold);
							 break;
						 }
					 }

					 index += token->Length;
					 
					 // Restore the users current selection point.
					 rtfScript->SelectionStart = selectionStart;
					 rtfScript->SelectionLength = selectionLength;
				 }

				 if(comment != nullptr)
				 {
					 rtfScript->SelectionStart = indexComment + start;
					 rtfScript->SelectionLength = comment->Length;
					 rtfScript->SelectionColor = Color::Green;
					 rtfScript->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Italic);

					 // Restore the users current selection point.
					 rtfScript->SelectionStart = selectionStart;
					 rtfScript->SelectionLength = selectionLength;
				 }

				 //UnfreezeDraw();
			 }
	private: System::Void btnRunScript_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void btnClean_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 rtfScript->Text = "";
		 }
};
}


#endif