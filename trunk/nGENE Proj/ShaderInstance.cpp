/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderInstance.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Shader.h"
#include "ShaderConstant.h"
#include "ShaderInstance.h"
#include "ShaderManager.h"
#include "XMLDocument.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ShaderInstance::Type(L"ShaderInstance", NULL);


	ShaderInstance::ShaderInstance():
		m_pShader(NULL)
	{
	}
//----------------------------------------------------------------------
	ShaderInstance::ShaderInstance(const ShaderInstance& src):
		XMLNode(src)
	{
		m_pShader = src.m_pShader;
		
		if(!src.m_pVariables.empty())
		{
			vector <ShaderConstant*>::iterator iter;
			for(iter = src.m_pVariables.begin(); iter != src.m_pVariables.end(); ++iter)
			{
				m_pVariables.push_back(new ShaderConstant(**iter));
				m_Variables[m_pVariables.back()->getName()] = m_pVariables.back();
			}
		}
	}
//----------------------------------------------------------------------
	ShaderInstance::~ShaderInstance()
	{
		if(!m_pVariables.empty())
		{
			vector <ShaderConstant*>::iterator iter;
			for(iter = m_pVariables.begin(); iter != m_pVariables.end(); ++iter)
				delete (*iter);

			m_pVariables.clear();
			m_Variables.clear();
		}
	}
//----------------------------------------------------------------------
	ShaderInstance& ShaderInstance::operator=(const ShaderInstance& rhs)
	{
		if(this == &rhs)
			return (*this);

		XMLNode::operator=(rhs);

		m_pShader = rhs.m_pShader;
		
		if(!rhs.m_pVariables.empty())
		{
			vector <ShaderConstant*>::iterator iter;
			for(iter = rhs.m_pVariables.begin(); iter != rhs.m_pVariables.end(); ++iter)
			{
				m_pVariables.push_back(new ShaderConstant(**iter));
				m_Variables[m_pVariables.back()->getName()] = m_pVariables.back();
			}
		}
		
		return (*this);
	}
//----------------------------------------------------------------------
	void ShaderInstance::update() const
	{
		// Shader instance has no valid shader assigned!
		if(!m_pShader)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader instance has no valid shader assigned!");
			return;
		}

		// Update variables
		if(!m_pVariables.empty())
		{
			vector <ShaderConstant*>::iterator iter;
			for(iter = m_pVariables.begin(); iter != m_pVariables.end(); ++iter)
			{
				ShaderConstant* pConstant = (*iter);
				if(pConstant->isDynamic())
				{
					pConstant->update();
					
					switch(pConstant->getType())
					{
					case SShaderConstantType::BT_FLOAT:
						m_pShader->setFloatArray(pConstant->getName().c_str(),
							(float*)pConstant->getValues(),
							pConstant->getValuesCount());
						break;
					case SShaderConstantType::BT_INT:
						m_pShader->setIntegerArray(pConstant->getName().c_str(),
							(int*)pConstant->getValues(),
							pConstant->getValuesCount());
						break;
					case SShaderConstantType::BT_BOOL:
						m_pShader->setBooleanArray(pConstant->getName().c_str(),
							(bool*)pConstant->getValues(),
							pConstant->getValuesCount());
						break;
					}
				}
			}
		}
	}
//----------------------------------------------------------------------
	void ShaderInstance::bind() const
	{
		// Shader instance has no valid shader assigned!
		if(!m_pShader)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader instance has no valid shader assigned!");
			return;
		}

		ShaderManager::getSingleton().bindShader(m_pShader);

		if(!m_pVariables.empty())
		{
			vector <ShaderConstant*>::iterator iter;
			for(iter = m_pVariables.begin(); iter != m_pVariables.end(); ++iter)
			{
				ShaderConstant* pConstant = (*iter);
				if(!pConstant->isDynamic())
				{
					pConstant->update();
					
					switch(pConstant->getType())
					{
					case SShaderConstantType::BT_FLOAT:
						m_pShader->setFloatArray(pConstant->getName().c_str(),
							(float*)pConstant->getValues(),
							pConstant->getValuesCount());
						break;
					case SShaderConstantType::BT_INT:
						m_pShader->setIntegerArray(pConstant->getName().c_str(),
							(int*)pConstant->getValues(),
							pConstant->getValuesCount());
						break;
					case SShaderConstantType::BT_BOOL:
						m_pShader->setBooleanArray(pConstant->getName().c_str(),
							(bool*)pConstant->getValues(),
							pConstant->getValuesCount());
						break;
					}
				}
			}
		}
	}
//----------------------------------------------------------------------
	void ShaderInstance::unbind() const
	{
	}
//----------------------------------------------------------------------
	void ShaderInstance::parse(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;

		// Parse data
		string key = pNode->getAttribute("name");
		string m_stProfile = pNode->getAttribute("profile");
		string m_stFunction= pNode->getAttribute("function");

		// Parse child nodes
		XMLDocument* pChild = pNode->getFirstChild();
		XMLDocument* pTemp = NULL;
		while(pChild)
		{
			// Set constants
			if(pChild->getTagName() == "constant")
			{
				ShaderConstant constant;
				constant.parse(pChild);

				m_pVariables.push_back(new ShaderConstant(constant));
				m_Variables[constant.getName()] = m_pVariables.back();
			}

			pTemp = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void ShaderInstance::addShaderConstant(const ShaderConstant& _constant)
	{
		m_pVariables.push_back(new ShaderConstant(_constant));
		m_Variables[_constant.getName()] = m_pVariables.back();
	}
//----------------------------------------------------------------------
	ShaderConstant* ShaderInstance::getConstant(const string& _name)
	{
		// No shader constant, so return NULL
		if(m_Variables.find(_name) == m_Variables.end())
		{
			wstring name(_name.begin(), _name.end());
			Log::log(nGENE::LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Shader constant: %ls not found in shader instance",
				name.c_str());

			return NULL;
		}

		return m_Variables[_name];
	}
//----------------------------------------------------------------------
	ShaderConstant* ShaderInstance::getConstant(uint _index)
	{
		if(_index >= m_pVariables.size())
		{
			Log::log(nGENE::LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__,
				L"Shader constant with index: %d not found in shader instance",
				_index);

			return NULL;
		}
		

		return m_pVariables[_index];
	}
//----------------------------------------------------------------------
	void ShaderInstance::save(File* _file)
	{
		SHADER_DESC& desc = m_pShader->getDesc();

		switch(desc.type)
		{
		case SHADER_VERTEX: XMLNode::setName("vertex_shader");
			break;
		case SHADER_GEOMETRY: XMLNode::setName("geometry_shader");
			break;
		case SHADER_PIXEL: XMLNode::setName("pixel_shader");
			break;
		}

		// Add attributes
		Attribute atName;
		atName.first = "name";
		atName.second.assign(desc.name.begin(), desc.name.end());

		Attribute atFileName;
		atFileName.first = "file";
		atFileName.second.assign(desc.fileName.begin(), desc.fileName.end());

		Attribute atFunction;
		atFunction.first = "function";
		atFunction.second = desc.function;

		Attribute atProfile;
		atProfile.first = "profile";
		atProfile.second = desc.profile;


		addAttribute(atName);
		addAttribute(atFileName);
		addAttribute(atFunction);
		addAttribute(atProfile);


		// Save shader constants
		vector <ShaderConstant*>::iterator iter;
		for(iter = m_pVariables.begin(); iter != m_pVariables.end(); ++iter)
			addChild(*iter);


		XMLNode::save(_file);
	}
//----------------------------------------------------------------------
}