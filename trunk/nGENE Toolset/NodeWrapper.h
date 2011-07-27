/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeWrapper.h
Version:	0.04
---------------------------------------------------------------------------
*/


#ifndef __INC_NODEWRAPPER_H_
#define __INC_NODEWRAPPER_H_


#include "nGENE.h"
#include "MathWrapper.h"


namespace nGENEToolset
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Drawing;
	using namespace System::Text;
	using System::String;


	using nGENE::Node;
	using nGENE::NodeVisible;
	using nGENE::Surface;
	using nGENE::uint;


	/// Wrapper for a TimeDate class.
	[TypeConverterAttribute(ExpandableObjectConverter::typeid)]
	ref struct TimeDate
	{
	private:
		nGENE::TimeDate* m_pTime;
		bool timePassed;

		uint _seconds;
		uint _minutes;
		uint _hours;

	public:
		TimeDate()
		{
			_seconds = 0;
			_minutes = 0;
			_hours = 0;

			m_pTime = new nGENE::TimeDate();
			timePassed = false;
		}

		TimeDate(nGENE::TimeDate* _time)
		{
			_seconds = _time->seconds;
			_minutes = _time->minutes;
			_hours = _time->hours;

			m_pTime = _time;
			timePassed = true;
		}

		~TimeDate()
		{
			if(!timePassed)
				NGENE_DELETE(m_pTime);
		}
		
		property float seconds
		{
			float get()
			{
				return _seconds;
			}

			void set(float value)
			{
				_seconds = value;
				m_pTime->seconds = _seconds;
			}
		}
		property float minutes
		{
			float get()
			{
				return _minutes;
			}

			void set(float value)
			{
				_minutes = value;
				m_pTime->minutes = _minutes;
			}
		}

		property float hours
		{
			float get()
			{
				return _hours;
			}

			void set(float value)
			{
				_hours = value;
				m_pTime->hours = _hours;
			}
		}
		virtual String^ ToString(void) override
		{
			return(L"" + hours + L":" + minutes + L":" + seconds);
		}
	};

	/// A single scene node.
	public ref class SceneNode: public System::Object
	{
	protected:
		StringBuilder^ m_Name;
		StringBuilder^ m_Key;

		static int m_nKey;

		int m_ImageIndex;

	public:
		SceneNode()
		{
			m_ImageIndex = 0;

			Object^ o = m_nKey++;
			Key = String::Concat(o);
		}

		[Browsable(true), CategoryAttribute("Node"),
		DescriptionAttribute("Name of a node")]
		property String^ Name
		{
			String^ get()
			{
				return m_Name->ToString();
			}

			void set(String^ value)
			{
				if(m_Name != nullptr)
					delete m_Name;

				m_Name = gcnew StringBuilder(value);
			}
		}

		[Browsable(false), CategoryAttribute("Node"),
		DescriptionAttribute("Key of a node")]
		property String^ Key
		{
			String^ get()
			{
				return m_Key->ToString();
			}

			void set(String^ value)
			{
				if(m_Key != nullptr)
					delete m_Key;

				m_Key = gcnew StringBuilder(value);
			}
		}

		[Browsable(false), CategoryAttribute("Node"),
		DescriptionAttribute("Image index of a node")]
		property int ImageIndex
		{
			int get()
			{
				return m_ImageIndex;
			}

			void set(int value)
			{
				m_ImageIndex = value;
			}
		}
	};

	/// Wrapper for a Surface class
	ref class SurfaceWrapper: public SceneNode
	{
	protected:
		Surface* m_pSurface;
		
		bool m_bFlippedWinding;
		bool m_bFlippedNormals;

	public:
		SurfaceWrapper(Surface* _surf, String^ _name);

		virtual ~SurfaceWrapper()
		{
			NGENE_DELETE(m_pSurface);
		}

		[Browsable(true), CategoryAttribute("Surface"),
		DescriptionAttribute("Specifies if the surfaces can be 'picked' i.e. selected with mouse")]
		property bool Pickable
		{
			bool get()
			{
				return m_pSurface->isPickable();
			}

			void set(bool value)
			{
				m_pSurface->setPickable(value);
			}
		}

		[Browsable(true), CategoryAttribute("Surface"),
		DescriptionAttribute("Specifies whether normals should be flipped")]
		property bool FlipNormals
		{
			bool get()
			{
				return m_pSurface->hasFlippedNormals();
			}

			void set(bool value)
			{
				m_bFlippedNormals = value;
				m_pSurface->flipNormals();
			}
		}

		[Browsable(true), CategoryAttribute("Surface"),
		DescriptionAttribute("Specifies whether winding order should be flipped")]
		property bool FlipWindingOrder
		{
			bool get()
			{
				return m_pSurface->hasFlippedWindingOrder();
			}

			void set(bool value)
			{
				m_bFlippedWinding = value;
				m_pSurface->flipWindingOrder();
			}
		}

		[Browsable(true), CategoryAttribute("Surface"),
		DescriptionAttribute("Specifies whether object is active or not. Only active ones are updated and rendered")]
		property bool Enabled
		{
			bool get()
			{
				return m_pSurface->isEnabled();
			}

			void set(bool value)
			{
				m_pSurface->setEnabled(value);
			}
		}

	public: Surface* getSurface()
			{
				return m_pSurface;
			}
	};

	/// Wrapper for a Node class.
	ref class NodeWrapper: public SceneNode
	{
	protected:
		Node* m_pNode;

		nGENEToolset::Vector3^ m_Position;
		nGENEToolset::Vector3^ m_PositionWorld;
		nGENEToolset::Vector3^ m_Dimensions;
		nGENEToolset::Quaternion^ m_Rotation;

	public:
		NodeWrapper()
		{
			m_Name = gcnew StringBuilder();
			m_PositionWorld = gcnew Vector3(NULL);
		}

		NodeWrapper(Node* _node, String^ _name, int _imageIndex);

		virtual ~NodeWrapper()
		{
			NGENE_DELETE(m_pNode);
		}

		[Browsable(true), CategoryAttribute("Node"),
		DescriptionAttribute("Position of a node in relation to its parent")]
		property nGENEToolset::Vector3^ Position
		{
			nGENEToolset::Vector3^ get()
			{
				m_pNode->setPosition(m_Position->x,
									 m_Position->y,
									 m_Position->z);

				return m_Position;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Position->x = value->x;
				m_Position->y = value->y;
				m_Position->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Node"),
		DescriptionAttribute("Specifies whether object is active or not. Only active ones are updated and rendered")]
		property bool Enabled
		{
			bool get()
			{
				return m_pNode->isEnabled();
			}

			void set(bool value)
			{
				m_pNode->setEnabled(value);
			}
		}

		[Browsable(true), CategoryAttribute("Node"), DescriptionAttribute("Scale of the node")]
		property nGENEToolset::Vector3^ Scale
		{
			nGENEToolset::Vector3^ get()
			{
				m_pNode->setScale(m_Dimensions->x,
								  m_Dimensions->y,
								  m_Dimensions->z);

				return m_Dimensions;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Dimensions->x = value->x;
				m_Dimensions->y = value->y;
				m_Dimensions->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Node"),
		DescriptionAttribute("Rotation of the node presented as 3 angles in degress")]
		property nGENEToolset::Quaternion^ Rotation
		{
			nGENEToolset::Quaternion^ get()
			{
				m_pNode->setRotation(m_Rotation->getQuaternion());

				return m_Rotation;
			}

			void set(nGENEToolset::Quaternion^ value)
			{
				m_Rotation->x = value->x;
				m_Rotation->y = value->y;
				m_Rotation->z = value->z;
				m_Rotation->w = value->w;
			}
		}

		Node* getnGENENode()
		{
			return m_pNode;
		}
	};

	ref class NodeVisibleWrapper: public NodeWrapper
	{
	public:
		NodeVisibleWrapper():
			NodeWrapper()
		{
			ImageIndex = 0;
		}

		NodeVisibleWrapper(NodeVisible* _node, String^ _name, int _imageIndex):
			NodeWrapper(_node, _name, _imageIndex)
		{
		}
	};
}


#endif