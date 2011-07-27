/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IPickable.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_IPICKABLE_H_
#define __INC_IPICKABLE_H_


#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Pure interface for all classes which have ability
		of being picked i.e. selected using mouse.
	*/
	class nGENEDLL IPickable
	{
		EXPOSE_TYPE
	protected:
		bool m_bPickable;					///< Is object pickable?

	public:
		/** Sets whether object is pickable i.e. whether it could be
			picked via mouse clicking.
		*/
		void setPickable(bool _value);
		/// Checks if this object responds to picking request.
		bool isPickable() const;

		/** Tries to pick the object using provided ray.
			@param _ray to test against.
			@returns
				true if the object can be picked using provided
				ray and false otherwise.
		*/
		virtual bool tryToPick(const Ray& _ray)=0;
	};



	inline void IPickable::setPickable(bool _value)
	{
		m_bPickable = _value;
	}
//----------------------------------------------------------------------
	inline bool IPickable::isPickable() const
	{
		return m_bPickable;
	}
//----------------------------------------------------------------------
}


#endif