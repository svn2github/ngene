/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeDebugPath.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEDEBUGPATH_H_
#define __INC_NODEDEBUGPATH_H_


#include "NodeVisible.h"


namespace nGENE
{
	/** Node to render path of position controller.
	*/
	template <typename Interpolator>
	class NodeDebugPath: public NodeVisible
	{
	protected:
		PositionController <Interpolator>* m_pController;
		Interpolator m_Interpolator;
		uint m_nSegments;

		const dword m_kColour;

	public:
		/** NodeDebugPath constructor.
			@param
				_controller a position controller for which
				path will be drawn.
			@param
				_segments number of segments a path will be built-of.
				The larger this number is, the smoother the path can be (if
				interpolation will allow it).
			@param
				_colour colour of the path.
		*/
		NodeDebugPath(PositionController <Interpolator>* _controller, uint _segments, const Colour& _colour);
		~NodeDebugPath();

		void init();
	};



	template <typename Interpolator>
	NodeDebugPath <Interpolator>::NodeDebugPath(PositionController <Interpolator>* _controller, uint _segments, const Colour& _colour):
		m_pController(_controller),
		m_nSegments(_segments),
		m_kColour(_colour.getDwordARGB())
	{
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	NodeDebugPath <Interpolator>::~NodeDebugPath()
	{
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void NodeDebugPath <Interpolator>::init()
	{
		// Create curve
		dword dwNumVertices = m_nSegments + 1;
		SVertexNonLightable* pVertices = new SVertexNonLightable[dwNumVertices];
		memset(pVertices, 0, dwNumVertices * sizeof(SVertexNonLightable));

		AnimationCurve& Xs = m_pController->getCurve(0);
		AnimationCurve& Ys = m_pController->getCurve(1);
		AnimationCurve& Zs = m_pController->getCurve(2);
		for(uint i = 0; i < dwNumVertices; ++i)
		{
			float t = (float)i / (float)(m_nSegments + 1);
			pVertices[i].vecPosition = Vector3(m_Interpolator.interpolate(t, Xs),
				m_Interpolator.interpolate(t, Ys),
				m_Interpolator.interpolate(t, Zs));

			pVertices[i].dwColour = m_kColour;
		}

		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = pVertices;
		vbDesc.verticesNum = dwNumVertices;
		vbDesc.primitivesNum = m_nSegments;
		vbDesc.primitivesType = PT_LINESTRIP;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"NonLightable");
		vbDesc.vertexSize = sizeof(SVertexNonLightable);
		VertexBuffer* vb = manager.createVertexBuffer(vbDesc);

		Surface temp;
		temp.setVertexBuffer(vb);
		addSurface(L"Base", temp);

		NGENE_DELETE_ARRAY(pVertices);


		// Create points
		dwNumVertices = m_pController->getCurve(0).size();
		pVertices = new SVertexNonLightable[dwNumVertices];
		memset(pVertices, 0, dwNumVertices * sizeof(SVertexNonLightable));

		for(uint i = 0; i < dwNumVertices; ++i)
		{
			pVertices[i].vecPosition = Vector3(Xs[i],
				Ys[i],
				Zs[i]);

			pVertices[i].dwColour = 0xffffff00;
		}

		vbDesc.vertices = pVertices;
		vbDesc.verticesNum = dwNumVertices;
		vbDesc.primitivesNum = dwNumVertices;
		vbDesc.primitivesType = PT_POINT;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"NonLightable");
		vbDesc.vertexSize = sizeof(SVertexNonLightable);
		vb = manager.createVertexBuffer(vbDesc);

		Surface points;
		points.setVertexBuffer(vb);
		addSurface(L"Points", points);

		NGENE_DELETE_ARRAY(pVertices);


		calculateBoundingBox();
	}
//----------------------------------------------------------------------
}


#endif