/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmSceneGraph.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCENEGRAPH_H_
#define __INC_SCENEGRAPH_H_


#include "SceneGraph.h"
#include "frmPropertyManager.h"
#include "frmRenderTarget.h"


namespace nGENEToolset {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	using System::String;

	using namespace WeifenLuo::WinFormsUI::Docking;

	/// <summary>
	/// Summary for frmSceneGraph
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmSceneGraph : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	private:
		Hashtable^ m_AllNodes;

	public:
		System::Void addNode(String^ name, String^ key, int imageIndex)
		{
			trvScene->BeginUpdate();
				TreeNode^ node;
				if(trvScene->SelectedNode)
					node = trvScene->SelectedNode->Nodes->Add(key, name);
				else
					node = trvScene->Nodes->Add(key, name);
				node->ImageIndex = imageIndex;
				node->SelectedImageIndex = imageIndex;
				trvScene->SelectedNode = node;
			trvScene->EndUpdate();

			m_AllNodes->Add(key, node);
		}

		System::Void beginAdd()
		{
			trvScene->BeginUpdate();
		}

		System::Void endAdd()
		{
			trvScene->EndUpdate();
		}

		System::Void cleanup()
		{
			m_AllNodes->Clear();
			trvScene->BeginUpdate();
				trvScene->Nodes->Clear();
			trvScene->EndUpdate();
		}

		System::Void add(String^ name, String^ key)
		{
			TreeNode^ node;
			if(trvScene->SelectedNode)
				node = trvScene->SelectedNode->Nodes->Add(key, name);
			else
				node = trvScene->Nodes->Add(key, name);

			m_AllNodes->Add(key, node);
		}

		System::Void remove(String^ key)
		{
			trvScene->BeginUpdate();
				trvScene->Nodes->Remove(trvScene->SelectedNode);
				m_AllNodes->Remove(key);
			trvScene->EndUpdate();
		}

		void selectParent()
		{
			if(trvScene->SelectedNode != nullptr)
				trvScene->SelectedNode = trvScene->SelectedNode->Parent;
			if(trvScene->SelectedNode != nullptr)
				trvScene->SelectedNode->Collapse();
		}

		void selectNode(String^ name)
		{
			TreeNode^ node = (TreeNode^)m_AllNodes[name];//trvScene->Nodes->Find(name, true)[0];
			if(node != nullptr)
			{
				trvScene->SelectedNode = node;
			}
		}

		SceneNode^ getSelected()
		{
			try
			{
				if(trvScene->SelectedNode != nullptr)
					return graph->findNode(trvScene->SelectedNode->Name);
				else
					return nullptr;
			}
			catch(Exception^ e)
			{
				System::Console::WriteLine(e->Message);
				return nullptr;
			}
		}

		frmSceneGraph(frmPropertyManager^ propertyManager)
		{
			InitializeComponent();


			if(graph == nullptr)
				graph = gcnew SceneGraph(this);

			frmProperty = propertyManager;

			trvScene->Sorted = true;

			m_AllNodes = gcnew Hashtable();
		}
	private: System::Windows::Forms::ContextMenuStrip^  cmnuContext; 
	private: System::Windows::Forms::ToolStripMenuItem^  addMaterialToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addPhysicsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sphereToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  planeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sphereToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  cylinderToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  trianglemeshToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addAIActorToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  propertiesToolStripMenuItem;
private: System::Windows::Forms::ImageList^  imgList;

	public:
		static SceneGraph^ graph;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmSceneGraph()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TreeView^  trvScene;
			 frmPropertyManager^ frmProperty;
	private: System::ComponentModel::IContainer^  components;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmSceneGraph::typeid));
			this->trvScene = (gcnew System::Windows::Forms::TreeView());
			this->cmnuContext = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->addMaterialToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addPhysicsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sphereToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->planeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sphereToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cylinderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->trianglemeshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addAIActorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->propertiesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imgList = (gcnew System::Windows::Forms::ImageList(this->components));
			this->cmnuContext->SuspendLayout();
			this->SuspendLayout();
			// 
			// trvScene
			// 
			this->trvScene->ContextMenuStrip = this->cmnuContext;
			this->trvScene->Dock = System::Windows::Forms::DockStyle::Fill;
			this->trvScene->ImageIndex = 0;
			this->trvScene->ImageList = this->imgList;
			this->trvScene->Location = System::Drawing::Point(0, 0);
			this->trvScene->Margin = System::Windows::Forms::Padding(2);
			this->trvScene->Name = L"trvScene";
			this->trvScene->SelectedImageIndex = 0;
			this->trvScene->Size = System::Drawing::Size(276, 460);
			this->trvScene->TabIndex = 0;
			this->trvScene->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &frmSceneGraph::trvScene_AfterSelect);
			// 
			// cmnuContext
			// 
			this->cmnuContext->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->addMaterialToolStripMenuItem, 
				this->addPhysicsToolStripMenuItem, this->addAIActorToolStripMenuItem, this->toolStripSeparator1, this->propertiesToolStripMenuItem});
			this->cmnuContext->Name = L"contextMenuStrip1";
			this->cmnuContext->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->cmnuContext->Size = System::Drawing::Size(143, 98);
			this->cmnuContext->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &frmSceneGraph::cmnuContext_Opening);
			// 
			// addMaterialToolStripMenuItem
			// 
			this->addMaterialToolStripMenuItem->Name = L"addMaterialToolStripMenuItem";
			this->addMaterialToolStripMenuItem->Size = System::Drawing::Size(142, 22);
			this->addMaterialToolStripMenuItem->Text = L"Add material";
			// 
			// addPhysicsToolStripMenuItem
			// 
			this->addPhysicsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->sphereToolStripMenuItem, 
				this->planeToolStripMenuItem, this->sphereToolStripMenuItem1, this->cylinderToolStripMenuItem, this->trianglemeshToolStripMenuItem});
			this->addPhysicsToolStripMenuItem->Name = L"addPhysicsToolStripMenuItem";
			this->addPhysicsToolStripMenuItem->Size = System::Drawing::Size(142, 22);
			this->addPhysicsToolStripMenuItem->Text = L"Add physics";
			// 
			// sphereToolStripMenuItem
			// 
			this->sphereToolStripMenuItem->Name = L"sphereToolStripMenuItem";
			this->sphereToolStripMenuItem->Size = System::Drawing::Size(151, 22);
			this->sphereToolStripMenuItem->Text = L"Box";
			// 
			// planeToolStripMenuItem
			// 
			this->planeToolStripMenuItem->Name = L"planeToolStripMenuItem";
			this->planeToolStripMenuItem->Size = System::Drawing::Size(151, 22);
			this->planeToolStripMenuItem->Text = L"Plane";
			// 
			// sphereToolStripMenuItem1
			// 
			this->sphereToolStripMenuItem1->Name = L"sphereToolStripMenuItem1";
			this->sphereToolStripMenuItem1->Size = System::Drawing::Size(151, 22);
			this->sphereToolStripMenuItem1->Text = L"Sphere";
			// 
			// cylinderToolStripMenuItem
			// 
			this->cylinderToolStripMenuItem->Name = L"cylinderToolStripMenuItem";
			this->cylinderToolStripMenuItem->Size = System::Drawing::Size(151, 22);
			this->cylinderToolStripMenuItem->Text = L"Cylinder";
			// 
			// trianglemeshToolStripMenuItem
			// 
			this->trianglemeshToolStripMenuItem->Name = L"trianglemeshToolStripMenuItem";
			this->trianglemeshToolStripMenuItem->Size = System::Drawing::Size(151, 22);
			this->trianglemeshToolStripMenuItem->Text = L"Triangle-mesh";
			// 
			// addAIActorToolStripMenuItem
			// 
			this->addAIActorToolStripMenuItem->Name = L"addAIActorToolStripMenuItem";
			this->addAIActorToolStripMenuItem->Size = System::Drawing::Size(142, 22);
			this->addAIActorToolStripMenuItem->Text = L"Add AI actor";
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(139, 6);
			// 
			// propertiesToolStripMenuItem
			// 
			this->propertiesToolStripMenuItem->Name = L"propertiesToolStripMenuItem";
			this->propertiesToolStripMenuItem->Size = System::Drawing::Size(142, 22);
			this->propertiesToolStripMenuItem->Text = L"Properties";
			// 
			// imgList
			// 
			this->imgList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imgList.ImageStream")));
			this->imgList->TransparentColor = System::Drawing::Color::Transparent;
			this->imgList->Images->SetKeyName(0, L"box.jpg");
			this->imgList->Images->SetKeyName(1, L"camera.gif");
			this->imgList->Images->SetKeyName(2, L"clouds.jpg");
			this->imgList->Images->SetKeyName(3, L"light.png");
			this->imgList->Images->SetKeyName(4, L"mesh.gif");
			this->imgList->Images->SetKeyName(5, L"particle_emitter.gif");
			this->imgList->Images->SetKeyName(6, L"plane.jpg");
			this->imgList->Images->SetKeyName(7, L"sky.gif");
			this->imgList->Images->SetKeyName(8, L"sphere.png");
			this->imgList->Images->SetKeyName(9, L"terrain.gif");
			this->imgList->Images->SetKeyName(10, L"water.gif");
			this->imgList->Images->SetKeyName(11, L"water.jpg");
			// 
			// frmSceneGraph
			// 
			this->AllowEndUserDocking = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(276, 460);
			this->Controls->Add(this->trvScene);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>((((WeifenLuo::WinFormsUI::Docking::DockAreas::Float | WeifenLuo::WinFormsUI::Docking::DockAreas::DockLeft) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockRight) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::Document));
			this->HideOnClose = true;
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"frmSceneGraph";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockLeft;
			this->Text = L"Scene Graph";
			this->TabText = L"Scene Graph";
			this->cmnuContext->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void trvScene_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
			 {
				 if(e->Node != nullptr)
				 {
					 SceneNode^ node = graph->findNode(e->Node->Name);
					 frmProperty->SelectedObject = node;

					 frmRenderTarget::engine->drawAABB(node);
				 }
				 else
					 frmProperty->SelectedObject = nullptr;
			 }
	private: System::Void cmnuContext_Opening(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
		 {
		 }
};
}


#endif