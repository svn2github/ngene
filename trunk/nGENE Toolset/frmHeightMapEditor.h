/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmHeightMapEditor.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMHEIGHTMAPEDITOR_H_
#define __INC_FRMHEIGHTMAPEDITOR_H_


#include "ParticleDeposition.h"


namespace nGENEToolset
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace WeifenLuo::WinFormsUI::Docking;

	using nGENE::Nature::ParticleDeposition;
	using nGENE::Nature::ParticlePositionPolicyConstant;
	using nGENE::Nature::ParticlePositionPolicyPoints;
	using nGENE::Nature::ParticlePositionPolicyVolcano;
	using nGENE::Nature::ParticlePositionPolicyMountain;
	using nGENE::Nature::ParticleDeformPolicyNone;
	using nGENE::Nature::ParticleDeformPolicyVolcano;
	using nGENE::Nature::ParticleDeformPolicyFlip;

	using nGENE::Maths;

	using nGENE::Log;
	using nGENE::LOG_EVENT_TYPE;

	/// <summary>
	/// Summary for frmHeightMapEditor
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmHeightMapEditor : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	private:
		System::Windows::Forms::ToolStripStatusLabel^  tssLabel;

	public:

		frmHeightMapEditor(System::Windows::Forms::ToolStripStatusLabel^ label)
		{
			InitializeComponent();
			
			tssLabel = label;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmHeightMapEditor()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Button^  cmdGenerate;
	protected: 

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  cmbGenerationMethod;

	private: System::Windows::Forms::Button^  cmdCancel;
	private: System::Windows::Forms::PictureBox^  pctHeightMap;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  txtWidth;
	private: System::Windows::Forms::TextBox^  txtHeight;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  txtSeed;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  txtIterations;
	private: System::Windows::Forms::Button^  btnSave;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::TextBox^  txtMaxRadius;
	private: System::Windows::Forms::TextBox^  txtMinRadius;
	private: System::Windows::Forms::TextBox^  txtMinElevation;
	private: System::Windows::Forms::TextBox^  txtMaxElevation;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  txtMainX;
	private: System::Windows::Forms::TextBox^  txtMainY;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::ComboBox^  cmbForm;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::TextBox^  txtStreamsCount;
	private: System::Windows::Forms::TextBox^  txtStreamLength;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::TextBox^  txtCrater;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::TextBox^  txtDensity;




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
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->cmdGenerate = (gcnew System::Windows::Forms::Button());
			this->cmdCancel = (gcnew System::Windows::Forms::Button());
			this->pctHeightMap = (gcnew System::Windows::Forms::PictureBox());
			this->txtWidth = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->cmbGenerationMethod = (gcnew System::Windows::Forms::ComboBox());
			this->txtHeight = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->txtSeed = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->txtIterations = (gcnew System::Windows::Forms::TextBox());
			this->btnSave = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->txtMaxRadius = (gcnew System::Windows::Forms::TextBox());
			this->txtMinRadius = (gcnew System::Windows::Forms::TextBox());
			this->txtMinElevation = (gcnew System::Windows::Forms::TextBox());
			this->txtMaxElevation = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->txtMainX = (gcnew System::Windows::Forms::TextBox());
			this->txtMainY = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->cmbForm = (gcnew System::Windows::Forms::ComboBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->txtStreamsCount = (gcnew System::Windows::Forms::TextBox());
			this->txtStreamLength = (gcnew System::Windows::Forms::TextBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->txtCrater = (gcnew System::Windows::Forms::TextBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->txtDensity = (gcnew System::Windows::Forms::TextBox());
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pctHeightMap))->BeginInit();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 3;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				100)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel1->Controls->Add(this->cmdGenerate, 1, 19);
			this->tableLayoutPanel1->Controls->Add(this->cmdCancel, 1, 20);
			this->tableLayoutPanel1->Controls->Add(this->pctHeightMap, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->txtWidth, 2, 3);
			this->tableLayoutPanel1->Controls->Add(this->label2, 1, 3);
			this->tableLayoutPanel1->Controls->Add(this->label1, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->cmbGenerationMethod, 2, 1);
			this->tableLayoutPanel1->Controls->Add(this->txtHeight, 2, 4);
			this->tableLayoutPanel1->Controls->Add(this->label4, 1, 4);
			this->tableLayoutPanel1->Controls->Add(this->label3, 1, 5);
			this->tableLayoutPanel1->Controls->Add(this->txtSeed, 2, 5);
			this->tableLayoutPanel1->Controls->Add(this->label5, 1, 6);
			this->tableLayoutPanel1->Controls->Add(this->txtIterations, 2, 6);
			this->tableLayoutPanel1->Controls->Add(this->btnSave, 1, 18);
			this->tableLayoutPanel1->Controls->Add(this->label6, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->label7, 1, 7);
			this->tableLayoutPanel1->Controls->Add(this->label8, 1, 8);
			this->tableLayoutPanel1->Controls->Add(this->label9, 1, 9);
			this->tableLayoutPanel1->Controls->Add(this->label10, 1, 10);
			this->tableLayoutPanel1->Controls->Add(this->txtMaxRadius, 2, 8);
			this->tableLayoutPanel1->Controls->Add(this->txtMinRadius, 2, 7);
			this->tableLayoutPanel1->Controls->Add(this->txtMinElevation, 2, 9);
			this->tableLayoutPanel1->Controls->Add(this->txtMaxElevation, 2, 10);
			this->tableLayoutPanel1->Controls->Add(this->label11, 1, 11);
			this->tableLayoutPanel1->Controls->Add(this->label12, 1, 12);
			this->tableLayoutPanel1->Controls->Add(this->txtMainX, 2, 11);
			this->tableLayoutPanel1->Controls->Add(this->txtMainY, 2, 12);
			this->tableLayoutPanel1->Controls->Add(this->label13, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->cmbForm, 2, 2);
			this->tableLayoutPanel1->Controls->Add(this->label14, 1, 13);
			this->tableLayoutPanel1->Controls->Add(this->label15, 1, 14);
			this->tableLayoutPanel1->Controls->Add(this->txtStreamsCount, 2, 13);
			this->tableLayoutPanel1->Controls->Add(this->txtStreamLength, 2, 14);
			this->tableLayoutPanel1->Controls->Add(this->label16, 1, 15);
			this->tableLayoutPanel1->Controls->Add(this->txtCrater, 2, 15);
			this->tableLayoutPanel1->Controls->Add(this->label17, 1, 16);
			this->tableLayoutPanel1->Controls->Add(this->txtDensity, 2, 16);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 21;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->Size = System::Drawing::Size(800, 600);
			this->tableLayoutPanel1->TabIndex = 0;
			this->tableLayoutPanel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &frmHeightMapEditor::tableLayoutPanel1_Paint);
			// 
			// cmdGenerate
			// 
			this->tableLayoutPanel1->SetColumnSpan(this->cmdGenerate, 2);
			this->cmdGenerate->Enabled = false;
			this->cmdGenerate->Location = System::Drawing::Point(490, 542);
			this->cmdGenerate->Name = L"cmdGenerate";
			this->cmdGenerate->Size = System::Drawing::Size(307, 23);
			this->cmdGenerate->TabIndex = 1;
			this->cmdGenerate->Text = L"Generate height-map";
			this->cmdGenerate->UseVisualStyleBackColor = true;
			this->cmdGenerate->Click += gcnew System::EventHandler(this, &frmHeightMapEditor::cmdGenerate_Click);
			// 
			// cmdCancel
			// 
			this->tableLayoutPanel1->SetColumnSpan(this->cmdCancel, 2);
			this->cmdCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->cmdCancel->Location = System::Drawing::Point(490, 571);
			this->cmdCancel->Name = L"cmdCancel";
			this->cmdCancel->Size = System::Drawing::Size(307, 23);
			this->cmdCancel->TabIndex = 3;
			this->cmdCancel->Text = L"Cancel";
			this->cmdCancel->UseVisualStyleBackColor = true;
			this->cmdCancel->Click += gcnew System::EventHandler(this, &frmHeightMapEditor::cmdCancel_Click);
			// 
			// pctHeightMap
			// 
			this->pctHeightMap->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pctHeightMap->Cursor = System::Windows::Forms::Cursors::Default;
			this->pctHeightMap->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pctHeightMap->Location = System::Drawing::Point(3, 68);
			this->pctHeightMap->Name = L"pctHeightMap";
			this->tableLayoutPanel1->SetRowSpan(this->pctHeightMap, 19);
			this->pctHeightMap->Size = System::Drawing::Size(481, 497);
			this->pctHeightMap->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pctHeightMap->TabIndex = 4;
			this->pctHeightMap->TabStop = false;
			// 
			// txtWidth
			// 
			this->txtWidth->Location = System::Drawing::Point(639, 122);
			this->txtWidth->Name = L"txtWidth";
			this->txtWidth->Size = System::Drawing::Size(158, 20);
			this->txtWidth->TabIndex = 7;
			this->txtWidth->Text = L"256";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(490, 119);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Width";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(490, 65);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(97, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Generation method";
			// 
			// cmbGenerationMethod
			// 
			this->cmbGenerationMethod->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbGenerationMethod->FormattingEnabled = true;
			this->cmbGenerationMethod->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"Particle deposition"});
			this->cmbGenerationMethod->Location = System::Drawing::Point(639, 68);
			this->cmbGenerationMethod->Name = L"cmbGenerationMethod";
			this->cmbGenerationMethod->Size = System::Drawing::Size(158, 21);
			this->cmbGenerationMethod->TabIndex = 2;
			this->cmbGenerationMethod->SelectedIndexChanged += gcnew System::EventHandler(this, &frmHeightMapEditor::cmbGenerationMethod_SelectedIndexChanged);
			// 
			// txtHeight
			// 
			this->txtHeight->Location = System::Drawing::Point(639, 148);
			this->txtHeight->Name = L"txtHeight";
			this->txtHeight->Size = System::Drawing::Size(158, 20);
			this->txtHeight->TabIndex = 8;
			this->txtHeight->Text = L"256";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(490, 145);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(38, 13);
			this->label4->TabIndex = 9;
			this->label4->Text = L"Height";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(490, 171);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(32, 13);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Seed";
			// 
			// txtSeed
			// 
			this->txtSeed->Location = System::Drawing::Point(639, 174);
			this->txtSeed->Name = L"txtSeed";
			this->txtSeed->Size = System::Drawing::Size(158, 20);
			this->txtSeed->TabIndex = 10;
			this->txtSeed->Text = L"1";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(490, 197);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(50, 13);
			this->label5->TabIndex = 11;
			this->label5->Text = L"Iterations";
			// 
			// txtIterations
			// 
			this->txtIterations->Location = System::Drawing::Point(639, 200);
			this->txtIterations->Name = L"txtIterations";
			this->txtIterations->Size = System::Drawing::Size(158, 20);
			this->txtIterations->TabIndex = 12;
			this->txtIterations->Text = L"500000";
			// 
			// btnSave
			// 
			this->tableLayoutPanel1->SetColumnSpan(this->btnSave, 2);
			this->btnSave->Enabled = false;
			this->btnSave->Location = System::Drawing::Point(490, 513);
			this->btnSave->Name = L"btnSave";
			this->btnSave->Size = System::Drawing::Size(307, 23);
			this->btnSave->TabIndex = 13;
			this->btnSave->Text = L"Save height-map";
			this->btnSave->UseVisualStyleBackColor = true;
			this->btnSave->Click += gcnew System::EventHandler(this, &frmHeightMapEditor::btnSave_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->tableLayoutPanel1->SetColumnSpan(this->label6, 3);
			this->label6->Location = System::Drawing::Point(3, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(282, 65);
			this->label6->TabIndex = 14;
			this->label6->Text = L"Use this editor to generate height-maps for terrain creation.\r\n1. Select generati" 
				L"on method\r\n2. Select form to generate\r\n3. Click generate height-map\r\n4. Save hei" 
				L"ght-map";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(490, 223);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(114, 13);
			this->label7->TabIndex = 15;
			this->label7->Text = L"Minimum search radius";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(490, 249);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(117, 13);
			this->label8->TabIndex = 16;
			this->label8->Text = L"Maximum search radius";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(490, 275);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(140, 13);
			this->label9->TabIndex = 17;
			this->label9->Text = L"Minimum elevation threshold";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(490, 301);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(143, 13);
			this->label10->TabIndex = 18;
			this->label10->Text = L"Maximum elevation threshold";
			// 
			// txtMaxRadius
			// 
			this->txtMaxRadius->Location = System::Drawing::Point(639, 252);
			this->txtMaxRadius->Name = L"txtMaxRadius";
			this->txtMaxRadius->Size = System::Drawing::Size(158, 20);
			this->txtMaxRadius->TabIndex = 19;
			this->txtMaxRadius->Text = L"1";
			// 
			// txtMinRadius
			// 
			this->txtMinRadius->Location = System::Drawing::Point(639, 226);
			this->txtMinRadius->Name = L"txtMinRadius";
			this->txtMinRadius->Size = System::Drawing::Size(158, 20);
			this->txtMinRadius->TabIndex = 20;
			this->txtMinRadius->Text = L"1";
			// 
			// txtMinElevation
			// 
			this->txtMinElevation->Location = System::Drawing::Point(639, 278);
			this->txtMinElevation->Name = L"txtMinElevation";
			this->txtMinElevation->Size = System::Drawing::Size(158, 20);
			this->txtMinElevation->TabIndex = 21;
			this->txtMinElevation->Text = L"0";
			// 
			// txtMaxElevation
			// 
			this->txtMaxElevation->Location = System::Drawing::Point(639, 304);
			this->txtMaxElevation->Name = L"txtMaxElevation";
			this->txtMaxElevation->Size = System::Drawing::Size(158, 20);
			this->txtMaxElevation->TabIndex = 22;
			this->txtMaxElevation->Text = L"0";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(490, 327);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(78, 13);
			this->label11->TabIndex = 23;
			this->label11->Text = L"Main element x";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(490, 353);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(78, 13);
			this->label12->TabIndex = 24;
			this->label12->Text = L"Main element y";
			// 
			// txtMainX
			// 
			this->txtMainX->Location = System::Drawing::Point(639, 330);
			this->txtMainX->Name = L"txtMainX";
			this->txtMainX->Size = System::Drawing::Size(158, 20);
			this->txtMainX->TabIndex = 26;
			this->txtMainX->Text = L"0";
			// 
			// txtMainY
			// 
			this->txtMainY->Location = System::Drawing::Point(639, 356);
			this->txtMainY->Name = L"txtMainY";
			this->txtMainY->Size = System::Drawing::Size(158, 20);
			this->txtMainY->TabIndex = 25;
			this->txtMainY->Text = L"0";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(490, 92);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(87, 13);
			this->label13->TabIndex = 27;
			this->label13->Text = L"Form to generate";
			// 
			// cmbForm
			// 
			this->cmbForm->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbForm->Enabled = false;
			this->cmbForm->FormattingEnabled = true;
			this->cmbForm->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"Simple mountain", L"Volcano", L"Mountains", L"Trench", 
				L"Volcanic area"});
			this->cmbForm->Location = System::Drawing::Point(639, 95);
			this->cmbForm->Name = L"cmbForm";
			this->cmbForm->Size = System::Drawing::Size(158, 21);
			this->cmbForm->TabIndex = 28;
			this->cmbForm->SelectedIndexChanged += gcnew System::EventHandler(this, &frmHeightMapEditor::cmbForm_SelectedIndexChanged);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(490, 379);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(100, 13);
			this->label14->TabIndex = 29;
			this->label14->Text = L"Lava streams count";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(490, 405);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(117, 13);
			this->label15->TabIndex = 30;
			this->label15->Text = L"Maximum stream length";
			// 
			// txtStreamsCount
			// 
			this->txtStreamsCount->Enabled = false;
			this->txtStreamsCount->Location = System::Drawing::Point(639, 382);
			this->txtStreamsCount->Name = L"txtStreamsCount";
			this->txtStreamsCount->Size = System::Drawing::Size(158, 20);
			this->txtStreamsCount->TabIndex = 31;
			this->txtStreamsCount->Text = L"5";
			// 
			// txtStreamLength
			// 
			this->txtStreamLength->Enabled = false;
			this->txtStreamLength->Location = System::Drawing::Point(639, 408);
			this->txtStreamLength->Name = L"txtStreamLength";
			this->txtStreamLength->Size = System::Drawing::Size(158, 20);
			this->txtStreamLength->TabIndex = 32;
			this->txtStreamLength->Text = L"40";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(490, 431);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(96, 13);
			this->label16->TabIndex = 33;
			this->label16->Text = L"Beginning of crater";
			// 
			// txtCrater
			// 
			this->txtCrater->Enabled = false;
			this->txtCrater->Location = System::Drawing::Point(639, 434);
			this->txtCrater->Name = L"txtCrater";
			this->txtCrater->Size = System::Drawing::Size(158, 20);
			this->txtCrater->TabIndex = 34;
			this->txtCrater->Text = L"40";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(490, 457);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(92, 13);
			this->label17->TabIndex = 35;
			this->label17->Text = L"Mountains density";
			// 
			// txtDensity
			// 
			this->txtDensity->Enabled = false;
			this->txtDensity->Location = System::Drawing::Point(639, 460);
			this->txtDensity->Name = L"txtDensity";
			this->txtDensity->Size = System::Drawing::Size(158, 20);
			this->txtDensity->TabIndex = 36;
			this->txtDensity->Text = L"6000";
			// 
			// frmHeightMapEditor
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->ClientSize = System::Drawing::Size(800, 600);
			this->Controls->Add(this->tableLayoutPanel1);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>((WeifenLuo::WinFormsUI::Docking::DockAreas::Float | WeifenLuo::WinFormsUI::Docking::DockAreas::Document));
			this->HideOnClose = true;
			this->Name = L"frmHeightMapEditor";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::Document;
			this->Text = L"Height-map Editor";
			this->TabText = L"Height-map Editor";
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pctHeightMap))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void tableLayoutPanel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 }
private: System::Void cmbGenerationMethod_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(cmbGenerationMethod->SelectedItem != nullptr)
				 cmbForm->Enabled = true;
		 }
private: System::Void cmdGenerate_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 tssLabel->Text = "Gerating height-map...";
			 tssLabel->Invalidate();

			 int width;
			 int height;
			 int seed;
			 int iterations;
			 int minRadius;
			 int maxRadius;
			 int minElevation;
			 int maxElevation;
			 int mainX;
			 int mainY;
			 
			 try {
				 width = Convert::ToInt32(txtWidth->Text);
			 } catch(System::ArgumentNullException^ exc) {
				 MessageBox::Show("Width cannot be null value",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtWidth->Focus();
				 return;
			 } catch(System::FormatException^ exc) {
				 MessageBox::Show("Width has to be integer",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtWidth->Focus();
				 return;
			 }

			 try {
				 height = Convert::ToInt32(txtHeight->Text);
			 } catch(System::ArgumentNullException^ exc) {
				 MessageBox::Show("Height cannot be null value",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtHeight->Focus();
				 return;
			 } catch(System::FormatException^ exc) {
				 MessageBox::Show("Height has to be integer",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtHeight->Focus();
				 return;
			 }

			 try {
				 seed = Convert::ToInt32(txtSeed->Text);
			 } catch(System::ArgumentNullException^ exc) {
				 MessageBox::Show("Seed cannot be null value",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtSeed->Focus();
				 return;
			 } catch(System::FormatException^ exc) {
				 MessageBox::Show("Seed has to be integer",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtSeed->Focus();
				 return;
			 }

			 try {
				 iterations = Convert::ToInt32(txtIterations->Text);
			 } catch(System::ArgumentNullException^ exc) {
				 MessageBox::Show("Iterations cannot be null value",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtIterations->Focus();
				 return;
			 } catch(System::FormatException^ exc) {
				 MessageBox::Show("Iterations has to be integer",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtIterations->Focus();
				 return;
			 }

			 try {
				 minRadius = Convert::ToInt32(txtMinRadius->Text);
			 } catch(System::ArgumentNullException^ exc) {
				 MessageBox::Show("Radius cannot be null value",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMinRadius->Focus();
				 return;
			 } catch(System::FormatException^ exc) {
				 MessageBox::Show("Radius has to be integer",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMinRadius->Focus();
				 return;
			 }

			 try {
				 maxRadius = Convert::ToInt32(txtMaxRadius->Text);
			 } catch(System::ArgumentNullException^ exc) {
				 MessageBox::Show("Radius cannot be null value",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMaxRadius->Focus();
				 return;
			 } catch(System::FormatException^ exc) {
				 MessageBox::Show("Radius has to be integer",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMaxRadius->Focus();
				 return;
			 }

			 if(minRadius > maxRadius)
			 {
				 MessageBox::Show("Minimum radius has to be less or equal maximum radius",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMaxRadius->Focus();
				 return;
			 }

			 try {
				 minElevation = Convert::ToInt32(txtMinElevation->Text);
			 } catch(System::ArgumentNullException^ exc) {
				 MessageBox::Show("Elevation cannot be null value",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMinElevation->Focus();
				 return;
			 } catch(System::FormatException^ exc) {
				 MessageBox::Show("Elevation has to be integer",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMinElevation->Focus();
				 return;
			 }

			 try {
				 maxElevation = Convert::ToInt32(txtMaxElevation->Text);
			 } catch(System::ArgumentNullException^ exc) {
				 MessageBox::Show("Elevation cannot be null value",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMaxElevation->Focus();
				 return;
			 } catch(System::FormatException^ exc) {
				 MessageBox::Show("Elevation has to be integer",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMaxElevation->Focus();
				 return;
			 }

			 if(minElevation > maxElevation)
			 {
				 MessageBox::Show("Minimum elevation has to be less or equal maximum elevation",
					 "Error",
					 MessageBoxButtons::OK,
					 MessageBoxIcon::Error);
				 txtMinElevation->Focus();
				 return;
			 }

			 try {
				 mainX = Convert::ToInt32(txtMainX->Text);
			 } catch(System::ArgumentNullException^ exc) {
				 MessageBox::Show("Position cannot be null value",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMainX->Focus();
				 return;
			 } catch(System::FormatException^ exc) {
				 MessageBox::Show("Position has to be integer",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMainX->Focus();
				 return;
			 }

			 try {
				 mainY = Convert::ToInt32(txtMainY->Text);
			 } catch(System::ArgumentNullException^ exc) {
				 MessageBox::Show("Position cannot be null value",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMainY->Focus();
				 return;
			 } catch(System::FormatException^ exc) {
				 MessageBox::Show("Position has to be integer",
								  "Error",
								  MessageBoxButtons::OK,
								  MessageBoxIcon::Error);
				 txtMainY->Focus();
				 return;
			 }

			 // Particle deposition
			 if(cmbGenerationMethod->SelectedIndex == 0)
			 {
				 Bitmap^ image = gcnew Bitmap(width, height);
				 nGENE::byte* pTempHeights = NULL;
				 nGENE::Vector2 limit(width, height);

				 srand(seed);

				 if(cmbForm->SelectedItem->ToString() == "Volcano")
				 {
					 int streamsCount;
					 int streamLength;
					 int craterHeight;

					 try {
						 streamsCount = Convert::ToInt32(txtStreamsCount->Text);
					 } catch(System::ArgumentNullException^ exc) {
						 MessageBox::Show("Streams count cannot be null value",
										  "Error",
										  MessageBoxButtons::OK,
										  MessageBoxIcon::Error);
						 txtStreamsCount->Focus();
						 return;
					 } catch(System::FormatException^ exc) {
						 MessageBox::Show("Streams count has to be integer",
										  "Error",
										  MessageBoxButtons::OK,
										  MessageBoxIcon::Error);
						 txtStreamsCount->Focus();
						 return;
					 }

					 try {
						 streamLength = Convert::ToInt32(txtStreamLength->Text);
					 } catch(System::ArgumentNullException^ exc) {
						 MessageBox::Show("Stream length cannot be null value",
										  "Error",
										  MessageBoxButtons::OK,
										  MessageBoxIcon::Error);
						 txtStreamLength->Focus();
						 return;
					 } catch(System::FormatException^ exc) {
						 MessageBox::Show("Stream length has to be integer",
										  "Error",
										  MessageBoxButtons::OK,
										  MessageBoxIcon::Error);
						 txtStreamLength->Focus();
						 return;
					 }

					 try {
						 craterHeight = Convert::ToInt32(txtCrater->Text);
					 } catch(System::ArgumentNullException^ exc) {
						 MessageBox::Show("Crater height cannot be null value",
										  "Error",
										  MessageBoxButtons::OK,
										  MessageBoxIcon::Error);
						 txtCrater->Focus();
						 return;
					 } catch(System::FormatException^ exc) {
						 MessageBox::Show("Crater height has to be integer",
										  "Error",
										  MessageBoxButtons::OK,
										  MessageBoxIcon::Error);
						 txtCrater->Focus();
						 return;
					 }

					 ParticleDeposition <ParticlePositionPolicyVolcano, ParticleDeformPolicyVolcano> creator;
					 creator.setLimit(limit);
					 creator.setPosition(nGENE::Vector2(mainX, mainY));
					 creator.setMaximumStreamLength(streamLength);
					 creator.setStreamsCount(streamsCount);
					 creator.setCraterHeight(craterHeight);
					 pTempHeights = creator.generate(width, height, seed, iterations,
						 minRadius, maxRadius, minElevation, maxElevation);
				 }
				 else if(cmbForm->SelectedItem->ToString() == "Simple mountain")
				 {
					 ParticleDeposition <ParticlePositionPolicyConstant, ParticleDeformPolicyNone> creator;
					 creator.setLimit(limit);
					 creator.setPosition(nGENE::Vector2(mainX, mainY));
					 pTempHeights = creator.generate(width, height, seed, iterations,
						 minRadius, maxRadius, minElevation, maxElevation);
				 }
				 else if(cmbForm->SelectedItem->ToString() == "Volcanic area")
				 {
					 ParticleDeposition <ParticlePositionPolicyPoints, ParticleDeformPolicyNone> creator;
					 creator.setLimit(limit);
					 for(int i = 0; i < 15; ++i)
					 {
						 nGENE::Vector2 pos = nGENE::Vector2(Maths::perlinNoiseGenerator(rand(), rand(), rand()) * limit.x * 0.5f,
							 Maths::perlinNoiseGenerator(rand(), rand(), rand()) * limit.y * 0.5f);
						 pos += limit * 0.5f;

						 creator.setPosition(pos);
					 }
					 pTempHeights = creator.generate(width, height, seed, iterations,
						 minRadius, maxRadius, minElevation, maxElevation);
				 }
				 else if(cmbForm->SelectedItem->ToString() == "Mountains")
				 {
					 int walkers;
					 try {
						 walkers = Convert::ToInt32(txtDensity->Text);
					 } catch(System::ArgumentNullException^ exc) {
						 MessageBox::Show("Density cannot be null value",
										  "Error",
										  MessageBoxButtons::OK,
										  MessageBoxIcon::Error);
						 txtDensity->Focus();
						 return;
					 } catch(System::FormatException^ exc) {
						 MessageBox::Show("Density has to be integer",
										  "Error",
										  MessageBoxButtons::OK,
										  MessageBoxIcon::Error);
						 txtDensity->Focus();
						 return;
					 }

					 ParticleDeposition <ParticlePositionPolicyMountain, ParticleDeformPolicyNone> creator;
					 creator.setLimit(limit);
					 creator.setWalkersNum(walkers);
					 creator.setPosition(nGENE::Vector2(mainX, mainY));
					 pTempHeights = creator.generate(width, height, seed, iterations,
						 minRadius, maxRadius, minElevation, maxElevation);
				 }
				 else if(cmbForm->SelectedItem->ToString() == "Trench")
				 {
					 int walkers;
					 try {
						 walkers = Convert::ToInt32(txtDensity->Text);
					 } catch(System::ArgumentNullException^ exc) {
						 MessageBox::Show("Density cannot be null value",
										  "Error",
										  MessageBoxButtons::OK,
										  MessageBoxIcon::Error);
						 txtDensity->Focus();
						 return;
					 } catch(System::FormatException^ exc) {
						 MessageBox::Show("Density has to be integer",
										  "Error",
										  MessageBoxButtons::OK,
										  MessageBoxIcon::Error);
						 txtDensity->Focus();
						 return;
					 }

					 ParticleDeposition <ParticlePositionPolicyMountain, ParticleDeformPolicyFlip> creator;
					 creator.setLimit(limit);
					 creator.setWalkersNum(walkers);
					 creator.setPosition(nGENE::Vector2(mainX, mainY));
					 pTempHeights = creator.generate(width, height, seed, iterations,
						 minRadius, maxRadius, minElevation, maxElevation);
				 }
				 else
				 {
					 // ... not implemented ...
					 return;
				 }

				 // Create image
				 int index = 0;
				 for(int y = 0; y < width; ++y)
				 {
					 for(int x = 0; x < height; ++x)
					 {
						 nGENE::byte clr = pTempHeights[index++];
						 image->SetPixel(x, y, Color::FromArgb(clr, clr, clr));
					 }
				 }

				 NGENE_DELETE_ARRAY(pTempHeights);

				 pctHeightMap->Image = image;
				 btnSave->Enabled = true;

				 //Log::log(LOG_EVENT_TYPE::LET_EVENT, L"Editor", __WFILE__, __WFUNCTION__, __LINE__,
				//	 L"Height-map generated successfully.");
				 
				 tssLabel->Text = "Ready";
			 }
			 else
			 {
				 // ... to be implemented ...
			 }
		 }
private: System::Void cmdCancel_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->Hide();
		 }
private: System::Void btnSave_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			SaveFileDialog^ dlgSaveFile = gcnew SaveFileDialog();
			dlgSaveFile->Title = "Save height-map file";
			dlgSaveFile->Filter = ".bmp (Bitmap files)|*.bmp";
			dlgSaveFile->FilterIndex = 1;
			dlgSaveFile->AddExtension = true;

			if(dlgSaveFile->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				pctHeightMap->Image->Save(dlgSaveFile->FileName);
			}
		 }
private: System::Void cmbForm_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(cmbForm->SelectedItem != nullptr)
				 cmdGenerate->Enabled = true;

			 if(cmbForm->SelectedItem->ToString() == "Volcano")
			 {
				 txtStreamsCount->Enabled = true;
				 txtStreamLength->Enabled = true;
				 txtCrater->Enabled = true;
			 }
			 else
			 {
				 txtStreamsCount->Enabled = false;
				 txtStreamLength->Enabled = false;
				 txtCrater->Enabled = false;
			 }

			 if(cmbForm->SelectedItem->ToString() == "Trench" ||
				cmbForm->SelectedItem->ToString() == "Mountains")
				 txtDensity->Enabled = true;
			 else
				 txtDensity->Enabled = false;
		 }
};
}


#endif
