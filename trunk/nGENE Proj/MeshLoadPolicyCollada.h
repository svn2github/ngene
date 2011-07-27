/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MeshLoadPolicyCollada.h
Version:	0.08

Author:     Thomas Enzenebner
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MESHLOADPOLICYCOLLADA_H_
#define __INC_MESHLOADPOLICYCOLLADA_H_


#include "Prerequisities.h"
#include "Surface.h"
#include "TypeInfo.h"
#include "Vertex.h"
#include "Matrix4x4.h"


namespace nGENE
{
	/// Enum specifying type of collada semantic
	enum nGENE_COLLADA_SEMANTIC_TYPE
	{
		COLLADA_SEMANTIC_POSITION = 0,
		COLLADA_SEMANTIC_VERTEX,		
		COLLADA_SEMANTIC_NORMAL,
		COLLADA_SEMANTIC_TEXCOORD,
		COLLADA_SEMANTIC_TEXTANGENT,
		COLLADA_SEMANTIC_TEXBINORMAL,
		COLLADA_SEMANTIC_INPUT,
		COLLADA_SEMANTIC_OUTPUT,
		COLLADA_SEMANTIC_INTERPOLATION,
		COLLADA_SEMANTIC_JOINT,
		COLLADA_SEMANTIC_INV_BIND_MATRIX,
		COLLADA_SEMANTIC_WEIGHT
	};

	/// Enum specifying direction of up vector
	enum COLLADA_UP_VECTOR
	{
		CUV_Y = 0,
		CUV_Z = 1
	};

	/** Mesh loader for the collada file format.
	*/
	class nGENEDLL MeshLoadPolicyCollada
	{
		EXPOSE_TYPE
	protected:
		struct SColladaSemantic
		{
			uint offset;
			uint set;
			string source;
			nGENE_COLLADA_SEMANTIC_TYPE type;
		};

		struct STriangleSet
		{
			vector <ushort> pIndices;						///< Model triangles (indices)
			vector <SColladaSemantic> semanticMapping;		///< Always stores the semantic for the give mesh part that is process
			vector <SVertex> pSurface;						///< Surfaces for the model
			vector <SVertexSkinning> pSurface2;				///< Skinning vertices for the model

			uint nVerticesCount;							///< Vertex count
			uint nIndicesCount;								///< Indices count

			string indicesArray;							///< String holding real data
		};

		struct SModel
		{
			vector <Surface> surfaces;						///< Surfaces building this model
			vector <STriangleSet> sets;						///< Sets of triangles
			
			string vertexSource;							///< Source of vertex data
		};

		struct SNumberArray 
		{
			string Id;
			vector <Real> Data;
		};

		struct SColladaParam
		{
			string Name;
			string Type;
		};

		struct SAccessor
		{
			uint Count;				
			uint Stride;

			vector<SColladaParam> Parameters;


			SAccessor():
				Count(0),
				Stride(1)
			{
			}
		};

		struct SSource
		{
			string Id;
			string Name;
			SNumberArray Array;
			SAccessor Accessors;
		};			

		struct SVertexKey
		{
		   uint nVertexIndex;
		   uint nNormalIndex;
		   uint nTextureIndex;	
		   uint nTextureTangent;
		   uint nTextureBinormal;

		   ushort index;
		};
		
		struct SAssetContributor
		{	
			string stAuthor;
			string stAuthoringTool;
			string stCopyright;
			string stComments;
			string stSourceData;
		};

		struct SAsset
		{
			vector <SAssetContributor> pContributor;
			string stCreated;
			string stModified;
			string stUnitMeter;
			string stUnitName;
			
			COLLADA_UP_VECTOR upVector;
		};

		struct SImages
		{
			string id;
			string name;
			string fileName;
		};

		struct SMaterials
		{
			string id;
			string name;
			string url;
		};

		XMLDocument* m_pDocument;			
		string VertexPositionSource;

		SAsset m_Asset;
		string stMeshName;
		string stMeshId;
		bool bDoubleSided;

		Matrix4x4 m_matTransform;
		bool m_bMoveToCentre;

		//////// Specific globals for Collada parsing //////////////
		HashTable <string, SSource> m_ColladaSource;		///< Stores a source node, key is the id
		HashTable <string, SModel> m_Models;				///< Models which will be used to create surfaces
		vector <SImages> ColladaImages;						///< Stores all the images found in node "library_images" tag
		vector <SMaterials> ColladaMaterials;				///< Stores every material

		////////////////////////////////////////////////////////////
		
		void loadAsset(XMLDocument* _node);
		void loadImages(XMLDocument* _node);
		void loadMaterials(XMLDocument* _node);
		void loadGeometry(XMLDocument* _node, vector <Surface>& _surfaces);
		void loadScenes(XMLDocument* _node, vector <Surface>& _surfaces);
		void loadScene(XMLDocument* _node, vector <Surface>& _surfaces);
		void loadNode(XMLDocument* _node, const string& _nodeName, vector <Surface>& _surfaces,
			const string& _type, const string& _parentName, const string& _parentType,
			const Matrix4x4* _parentTransform);

		virtual void processNodeType(const string& _type, SModel* _model, const Matrix4x4& _transform,
			vector <Surface>& _surfaces, const string& _nodeName, const string& _parentName,
			const string& _parentType, const Matrix4x4* _parentTransform);
		void processNodeTypeNode(SModel* _model, const Matrix4x4& _transform,
			vector <Surface>& _surfaces);
		void processMeshNode(XMLDocument* _node, vector <Surface>& _surfaces);			
		void processTriangles(XMLDocument* _node, STriangleSet* _model, Surface& _surface, string geometryType);
		void processSourceNode(XMLDocument* _node, string id, string name);
		void processPrimitives(XMLDocument* _node, vector <ushort> _indicesArray);

		virtual void createMesh(SModel* _model, STriangleSet* _set, Surface& _surface,
			const Matrix4x4& _transform);

		void readArray(XMLDocument* _node, SSource* _source);			
		int splitStr(vector <string>& L, const string& seq, const string& _1cdelim, bool keeptoken, bool _removews);


		~MeshLoadPolicyCollada() {}
	
	public:
		MeshLoadPolicyCollada();

		/** Loads contents of the file.
			@remarks
				Simply call it on host application to obtain
				all mesh data contained in the file.
		*/
		virtual vector <Surface> loadMesh(const wstring& _fileName, const Matrix4x4& _transform,
			bool _moveToCentre);
	};
}

#endif