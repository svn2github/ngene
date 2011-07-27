/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Material.h
Version:	0.26
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MATERIAL_H_
#define __INC_MATERIAL_H_


#include "Colour.h"
#include "Prerequisities.h"
#include "RenderTechnique.h"
#include "Thread.h"
#include "TypeInfo.h"
#include "XMLNode.h"


namespace nGENE
{
	/** Material describes physical attributes of the object.
		@par
			Materials could be assigned to all objects of NodeVisible
			class (or to be more precise - to its surfaces). Although
			each node can have multiple materials, a surface can have
			only one.
		@par
			Only one material can be active at once. In sake of efficiency,
			it is recommended that Nodes in the scene are sorted so less
			material changes are required (nGENE takes care of this, unless
			you provide your own SceneManager or ResourceManager).
		@par
			It is only essential to note that even though Material class specifies
			numerous colour parameters (namely diffuse, ambient, specular and emissive)
			it is renderer responsibility to interpret and use them. Otherwise they
			will be useless.
	*/
	class nGENEDLL Material: public XMLNode
	{
		EXPOSE_TYPE
	private:
		XMLDocument* m_pData;					///< Material data
		MaterialLibrary* m_pMaterialLib;		///< Pointer to material library

		wstring m_stName;						///< Material's name

		Colour m_Ambient;						///< Ambient colour
		Colour m_Diffuse;						///< Diffuse colour
		Colour m_Specular;						///< Specular colour
		
		Real m_fEmissive;						///< Emissive factor

		float m_fSpecularPower;					///< Shininess
		float m_fZBias;							///< Value of polygon offset.

		DRAW_ORDER m_DrawOrder;					///< Material rendering order
		BLEND_OPERATION m_BlendOP;				///< Blending operation

		uint m_SrcBlend;
		uint m_DestBlend;

		dword m_dwAlphaTestRef;					///< Alpha testing reference
		FILL_MODE m_FillMode;					///< Material fill mode (solid by default)

		bool m_bTwoSided;						///< Two sided material causes culling disabling
		bool m_bZWrite;							///< Is Z buffer writing enabled?
		bool m_bZBuffer;						///< Is Z-buffer enabled for this material?
		bool m_bCastShadow;						///< Is casting shadow?
		bool m_bTransparent;					///< Is transparent?
		bool m_bAlphaTest;						///< Is alpha tested?

		bool m_bLightable;						///< Could be lit?
		bool m_bEnabled;						///< Is material enabled?
		bool m_bFullscreenScissor;				///< Is scissor test enabled?

		uint m_nPriority;						///< Priority of this material


		vector <RenderTechnique> m_vTechniques;	///< Techniques in which material is rendered

		Material* m_pParent;					///< Parental material
		RenderTechnique* m_pActiveTechnique;	///< Active RenderTechnique for this material

		vector <Surface*> m_vSurfaces;			///< Surfaces this material is applied to

		bool m_bCreated;						///< Has this material been created?

		CriticalSection m_CriticalSection;


		void createMaterial(XMLDocument* _node);

	public:
		Material();
		explicit Material(const wstring& _name);
		~Material();

		void cleanup();


		void parse(XMLDocument* _node);
		void load() {}
		void save(File* _file);

		/// Sets the material.
		void bind();
		/// Disables the material.
		void unbind();


		/// Sets material library to which material is assigned.
		void setMaterialLibrary(MaterialLibrary* _library);
		/// Returns pointer to the MaterialLibrary this material belongs to.
		MaterialLibrary* getMaterialLibrary() const;

		/// Returns the parental material.
		void setParent(Material* _parent);
		/// Returns pointer to the parent Material object.
		Material* getParent() const;

		/// Sets the name of the material.
		void setName(const wstring& _name);
		/// Returns the name of the material.
		wstring& getName();

		/** Sets whether material is two-sided.
			@param _value if true then material will be consider two-sided.
			@remarks
				Useful for such objects as billboards or impostors.
		*/
		void setTwoSided(bool _value);
		/// Checks if this material is two sided.
		bool isTwoSided() const;

		/** Specifies whether rendering of this material writes anything
			to the Z-Buffer.
		*/
		void setZWrite(bool _value);
		/// Checks if Z-Write is enabled for this material.
		bool isZWrite() const;

		/// Specifies whether Z-buffer is used when rendering this material.
		void setZEnabled(bool _value);
		/// Checks if Z-buffer is enabled for this material.
		bool isZEnabled() const;

		/// Sets ambient colour for this material.
		void setAmbient(Colour& _colour) {m_Ambient=_colour;}
		Colour& getAmbient() {return m_Ambient;}

		/// Sets diffuse colour for this material.
		void setDiffuse(Colour& _colour) {m_Diffuse=_colour;}
		Colour& getDiffuse() {return m_Diffuse;}

		/// Sets specular colour for this material.
		void setSpecular(Colour& _colour) {m_Specular=_colour;}
		Colour& getSpecular() {return m_Specular;}

		/// Sets emissive factor for this material.
		void setEmissive(Real _emissive);
		/// Returns emissive factor for this material.
		Real getEmissive() const;

		/** Sets Z-Bias for this material.
			@remarks
				Z-Bias is also known as polygon offset and is used frequently
				to solve coplanar materials Z-fighting problem by offsetting
				one of them.
		*/
		void setZBias(float _value);
		/// Returns Z-Bias value for this material.
		float getZBias() const;

		/// Sets blending operation for this material.
		void setBlendOperation(BLEND_OPERATION _operation);
		/// Returns blending operation for this material.
		BLEND_OPERATION getBlendOperation() const;

		/// Specifies whether this material casts shadows.
		void setCastShadow(bool _value);
		/// Checks if this material is casting shadows.
		bool isCastingShadow() const;

		/// Sets material transparency.
		void setTransparent(bool _value);
		/// Checks if this material is transparent.
		bool isTransparent() const;

		/// Specifies whether alpha testing should be used for this material.
		void setAlphaTest(bool _value);
		/// Checks if is alpha tested.
		bool isAlphaTest() const;

		/// Sets alpha test reference value.
		void setAlphaTestReference(dword _value);
		/// Returns alpha test reference value.
		dword getAlphaTestReference() const;

		/** Specifies whether material is enabled.
			@remarks
				Only enabled materials can be rendered.
		*/
		void setEnabled(bool _value);
		/// Toggles 'enabled' parameter value.
		void toggleEnabled();
		/// Checks if this material is enabled.
		bool isEnabled() const;

		/// Specifies whether lights affect this material.
		void setLightable(bool _value);
		/// Checks if lights affect this material.
		bool isLightable() const;

		/// Sets fill mode.
		void setFillMode(FILL_MODE _mode);
		/// Returns fill mode.
		FILL_MODE getFillMode() const;

		/// Returns rendering order for this material.
		DRAW_ORDER getRenderingOrder() const;

		/// Sets source alpha blending mode.
		void setSrcBlend(uint _value);
		/// Returns source alpha blending mode.
		uint getSrcBlend() const;

		/// Sets destination alpha blending mode.
		void setDestBlend(uint _value);
		/// Returns destination alpha blending mode.
		uint getDestBlend() const;

		/// Adds new render technique to the material.
		void addRenderTechnique(RenderTechnique& _technique);

		/** Performs rendering of the material.
			@remarks
				Active technique is rendered.
		*/
		void render(RenderQueue* _queue);

		/// Returns render technique with the given index or NULL if not found.
		RenderTechnique* getRenderTechnique(uint _index);
		/// Returns render technique with the given name or NULL if not found.
		RenderTechnique* getRenderTechnique(const wstring& _name);

		/// Checks if material have technique with the given name.
		bool hasRenderTechnique(const wstring& _name);


		/// Sets render technique with the given index as active.
		void setActiveRenderTechnique(uint _index);
		/** Sets render technique for this material.
			@param _technique to be set. It cannot be NULL.
		*/
		void setActiveRenderTechnique(RenderTechnique* _technique);

		/// Returns active render technique.
		RenderTechnique* getActiveRenderTechnique();

		/// Returns number of render techniques.
		uint getRenderTechniquesNum() const;


		/// Sets material priority.
		void setPriority(uint _priority);
		/// Returns material priority.
		uint getPriority() const;

		/// Remove surface.
		void removeSurface(Surface* _surface);
		/// Add surface.
		void addSurface(Surface* _surface);
		/// Returns surface with the given index.
		Surface* getSurface(uint _index) const;
		/// Returns number of surfaces to which this material is bound.
		uint getSurfacesBoundNum() const;

		void materialAccessed();
	};



	inline void Material::setMaterialLibrary(MaterialLibrary* _library)
	{
		m_pMaterialLib = _library;
	}
//----------------------------------------------------------------------
	inline MaterialLibrary* Material::getMaterialLibrary() const
	{
		return m_pMaterialLib;
	}
//----------------------------------------------------------------------
	inline void Material::setParent(Material* _parent)
	{
		m_pParent = _parent;
	}
//----------------------------------------------------------------------
	inline Material* Material::getParent() const
	{
		return m_pParent;
	}
//----------------------------------------------------------------------
	inline void Material::setEnabled(bool _value)
	{
		m_bEnabled = _value;
	}
//----------------------------------------------------------------------
	inline void Material::toggleEnabled()
	{
		m_bEnabled = m_bEnabled ? false : true;
	}
//----------------------------------------------------------------------
	inline bool Material::isEnabled() const
	{
		return m_bEnabled;
	}
//----------------------------------------------------------------------
	inline uint Material::getRenderTechniquesNum() const
	{
		return m_vTechniques.size();
	}
//----------------------------------------------------------------------
	inline void Material::setAlphaTestReference(dword _value)
	{
		m_dwAlphaTestRef = _value;
	}
//----------------------------------------------------------------------
	inline dword Material::getAlphaTestReference() const
	{
		return m_dwAlphaTestRef;
	}
//----------------------------------------------------------------------
	inline wstring& Material::getName()
	{
		return m_stName;
	}
//----------------------------------------------------------------------
	inline void Material::setName(const wstring& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
	inline void Material::setTwoSided(bool _value)
	{
		m_bTwoSided = _value;
	}
//----------------------------------------------------------------------
	inline bool Material::isTwoSided() const
	{
		return m_bTwoSided;
	}
//----------------------------------------------------------------------
	inline void Material::setZWrite(bool _value)
	{
		m_bZWrite = _value;
	}
//----------------------------------------------------------------------
	inline bool Material::isZWrite() const
	{
		return m_bZWrite;
	}
//----------------------------------------------------------------------
	inline void Material::setZEnabled(bool _value)
	{
		m_bZBuffer = _value;
	}
//----------------------------------------------------------------------
	inline bool Material::isZEnabled() const
	{
		return m_bZBuffer;
	}
//----------------------------------------------------------------------
	inline void Material::setFillMode(FILL_MODE _mode)
	{
		m_FillMode = _mode;
	}
//----------------------------------------------------------------------
	inline FILL_MODE Material::getFillMode() const
	{
		return m_FillMode;
	}
//----------------------------------------------------------------------
	inline void Material::setLightable(bool _value)
	{
		m_bLightable = _value;
	}
//----------------------------------------------------------------------
	inline bool Material::isLightable() const
	{
		return m_bLightable;
	}
//----------------------------------------------------------------------
	inline void Material::setAlphaTest(bool _value)
	{
		m_bAlphaTest = _value;
	}
//----------------------------------------------------------------------
	inline bool Material::isAlphaTest() const
	{
		return m_bAlphaTest;
	}
//----------------------------------------------------------------------
	inline void Material::setTransparent(bool _value)
	{
		m_bTransparent = _value;
	}
//----------------------------------------------------------------------
	inline bool Material::isTransparent() const
	{
		return m_bTransparent;
	}
//----------------------------------------------------------------------
	inline void Material::setCastShadow(bool _value)
	{
		m_bCastShadow = _value;
	}
//----------------------------------------------------------------------
	inline bool Material::isCastingShadow() const
	{
		return m_bCastShadow;
	}
//----------------------------------------------------------------------
	inline DRAW_ORDER Material::getRenderingOrder() const
	{
		return m_DrawOrder;
	}
//----------------------------------------------------------------------
	inline RenderTechnique* Material::getRenderTechnique(uint _index)
	{
		materialAccessed();

		return &m_vTechniques[_index];
	}
//----------------------------------------------------------------------
	inline void Material::setZBias(float _value)
	{
		m_fZBias = _value;
	}
//----------------------------------------------------------------------
	inline float Material::getZBias() const
	{
		return m_fZBias;
	}
//----------------------------------------------------------------------
	inline void Material::setEmissive(Real _emissive)
	{
		m_fEmissive = _emissive;
	}
//----------------------------------------------------------------------
	inline Real Material::getEmissive() const
	{
		return m_fEmissive;
	}
//----------------------------------------------------------------------
	inline void Material::setBlendOperation(BLEND_OPERATION _operation)
	{
		m_BlendOP = _operation;
	}
//----------------------------------------------------------------------
	inline BLEND_OPERATION Material::getBlendOperation() const
	{
		return m_BlendOP;
	}
//----------------------------------------------------------------------
	inline void Material::setSrcBlend(uint _value)
	{
		m_SrcBlend = _value;
	}
//----------------------------------------------------------------------
	inline uint Material::getSrcBlend() const
	{
		return m_SrcBlend;
	}
//----------------------------------------------------------------------
	inline void Material::setDestBlend(uint _value)
	{
		m_DestBlend = _value;
	}
//----------------------------------------------------------------------
	inline uint Material::getDestBlend() const
	{
		return m_DestBlend;
	}
//----------------------------------------------------------------------
	inline void Material::setPriority(uint _priority)
	{
		m_nPriority = _priority;
	}
//----------------------------------------------------------------------
	inline uint Material::getPriority() const
	{
		return m_nPriority;
	}
//----------------------------------------------------------------------
	inline void Material::removeSurface(Surface* _surface)
	{
		for(uint i = 0; i < m_vSurfaces.size(); ++i)
		{
			if(m_vSurfaces[i] == _surface)
			{
				m_vSurfaces.erase(m_vSurfaces.begin() + i);
				return;
			}
		}
	}
//----------------------------------------------------------------------
	inline void Material::addSurface(Surface* _surface)
	{
		m_vSurfaces.push_back(_surface);
	}
//----------------------------------------------------------------------
	inline Surface* Material::getSurface(uint _index) const
	{
		if(_index >= m_vSurfaces.size())
			return NULL;

		return m_vSurfaces[_index];
	}
//----------------------------------------------------------------------
	inline uint Material::getSurfacesBoundNum() const
	{
		return m_vSurfaces.size();
	}
//----------------------------------------------------------------------
}


#endif