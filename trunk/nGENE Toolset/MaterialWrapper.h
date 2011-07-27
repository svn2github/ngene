/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Material.h
Version:	0.01
---------------------------------------------------------------------------
*/


#pragma once
#ifndef __INC_MATERIALWRAPPER_H_
#define __INC_MATERIALWRAPPER_H_


#include "nGENE.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Collections;
	using namespace System::ComponentModel;
	using namespace System::Drawing;
	using namespace System::Text;

	using nGENE::Material;
	using nGENE::ShaderConstant;
	using nGENE::BLEND_OPERATION;
	using nGENE::FILL_MODE;
	using nGENE::DRAW_ORDER;
	using nGENE::RenderPass;
	using nGENE::SShaderConstantType;
	using nGENE::RenderTechnique;
	using nGENE::ShaderInstance;
	using nGENE::SPriorityMaterial;


	ref class ShaderConstantWrapperCollection;
	ref class ShaderConstantWrapper;


	ref class ShaderConstantWrapperConverter: public ExpandableObjectConverter
	{
	public:
		virtual Object^ ConvertTo(ITypeDescriptorContext^ context,
				System::Globalization::CultureInfo^ culture,
				Object^ value, Type^ destType) override;
	};

	// This is a special type converter which will be associated with the ShaderConstatnWrapperCollection class.
	// It converts an ShaderConstantWrapperCollection object to a string representation for use in a property grid.
	ref class ShaderConstantWrapperCollectionConverter: public ExpandableObjectConverter
	{
	public:
		virtual Object^ ConvertTo(ITypeDescriptorContext^ context,
			System::Globalization::CultureInfo^ culture,
			Object^ value, Type^ destType) override;
	};

	[TypeConverterAttribute(ShaderConstantWrapperConverter::typeid)]
	public ref class ShaderConstantWrapper
	{
	private:
		ShaderConstant* m_pConstant;

		String^ m_Name;
		float m_Value;

		System::Collections::Generic::List <float>^ m_Values;

	public: ShaderConstantWrapper(ShaderConstant* _constant)
			{
				m_Name = "";

				m_Values = gcnew System::Collections::Generic::List <float>();
				m_pConstant = _constant;
			}

			property System::Collections::Generic::List <float>^ Values
			{
				System::Collections::Generic::List <float>^ get()
				{
					if(m_Values->Count == m_pConstant->getValuesCount())
					{
						float* pData = new float[m_pConstant->getValuesCount()];
						for(int i = 0; i < m_pConstant->getValuesCount(); ++i)
						{
							pData[i] = m_Values[i];
						}
						m_pConstant->setValues(pData);

						NGENE_DELETE_ARRAY(pData);
					}

					return m_Values;
				}

				void set(System::Collections::Generic::List <float>^ value)
				{
					m_Values = value;
				}
			}

			property bool Dynamic
			{
				bool get()
				{
					return m_pConstant->isDynamic();
				}
			}

			property String^ DataType
			{
				String^ get()
				{
					switch(m_pConstant->getType())
					{
					case SShaderConstantType::BASE_TYPE::BT_FLOAT: return "float";
					case SShaderConstantType::BASE_TYPE::BT_INT: return "integer";
					case SShaderConstantType::BASE_TYPE::BT_BOOL: return "boolean";
					default: return "float";
					}
				}
			}

			// Uncomment the next line to see the attribute in action: 
			[Category("Required")]
			property String^ Name
			{
				String^ get()
				{
					return m_Name;
				}

				void set(String^ value)
				{
					m_Name = value;
				}
			}

			virtual String^ ToString() override
			{
				StringBuilder^ sb = gcnew StringBuilder();
				sb->Append(Name);
				
				return sb->ToString();
			}
	};

	public ref class ShaderConstantWrapperCollection: public CollectionBase, public ICustomTypeDescriptor
	{
#pragma region collection impl

		/// <summary>
		/// Adds a shader constant object to the collection
		/// </summary>
		/// <param name="emp"></param>
	public: void Add( ShaderConstantWrapper^ emp )
			{
				this->List->Add( emp );
			}

			/// <summary>
			/// Removes a shader constant object from the collection
			/// </summary>
			/// <param name="emp"></param>
	public: void Remove( ShaderConstantWrapper^ emp )
			{
				this->List->Remove( emp );
			}

			/// <summary>
			/// Returns a shader constant object at index position.
			/// </summary>
	public: property ShaderConstantWrapper^ default[ int ] 
			{
				ShaderConstantWrapper^ get(int index)
				{
					return (ShaderConstantWrapper^)this->List[index];
				}
			}

#pragma endregion

			// Implementation of interface ICustomTypeDescriptor 
			//#pragma region ICustomTypeDescriptor impl 

	public: virtual String^ GetClassName()
			{
				return TypeDescriptor::GetClassName(this,true);
			}

	public: virtual AttributeCollection^ GetAttributes()
			{
				return TypeDescriptor::GetAttributes(this,true);
			}

	public: virtual String^ GetComponentName()
			{
				return TypeDescriptor::GetComponentName(this, true);
			}

	public: virtual TypeConverter^ GetConverter()
			{
				return TypeDescriptor::GetConverter(this, true);
			}

	public: virtual EventDescriptor^ GetDefaultEvent() 
			{
				return TypeDescriptor::GetDefaultEvent(this, true);
			}

	public: virtual PropertyDescriptor^ GetDefaultProperty() 
			{
				return TypeDescriptor::GetDefaultProperty(this, true);
			}

	public: virtual Object^ GetEditor(Type^ editorBaseType) 
			{
				return TypeDescriptor::GetEditor(this, editorBaseType, true);
			}

	public: virtual EventDescriptorCollection^ GetEvents(cli::array<Attribute^>^ attributes) 
			{
				return TypeDescriptor::GetEvents(this, attributes, true);
			}

	public: virtual EventDescriptorCollection^ GetEvents()
			{
				return TypeDescriptor::GetEvents(this, true);
			}

	public: virtual Object^ GetPropertyOwner(PropertyDescriptor^ pd)
			{
				return this;
			}


			/// <summary>
			/// Called to get the properties of this type. Returns properties with certain
			/// attributes. this restriction is not implemented here.
			/// </summary>
			/// <param name="attributes"></param>
			/// <returns></returns>
	public: virtual PropertyDescriptorCollection^ GetProperties(cli::array<Attribute^>^ attributes)
			{
				return GetProperties();
			}

			/// <summary>
			/// Called to get the properties of this type.
			/// </summary>
			/// <returns></returns>
	public: virtual PropertyDescriptorCollection^ GetProperties();

			//#pragma endregion
	};

	public ref class ShaderConstantWrapperCollectionPropertyDescriptor : public PropertyDescriptor
	{
	private:
		ShaderConstantWrapperCollection^ collection;
		int index;

	public:
		ShaderConstantWrapperCollectionPropertyDescriptor(ShaderConstantWrapperCollection^ coll, int idx) : 
		PropertyDescriptor("#"+idx.ToString(), nullptr)
		{
			this->collection = coll;
			this->index = idx;
		} 

	public: virtual property AttributeCollection^ Attributes
			{
				AttributeCollection^ get() override
				{ 
					return gcnew AttributeCollection(nullptr);
				}
			}

	public: virtual bool CanResetValue(Object^ component) override
			{
				return true;
			}

	public: virtual property Type^ ComponentType
			{
				Type^ get() override
				{ 
					return this->collection->GetType();
				}
			}

	public: virtual property String^ DisplayName
			{
				String^ get() override
				{
					ShaderConstantWrapper^ emp = this->collection[index]; 
					return emp->Name;
				}
			}

	public: virtual property String^ Description
			{
				String^ get() override
				{
					ShaderConstantWrapper^ emp = this->collection[index]; 
					StringBuilder^ sb = gcnew StringBuilder();
					sb->Append(emp->Name);

					return sb->ToString();
				}
			}

	public: virtual Object^ GetValue(Object^ component) override
			{
				return this->collection[index];
			}

	public: virtual property bool IsReadOnly
			{
				bool get() override
				{
					return false;
				}
			}

	public: virtual property String^ Name
			{
				String^ get() override
				{
					return "#"+index.ToString();
				}
			}

	public: virtual property Type^ PropertyType
			{
				Type^ get() override
				{
					return this->collection[index]->GetType();
				}
			}

	public: virtual void ResetValue(Object^ component) override
			{
			}

	public: virtual bool ShouldSerializeValue(Object^ component) override
			{
				return true;
			}

	public: virtual void SetValue(Object^ component, Object^ value) override
			{
				// this.collection[index] = value;
			}
	};

	ref class frmMaterialLib;


	enum class RenderMode
	{
		Solid,
		Wireframe,
		Dot
	};

	enum class BlendingOp
	{
		Add,
		Subtract,
		Revsubtract,
		Min,
		Max
	};

	enum class BlendType
	{
		Zero = 1,
		One = 2,
		SrcColor = 3,
		InvSrcColor = 4,
		SrcAlpha = 5,
		InvSrcAlpha = 6,
		DestAlpha = 7,
		InvDestAlpha = 8,
		DestColor = 9,
		InvDestColor = 10,
		SrcAlphaSat = 11,
		BothSrcAlpha = 12,
		BothInvSrcAlpha = 13,
		BlendFactor = 14,
		InvBlendFactor = 15,
	};


	ref class FXWrapper
	{
	protected:
		SPriorityMaterial* m_pMaterial;
		int m_nPriority;

	public:
		FXWrapper(SPriorityMaterial* _material)
		{
			m_pMaterial = _material;
		}

		[Browsable(true), CategoryAttribute("Material"),
		DescriptionAttribute("Priority of the material")]
		property int Priority
		{
			int get()
			{
				return m_pMaterial->priority;
			}

			void set(int value)
			{
				m_pMaterial->priority = value;
			}
		}

	public: virtual String^ ToString() override
			{
				return gcnew String(m_pMaterial->material->getName().c_str());
			}

	public: SPriorityMaterial* getMaterial()
			{
				return m_pMaterial;
			}
	};

	
	/// Wrapper for a Material class.
	public ref class MaterialWrapper
	{
	protected:
		Material* m_pMaterial;
		frmMaterialLib^ m_MatLib;

		StringBuilder^ m_Name;
		StringBuilder^ m_Key;

		bool m_PostProcess;

		SPriorityMaterial* m_PostMat;

		ShaderConstantWrapperCollection^ constants; 

	public:
		MaterialWrapper(Material* _material, frmMaterialLib^ _matLib, Hashtable^ _lib);

		virtual ~MaterialWrapper()
		{
			NGENE_DELETE(m_PostMat);
		}

		Material* getMaterial()
		{
			return m_pMaterial;
		}

		public:
		
		[TypeConverterAttribute(ShaderConstantWrapperCollectionConverter::typeid),
		Browsable(true), CategoryAttribute("Constants"),
		DescriptionAttribute("Name of a material")]
		property ShaderConstantWrapperCollection^ Constants
		{
			ShaderConstantWrapperCollection^ get()
			{
				return constants;
			}
		}

		[Browsable(true), CategoryAttribute("Material"),
		DescriptionAttribute("Name of a material")]
		property String^ Name
		{
			String^ get()
			{
				return m_Name->ToString();
			}

			void set(String^ value)
			{
				if(m_Name != nullptr)
					delete m_Name;

				m_Name = gcnew StringBuilder(value);
			}
		}

		[Browsable(false), CategoryAttribute("Material"),
		DescriptionAttribute("Key of a material")]
		property String^ Key
		{
			String^ get()
			{
				return m_Key->ToString();
			}

			void set(String^ value)
			{
				if(m_Key != nullptr)
					delete m_Key;

				m_Key = gcnew StringBuilder(value);
			}
		}

		[Browsable(true), CategoryAttribute("Colour"),
		DescriptionAttribute("Emissive factor of the material")]
		property float Emissive
		{
			float get()
			{
				return m_pMaterial->getEmissive();
			}

			void set(float value)
			{
				m_pMaterial->setEmissive(value);
			}
		}

		[Browsable(true), CategoryAttribute("Blending"),
		DescriptionAttribute("Specifies if material is transparent")]
		property bool Transparent
		{
			bool get()
			{
				return m_pMaterial->isTransparent();
			}

			void set(bool value)
			{
				m_pMaterial->setTransparent(value);
			}
		}

		[Browsable(true), CategoryAttribute("Blending"),
		DescriptionAttribute("Specifies if material uses alpha testing")]
		property bool AlphaTest
		{
			bool get()
			{
				return m_pMaterial->isAlphaTest();
			}

			void set(bool value)
			{
				m_pMaterial->setAlphaTest(value);
			}
		}

		[Browsable(true), CategoryAttribute("Material"),
		DescriptionAttribute("Specifies if material is two-sided")]
		property bool TwoSided
		{
			bool get()
			{
				return m_pMaterial->isTwoSided();
			}

			void set(bool value)
			{
				m_pMaterial->setTwoSided(value);
			}
		}

		[Browsable(true), CategoryAttribute("Material"),
		DescriptionAttribute("Specifies if material writes to the z-buffer")]
		property bool ZWrite
		{
			bool get()
			{
				return m_pMaterial->isZWrite();
			}

			void set(bool value)
			{
				m_pMaterial->setZWrite(value);
			}
		}

		[Browsable(true), CategoryAttribute("Material"),
		DescriptionAttribute("Specifies if material is enabled")]
		property bool Enabled
		{
			bool get()
			{
				return m_pMaterial->isEnabled();
			}

			void set(bool value)
			{
				m_pMaterial->setEnabled(value);
			}
		}

		[Browsable(true), CategoryAttribute("Material"),
		DescriptionAttribute("Specifies source blending")]
		property BlendType SourceBlending
		{
			BlendType get()
			{
				return (BlendType)m_pMaterial->getSrcBlend();
			}

			void set(BlendType value)
			{
				m_pMaterial->setSrcBlend((int)value);
			}
		}

		[Browsable(true), CategoryAttribute("Material"),
		DescriptionAttribute("Specifies destination blending")]
		property BlendType DestinationBlending
		{
			BlendType get()
			{
				return (BlendType)m_pMaterial->getDestBlend();
			}

			void set(BlendType value)
			{
				m_pMaterial->setDestBlend((int)value);
			}
		}

		[Browsable(true), CategoryAttribute("Material"),
		DescriptionAttribute("Specifies rendering mode of the material")]
		property RenderMode RenderingMode
		{
			RenderMode get()
			{
				FILL_MODE value = (FILL_MODE)m_pMaterial->getFillMode();

				switch(value)
				{
				case FILL_MODE::FILL_SOLID: return RenderMode::Solid;
				case FILL_MODE::FILL_WIREFRAME: return RenderMode::Wireframe;
				case FILL_MODE::FILL_DOT: return RenderMode::Dot;
				default: return RenderMode::Solid;
				}
			}

			void set(RenderMode value)
			{
				switch(value)
				{
				case RenderMode::Solid: m_pMaterial->setFillMode(FILL_MODE::FILL_SOLID);
							break;
				case RenderMode::Wireframe: m_pMaterial->setFillMode(FILL_MODE::FILL_WIREFRAME);
							break;
				case RenderMode::Dot: m_pMaterial->setFillMode(FILL_MODE::FILL_DOT);
							break;
				}
			}
		}

		[Browsable(true), CategoryAttribute("Material"),
		DescriptionAttribute("Specifies blending operation for this material")]
		property BlendingOp BlendingOperation
		{
			BlendingOp get()
			{
				BLEND_OPERATION value = m_pMaterial->getBlendOperation();

				switch(value)
				{
				case BLEND_OPERATION::BO_ADD: return BlendingOp::Add;
				case BLEND_OPERATION::BO_SUBTRACT: return BlendingOp::Subtract;
				case BLEND_OPERATION::BO_REVSUBTRACT: return BlendingOp::Revsubtract;
				case BLEND_OPERATION::BO_MIN: return BlendingOp::Min;
				case BLEND_OPERATION::BO_MAX: return BlendingOp::Max;
				default: return BlendingOp::Add;
				}
			}

			void set(BlendingOp value)
			{
				switch(value)
				{
				case BlendingOp::Add: m_pMaterial->setBlendOperation(BLEND_OPERATION::BO_ADD);
					break;
				case BlendingOp::Subtract: m_pMaterial->setBlendOperation(BLEND_OPERATION::BO_SUBTRACT);
					break;
				case BlendingOp::Revsubtract: m_pMaterial->setBlendOperation(BLEND_OPERATION::BO_REVSUBTRACT);
					break;
				case BlendingOp::Min: m_pMaterial->setBlendOperation(BLEND_OPERATION::BO_MIN);
					break;
				case BlendingOp::Max: m_pMaterial->setBlendOperation(BLEND_OPERATION::BO_MAX);
					break;
				}
			}
		}

		[Browsable(true), CategoryAttribute("Light"),
		DescriptionAttribute("Specifies if material casts shadows")]
		property bool CastShadow
		{
			bool get()
			{
				return m_pMaterial->isCastingShadow();
			}

			void set(bool value)
			{
				m_pMaterial->setCastShadow(value);
			}
		}

		[Browsable(true), CategoryAttribute("Light"),
		DescriptionAttribute("Specifies if material is affected by lights")]
		property bool Lightable
		{
			bool get()
			{
				return m_pMaterial->isLightable();
			}

			void set(bool value)
			{
				m_pMaterial->setLightable(value);
			}
		}

		[Browsable(false), CategoryAttribute("Material")]
		property bool PostProcess
		{
			bool get()
			{
				return (m_pMaterial->getRenderingOrder() == DRAW_ORDER::DO_POST ? true : false);
			}

			void set(bool value)
			{
				m_PostProcess = (value ? DRAW_ORDER::DO_POST : DRAW_ORDER::DO_SCENE);
			}
		}

		public: SPriorityMaterial* applyPost()
				{
					m_PostMat = new SPriorityMaterial;

					m_PostMat->priority = 0;
					m_PostMat->material = m_pMaterial;

					((nGENE::PostStage*)nGENE::Renderer::getSingleton().getRenderStage(L"PostProcess"))->addToRender(m_PostMat);

					return m_PostMat;
				}
	};

}


#endif