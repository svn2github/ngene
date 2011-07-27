/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmMaterialLib.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "StdAfx.h"
#include "frmMaterialLib.h"
#include "frmSceneGraph.h"

#include "MaterialWrapper.h"

#include <vcclr.h>


namespace nGENEToolset
{
	System::Void frmMaterialLib::addMaterial(MaterialWrapper^ _material, Hashtable^ _matlib)
	{
		_matlib->Add(_material->Key, _material);
	}
//----------------------------------------------------------------------
	System::Void frmMaterialLib::applyToTheSelectedObjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(trvMaterials->SelectedNode)
		{
			if(frmSceneGraph::graph->getActiveSceneNode() != nullptr)
			{
				try {
					SurfaceWrapper^ surf = safe_cast<SurfaceWrapper^>(frmSceneGraph::graph->getActiveSceneNode());
					/// @todo change this to accept different material libs
					pin_ptr <const wchar_t> str1 = PtrToStringChars(trvMaterials->SelectedNode->Name);
					wstring matName(str1);

					pin_ptr <const wchar_t> str2 = PtrToStringChars(cmbLibrary->SelectedItem->ToString());
					wstring libName(str2);
					surf->getSurface()->setMaterial(MaterialManager::getSingleton().getLibrary(libName)->getMaterial(matName));
				} catch(InvalidCastException^)
				{
					try {
						NodeVisibleWrapper^ node = safe_cast<NodeVisibleWrapper^>(frmSceneGraph::graph->getActiveSceneNode());
						/// @todo change this to accept different material libs
						pin_ptr <const wchar_t> str1 = PtrToStringChars(trvMaterials->SelectedNode->Name);
						wstring matName(str1);
						pin_ptr <const wchar_t> str2 = PtrToStringChars(cmbLibrary->SelectedItem->ToString());
						wstring libName(str2);
						NodeVisible* pNode = (NodeVisible*)node->getnGENENode();
						for(uint i = 0; i < pNode->getSurfacesNum(); ++i)
							pNode->getSurface(i)->setMaterial(MaterialManager::getSingleton().getLibrary(libName)->getMaterial(matName));
					} catch(InvalidCastException^)
					{
						MessageBox::Show("Please select node visible or surface object",
										 "Select different node",
									     System::Windows::Forms::MessageBoxButtons::OK,
									     System::Windows::Forms::MessageBoxIcon::Exclamation);
					}
				}
			}
		}
	}
//----------------------------------------------------------------------
	System::Void frmMaterialLib::mnuApplyToScene_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(trvMaterials->SelectedNode)
		{
			if(frmSceneGraph::graph->getActiveSceneNode() != nullptr)
			{
				pin_ptr <const wchar_t> str1 = PtrToStringChars(trvMaterials->SelectedNode->Name);
				wstring matName(str1);

				pin_ptr <const wchar_t> str2 = PtrToStringChars(cmbLibrary->SelectedItem->ToString());
				wstring libName(str2);
				Material* pMaterial = MaterialManager::getSingleton().getLibrary(libName)->getMaterial(matName);

				Hashtable^ lib = (Hashtable^)m_AllMaterials[cmbLibrary->SelectedItem->ToString()];
				MaterialWrapper^ material = (MaterialWrapper^)lib[trvMaterials->SelectedNode->Name];

				if(pMaterial->getRenderingOrder() == DO_POST)
				{
					SPriorityMaterial* pPost = material->applyPost();
					FXWrapper^ fx = gcnew FXWrapper(pPost);
					this->frmFX->addEffect(fx);
				}
			}
		}
	}
//----------------------------------------------------------------------
	System::Void frmMaterialLib::addLibrary(const wstring& _name)
	{
		String^ libName = gcnew String(_name.c_str());
		m_AllMaterials[libName] = gcnew Hashtable();

		MaterialLibrary* matLib = MaterialManager::getSingleton().getLibrary(_name);
		for(uint i = 0; i < matLib->getMaterialsNum(); ++i)
		{
			MaterialWrapper^ material = gcnew MaterialWrapper(matLib->getMaterial(i),
				this, (Hashtable^)m_AllMaterials[libName]);
		}

		/// @todo remove this
		/*nGENE::File* pFile = static_cast<nGENE::File*>(FileManager::getSingleton().openFile(L"test2.lib", OPEN_WRITE));
		matLib->save(pFile);
		pFile->close();*/

		cmbLibrary->Items->Add(libName);

		if(cmbLibrary->Items->Count == 1)
			cmbLibrary->SelectedIndex = 0;
	}
//----------------------------------------------------------------------
	System::Void frmMaterialLib::mnuLoadLibrary_Click(System::Object^  sender, System::EventArgs^  e)
	{
		OpenFileDialog^ dlgOpenFile = gcnew OpenFileDialog();
		dlgOpenFile->Title = "Open material library file";
		dlgOpenFile->Filter = ".lib (nGENE material library files)|*.lib";
		dlgOpenFile->FilterIndex = 1;

		if(dlgOpenFile->ShowDialog() != System::Windows::Forms::DialogResult::OK)
			return;

		pin_ptr <const wchar_t> str1 = PtrToStringChars(dlgOpenFile->FileName);
		wstring fileName(str1);

		FileInfo^ info = gcnew FileInfo(dlgOpenFile->FileName);
		String^ ext = info->Extension;
		int length = info->Name->Length - info->Extension->Length;
		String^ shortName = info->Name->Substring(0, length);

		pin_ptr <const wchar_t> str2 = PtrToStringChars(shortName);
		wstring libName(str2);

		if(m_AllMaterials->ContainsKey(shortName))
		{
			MessageBox::Show("Material library '" + shortName + ".lib' is already loaded",
							 "Error",
							 System::Windows::Forms::MessageBoxButtons::OK,
							 System::Windows::Forms::MessageBoxIcon::Error);
			return;
		}

		MaterialManager::getSingleton().createLibraryFromFile(fileName, libName);
		addLibrary(libName);
	}
//----------------------------------------------------------------------
}