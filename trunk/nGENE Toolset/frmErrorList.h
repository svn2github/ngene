/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmErrorList.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMERRORLIST_H_
#define __INC_FRMERRORLIST_H_


#include "Logger.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace WeifenLuo::WinFormsUI::Docking;

using System::String;


namespace nGENEToolset
{

	/// <summary>
	/// Summary for frmErrorList
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmErrorList : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	private:
		ToolsetLogManaged^ logger;

		int m_nErrors;
		int m_nWarnings;
		int m_nMessages;

	public:
		frmErrorList(void)
		{
			InitializeComponent();
			
			logger = gcnew ToolsetLogManaged();
			logger->log += gcnew LogEventHandler(this, &frmErrorList::frmErrorList_Log);

			m_nErrors = 0;
			m_nWarnings = 0;
			m_nMessages = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmErrorList()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID;
	protected: 
	private: System::Windows::Forms::DataGridView^  dgvLog;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  TypeID;
	private: System::Windows::Forms::DataGridViewImageColumn^  Type;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Description;
	private: System::Windows::Forms::ToolStripButton^  btnMessages;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStrip^  tstMsgType;
	private: System::Windows::Forms::ToolStripButton^  btnErrors;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripButton^  btnWarnings;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmErrorList::typeid));
			this->ID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dgvLog = (gcnew System::Windows::Forms::DataGridView());
			this->TypeID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Type = (gcnew System::Windows::Forms::DataGridViewImageColumn());
			this->Description = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->btnMessages = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tstMsgType = (gcnew System::Windows::Forms::ToolStrip());
			this->btnErrors = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->btnWarnings = (gcnew System::Windows::Forms::ToolStripButton());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dgvLog))->BeginInit();
			this->tstMsgType->SuspendLayout();
			this->SuspendLayout();
			// 
			// ID
			// 
			this->ID->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->ID->HeaderText = L"ID";
			this->ID->Name = L"ID";
			this->ID->ReadOnly = true;
			this->ID->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->ID->Width = 43;
			// 
			// dgvLog
			// 
			this->dgvLog->AllowUserToAddRows = false;
			this->dgvLog->AllowUserToDeleteRows = false;
			this->dgvLog->AllowUserToResizeColumns = false;
			this->dgvLog->AllowUserToResizeRows = false;
			this->dgvLog->BackgroundColor = System::Drawing::SystemColors::ControlLightLight;
			this->dgvLog->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
			this->dgvLog->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dgvLog->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(4) {this->ID, this->TypeID, 
				this->Type, this->Description});
			this->dgvLog->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dgvLog->GridColor = System::Drawing::Color::Gainsboro;
			this->dgvLog->Location = System::Drawing::Point(0, 23);
			this->dgvLog->MultiSelect = false;
			this->dgvLog->Name = L"dgvLog";
			this->dgvLog->ReadOnly = true;
			this->dgvLog->RowHeadersVisible = false;
			this->dgvLog->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->dgvLog->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dgvLog->Size = System::Drawing::Size(698, 187);
			this->dgvLog->TabIndex = 6;
			// 
			// TypeID
			// 
			this->TypeID->HeaderText = L"TypeID";
			this->TypeID->Name = L"TypeID";
			this->TypeID->ReadOnly = true;
			this->TypeID->Visible = false;
			// 
			// Type
			// 
			this->Type->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->Type->HeaderText = L"Type";
			this->Type->Name = L"Type";
			this->Type->ReadOnly = true;
			this->Type->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->Type->Width = 37;
			// 
			// Description
			// 
			this->Description->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->Description->HeaderText = L"Description";
			this->Description->Name = L"Description";
			this->Description->ReadOnly = true;
			this->Description->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// btnMessages
			// 
			this->btnMessages->Checked = true;
			this->btnMessages->CheckOnClick = true;
			this->btnMessages->CheckState = System::Windows::Forms::CheckState::Checked;
			this->btnMessages->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnMessages.Image")));
			this->btnMessages->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnMessages->Name = L"btnMessages";
			this->btnMessages->Size = System::Drawing::Size(78, 20);
			this->btnMessages->Text = L"0 Messages";
			this->btnMessages->Click += gcnew System::EventHandler(this, &frmErrorList::btnMessages_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(6, 23);
			// 
			// tstMsgType
			// 
			this->tstMsgType->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->btnErrors, this->toolStripSeparator1, 
				this->btnWarnings, this->toolStripSeparator2, this->btnMessages});
			this->tstMsgType->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Flow;
			this->tstMsgType->Location = System::Drawing::Point(0, 0);
			this->tstMsgType->Name = L"tstMsgType";
			this->tstMsgType->Size = System::Drawing::Size(698, 23);
			this->tstMsgType->TabIndex = 5;
			this->tstMsgType->Text = L"toolStrip1";
			// 
			// btnErrors
			// 
			this->btnErrors->Checked = true;
			this->btnErrors->CheckOnClick = true;
			this->btnErrors->CheckState = System::Windows::Forms::CheckState::Checked;
			this->btnErrors->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnErrors.Image")));
			this->btnErrors->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnErrors->Name = L"btnErrors";
			this->btnErrors->Size = System::Drawing::Size(57, 20);
			this->btnErrors->Text = L"0 Errors";
			this->btnErrors->Click += gcnew System::EventHandler(this, &frmErrorList::btnErrors_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(6, 23);
			// 
			// btnWarnings
			// 
			this->btnWarnings->Checked = true;
			this->btnWarnings->CheckOnClick = true;
			this->btnWarnings->CheckState = System::Windows::Forms::CheckState::Checked;
			this->btnWarnings->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnWarnings.Image")));
			this->btnWarnings->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->btnWarnings->Name = L"btnWarnings";
			this->btnWarnings->Size = System::Drawing::Size(77, 20);
			this->btnWarnings->Text = L"0 Warnings";
			this->btnWarnings->Click += gcnew System::EventHandler(this, &frmErrorList::btnWarnings_Click);
			// 
			// frmErrorList
			// 
			this->AllowEndUserDocking = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(698, 210);
			this->Controls->Add(this->dgvLog);
			this->Controls->Add(this->tstMsgType);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>((((WeifenLuo::WinFormsUI::Docking::DockAreas::Float
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockTop) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockBottom) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::Document));
			this->Name = L"frmErrorList";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockBottom;
			this->Text = L"Error List";
			this->TabText = L"Error List";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dgvLog))->EndInit();
			this->tstMsgType->ResumeLayout(false);
			this->tstMsgType->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	public:
		System::Void frmErrorList_Log(String^ str, int type)
		{
			bool visible = true;
			cli::array<Object^>^ values = gcnew cli::array<Object^>(4);
			static int msgID;
			++msgID;

			values[0] = msgID;
			values[1] = type;
			switch(type)
			{
			case 0: values[2] = btnErrors->Image;
				    visible = btnErrors->Checked;
					++m_nErrors;
					btnErrors->Text = m_nErrors + " Errors";

					break;
			case 1: values[2] = btnWarnings->Image;
					visible = btnWarnings->Checked;
					++m_nWarnings;
					btnWarnings->Text = m_nWarnings + " Warnings";

				    break;
			case 2: values[2] = btnMessages->Image;
					visible = btnMessages->Checked;
					++m_nMessages;
					btnMessages->Text = m_nMessages + " Messages";

				    break;
			}

			values[3] = str;

			try
			{
				this->dgvLog->Rows->Add(values);
				this->dgvLog->Rows[dgvLog->Rows->Count - 1]->Visible = visible;
			}
			catch(Exception^ e)
			{
				System::Console::WriteLine(e->Message);
			}
		}
private: System::Void btnErrors_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 bool visible = btnErrors->Checked;
			 int size = dgvLog->Rows->Count;

			 dgvLog->SuspendLayout();

			 for(int i = 0; i < size; ++i)
			 {
				 if(dgvLog->Rows[i]->Cells[1]->Value->ToString()->CompareTo("0") == 0)
					 dgvLog->Rows[i]->Visible = visible;
			 }

			 dgvLog->ResumeLayout();
		 }
private: System::Void btnWarnings_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 bool visible = btnWarnings->Checked;
			 int size = dgvLog->Rows->Count;

			 dgvLog->SuspendLayout();

			 for(int i = 0; i < size; ++i)
			 {
				 if(dgvLog->Rows[i]->Cells[1]->Value->ToString()->CompareTo("1") == 0)
					dgvLog->Rows[i]->Visible = visible;
			 }

			 dgvLog->ResumeLayout();
		 }
private: System::Void btnMessages_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 bool visible = btnMessages->Checked;
			 int size = dgvLog->Rows->Count;

			 dgvLog->SuspendLayout();

			 for(int i = 0; i < size; ++i)
			 {
				 if(dgvLog->Rows[i]->Cells[1]->Value->ToString()->CompareTo("2") == 0)
					dgvLog->Rows[i]->Visible = visible;
			 }

			 dgvLog->ResumeLayout();
		 }
	};
}


#endif