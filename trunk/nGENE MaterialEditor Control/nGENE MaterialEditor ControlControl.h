#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::ComponentModel::Design;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace nGENEMaterialEditorControl
{

	/// <summary>
	/// Summary for nGENEMaterialEditorControlControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	[DesignerAttribute("System.Windows.Forms.Design.ParentControlDesigner, System.Design", IDesigner::typeid)]
	public ref class nGENEMaterialEditorControlControl : public System::Windows::Forms::UserControl
	{
	private:
		ArrayList^ m_Functions;
		String^ m_Code;

	public:
		nGENEMaterialEditorControlControl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			m_Functions = gcnew ArrayList();
			m_Code = gcnew String("");
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~nGENEMaterialEditorControlControl()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// nGENEMaterialEditorControlControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->Name = L"MaterialEditorControl";
			this->Size = System::Drawing::Size(437, 454);
			this->Dock = DockStyle::Fill;
			this->ResumeLayout(false);

		}
#pragma endregion

	public:
		property ArrayList^ Functions
		{
			ArrayList^ get()
			{
				return m_Functions;
			}

			void set(ArrayList^ value)
			{
				m_Functions = value;
			}
		}

		property String^ Code
		{
			String^ get()
			{
				return m_Code;
			}
		}
	};
}
