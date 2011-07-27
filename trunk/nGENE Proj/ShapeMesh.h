/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeMesh.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHAPEMESH_H_
#define __INC_SHAPEMESH_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "ShapeFactory.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Mesh shape.
		@remarks
			Mesh shape provides precise collision detection
			handling to complex arbitrary 3D shapes.
		@par
			The biggest difference from other shapes is that
			in order to create valid ShapeMesh you have to
			provide geometrical data as a vertex buffer.
	*/
	class nGENEDLL ShapeMesh: public Shape
	{
		EXPOSE_TYPE
	private:
		NxTriangleMeshDesc* m_pMeshGeometry;

		/// PhysX triangle mesh descriptor
		NxTriangleMeshShapeDesc* m_pMeshDesc;

	public:
		ShapeMesh();
		~ShapeMesh();

		void init();
		void cleanup();

		void setTrigger(bool _value);

		/** Sets geometrical data of the mesh.
			@remarks
				Note that vertex buffer has to have indexed buffer
				assigned. Otherwise creation of PhysX triangle mesh
				will not be possible.
		*/
		void setGeometryData(const VertexBuffer& _vb, NxPhysicsSDK* _physx, bool _flipNormals);

		void setLocalPos(const Vector3& _pos);

		void setDimensions(const Vector3& _dims);
		NxShapeDesc* getShapeDesc();
	};

	/// Factory to be used for creating mesh shapes.
	class nGENEDLL MeshShapeFactory: public ShapeFactory
	{
	public:
		MeshShapeFactory();
		virtual ~MeshShapeFactory();

		Shape* createShape();
	};
}


#endif