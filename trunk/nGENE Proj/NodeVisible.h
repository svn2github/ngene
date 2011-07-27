/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeVisible.h
Version:	0.15
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEVISIBLE_H_
#define __INC_NODEVISIBLE_H_


#include "HashTable.h"
#include "IndexedBuffer.h"
#include "IRenderable.h"
#include "Node.h"
#include "Surface.h"
#include "TypeInfo.h"
#include "VertexBuffer.h"


namespace nGENE
{
	/** NodeVisible is type of Node which can be rendered.
		@remarks
			Each node visible consists of one or more surfaces.
			These surfaces can have Material applied to them.
		@par
			In order for node to be rendered it has to be enabled
			and have Material assigned to at least one of its
			surfaces.
	*/
	class nGENEDLL NodeVisible: public Node
	{
		EXPOSE_TYPE
	protected:
		/// HashTable containing all surfaces.
		HashTable <wstring, Surface> m_Surfaces;
		vector <Surface*> m_vSurfaces;

		bool m_bLightable;					///< Does light affect it?


		/// Updates the node.
		virtual void onUpdate();

	public:
		NodeVisible();
		NodeVisible(const NodeVisible& src);
		virtual ~NodeVisible();

		/// Assignment operator.
		NodeVisible& operator=(const NodeVisible& rhs);

		virtual void init();
		virtual void cleanup();

		/// Adds new surface to the node.
		virtual void addSurface(const wstring& _name, Surface& _surf);

		/** Returns surface with the given name.
			@returns
				Surface* if surface with the given name exists in the
				node, then pointer to it is returned. Otherwise function
				returns NULL.
		*/
		Surface* getSurface(const wstring& _name);
		/// Returns surface with the given index.
		Surface* getSurface(uint _index) const;
		/// Returns number of surfaces.
		uint getSurfacesNum() const;

		/// Checks if the given surface exists.
		bool findSurface(const wstring& _name);


		virtual void calculateBoundingBox();


		virtual void addToPartitioner(ScenePartitioner* _partitioner);
		virtual void removeFromPartitioner(ScenePartitioner* _partitioner);

		/** Sets whether object is pickable i.e. whether it could be
			picked via mouse clicking. */
		void setPickable(bool _value);


		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};



	inline uint NodeVisible::getSurfacesNum() const
	{
		return m_Surfaces.size();
	}
//----------------------------------------------------------------------
	inline Surface* NodeVisible::getSurface(uint _index) const
	{
		return m_vSurfaces[_index];
	}
//----------------------------------------------------------------------
}


#endif