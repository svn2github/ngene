/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Billboard.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEBILLBOARDSET_H_
#define __INC_NODEBILLBOARDSET_H_


#include "NodeVisible.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Camera oriented quad.
	*/
	class nGENEDLL Billboard
	{
		EXPOSE_TYPE
	public:
		/// Size of the billboard
		Real m_fWidth;
		Real m_fHeight;

		Vector3 m_vecPosition;				///< Position of the billboard
		Vector3 m_vecNormal;				///< Normal vector of the billboard

	public:
		Billboard();
		~Billboard();
	};



	/** A set of billboards.
		@remarks
			It is much more efficient to render multiple billboards at once,
			than one by one. It is also more common to have hundreds or thousands
			billboards at the same time (for example meadow or flowers).
		@par
			Note that all billboards share the same material.
	*/
	class nGENEDLL NodeBillboardSet: public NodeVisible
	{
		EXPOSE_TYPE
	private:
		vector <Billboard> m_vBillboards;

	public:
		NodeBillboardSet();
		~NodeBillboardSet();

		void init();
		void cleanup();

		/** Adds billboard to the set.
			@remarks
				When you finish adding billboards you have to call
				validate() method to make changes take place.
		*/
		void addBillboard(Billboard _billboard);

		/** Updates billboard set.
			@remarks
				As this method is rather time consuming try to use it as
				rarely as possible.
		*/
		void validate();
	};
}


#endif