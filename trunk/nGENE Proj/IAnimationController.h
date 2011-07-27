/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IAnimationController.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_IANIMATIONCONTROLLER_H_
#define __INC_IANIMATIONCONTROLLER_H_


#include "Prerequisities.h"
#include "ISerializable.h"
#include "NodeVisible.h"
#include "Task.h"
#include "Timer.h"
#include "Vector3.h"


namespace nGENE
{
	/** Abstract animation controller interface.
		@par
			In nGENE Tech all animations are performed in their own
			tasks.
		@par
			To control animation several methods are available,
			namely startAnimation(), stopAnimation() and pauseSimulation().
	*/
	class nGENEDLL IAnimationController: public Task, public ISerializable
	{
	protected:
		bool m_bAnimating;				///< Is controller currently animating?
		bool m_bStopped;				///< Is controller stopped?
		bool m_bFinished;				///< Is controller finished animating?

		dword m_nPrevTime;				///< Time of the previous update

		Node* m_pNode;					///< Pointer to the Node object

	public:
		IAnimationController();
		virtual ~IAnimationController();

		/// Initializes this controller.
		virtual void init();

		/// Attaches node with this controller.
		virtual void setNode(Node* _node);

		/// Starts animation of the controller.
		virtual void startAnimation();

		/// Stops animation of the controller.
		virtual void stopAnimation();

		/// Checks if the animation is running.
		bool isAnimating();
		/// Checks if the animation is finished.
		bool isFinished() const;

		/// Pauses animation of the controller.
		virtual void pauseAnimation();
		/// Unpauses animation of the controller.
		virtual void unpauseAnimation();

		/// Toggle pause animation
		virtual void togglePauseAnimation();

		/// Main animation function.
		virtual void update()=0;

		/** Serializes the controller.
			@todo provide implementation of this method for all subsequent classes.
		*/
		virtual void serialize(ISerializer* _serializer,
							   bool _serializeType=true,
							   bool _serializeChildren=true);

		/// @todo implement this
		virtual void deserialize(ISerializer* _serializer) {}

		/** Updates the node.
			@remarks
				This method should be called by client class.
		*/
		virtual void retrieveData()=0;
	};



	inline bool IAnimationController::isFinished() const
	{
		return m_bFinished;
	}
//----------------------------------------------------------------------
}


#endif