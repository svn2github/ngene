/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Serializable.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ISERIALIZABLE_H_
#define __INC_ISERIALIZABLE_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Pure interface for classes which should have the ability to serialize themselves.
		@remarks
			To serialize objects, all of derived classes should implement
			serialize() and deserialize() methods.
	*/
	class nGENEDLL ISerializable
	{
		EXPOSE_TYPE
	protected:
		bool m_bIsSerializable;					///< Specifies whether node will be serialized

	public:
		ISerializable();
		virtual ~ISerializable();

		/// Saves to object to file.
		virtual void serialize(ISerializer* _serializer,
							   bool _serializeType=true,
							   bool _serializeChildren=true)=0;
		/// Loads the object from file.
		virtual void deserialize(ISerializer* _serializer)=0;

		/** Specifies whether node should be serialized or not.
			@remarks
				In some rare cases you may want to avoid serializing particular
				nodes (eg. they are auxiliary). By passing false as this method
				parameter you can achieve this.
		*/
		void setSerializable(bool _value);

		/// Checks if the object is serializable.
		bool isSerializable() const;
	};



	inline void ISerializable::setSerializable(bool _value)
	{
		m_bIsSerializable = _value;
	}
//----------------------------------------------------------------------
	inline bool ISerializable::isSerializable() const
	{
		return m_bIsSerializable;
	}
//----------------------------------------------------------------------
}


#endif
