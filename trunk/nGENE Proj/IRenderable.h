/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IRenderable.h
Version:	0.81
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_IRENDERABLE_H_
#define __INC_IRENDERABLE_H_


namespace nGENE
{
	/** Pure interface for all classes which have ability
		of rendering themselves.
	*/
	class IRenderable
	{
	public:
		/// Renders the object.
		virtual void render()=0;
	};
}


#endif