/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RenderStage.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RENDERSTAGE_H_
#define __INC_RENDERSTAGE_H_


#include "Prerequisities.h"
#include "Thread.h"


namespace nGENE
{
	/** Stage of rendering chain.
	*/
	class nGENEDLL RenderStage
	{
	protected:
		Renderer* m_pRenderer;				///< Pointer to renderer object
		bool m_bEnabled;					///< Is render stage enabled?
		bool m_bIsEmbeddable;				///< Can be embedded in another render stage?

		/// Vector of pointers to embedded stages
		vector <RenderStage*> m_vEmbeddedStages;

		CriticalSection m_CriticalSection;

	private:
		uint m_nPriority;					///< Priority of the stage

	public:
		RenderStage();
		virtual ~RenderStage();

		virtual void beginRender();
		/// Perform pre-rendering setup.
		virtual void onBeginFrame();
		/// Renders stage content.
		virtual void render()=0;
		virtual void endRender();

		virtual void init();
		virtual void cleanup();

		void setEnabled(bool _value);
		void toggleEnabled();
		bool isEnabled() const;

		/// Sets renderer.
		void setRenderer(Renderer* _renderer);

		// Returns priority of the render stage.
		uint getPriority() const;
		/** Set priority.
			@param
				_priority priority value for this stage. The higher it is, the
				higher the priority is (Windows-way).
		*/
		void setPriority(uint _priority);

		/// Checks whether this render stage is embeddable.
		bool isEmbeddable() const;
		/// Sets whether this render stage is embeddable.
		void setEmbeddable(bool _value);

		/// Adds given render stage as embedded one.
		void addRenderStage(RenderStage* _stage);
		/// Returns number of embedded render stages.
		uint getRenderStagesNum() const;
		/// Removes embedded render stage with the given index.
		void removeRenderStage(uint _index);


		friend bool operator<(const RenderStage& lhs, const RenderStage& rhs);
	};



	inline void RenderStage::setEnabled(bool _value)
	{
		m_bEnabled = _value;
	}
//----------------------------------------------------------------------
	inline void RenderStage::toggleEnabled()
	{
		m_bEnabled = m_bEnabled ? false : true;
	}
//----------------------------------------------------------------------
	inline bool RenderStage::isEnabled() const
	{
		return m_bEnabled;
	}
//----------------------------------------------------------------------
	inline void RenderStage::setRenderer(Renderer* _renderer)
	{
		m_pRenderer = _renderer;
	}
//----------------------------------------------------------------------
	inline uint RenderStage::getPriority() const
	{
		return m_nPriority;
	}
//----------------------------------------------------------------------
	inline bool operator<(const RenderStage& lhs, const RenderStage& rhs)
	{
		return (lhs.m_nPriority < rhs.m_nPriority);
	}
//----------------------------------------------------------------------
	inline void RenderStage::setEmbeddable(bool _value)
	{
		m_bIsEmbeddable = _value;
	}
//----------------------------------------------------------------------
	inline bool RenderStage::isEmbeddable() const
	{
		return m_bIsEmbeddable;
	}
//----------------------------------------------------------------------
}


#endif