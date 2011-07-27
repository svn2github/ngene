/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIListBox.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUILISTBOX_H_
#define __INC_GUILISTBOX_H_


#include "GUIControl.h"
#include "GUIScrollbar.h"
#include "Prerequisities.h"


namespace nGENE
{
	/// List box item
	typedef struct nGENEDLL SListBoxItem
	{
		wstring text;							///< Item text
		bool selected;							///< Is item currently selected?
	} LIST_BOX_ITEM;

	/** List box control.
	*/
	class nGENEDLL GUIListBox: public GUIControl
	{
	protected:
		/// Critical section for multiple threads support
		CriticalSection m_CriticalSection;

		vector <LIST_BOX_ITEM> m_vItems;		///< List box items
		vector <LIST_BOX_ITEM*> m_vSelectedItems;
		vector <uint> m_vSelectedIndices;

		GUIScrollbar m_VerticalScrollbar;		///< Vertical scrollbar

		bool m_bHasScrollbar;					///< Has list box vertical scrollbar?
		bool m_bMultiSelection;					///< Is multi-selection enabled?

		uint m_nColumnsNum;						///< Number of columns

		uint m_nFontSize;						///< Font size
		bool m_bBorder;							///< Has button border

		Colour m_TextColour;					///< Colour of the text

	public:
		ON_PAINT_DELEGATE onChange;
	
	public:
		GUIListBox();
		~GUIListBox();

		virtual void render(ScreenOverlay* _overlay, Vector2& _position);

		void setParent(GUIControl* _parent);

		void setHeight(Real _height);

		void setPosition(const Vector2& _position);
		void setPosition(Real _x, Real _y);

		/** Adds new item to the list.
			@param _text item to be added.
		*/
		void addItem(const wstring& _text);
		/** Removes given item from the list.
			@param _index index of item to remove.
		*/
		void removeItem(uint _index);
		/// Removes all elements from the list.
		void clear();

		/// Specifies whether scrollbars are visible.
		void setHasScrollbar(bool _value);
		/// Checks if the scrollbars are visible.
		bool hasScrollbar() const;

		/// Specifies whether user can simultaneously select multiple items.
		void setMultiSelection(bool _value);
		/// Checks whether user can simultaneously select multiple items.
		bool hasMultiSelection() const;

		void setVisible(bool _value);
		void toggleVisible();
		void show();
		void hide();

		/// Returns number of items.
		uint getItemsNum() const;

		/// Specifies whether border should be used.
		void setBorder(bool _value);
		/// Checks if border is displayed.
		bool getBorder() const;

		/// Sets text font size.
		virtual void setFontSize(uint _size);
		/// Returns text font size.
		virtual uint getFontSize() const;

		/// Sets text font colour.
		virtual void setTextColour(const Colour& _clr);
		/// Returns text font colour.
		virtual Colour& getTextColour();


		virtual void loosingFocus();

		vector <LIST_BOX_ITEM*>* getSelected();
		vector <uint>* getSelectedIndices();

		LIST_BOX_ITEM* getItem(uint _index);


		void selectItem(uint _index);
		void deselect();


		virtual void mouseClick(uint _x, uint _y);
		virtual void mouseDown(uint _x, uint _y);
		virtual void mouseRelease(uint _x, uint _y);
		virtual void mouseOver(uint _x, uint _y);

		virtual void keyboardDown(const KeyboardEvent& _evt);
	};



	inline uint GUIListBox::getItemsNum() const
	{
		return m_vItems.size();
	}
//----------------------------------------------------------------------
	inline vector <LIST_BOX_ITEM*>* GUIListBox::getSelected()
	{
		return &m_vSelectedItems;
	}
//----------------------------------------------------------------------
	inline vector <uint>* GUIListBox::getSelectedIndices()
	{
		return &m_vSelectedIndices;
	}
//----------------------------------------------------------------------
	inline void GUIListBox::mouseDown(uint _x, uint _y)
	{
		GUIControl::mouseDown(_x, _y);
	}
//----------------------------------------------------------------------
	inline void GUIListBox::mouseRelease(uint _x, uint _y)
	{
		GUIControl::mouseRelease(_x, _y);

		//m_bDragged = false;
	}
//----------------------------------------------------------------------
	inline void GUIListBox::mouseOver(uint _x, uint _y)
	{
		GUIControl::mouseOver(_x, _y);
	}
//----------------------------------------------------------------------
	inline void GUIListBox::loosingFocus()
	{
		GUIControl::loosingFocus();
	}
//----------------------------------------------------------------------
	inline bool GUIListBox::hasScrollbar() const
	{
		return m_bHasScrollbar;
	}
//----------------------------------------------------------------------
	inline void GUIListBox::setMultiSelection(bool _value)
	{
		m_bMultiSelection = _value;
	}
//----------------------------------------------------------------------
	inline bool GUIListBox::hasMultiSelection() const
	{
		return m_bMultiSelection;
	}
//----------------------------------------------------------------------
	inline void GUIListBox::setFontSize(uint _size)
	{
		m_nFontSize = _size;
	}
//----------------------------------------------------------------------
	inline uint GUIListBox::getFontSize() const
	{
		return m_nFontSize;
	}
//----------------------------------------------------------------------
	inline void GUIListBox::setTextColour(const Colour& _clr)
	{
		m_TextColour = _clr;
	}
//----------------------------------------------------------------------
	inline Colour& GUIListBox::getTextColour()
	{
		return m_TextColour;
	}
//----------------------------------------------------------------------
	inline void GUIListBox::setBorder(bool _value)
	{
		m_bBorder = _value;
	}
	//----------------------------------------------------------------------
	inline bool GUIListBox::getBorder() const
	{
		return m_bBorder;
	}
	//----------------------------------------------------------------------
}


#endif