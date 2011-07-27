/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MaterialLibrary.h
Version:	0.22
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "MaterialLibrary.h"
#include "Material.h"
#include "ITexture.h"
#include "TextureManager.h"
#include "XMLParser.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo MaterialLibrary::Type(L"MaterialLibrary", &BaseClass::Type);


	MaterialLibrary::MaterialLibrary(bool _loadOnDemand):
		m_stName(L""),
		m_stAuthor(L""),
		m_fVersion(NGENE_SDK_VERSION),
		m_bLoadOnDemand(_loadOnDemand)
	{
		init();
	}
//----------------------------------------------------------------------
	MaterialLibrary::~MaterialLibrary()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void MaterialLibrary::init()
	{
	}
//----------------------------------------------------------------------
	void MaterialLibrary::cleanup()
	{
		if(!m_pMaterials.empty())
		{
			HashTable <wstring, Material*>::iterator iter;
			for(iter = m_pMaterials.begin(); iter != m_pMaterials.end(); ++iter)
				NGENE_DELETE(iter->second);

			m_pMaterials.clear();
			m_vMaterials.clear();
		}

		if(!m_vTexturesNames.empty())
			m_vTexturesNames.clear();
	}
//----------------------------------------------------------------------
	void MaterialLibrary::load(const wstring& _fileName)
	{
		// Open file
		FileManager& fileManager = FileManager::getSingleton();
		IFile* file = fileManager.openFile(_fileName, OPEN_READ);
		if(!file)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Library file: %ls does not exist", _fileName.c_str());
			return;
		}

		wstring temp = file->getBuffer();
		fileManager.closeFile(_fileName);

		XMLDocument* pDocument = XMLParser::getSingleton().parse(temp, true);
		if(pDocument->getTagName() != "library")
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Unexpected tag: %s while loading: %ls Material Library", pDocument->getTagName().c_str(),
				_fileName.c_str());
			return;
		}

		parse(pDocument);

		// Release document
		NGENE_DELETE(pDocument);
	}
//----------------------------------------------------------------------
	Material* MaterialLibrary::getMaterial(const wstring& _name)
	{
		// Material doesn't exist, so exit
		if(m_pMaterials.find(_name) == m_pMaterials.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material: %ls does not exist", _name.c_str());
			return NULL;
		}

		m_pMaterials[_name]->materialAccessed();

		return m_pMaterials[_name];
	}
//----------------------------------------------------------------------
	Material* MaterialLibrary::getMaterial(uint _index)
	{
		// Material doesn't exist, so exit
		if(_index > m_vMaterials.size())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material with index: %d does not exist", _index);
			return NULL;
		}

		m_vMaterials[_index]->materialAccessed();

		return m_vMaterials[_index];
	}
//----------------------------------------------------------------------
	Material* MaterialLibrary::createMaterial(const wstring& _name)
	{
		// If material exists, return it
		if(m_pMaterials.find(_name) != m_pMaterials.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material with the name: %ls already exists", _name.c_str());
			return m_pMaterials[_name];
		}

		m_pMaterials[_name] = new Material(_name);
		m_vMaterials.push_back(m_pMaterials[_name]);

		return m_pMaterials[_name];
	}
//----------------------------------------------------------------------
	void MaterialLibrary::addMaterial(const wstring& _name, const Material& _material)
	{
		// If material exists, exit
		if(m_pMaterials.find(_name) != m_pMaterials.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material with the name: %ls already exists", _name.c_str());
			return;
		}

		m_pMaterials[_name] = new Material(_material);
		m_vMaterials.push_back(m_pMaterials[_name]);
	}
//----------------------------------------------------------------------
	void MaterialLibrary::addMaterial(const wstring& _name, const Material* _material)
	{
		// If material exists, exit
		if(m_pMaterials.find(_name) != m_pMaterials.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material with the name: %ls already exists", _name.c_str());
			return;
		}

		m_pMaterials[_name] = new Material(*_material);
		m_vMaterials.push_back(m_pMaterials[_name]);
	}
//----------------------------------------------------------------------
	void MaterialLibrary::parseTextures(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		TextureManager& texMgr = TextureManager::getSingleton();

		bool bDelete = true;
		while(pNode)
		{
			string tagName = pNode->getTagName();
			bDelete = true;

			// Load shared texture
			if(tagName == "texture")
			{
				texMgr.createTextureFromXML(pNode, m_bLoadOnDemand);

				string name = pNode->getAttribute("name");
				m_vTexturesNames.push_back(name);

				if(m_bLoadOnDemand)
					bDelete = false;
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			if(bDelete)
				NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void MaterialLibrary::parseMaterials(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		bool bDelete = true;
		while(pNode)
		{
			string tagName = pNode->getTagName();
			bDelete = true;

			// Load shared texture
			if(tagName == "material")
			{
				Material* tempMat = new Material();

				tempMat->setParent(NULL);
				tempMat->setMaterialLibrary(this);
				tempMat->parse(pNode);

				m_pMaterials[tempMat->getName()] = tempMat;
				m_vMaterials.push_back(tempMat);

				if(m_bLoadOnDemand)
					bDelete = false;
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			if(bDelete)
				NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void MaterialLibrary::parse(XMLDocument* _document)
	{
		XMLDocument* pDocument = _document;

		// Parse data
		string key = pDocument->getAttribute("name");
		MaterialLibrary::m_stName.assign(key.begin(), key.end());
		
		key = pDocument->getAttribute("author");
		MaterialLibrary::m_stAuthor.assign(key.begin(), key.end());

		key = pDocument->getAttribute("version");
		MaterialLibrary::m_fVersion = atof(key.c_str());
		if(Maths::floatCompare(m_fVersion, NGENE_SDK_VERSION))
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"This library might not be compatible with current nGENE Tech version");
		}
		
		// Parse child nodes
		XMLDocument* pChild = pDocument->getFirstChild();
		XMLDocument* pTemp = NULL;
		while(pChild)
		{
			string tagName = pChild->getTagName();

			// Load shared texture
			if(tagName == "textures")
			{
				parseTextures(pChild->getFirstChild());
			}
			// Load material
			else if(tagName == "materials")
			{
				parseMaterials(pChild->getFirstChild());
			}

			pTemp = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(pTemp);
		}

		// Release document
		pDocument = NULL;
	}
//----------------------------------------------------------------------
	void MaterialLibrary::save(File* _file)
	{
		XMLNode::setName("library");

		// Add attributes
		Attribute atName;
		atName.first = "name";
		atName.second.assign(m_stName.begin(), m_stName.end());

		Attribute atAuthor;
		atAuthor.first = "author";
		atAuthor.second.assign(m_stAuthor.begin(), m_stAuthor.end());

		Attribute atVersion;
		stringstream buffer;
		buffer << m_fVersion;
		atVersion.first = "version";
		atVersion.second= buffer.str();

		Attribute atNamespace;
		atNamespace.first = "xmlns:xsi";
		atNamespace.second = "http://www.w3.org/2001/XMLSchema-instance";

		Attribute atSchema;
		atSchema.first = "xsi:noNamespaceSchemaLocation";
		atSchema.second = "materialLib.xsd";


		addAttribute(atName);
		addAttribute(atAuthor);
		addAttribute(atVersion);
		addAttribute(atNamespace);
		addAttribute(atSchema);

		// Add textures node
		XMLNode textures;
		if(m_vTexturesNames.size())
		{
			saveTextures(textures);
		}

		// Add materials node
		XMLNode materials;
		saveMaterials(materials);

		// Save
		XMLNode::save(_file);


		textures.cleanup();
	}
//----------------------------------------------------------------------
	void MaterialLibrary::saveTextures(XMLNode& _textures)
	{
		_textures.setName("textures");
		addChild(&_textures);

		TextureManager& mgr = TextureManager::getSingleton();

		// Add textures nodes
		for(uint i = 0; i < m_vTexturesNames.size(); ++i)
		{
			string& name = m_vTexturesNames[i];

			wstring stName(name.begin(), name.end());
			ITexture* pTexture = mgr.getTexture(stName);
			if(pTexture)
			{
				XMLNode* pNode = mgr.saveTexture(pTexture, name);

				_textures.addChild(pNode);
			}
		}
	}
//----------------------------------------------------------------------
	void MaterialLibrary::saveMaterials(XMLNode& _materials)
	{
		_materials.setName("materials");
		addChild(&_materials);

		// Add materials nodes
		vector <Material*>::iterator iter;
		for(iter = m_vMaterials.begin(); iter != m_vMaterials.end(); ++iter)
			_materials.addChild(*iter);
	}
//----------------------------------------------------------------------
}