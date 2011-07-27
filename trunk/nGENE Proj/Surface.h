/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Surface.h
Version:	0.14
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SURFACE_H_
#define __INC_SURFACE_H_


#include "FastDelegate.h"
#include "ICullable.h"
#include "IRenderable.h"
#include "Prerequisities.h"
#include "TypeInfo.h"
#include "ISerializable.h"
#include "ISerializer.h"


namespace nGENE
{
	/** Surface - graphical representation of NodeVisible.
		@par
			Surface is a vertex data part of a NodeVisible object, which
			shares one material.
		@par
			You can set custom routine to change AABB. It is especially
			useful for dynamic surfaces. To do this simply bind onUpdateAABBB()
			delegate.
		@par
			It is also possible to modify rendering of the surface by binding
			onBeginRender() and onEndRender() delegates. The first is called
			before surface rendering is done and the latter just after it
			finishes.
	*/
	class nGENEDLL Surface: public ICullable, public IRenderable,
		public ISerializable
	{
		EXPOSE_TYPE
	private:
		typedef pair<uint, VertexBuffer*> VERTEX_STREAM;

		wstring m_stName;						///< Surface's name
		Material* m_pMaterial;					///< Surface's material
		NodeVisible* m_pNode;					///< Node it belongs to

		bool m_bEnabled;						///< Is surface enabled?
		bool m_bLightable;						///< Does light affect it?
		bool m_bFlippedNormals;					///< Are normals flipped?
		bool m_bFlippedOrder;					///< Is wind-order flipped?

		Vector2 m_vecTexCoordMult;				///< Texture coordinates multiplier.

		/// Pointer to the vertex data
		vector <VERTEX_STREAM> m_pVBs;


		void destroyVB(uint _index);

	public:
		// Here goes delegates
		typedef fastdelegate::FastDelegate0 <> ON_FRAME_EVENT;
		ON_FRAME_EVENT onUpdateAABB;			///< Action when AABB is updated
		ON_FRAME_EVENT onBeginRender;			///< Action when rendering is beginning
		ON_FRAME_EVENT onEndRender;				///< Action when rendering is finishing

	public:
		Surface();
		Surface(const Surface& src);
		explicit Surface(const wstring& _name);
		virtual ~Surface();

		/// Assignment operator.
		Surface& operator=(const Surface& rhs);

		/// Renders the surface with its underlying material.
		void render();

		/// Sets vertex buffer.
		void setVertexBuffer(VertexBuffer* _vb, uint _index=0);
		/// Returns pointer to the surface vertex buffer.
		VertexBuffer* getVertexBuffer(uint _index=0) const;

		/// Sets indices buffer.
		void setIndexedBuffer(IndexedBuffer* _ivb, uint _index=0);
		/// Returns indices buffer.
		IndexedBuffer* getIndexedBuffer(uint _index=0) const;

		/** Lets you set Surface's material.
			@remarks
				Surface can have zero or one material at time. Surface with
				number of materials equal to 0 will not be renderer.
		*/
		void setMaterial(Material* _material);
		/// Returns pointer to the surface material.
		Material* getMaterialPtr() const;
		/// Returns reference to the surface material.
		Material& getMaterial();

		/// Sets surface name.
		void setName(const wstring& _name);
		/// Returns surface name.
		wstring& getName();

		/// Sets pointer to the NodeVisible object.
		void setNode(NodeVisible* _node);
		/// Returns pointer to the NodeVisible object this surface belongs to.
		NodeVisible* getNode() const;

		/** Sets whether this surface is enabled.
			@param
				_value if true, surface is enabled. Only enabled surfaces
				could be rendered.
		*/
		void setEnabled(bool _value);
		/** Checks whether Surface is enabled.
			@remarks
				Only enabled Surfaces are rendered.
			@returns
				bool value specifying if surface is enabled. true if enabled
				and false if disabled.
		*/
		bool isEnabled() const;

		/// Does light affect it.
		bool isLightable() const;


		/// Prepares surface for rendering.
		void prepare();

		void calculateBoundingBox();
		void updateBoundingBox();

		/** Flips all surface normals.
			@remarks
				This method is very slow and therefore it should
				be called as rarely as possible.
		*/
		void flipNormals();
		/** Checks if surface's normal vectors have been flipped.
			@returns
				bool true if normals have been flipped and false
				otherwise.
		*/
		bool hasFlippedNormals() const;

		/** Flips vertices winding order (from clockwise to counter-clockwise
			and vice versa).
			@remarks
				This method is very slow and therefore it should
				be called as rarely as possible.
		*/
		void flipWindingOrder();
		/** Checks if surface's winding order has been flipped.
			@returns
				bool true if winding order has been flipped and false
				otherwise.
		*/
		bool hasFlippedWindingOrder() const;

		bool tryToPick(const Ray& _ray);


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline VertexBuffer* Surface::getVertexBuffer(uint _index) const
	{
		Assert(_index < m_pVBs.size(), "Index out of bound");

		return m_pVBs[_index].second;
	}
//----------------------------------------------------------------------
	inline Material* Surface::getMaterialPtr() const
	{
		return m_pMaterial;
	}
//----------------------------------------------------------------------
	inline Material& Surface::getMaterial()
	{
		return *m_pMaterial;
	}
//----------------------------------------------------------------------
	inline void Surface::setNode(NodeVisible* _node)
	{
		m_pNode = _node;
	}
//----------------------------------------------------------------------
	inline void Surface::setName(const wstring& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
	inline wstring& Surface::getName()
	{
		return m_stName;
	}
//----------------------------------------------------------------------
	inline NodeVisible* Surface::getNode() const
	{
		return m_pNode;
	}
//----------------------------------------------------------------------
	inline void Surface::setEnabled(bool _value)
	{
		m_bEnabled = _value;
	}
//----------------------------------------------------------------------
	inline bool Surface::isEnabled() const
	{
		return m_bEnabled;
	}
//----------------------------------------------------------------------
	inline bool Surface::isLightable() const
	{
		return m_bLightable;
	}
//----------------------------------------------------------------------
	inline bool Surface::hasFlippedNormals() const
	{
		return m_bFlippedNormals;
	}
//----------------------------------------------------------------------
	inline bool Surface::hasFlippedWindingOrder() const
	{
		return m_bFlippedOrder;
	}
//----------------------------------------------------------------------
}


#endif