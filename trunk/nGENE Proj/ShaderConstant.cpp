/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderConstant.cpp
Version:	0.16
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FuncSplit.h"
#include "ShaderConstant.h"
#include "ShaderManager.h"
#include "XMLDocument.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ShaderConstant::Type(L"ShaderConstant", NULL);


	ShaderConstant::ShaderConstant():
		m_pValues(NULL),
		m_pDefaultValues(NULL),
		m_nValuesCount(0),
		m_pSemantic(NULL),
		m_pDataType(NULL),
		m_Type(SShaderConstantType::BT_FLOAT),
		m_bDynamic(true),
		m_nOffset(0),
		m_bArray(false)
	{
	}
//----------------------------------------------------------------------
	ShaderConstant::ShaderConstant(const ShaderConstant& src):
		XMLNode(src)
	{
		m_stConstantName = src.m_stConstantName;
		m_stValue = src.m_stValue;
		m_nValuesCount = src.m_nValuesCount;
		m_nOffset = src.m_nOffset;
		m_bDynamic = src.m_bDynamic;


		m_Type = src.m_Type;

		uint size = 0;
		switch(src.getType())
		{
		case SShaderConstantType::BT_FLOAT:
			m_pDefaultValues = new float[src.m_nValuesCount];
			m_pValues = new float[src.m_nValuesCount];
			size = sizeof(float);
			break;

		case SShaderConstantType::BT_INT:
			m_pDefaultValues = new int[src.m_nValuesCount];
			m_pValues = new int[src.m_nValuesCount];
			size = sizeof(int);
			break;

		case SShaderConstantType::BT_BOOL:
			m_pDefaultValues = new bool[src.m_nValuesCount];
			m_pValues = new bool[src.m_nValuesCount];
			size = sizeof(bool);
			break;
		}
		
		if(src.m_pValues && src.m_nValuesCount)
			memcpy(m_pValues, src.m_pValues, src.m_nValuesCount * size);
		if(src.m_pDefaultValues && src.m_nValuesCount)
			memcpy(m_pDefaultValues, src.m_pDefaultValues, src.m_nValuesCount * size);

		m_pSemantic = src.m_pSemantic;
		m_pDataType = src.m_pDataType;
		m_bArray = src.m_bArray;
	}
//----------------------------------------------------------------------
	ShaderConstant& ShaderConstant::operator=(const ShaderConstant& rhs)
	{
		if(this == &rhs)
			return (*this);

		XMLNode::operator=(rhs);

		NGENE_DELETE_ARRAY(m_pValues);
		NGENE_DELETE_ARRAY(m_pDefaultValues);

		m_stConstantName = rhs.m_stConstantName;
		m_stValue = rhs.m_stValue;
		m_nValuesCount = rhs.m_nValuesCount;
		m_nOffset = rhs.m_nOffset;
		m_bDynamic = rhs.m_bDynamic;


		m_Type = rhs.m_Type;

		uint size = 0;
		switch(rhs.getType())
		{
		case SShaderConstantType::BT_FLOAT:
			m_pDefaultValues = new float[rhs.m_nValuesCount];
			m_pValues = new float[rhs.m_nValuesCount];
			size = sizeof(float);
			break;

		case SShaderConstantType::BT_INT:
			m_pDefaultValues = new int[rhs.m_nValuesCount];
			m_pValues = new int[rhs.m_nValuesCount];
			size = sizeof(int);
			break;

		case SShaderConstantType::BT_BOOL:
			m_pDefaultValues = new bool[rhs.m_nValuesCount];
			m_pValues = new bool[rhs.m_nValuesCount];
			size = sizeof(bool);
			break;
		}

		if(rhs.m_pValues && rhs.m_nValuesCount)
			memcpy(m_pValues, rhs.m_pValues, rhs.m_nValuesCount * size);
		if(rhs.m_pDefaultValues && rhs.m_nValuesCount)
			memcpy(m_pDefaultValues, rhs.m_pDefaultValues, rhs.m_nValuesCount * size);

		m_pSemantic = rhs.m_pSemantic;
		m_pDataType = rhs.m_pDataType;
		m_bArray = rhs.m_bArray;

		return (*this);
	}
//----------------------------------------------------------------------
	ShaderConstant::~ShaderConstant()
	{
		NGENE_DELETE_ARRAY(m_pValues);
		NGENE_DELETE_ARRAY(m_pDefaultValues);

		m_pSemantic = NULL;
		m_pDataType = NULL;
	}
//----------------------------------------------------------------------
	void ShaderConstant::update()
	{
		if(!m_pSemantic)
			return;

		uint size = 0;
		switch(getType())
		{
		case SShaderConstantType::BT_FLOAT: size = sizeof(float);
			break;
		case SShaderConstantType::BT_INT: size = sizeof(int);
			break;
		case SShaderConstantType::BT_BOOL: size = sizeof(bool);
			break;
		}

		if(m_pSemantic->pFunction)
		{
			m_pSemantic->pFunction(m_pValues, m_nValuesCount * size);
		}

		if(m_pSemantic->function)
		{
			m_pSemantic->function->operator ()(m_pValues, m_nValuesCount);
		}
	}
//----------------------------------------------------------------------
	void ShaderConstant::parse(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;

		// Parse data
		m_stConstantName = pNode->getAttribute("name");
		string stType = pNode->getAttribute("type");
		string stSemantic = pNode->getAttribute("semantic");
		m_stValue = pNode->getValue();

		string stDynamic = pNode->getAttribute("dynamic");
		if(stDynamic.compare("1") == 0 || stDynamic.compare("true") == 0)
			m_bDynamic = true;
		else if	(stDynamic.compare("0") == 0 || stDynamic.compare("false") == 0)
			m_bDynamic = false;

		string stElements = pNode->getAttribute("elements");
		if(stElements != "")
		{
			m_nValuesCount = atoi(stElements.c_str());
			m_bArray = true;
		}


		if((m_stValue.compare("") == 0) && (stSemantic.compare("") == 0))
		{
			wstring stName;
			stName.assign(m_stConstantName.begin(), m_stConstantName.end());
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Value of the constant: %ls not found!", stName.c_str());
		}

		init(stType, stSemantic);
	}
//----------------------------------------------------------------------
	void ShaderConstant::save(File* _file)
	{
		XMLNode::setName("constant");

		Attribute atName;
		atName.first = "name";
		atName.second = m_stConstantName;

		if(m_pSemantic && m_pSemantic->stName != L"")
		{
			Attribute atSemantic;
			atSemantic.first = "semantic";
			atSemantic.second.assign(m_pSemantic->stName.begin(),
				m_pSemantic->stName.end());

			addAttribute(atSemantic);
		}

		if(m_pDataType && m_pDataType->stName != L"")
		{
			Attribute atType;
			atType.first = "type";
			atType.second.assign(m_pDataType->stName.begin(),
				m_pDataType->stName.end());

			addAttribute(atType);
		}

		Attribute atDynamic;
		stringstream buffer;
		buffer << m_bDynamic;
		atDynamic.first = "dynamic";
		atDynamic.second = buffer.str();


		addAttribute(atName);
		addAttribute(atDynamic);


		XMLNode::save(_file);
	}
//----------------------------------------------------------------------
	void ShaderConstant::init(const string& _type, const string& _semantic)
	{
		// Some temporary objects
		ShaderManager& pManager = ShaderManager::getSingleton();

		wstring stType, stSemantic;
		stType.assign(_type.begin(), _type.end());
		stSemantic.assign(_semantic.begin(), _semantic.end());

		if(stSemantic.compare(L""))
		{
			m_pSemantic = pManager.getSemantic(stSemantic);
			// Semantic not found
			if(!m_pSemantic)
				return;
			m_pDataType = pManager.getDataType(m_pSemantic->stDataType);
			if(!m_bArray)
				m_nValuesCount = m_pDataType->nValuesCount;
		}
		else
		{
			m_pDataType = pManager.getDataType(stType);
			if(!m_bArray)
				m_nValuesCount = m_pDataType->nValuesCount;
		}

		m_Type = m_pDataType->baseType;

		// Data-type not found
		if(!m_pDataType)
			return;

		NGENE_DELETE_ARRAY(m_pValues);
		NGENE_DELETE_ARRAY(m_pDefaultValues);


		uint size = 0;
		switch(getType())
		{
		case SShaderConstantType::BT_FLOAT:
			m_pDefaultValues = new float[m_nValuesCount];
			m_pValues = new float[m_nValuesCount];
			size = sizeof(float);
			break;

		case SShaderConstantType::BT_INT:
			m_pDefaultValues = new int[m_nValuesCount];
			m_pValues = new int[m_nValuesCount];
			size = sizeof(int);
			break;

		case SShaderConstantType::BT_BOOL:
			m_pDefaultValues = new bool[m_nValuesCount];
			m_pValues = new bool[m_nValuesCount];
			size = sizeof(bool);
			break;
		}

		memset(m_pValues, 0, m_nValuesCount * size);
		memset(m_pDefaultValues, 0, m_nValuesCount * size);

		if(!m_pSemantic)
			parseValues();
	}
//----------------------------------------------------------------------
	void ShaderConstant::parseValues()
	{
		vector <string> vecValues;
		FuncSplit <string> tokenizer;
		tokenizer(m_stValue, vecValues, " ,");

		SShaderConstantType::BASE_TYPE type = getType();
		if(vecValues.size() != m_nValuesCount)
		{
			wstring stName;
			stName.assign(m_stConstantName.begin(), m_stConstantName.end());
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Invalid number of values: %d instead of: %d for constant: %ls, setting to 0",
				vecValues.size(), m_nValuesCount, stName.c_str());

			uint size = 0;
			switch(type)
			{
			case SShaderConstantType::BT_FLOAT:
				size = sizeof(float);
				break;

			case SShaderConstantType::BT_INT:
				size = sizeof(int);
				break;

			case SShaderConstantType::BT_BOOL:
				size = sizeof(bool);
				break;
			}

			memset(m_pValues, 0, m_nValuesCount * size);

			return;
		}
		
		vector <string>::iterator iter;
		uint i = 0;
		for(iter = vecValues.begin(); iter != vecValues.end(); ++iter)
		{
			switch(type)
			{
			case SShaderConstantType::BT_FLOAT:
				((float*)m_pValues)[i++] = atof(iter->c_str());
				break;

			case SShaderConstantType::BT_INT:
				((int*)m_pValues)[i++] = atoi(iter->c_str());
				break;

			case SShaderConstantType::BT_BOOL:
				int temp = atoi(iter->c_str());
				((bool*)m_pValues)[i++] = (temp ? true : false);
				break;
			}
		}
	}
//----------------------------------------------------------------------
	void ShaderConstant::setDefaultValue(void* _value)
	{
		if(_value)
		{
			uint size = 0;
			switch(getType())
			{
			case SShaderConstantType::BT_FLOAT:
				size = sizeof(float);
				break;

			case SShaderConstantType::BT_INT:
				size = sizeof(int);
				break;

			case SShaderConstantType::BT_BOOL:
				size = sizeof(bool);
				break;
			}

			memcpy(m_pValues, _value, m_nValuesCount * size);
			memcpy(m_pDefaultValues, _value, m_nValuesCount * size);
		}
	}
//----------------------------------------------------------------------
	void ShaderConstant::setDefaultValue(void* _value, uint _count)
	{
		if(_value && _count <= m_nValuesCount)
		{
			uint size = 0;
			switch(getType())
			{
			case SShaderConstantType::BT_FLOAT:
				size = sizeof(float);
				break;

			case SShaderConstantType::BT_INT:
				size = sizeof(int);
				break;

			case SShaderConstantType::BT_BOOL:
				size = sizeof(bool);
				break;
			}

			memcpy(m_pValues, _value, _count * size);
			memcpy(m_pDefaultValues, _value, _count * size);
		}
	}
//----------------------------------------------------------------------
	void ShaderConstant::setValuesCount(uint _count)
	{
		if(m_bArray)
			return;

		m_nValuesCount = _count;
		
		NGENE_DELETE_ARRAY(m_pValues);
		NGENE_DELETE_ARRAY(m_pDefaultValues);


		switch(getType())
		{
		case SShaderConstantType::BT_FLOAT:
			m_pDefaultValues = new float[m_nValuesCount];
			m_pValues = new float[m_nValuesCount];

			break;

		case SShaderConstantType::BT_INT:
			m_pDefaultValues = new int[m_nValuesCount];
			m_pValues = new int[m_nValuesCount];

			break;

		case SShaderConstantType::BT_BOOL:
			m_pDefaultValues = new bool[m_nValuesCount];
			m_pValues = new bool[m_nValuesCount];

			break;
		}
	}
//----------------------------------------------------------------------
	uint ShaderConstant::getValuesCount() const
	{
		return m_nValuesCount;
	}
//----------------------------------------------------------------------
	bool ShaderConstant::hasSemantic() const
	{
		return (m_pSemantic != 0);
	}
//----------------------------------------------------------------------
	void ShaderConstant::setValues(void* _values)
	{
		int size = 0;
		switch(getType())
		{
		case SShaderConstantType::BT_FLOAT:
			size = sizeof(float);
			break;

		case SShaderConstantType::BT_INT:
			size = sizeof(int);
			break;

		case SShaderConstantType::BT_BOOL:
			size = sizeof(bool);
			break;
		}

		memcpy(m_pValues, _values, m_nValuesCount * size);
	}
//----------------------------------------------------------------------
}
