/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MathWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_MATHWRAPPER_H_
#define __INC_MATHWRAPPER_H_


#include "nGENE.h"


namespace nGENEToolset
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Drawing;
	using namespace System::Text;
	using System::String;


	using nGENE::Maths;
	using nGENE::Node;
	using nGENE::NodeVisible;
	using nGENE::Surface;
	using nGENE::uint;


	/// Wrapper for a Vector2 class.
	[TypeConverterAttribute(ExpandableObjectConverter::typeid)]
	ref class Vector2
	{
	public:
		nGENE::Vector2* m_pVector;
		float _x;
		float _y;

	public:
		Vector2(nGENE::Vector2* vec)
		{
			m_pVector = vec;
			if(m_pVector)
			{
				_x = vec->x;
				_y = vec->y;
			}
		}

		/*Vector2(const Vector2& src)
		{
			x = src.x;
			y = src.y;
			m_pVector = src.m_pVector;
		}*/

		Vector2()
		{
			_x = 0.0f;
			_y = 0.0f;
			m_pVector = NULL;
		}
		
		[NotifyParentProperty(true)]
		property float x
		{
			float get()
			{
				return _x;
			}

			void set(float value)
			{
				_x = value;
				if(m_pVector)
					m_pVector->x = _x;
			}
		}

		[NotifyParentProperty(true)]
		property float y
		{
			float get()
			{
				return _y;
			}

			void set(float value)
			{
				_y = value;
				if(m_pVector)
					m_pVector->y = _y;
			}
		}

		virtual String^ ToString(void) override
		{
			return(L"" + x + L"; " + y);
		}

		nGENE::Vector2 getVector()
		{
			return (*m_pVector);
		}

		void setVector(nGENE::Vector2* _vec)
		{
			m_pVector = _vec;
			_x = _vec->x;
			_y = _vec->y;
		}
	};

	/// Wrapper for a Vector3 class.
	[TypeConverterAttribute(ExpandableObjectConverter::typeid)]
	ref struct Vector3
	{
	private:
		nGENE::Vector3* m_pVector;

		float _x, _y, _z;

	public:
		Vector3(nGENE::Vector3* vec)
		{
			m_pVector = vec;
			if(m_pVector)
			{
				_x = vec->x;
				_y = vec->y;
				_z = vec->z;
			}
		}
		
		[NotifyParentProperty(true)]
		property float x
		{
			float get()
			{
				return _x;
			}

			void set(float value)
			{
				_x = value;
				if(m_pVector)
					m_pVector->x = _x;
			}
		}

		[NotifyParentProperty(true)]
		property float y
		{
			float get()
			{
				return _y;
			}

			void set(float value)
			{
				_y = value;
				if(m_pVector)
					m_pVector->y = _y;
			}
		}

		[NotifyParentProperty(true)]
		property float z
		{
			float get()
			{
				return _z;
			}

			void set(float value)
			{
				_z = value;
				if(m_pVector)
					m_pVector->z = _z;
			}
		}

		virtual String^ ToString(void) override
		{
			return(L"" + x + L"; " + y + L"; " + z);
		}

		void setPointer(nGENE::Vector3* _vector)
		{
			m_pVector = _vector;
		}

		void setVector(nGENE::Vector3 _vec)
		{
			x = _vec.x;
			y = _vec.y;
			z = _vec.z;
		}

		nGENE::Vector3 getVector()
		{
			if(m_pVector)
				return (*m_pVector);
			else
			{
				return nGENE::Vector3(x, y, z);
			}
		}
	};

	/// Wrapper for a Quaternion class.
	[TypeConverterAttribute(ExpandableObjectConverter::typeid)]
	ref struct Quaternion
	{
	private:
		nGENE::Quaternion* m_pQuaternion;

		float _x, _y, _z, _w;

	public:
		Quaternion(nGENE::Quaternion* _quat)
		{
			_x = _quat->x;
			_y = _quat->y;
			_z = _quat->z;
			_w = _quat->w;

			m_pQuaternion = _quat;
		}

		[NotifyParentProperty(true)]
		property float x
		{
			float get()
			{
				return _x;
			}

			void set(float value)
			{
				_x = value;
				m_pQuaternion->x = _x;
			}
		}

		[NotifyParentProperty(true)]
		property float y
		{
			float get()
			{
				return _y;
			}

			void set(float value)
			{
				_y = value;
				m_pQuaternion->y = _y;
			}
		}

		[NotifyParentProperty(true)]
		property float z
		{
			float get()
			{
				return _z;
			}

			void set(float value)
			{
				_z = value;
				m_pQuaternion->z = _z;
			}
		}

		[NotifyParentProperty(true)]
		property float w
		{
			float get()
			{
				return _w;
			}

			void set(float value)
			{
				_w = value;
				m_pQuaternion->w = _w;
			}
		}

		virtual String^ ToString(void) override
		{
			return(L"" + x + L"; " + y + L"; " + z + L"; " + w);
		}

		void setPointer(nGENE::Quaternion* _quaternion)
		{
			m_pQuaternion = _quaternion;
		}

		nGENE::Quaternion getQuaternion()
		{
			return (*m_pQuaternion);
		}
	};
}


#endif