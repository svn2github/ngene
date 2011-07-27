/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmSolutionExplorer.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMSOLUTIONEXPLORER_H_
#define __INC_FRMSOLUTIONEXPLORER_H_


namespace nGENEToolset {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	using namespace WeifenLuo::WinFormsUI::Docking;


	/// <summary>
	/// Summary for frmSolutionExplorer
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmSolutionExplorer : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		frmSolutionExplorer(void)
		{
			InitializeComponent();

			trvFiles->BeginUpdate();
			trvFiles->Nodes->Add("Project");
			trvFiles->EndUpdate();

			//processTree("H:\\Inne\\nGene\\lib\\", trvFiles->Nodes[0]);

			trvFiles->Refresh();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmSolutionExplorer()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TreeView^  trvFiles;
	private: System::Windows::Forms::ImageList^  imgList;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmSolutionExplorer::typeid));
			this->trvFiles = (gcnew System::Windows::Forms::TreeView());
			this->imgList = (gcnew System::Windows::Forms::ImageList(this->components));
			this->SuspendLayout();
			// 
			// trvFiles
			// 
			this->trvFiles->Dock = System::Windows::Forms::DockStyle::Fill;
			this->trvFiles->ImageIndex = 0;
			this->trvFiles->ImageList = this->imgList;
			this->trvFiles->Location = System::Drawing::Point(0, 0);
			this->trvFiles->Name = L"trvFiles";
			this->trvFiles->SelectedImageIndex = 0;
			this->trvFiles->Size = System::Drawing::Size(292, 457);
			this->trvFiles->TabIndex = 0;
			// 
			// imgList
			// 
			this->imgList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imgList.ImageStream")));
			this->imgList->TransparentColor = System::Drawing::Color::Transparent;
			this->imgList->Images->SetKeyName(0, L"folder-closed_16.png");
			this->imgList->Images->SetKeyName(1, L"folder-open_16.png");
			this->imgList->Images->SetKeyName(2, L"documents_16.png");
			// 
			// frmSolutionExplorer
			// 
			this->AllowEndUserDocking = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 457);
			this->Controls->Add(this->trvFiles);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>(((WeifenLuo::WinFormsUI::Docking::DockAreas::DockLeft
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockRight) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::Document));
			this->HideOnClose = true;
			this->Name = L"frmSolutionExplorer";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockLeft;
			this->Text = L"Files Viewer";
			this->TabText = L"Files Viewer";
			this->ResumeLayout(false);

		}

	private:
		System::Void processTree(String^ dir, TreeNode^ node)
		{
			DirectoryInfo^ dirObj = gcnew DirectoryInfo(dir);

			trvFiles->BeginUpdate();
				node->Nodes->Add(gcnew TreeNode(dir, 0, 1));

				// Add files
				array <FileInfo^>^ files = dirObj->GetFiles("*.*", SearchOption::TopDirectoryOnly);
				FileInfo^ fileName;

				for(int i = 0; i < files->Length; ++i)
				{
					fileName = safe_cast<FileInfo^>(files->GetValue(i));
					trvFiles->BeginUpdate();
						node->Nodes[node->Nodes->Count - 1]->Nodes->Add(gcnew TreeNode(fileName->Name, 2, 2));
					trvFiles->EndUpdate();
				}
			trvFiles->EndUpdate();

			// Add directories
			array <DirectoryInfo^>^ dirs = dirObj->GetDirectories("*.*");
			DirectoryInfo^ dirName;
			
			for(int i = 0; i < dirs->Length; ++i)
			{
				dirName = safe_cast<DirectoryInfo^>(dirs->GetValue(i));
				try
				{
					processTree(dirName->FullName, node->Nodes[0]);
				}
				catch(Exception^ exception)
				{
					MessageBox::Show(exception->Message);
				}
			}
		}
#pragma endregion
	};
}


#endif