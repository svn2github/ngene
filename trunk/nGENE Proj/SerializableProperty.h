/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SerializableProperty.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SERIALIZABLEPROPERTY_H_
#define __INC_SERIALIZABLEPROPERTY_H_


#include "Prerequisities.h"
#include "TypeInfo.h"

#include <sstream>


namespace nGENE
{
	/// Serializable property abstract interface.
	class IProperty
	{
	public:
		virtual void fromString(const string& _str)=0;
		virtual string toString()=0;
	};

	/** Generic property class.
		@par
			Basic types (i.e. int, float etc.) are provided
			through template specialization.
	*/
	template <typename T>
	class Property: public IProperty
	{
	private:
		T& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(T& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value.fromString(_str);
		}

		string toString()
		{
			return m_Value.toString();
		}
	};


//----------------------------------------------------------------------
	// Floating types

	template <>
	class Property <float>: public IProperty
	{
	private:
		float& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(float& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = static_cast<float>(atof(_str.c_str()));
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};

	template <>
	class Property <double>: public IProperty
	{
	private:
		double& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(double& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = atof(_str.c_str());
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};

//----------------------------------------------------------------------
	// String types

	template <>
	class Property <string>: public IProperty
	{
	private:
		string& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(string& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = _str;
		}

		string toString()
		{
			return m_Value;
		}
	};

	template <>
	class Property <wstring>: public IProperty
	{
	private:
		wstring& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(wstring& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value.assign(_str.begin(), _str.end());
		}

		string toString()
		{
			return string(m_Value.begin(), m_Value.end());
		}
	};

//----------------------------------------------------------------------
	// Integer types

	template <>
	class Property <long>: public IProperty
	{
	private:
		long& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(long& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = static_cast<long>(atoi(_str.c_str()));
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};

	template <>
	class Property <dword>: public IProperty
	{
	private:
		dword& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(dword& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = static_cast<dword>(atoi(_str.c_str()));
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};

	template <>
	class Property <int>: public IProperty
	{
	private:
		int& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(int& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = static_cast<int>(atoi(_str.c_str()));
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};

	template <>
	class Property <uint>: public IProperty
	{
	private:
		uint& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(uint& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = static_cast<uint>(atoi(_str.c_str()));
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};

	template <>
	class Property <short>: public IProperty
	{
	private:
		short& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(short& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = static_cast<short>(atoi(_str.c_str()));
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};
	
	template <>
	class Property <ushort>: public IProperty
	{
	private:
		ushort& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(ushort& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = static_cast<ushort>(atoi(_str.c_str()));
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};

	template <>
	class Property <char>: public IProperty
	{
	private:
		char& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(char& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = static_cast<char>(atoi(_str.c_str()));
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};
	
	template <>
	class Property <byte>: public IProperty
	{
	private:
		byte& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(byte& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = static_cast<byte>(atoi(_str.c_str()));
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};

	template <>
	class Property <bool>: public IProperty
	{
	private:
		bool& m_Value;

		Property(const Property& src);
		Property& operator=(const Property& rhs);

	public:
		Property(bool& _value):
			m_Value(_value)
		{
		}

		void fromString(const string& _str)
		{
			m_Value = (_str == "1" ? true : false);
		}

		string toString()
		{
			stringstream buffer;
			buffer << m_Value;

			return buffer.str();
		}
	};
}


#endif