/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeDebugFrustum.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEDEBUGFRUSTUM_H_
#define __INC_NODEDEBUGFRUSTUM_H_


#include "NodeVisible.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	namespace Debug
	{
		/** Debug object for rendering frustum objects.
		*/
		class NodeDebugFrustum: public NodeVisible
		{
			EXPOSE_TYPE
		protected:
			Frustum* m_pFrustum;			///< Pointer to frustum object
			VertexBuffer* m_pVB;			///< Pointer to vertex buffer object


			/// Updates geometry.
			void updateFrustumGeometry();
			void onUpdate();

		public:
			/** Constructor of the NodeDebugFrustum object.
				@param _frustum pointer to the existing frustum object.
			*/
			NodeDebugFrustum(Frustum* _frustum);
			~NodeDebugFrustum();

			void init();
		};
	}
}


#endif