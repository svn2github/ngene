/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderEditor.h
Version:	0.02
---------------------------------------------------------------------------
*/


#pragma once
#ifndef __INC_FRM_SHADER_EDITOR_H_
#define __INC_FRM_SHADER_EDITOR_H_


namespace nGENEToolset
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text::RegularExpressions;

	using namespace WeifenLuo::WinFormsUI::Docking;

	/// <summary>
	/// Summary for ShaderEditor
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ShaderEditor : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	private:
		System::Collections::ArrayList^ keywords;

	public:
		ShaderEditor(void)
		{
			InitializeComponent();
			
			// Add all keywords
			keywords = gcnew System::Collections::ArrayList();
			keywords->Add("half");
			keywords->Add("half2");
			keywords->Add("half3");
			keywords->Add("half4");
			keywords->Add("float");
			keywords->Add("float2");
			keywords->Add("float3");
			keywords->Add("float4");
			keywords->Add("int");
			keywords->Add("int2");
			keywords->Add("int3");
			keywords->Add("int4");
			keywords->Add("bool");
			keywords->Add("true");
			keywords->Add("false");
			keywords->Add("matrix3x3");
			keywords->Add("matrix3x4");
			keywords->Add("matrix4x3");
			keywords->Add("matrix4x4");
			keywords->Add("typedef");
			keywords->Add("if");
			keywords->Add("else");
			keywords->Add("for");
			keywords->Add("sampler");
			keywords->Add("const");
			keywords->Add("static");
			keywords->Add("struct");
			keywords->Add("return");
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ShaderEditor()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::RichTextBox^  rtfShader;
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
			this->rtfShader = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// rtfShader
			// 
			this->rtfShader->AllowDrop = true;
			this->rtfShader->Dock = System::Windows::Forms::DockStyle::Fill;
			this->rtfShader->Location = System::Drawing::Point(0, 0);
			this->rtfShader->Name = L"rtfShader";
			this->rtfShader->ShowSelectionMargin = true;
			this->rtfShader->Size = System::Drawing::Size(764, 264);
			this->rtfShader->TabIndex = 0;
			this->rtfShader->Text = L"";
			this->rtfShader->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &ShaderEditor::rtfShader_DragDrop);
			this->rtfShader->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &ShaderEditor::rtfShader_DragEnter);
			this->rtfShader->TextChanged += gcnew System::EventHandler(this, &ShaderEditor::rtfShader_TextChanged);
			// 
			// ShaderEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 264);
			this->Controls->Add(this->rtfShader);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>((WeifenLuo::WinFormsUI::Docking::DockAreas::Float | WeifenLuo::WinFormsUI::Docking::DockAreas::Document));
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->HideOnClose = true;
			this->Name = L"ShaderEditor";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::Document;
			this->TabText = L"Shader editor";
			this->Text = L"Shader editor";
			this->ResumeLayout(false);

		}
#pragma endregion

#pragma region "Avoid flickering" 

	#define WM_SETREDRAW 0x000B 

	private:
		    [System::Runtime::InteropServices::DllImportAttribute("User32")]
			static bool SendMessage(IntPtr hWnd, int msg, int wParam, int lParam); 
	private: void FreezeDraw()
	{
		//Disable drawing
		SendMessage(rtfShader->Handle, WM_SETREDRAW, 0, 0); 
	} 

	private: void UnfreezeDraw()
	{
		//Enable drawing and do a redraw.
		SendMessage(rtfShader->Handle, WM_SETREDRAW, 1, 0);
		rtfShader->Invalidate(true);
	} 

#pragma endregion

	private: System::Void rtfShader_DragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
			 {
				 if(e->Data->GetDataPresent(DataFormats::FileDrop))
					 e->Effect = DragDropEffects::All;
				 else
					 e->Effect = DragDropEffects::None;
			 }

	private: System::Void rtfShader_DragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
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
							 if(file.Extension->ToLower() == ".vsh" || file.Extension->ToLower() == ".psh")
							 {
								 rtfShader->Text = "";
								 System::IO::StreamReader^ reader = gcnew System::IO::StreamReader(files[0]);
								 String^ text = reader->ReadToEnd();

								Regex^ r = gcnew Regex("\\n");
								cli::array<String^>^ lines = r->Split(text);
								for(int k = 0; k < lines->Length; ++k)
								{
									 String^ tmpLine = lines[k];
									 String^ line = nullptr;
									 String^ comment = nullptr;
									 int index = tmpLine->IndexOf("//");
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
										 rtfShader->SelectionColor = Color::Black;
										 rtfShader->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Regular);
										 

										 for(int i = 0; i < keywords->Count; i++)
										 {
											 if ((String^)keywords[i] == token->ToLower())
											 {
												 // Apply alternative color and font to highlight keyword.
												 rtfShader->SelectionColor = Color::Blue;
												 rtfShader->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Bold);

												 break;
											 }
										 }
										 rtfShader->SelectedText = token;
									 }

									 if(comment != nullptr)
									 {
										 rtfShader->SelectionColor = Color::Green;
										 rtfShader->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Italic);
										 rtfShader->SelectedText = comment;
									 }
								}

								rtfShader->Refresh();
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
	
	private: System::Void rtfShader_TextChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 //FreezeDraw();

				 int start = 0;
				 int end = 0;

				 for(start = rtfShader->SelectionStart - 1; start > 0; start--)
				 {
					 if(rtfShader->Text[start] == '\n')
					 {
						 start++;
						 break;
					 }
				 }
				 if(start == -1)
					 return;

				 // Calculate the end position of the current line.
				 for(end = rtfShader->SelectionStart; end < rtfShader->Text->Length; end++)
				 {
					 if(rtfShader->Text[end] == '\n')
						 break;
				 }
				 // Extract the current line that is being edited.
				 String^ tmpLine = rtfShader->Text->Substring(start, end - start);

				 String^ line = nullptr;
				 String^ comment = nullptr;
				 int indexComment = tmpLine->IndexOf("//");
				 if(indexComment >= 0)
				 {
					 line = tmpLine->Substring(0, indexComment);
					 comment = tmpLine->Substring(indexComment);
				 }
				 else
					 line = tmpLine;

				 // Backup the users current selection point.
				 int selectionStart = rtfShader->SelectionStart;
				 int selectionLength = rtfShader->SelectionLength;
				 // Split the line into tokens.
				 Regex^ r = gcnew Regex("([ \\r\\n\\t{}();])");
				 cli::array<String^>^ tokens = r->Split(line);
				 int index = start;
				 for(int j = 0; j < tokens->Length; ++j)
				 {
					 String^ token = tokens[j];

					 // Set the token's default color and font.
					 rtfShader->SelectionStart = index;
					 rtfShader->SelectionLength = token->Length;
					 rtfShader->SelectionColor = Color::Black;
					 rtfShader->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Regular);
					 
					 // Check whether the token is a keyword.
					 for(int i = 0; i < keywords->Count; i++)
					 {
						 if((String^)keywords[i] == token->ToLower())
						 {
							 // Apply alternative color and font to highlight keyword.
							 rtfShader->SelectionColor = Color::Blue;
							 rtfShader->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Bold);
							 break;
						 }
					 }

					 index += token->Length;
					 
					 // Restore the users current selection point.
					 rtfShader->SelectionStart = selectionStart;
					 rtfShader->SelectionLength = selectionLength;
				 }

				 if(comment != nullptr)
				 {
					 rtfShader->SelectionStart = indexComment + start;
					 rtfShader->SelectionLength = comment->Length;
					 rtfShader->SelectionColor = Color::Green;
					 rtfShader->SelectionFont = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Italic);

					 // Restore the users current selection point.
					 rtfShader->SelectionStart = selectionStart;
					 rtfShader->SelectionLength = selectionLength;
				 }

				 //UnfreezeDraw();
			 }
	};
}


#endif