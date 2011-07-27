#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace nGENEMaterialEditorControl
{
	public enum class FunctionType
	{
		Function,
		Texture
	};

	/// <summary>
	/// Summary for MaterialFunction
	/// </summary>
	public ref class MaterialFunction : public System::Windows::Forms::UserControl
	{
	private:
		FunctionType m_Type;

	public:
		MaterialFunction(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			Type = FunctionType::Function;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MaterialFunction()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lblName;
	private: System::Windows::Forms::Button^  btnOutput;

	private: System::Windows::Forms::PictureBox^  pcbTexture;
	private: System::Windows::Forms::Button^  btnInput;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MaterialFunction::typeid));
			this->lblName = (gcnew System::Windows::Forms::Label());
			this->btnOutput = (gcnew System::Windows::Forms::Button());
			this->pcbTexture = (gcnew System::Windows::Forms::PictureBox());
			this->btnInput = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pcbTexture))->BeginInit();
			this->SuspendLayout();
			// 
			// lblName
			// 
			this->lblName->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->lblName->Location = System::Drawing::Point(0, 9);
			this->lblName->Name = L"lblName";
			this->lblName->Size = System::Drawing::Size(186, 21);
			this->lblName->TabIndex = 0;
			this->lblName->Text = L"MaterialFunction";
			this->lblName->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// btnOutput
			// 
			this->btnOutput->FlatAppearance->BorderSize = 0;
			this->btnOutput->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnOutput->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnOutput.Image")));
			this->btnOutput->Location = System::Drawing::Point(155, 114);
			this->btnOutput->Margin = System::Windows::Forms::Padding(0);
			this->btnOutput->Name = L"btnOutput";
			this->btnOutput->Size = System::Drawing::Size(19, 19);
			this->btnOutput->TabIndex = 1;
			this->btnOutput->UseVisualStyleBackColor = false;
			// 
			// pcbTexture
			// 
			this->pcbTexture->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pcbTexture->Location = System::Drawing::Point(39, 33);
			this->pcbTexture->Name = L"pcbTexture";
			this->pcbTexture->Size = System::Drawing::Size(108, 77);
			this->pcbTexture->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pcbTexture->TabIndex = 2;
			this->pcbTexture->TabStop = false;
			// 
			// btnInput
			// 
			this->btnInput->FlatAppearance->BorderSize = 0;
			this->btnInput->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnInput->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnInput.Image")));
			this->btnInput->Location = System::Drawing::Point(12, 114);
			this->btnInput->Margin = System::Windows::Forms::Padding(0);
			this->btnInput->Name = L"btnInput";
			this->btnInput->Size = System::Drawing::Size(19, 19);
			this->btnInput->TabIndex = 3;
			this->btnInput->UseVisualStyleBackColor = false;
			// 
			// MaterialFunction
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Controls->Add(this->btnInput);
			this->Controls->Add(this->pcbTexture);
			this->Controls->Add(this->btnOutput);
			this->Controls->Add(this->lblName);
			this->Name = L"MaterialFunction";
			this->Size = System::Drawing::Size(185, 143);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pcbTexture))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	public:
		[CategoryAttribute("Material function")]
		property int InputsNum;

		[CategoryAttribute("Material function")]
		property FunctionType Type
		{
			FunctionType get()
			{
				return m_Type;
			}

			void set(FunctionType value)
			{
				m_Type = value;
				if(m_Type == FunctionType::Function)
					pcbTexture->Visible = false;
				else if(m_Type == FunctionType::Texture)
					pcbTexture->Visible = true;
			}
		}

		[CategoryAttribute("Material function")]
		property String^ FunctionName
		{
			String^ get()
			{
				return lblName->Text;
			}

			void set(String^ value)
			{
				lblName->Text = value;
			}
		}

		[CategoryAttribute("Material function")]
		property Image^ Texture
		{
			Image^ get()
			{
				return pcbTexture->Image;
			}

			void set(Image^ value)
			{
				pcbTexture->Image = value;
			}
		}

		[CategoryAttribute("Material function")]
		property String^ Code;
	};
}
