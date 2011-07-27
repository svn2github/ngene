/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeSound.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODESOUND_H_
#define __INC_NODESOUND_H_


#include "Prerequisities.h"
#include "Node.h"


namespace nGENE
{
	/** Node for the sound objects.
	*/
	class nGENEDLL NodeSound: public Node
	{
		EXPOSE_TYPE
	protected:
		ISoundInstance* m_pSound;
		SOUND_DESC* m_pDesc;
		wstring* m_pName;

		/// Previous position of the sound
		Vector3 m_vecLastPos;

		/// Previous frame time
		dword m_nPrevTime;

		// Hide assignment operator and copy constructor to prevent
		// it from being used.
		NodeSound(const NodeSound& src);
		NodeSound& operator=(const NodeSound& rhs);

		void createSound(const wstring& _name, const SOUND_DESC& _desc);

	public:
		NodeSound();
		NodeSound(const wstring& _name, const SOUND_DESC& _desc);
		virtual ~NodeSound();

		void cleanup();

		void onUpdate();

		void translate(const Vector3& _translate);
		void translate(Real _x, Real _y, Real _z);

		void setPosition(const Vector3& _pos);
		void setPosition(Real _x, Real _y, Real _z);

		/// Returns ISoundInstance object associated with this node.
		ISoundInstance* getSound() const;

		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline ISoundInstance* NodeSound::getSound() const
	{
		return m_pSound;
	}
//----------------------------------------------------------------------
}


#endif
