/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ICullable.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ICULLABLE_H_
#define __INC_ICULLABLE_H_


#include "Prerequisities.h"
#include "IPickable.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Pure interface for all classes which have ability
		of being culled eg. all renderable objects.
		@remarks
			ICullable objects have their bounding volumes which
			help in determining if object is to be culled or not.
	*/
	class nGENEDLL ICullable: public IPickable
	{
		EXPOSE_TYPE
	protected:
		AABB* m_pAABB;						///< Axis-aligned bounding box
		OBB* m_pOBB;						///< Oriented bounding box

		bool m_bCulled;						///< Is object culled?
		bool m_bHasChanged;					///< Has changed since last frame?

	public:
		ICullable();
		ICullable(const ICullable& src);
		virtual ~ICullable();

		/// Assignment operator.
		ICullable& operator=(const ICullable& rhs);

		/// Specifies if the object is currently culled or not.
		void setCulled(bool _value);
		/// Checks if the object is culled.
		bool isCulled() const;

		/// You can also manually set AABB and OBB.
		void setAABB(const AABB& _aabb);
		/// Sets object's bounding box.
		void setOBB(const OBB& _obb);

		/// Returns object's bounding box.
		OBB* getOBB() const;
		/// Returns object's AABB.
		AABB* getBoundingBox() const;

		void setChanged(bool _value);
		/** Checks whether node's state (eg. position, orientation)
			has changed since the last frame rendered.
		*/
		bool hasChanged() const;


		/** Calculates bounding boxes.
			@remarks
				This function calculates both initial Oriented Bounding Box
				and Axis Aligned Bounding Box for the node.
		*/
		virtual void calculateBoundingBox()=0;
		/** Updates bounding boxes.
			@remarks
				In many applications computing both bounding boxes from
				scratch is much less efficient than calculating AABB
				based on OBB, so apart from startup this function should
				be used.
		*/
		virtual void updateBoundingBox()=0;

		/// Checks if the ICullable is enabled and thus requires testing.
		virtual bool isEnabled() const=0;
	};



	inline void ICullable::setCulled(bool _value)
	{
		m_bCulled = _value;
	}
//----------------------------------------------------------------------
	inline bool ICullable::isCulled() const
	{
		return m_bCulled;
	}
//----------------------------------------------------------------------
	inline OBB* ICullable::getOBB() const
	{
		return m_pOBB;
	}
//----------------------------------------------------------------------
	inline AABB* ICullable::getBoundingBox() const
	{
		return m_pAABB;
	}
//----------------------------------------------------------------------
	inline void ICullable::setChanged(bool _value)
	{
		m_bHasChanged = _value;
	}
//----------------------------------------------------------------------
	inline bool ICullable::hasChanged() const
	{
		return m_bHasChanged;
	}
//----------------------------------------------------------------------
}


#endif