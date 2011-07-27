/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsMaterial.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PHYSICSMATERIAL_H_
#define __INC_PHYSICSMATERIAL_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "TypeInfo.h"
#include "XMLNode.h"


namespace nGENE
{
	/** A wrapper for PhysX material.
		@remarks
			This class describes some basic physical attributes of the
			rigid body, eg. its restitution and friction (both static and
			dynamic).
	*/
	class nGENEDLL PhysicsMaterial: public BaseClass, public XMLNode
	{
		EXPOSE_TYPE
	private:
		static uint s_nMaterialCount;			///< How many materials exists
		uint m_nMaterialIndex;					///< Material index

		NxScene* m_pScene;						///< Pointer to the PhysX world
		NxMaterial* m_pMaterial;				///< Pointer to the PhysX material

		wstring m_stName;						///< Material's name

		float m_fRestitution;					///< Restitution
		float m_fStaticFriction;				///< Static friction
		float m_fDynamicFriction;				///< Dynamic friction

	public:
		PhysicsMaterial(NxScene* _scene);
		virtual ~PhysicsMaterial();

		void init();
		void cleanup();

		void parse(XMLDocument* _node);
		void load() {}
		void save(File* _file);

		/// Sets name of this material.
		void setName(const wstring& _name);
		/// Returns name of this material.
		wstring& getName();

		/** Sets dynamic friction coefficient.
			@param
				_restitution coefficient value. Valid values are in
				the range <0.0f, 1.0f>.
		*/
		void setRestitution(float _restitution);
		/// Returns restitution coefficient.
		Real getRestitution() const;

		/** Sets static friction coefficient.
			@param
				_friction coefficient value. Valid values are in
				the range <0.0f, INFINITY).
		*/
		void setStaticFriction(float _friction);
		/// Returns static friction coefficient.
		Real getStaticFriction() const;

		/** Sets dynamic friction coefficient.
			@param
				_friction coefficient value. Valid values are in
				the range <0.0f, INFINITY).
		*/
		void setDynamicFriction(float _friction);
		/// Returns dynamic friction coefficient.
		Real getDynamicFriction() const;

		uint getMaterialIndex() const;
	};



	inline void PhysicsMaterial::setName(const wstring& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
	inline wstring& PhysicsMaterial::getName()
	{
		return m_stName;
	}
//----------------------------------------------------------------------
	inline Real PhysicsMaterial::getRestitution() const
	{
		return m_fRestitution;
	}
//----------------------------------------------------------------------
	inline Real PhysicsMaterial::getStaticFriction() const
	{
		return m_fStaticFriction;
	}
//----------------------------------------------------------------------
	inline Real PhysicsMaterial::getDynamicFriction() const
	{
		return m_fDynamicFriction;
	}
//----------------------------------------------------------------------
	inline uint PhysicsMaterial::getMaterialIndex() const
	{
		return m_nMaterialIndex;
	}
//----------------------------------------------------------------------
}


#endif