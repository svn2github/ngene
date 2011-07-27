/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MaterialWrapper.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "MaterialWrapper.h"

#include "frmMaterialLib.h"


namespace nGENEToolset
{
	MaterialWrapper::MaterialWrapper(Material* _material, frmMaterialLib^ _matLib, Hashtable^ _lib)
	{
		m_MatLib = _matLib;
		m_pMaterial = _material;
		m_PostMat = NULL;
		
		String^ name = gcnew String(_material->getName().c_str());
		this->Name = name;
		this->Key = name;

		constants = gcnew ShaderConstantWrapperCollection();

		m_PostProcess = (m_pMaterial->getRenderingOrder() == DRAW_ORDER::DO_POST ? true : false);

		_matLib->addMaterial(this, _lib);


		for(int tech = 0; tech < m_pMaterial->getRenderTechniquesNum(); ++tech)
		{
			RenderTechnique* pTechnique = m_pMaterial->getRenderTechnique(tech);
			for(int pass = 0; pass < pTechnique->getPassesNum(); ++pass)
			{
				RenderPass& passRef = pTechnique->getRenderPass(pass);
				
				ShaderInstance* pVS = passRef.getVertexShader();
				ShaderInstance* pGS = passRef.getGeometryShader();
				ShaderInstance* pPS = passRef.getPixelShader();

				if(pVS)
				{
					for(int constant = 0; constant < pVS->getConstantsNum(); ++constant)
					{
						ShaderConstant* pConstant = pVS->getConstant(constant);
						if(pConstant->getValuesCount() && !pConstant->hasSemantic() && !pConstant->isArray())
						{
							float* pValues = (float*)pConstant->getValues();

							ShaderConstantWrapper^ tmp = gcnew ShaderConstantWrapper(pConstant);
							tmp->Name = gcnew String(pConstant->getName().c_str());
							//tmp->Value = pValues[0];
							for(int value = 0; value < pConstant->getValuesCount(); ++value)
							{
								tmp->Values->Add(pValues[value]);
							}

							this->Constants->Add(tmp);
						}
					}
				}

				if(pGS)
				{
					for(int constant = 0; constant < pGS->getConstantsNum(); ++constant)
					{
						ShaderConstant* pConstant = pGS->getConstant(constant);
						if(pConstant->getValuesCount() && !pConstant->hasSemantic() && !pConstant->isArray())
						{
							float* pValues = (float*)pConstant->getValues();

							ShaderConstantWrapper^ tmp = gcnew ShaderConstantWrapper(pConstant);
							tmp->Name = gcnew String(pConstant->getName().c_str());
							//tmp->Value = pValues[0];
							for(int value = 0; value < pConstant->getValuesCount(); ++value)
							{
								tmp->Values->Add(pValues[value]);
							}

							this->Constants->Add(tmp);
						}
					}
				}

				if(pPS)
				{
					for(int constant = 0; constant < pPS->getConstantsNum(); ++constant)
					{
						ShaderConstant* pConstant = pPS->getConstant(constant);
						if(pConstant->getValuesCount() && !pConstant->hasSemantic() && !pConstant->isArray())
						{
							float* pValues = (float*)pConstant->getValues();

							ShaderConstantWrapper^ tmp = gcnew ShaderConstantWrapper(pConstant);
							tmp->Name = gcnew String(pConstant->getName().c_str());
							//tmp->Value = pValues[0];
							for(int value = 0; value < pConstant->getValuesCount(); ++value)
							{
								tmp->Values->Add(pValues[value]);
							}

							this->Constants->Add(tmp);
						}
					}
				}
			}
		}

	}
//---------------------------------------------------------------------
	PropertyDescriptorCollection^ ShaderConstantWrapperCollection::GetProperties()
	{
		// Create a collection object to hold property descriptors
		PropertyDescriptorCollection^ pds = gcnew PropertyDescriptorCollection(nullptr);

		// Iterate the list of employees
		for(int i = 0; i < this->List->Count; ++i)
		{
			// Create a property descriptor for the employee item and add to the property descriptor collection
			ShaderConstantWrapperCollectionPropertyDescriptor^ pd =
				gcnew ShaderConstantWrapperCollectionPropertyDescriptor(this, i);
			pds->Add(pd);
		}

		// return the property descriptor collection
		return pds;
	}
//---------------------------------------------------------------------
	Object^ ShaderConstantWrapperConverter::ConvertTo(ITypeDescriptorContext^ context,
		System::Globalization::CultureInfo^ culture, Object^ value, Type^ destType)
	{
		if(destType == String::typeid &&
			dynamic_cast<ShaderConstantWrapper^>(value) != nullptr)
		{
			// Cast the value to an ShaderConstantWrapper type
			ShaderConstantWrapper^ emp = (ShaderConstantWrapper^)value;

			// Return department and department role separated by comma.
			return emp->Name;
		}

		return ExpandableObjectConverter::ConvertTo(context,
			culture, value, destType);
	}
//---------------------------------------------------------------------
	Object^ ShaderConstantWrapperCollectionConverter::ConvertTo(ITypeDescriptorContext^ context,
		System::Globalization::CultureInfo^ culture, Object^ value, Type^ destType)
	{
		if(destType == String::typeid &&
			dynamic_cast<ShaderConstantWrapperCollection^>(value) != nullptr)
		{
			// Return department and department role separated by comma.
			return "Shader constants";
		}

		return ExpandableObjectConverter::ConvertTo(context,
			culture, value, destType);
	}
//---------------------------------------------------------------------
}