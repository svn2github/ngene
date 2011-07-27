/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIImage.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUIIMAGE_H_
#define __INC_GUIIMAGE_H_


#include "GUIControl.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Image.
	*/
	class nGENEDLL GUIImage: public GUIControl
	{
	private:
		ITexture* m_pImage;					///< Pointer to the image

		ScreenOverlay* m_pOverlay;

	public:
		GUIImage();
		virtual ~GUIImage();

		virtual void render(ScreenOverlay* _overlay, Vector2& _position);


		void setImage(ITexture& _image);
		void setImage(ITexture* _image);
		void setImage(const wstring& _name, const wstring& _filename);
		ITexture* getImage() const;
	};



	inline void GUIImage::setImage(ITexture& _image)
	{
		m_pImage = &_image;
	}
//----------------------------------------------------------------------
	inline void GUIImage::setImage(ITexture* _image)
	{
		m_pImage = _image;
	}
//----------------------------------------------------------------------
	inline ITexture* GUIImage::getImage() const
	{
		return m_pImage;
	}
//----------------------------------------------------------------------
}


#endif