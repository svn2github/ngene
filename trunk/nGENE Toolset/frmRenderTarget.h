/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmRenderTarget.h
Version:	0.09
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMRENDERTARGET_H_
#define __INC_FRMRENDERTARGET_H_


#include "EngineWrapper.h"
#include "nGENE.h"



namespace nGENEToolset
{
	using namespace System;
	using namespace System::Collections;
	using namespace System::ComponentModel;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Windows::Forms;

	using namespace WeifenLuo::WinFormsUI::Docking;


	/** Render target form.
	*/
	public ref class frmRenderTarget: public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		static EngineWrapper^ engine;
		int passed;
		int prev;
		int frames;

		frmRenderTarget(ToolStripStatusLabel^ _label)
		{
			start = false;
			engine = gcnew EngineWrapper();

			label = _label;
			frames = 0;

			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmRenderTarget()
		{
			if(components)
				delete components;
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		ToolStripStatusLabel^ label;

		System::Windows::Forms::Timer^ timer;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();

			// 
			// frmRenderTarget
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(697, 428);
			this->KeyPreview = true;
			this->Name = L"frmRenderTarget";
			this->Text = L"Level View";
			this->TabText = L"Level View";
			this->Load += gcnew System::EventHandler(this, &frmRenderTarget::frmRenderTarget_Load);
			this->Resize += gcnew System::EventHandler(this, &frmRenderTarget::frmRenderTarget_Resize);
			this->GotFocus += gcnew System::EventHandler(this, &frmRenderTarget::frmRenderTarget_GotFocus);
			this->LostFocus += gcnew System::EventHandler(this, &frmRenderTarget::frmRenderTarget_LostFocus);
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::Document;
			this->ResumeLayout(false);

			timer = gcnew System::Windows::Forms::Timer();
			timer->Interval = 1;
			timer->Enabled = true;
			timer->Tick += gcnew System::EventHandler(this, &frmRenderTarget::timer_tick);
		}
#pragma endregion

	private:
		bool start;						///< @todo Temporary only.
		bool m_bContinuousRendering;
		bool m_bRequestRender;

	private:
		System::Void frmRenderTarget_Resize(System::Object^ sender, System::EventArgs^ e)
		{
			if(start)
			{
				Engine& engine = Engine::getSingleton();
				Camera* pCamera = engine.getActiveCamera();

				if(pCamera)
				{
					Frustum& frustum = pCamera->getFrustum();

					// Change viewing frustum
					frustum.setAspect((float)this->Width / (float)this->Height);
					pCamera->requestUpdate();
				}
			}
		}

		System::Void frmRenderTarget_Load(System::Object^  sender, System::EventArgs^  e)
		{
			m_bContinuousRendering = true;
			m_bRequestRender = false;
		}

		System::Void timer_tick(System::Object^ sender, System::EventArgs^ e)
		{
			if(m_bContinuousRendering || m_bRequestRender)
			{
				engine->update();
				if(m_bRequestRender)
					m_bRequestRender = false;

				if(m_bContinuousRendering)
				{
					int now = System::Environment::TickCount;

					int diff = now - prev;
					passed += diff;
					++frames;
					if(passed >= 1000)
					{
						float secs = ((float)passed) / 1000.0f;
						int fps = frames;
						try
						{
							label->Text = "FPS: " + fps;
						} catch(Exception^ e)
						{}

						frames = 0;
						passed = 0;
					}

					prev = now;
				}
			}
		}

	public: System::Void init()
		{
			engine->init((HWND)(this->Handle.ToPointer()));
			engine->enableInput(false);

			start = true;
		}

	private:
		System::Void frmRenderTarget_GotFocus(System::Object^  sender, System::EventArgs^  e)
		{
			engine->setWindowZero(this->Left, this->Top, this->Left + this->Width, this->Bottom);
			engine->enableInput(true);
		}

		System::Void frmRenderTarget_LostFocus(System::Object^  sender, System::EventArgs^  e)
		{
			engine->enableInput(false);
		}

	public:
		void requestRender()
		{
			m_bRequestRender = true;
		}

		void setContinuousRendering(bool _value)
		{
			m_bContinuousRendering = _value;
		}
	};
}


#endif