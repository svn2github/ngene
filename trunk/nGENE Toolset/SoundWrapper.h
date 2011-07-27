/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SoundWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_SOUNDWRAPPER_H_
#define __INC_SOUNDWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	
	/// Wrapper for a PrefabSound class.
	ref class SoundWrapper: public NodeWrapper
	{
	protected:
		NodeSound* m_pSound;
		static int s_nSoundCount;

	public:
		SoundWrapper(String^ fileName)
		{
			wostringstream buffer;
			buffer << "Sound";
			buffer << s_nSoundCount;
			wstring text = buffer.str();

			pin_ptr <const wchar_t> str1 = PtrToStringChars(fileName);
			wstring name(str1);

			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			SOUND_DESC desc;
			desc.isUsingHardware = false;
			desc.is3D = true;
			desc.fileName = name;
			m_pSound = sm->createSound(text, desc);
			m_pSound->setPosition(0.0f, 0.0f, 0.0f);

			m_pNode = m_pSound;

			m_Position = gcnew Vector3(&m_pSound->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pSound->getScale());

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());
			
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pSound);

			this->Name = "Sound" + (s_nSoundCount);
			this->Key = "Sound" + (s_nSoundCount++);


			/// @todo Use singleton instead.
			frmSceneGraph::graph->addNode(this);


			m_pSound->getSound()->getSound()->setLooped(true);
			m_pSound->getSound()->play();
		}

		SoundWrapper(NodeSound* _Sound, String^ _name):
			NodeWrapper(_Sound, _name, 0)
		{
			m_pSound = _Sound;
			s_nSoundCount++;
		}

		virtual ~SoundWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Sound"),
		DescriptionAttribute("Frequency of the sound")]
		property Real Frequency
		{
			Real get()
			{
				return m_pSound->getSound()->getFrequency();
			}

			void set(Real value)
			{
				m_pSound->getSound()->setFrequency(value);
			}
		}

		[Browsable(true), CategoryAttribute("Sound"),
		DescriptionAttribute("Volume of the sound")]
		property Real Volume
		{
			Real get()
			{
				return m_pSound->getSound()->getVolume();
			}

			void set(Real value)
			{
				m_pSound->getSound()->setVolume(value);
			}
		}

		[Browsable(true), CategoryAttribute("Sound"),
		DescriptionAttribute("Pan of the sound")]
		property Real Pan
		{
			Real get()
			{
				return m_pSound->getSound()->getPan();
			}

			void set(Real value)
			{
				m_pSound->getSound()->setPan(value);
			}
		}
	};
}


#endif