/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Vertex.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VERTEX_H_
#define __INC_VERTEX_H_


#include "Colour.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"


namespace nGENE
{
	/** The simplest vertex available in the engine having all
		necessary parameters.
	*/
	typedef struct SVertex
	{
		Vector3 vecPosition;				///< Position of the vertex in 3D space
		Vector3 vecNormal;					///< Vertex normal vector
		dword dwColour;						///< Colour of the vertex
		Vector2 vecTexCoord;				///< Vertex texture coordinates
	} VERTEX_SIMPLE;

	/** Simple vertex with only colour and position. Useful for drawing
		some debug info like AABBs for instance.
	*/
	typedef struct SVertexNonLightable
	{
		Vector3 vecPosition;				///< Position of the vertex in 3D space
		dword dwColour;						///< Colour of the vertex
	} VERTEX_NONLIGHTABLE;

	/** Vertex to be used for skinning animation.
	*/
	typedef struct SVertexSkinning
	{
		Vector3 position;					///< Position of the vertex in 3D space
		Vector3 normal;						///< Vertex normal vector
		dword colour;						///< Colour of the vertex
		Vector2 texCoord;					///< Vertex texture coordinates
		Vector4 boneWeights;				///< Weights for each of the bones
		dword boneIndices;					///< Indices of each of the bones
	} VERTEX_SKINNING;

	/// Type of vertex mainly useful for 2D rendering.
	typedef struct SVertex2D
	{
		Vector3 position;					///< Position of the vertex in 3D space
		dword colour;						///< Colour of the vertex
		Vector2 texCoord;					///< Vertex texture coordinates
	} VERTEX_2D;
}


#endif