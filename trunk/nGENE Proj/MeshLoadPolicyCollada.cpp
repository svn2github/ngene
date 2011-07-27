/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MeshLoadPolicyCollada.cpp
Version:	0.08

Author:		Thomas Enzenebner
Revisor:	Wojciech Toman
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "MeshLoadPolicyCollada.h"
#include "AABB.h"
#include "FileNarrow.h"
#include "FuncSplit.h"
#include "Point.h"
#include "Quaternion.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "VertexBufferManager.h"
#include "XMLDocument.h"
#include "XMLNode.h"
#include "XMLParser.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo MeshLoadPolicyCollada::Type(L"MeshLoadPolicyCollada", NULL);


	vector <Surface> MeshLoadPolicyCollada::loadMesh(const wstring& _fileName,
		const Matrix4x4& _transform, bool _moveToCentre)
	{
		m_matTransform = _transform;
		m_bMoveToCentre = _moveToCentre;

		const string assetSectionName = "asset";

		vector <Surface> vPolicySurfaces;

		// Open file
		File* m_pFile = static_cast<File*>(FileManager::getSingleton().openFile(_fileName, OPEN_READ));
		wstring temp = m_pFile->getBuffer();			
		FileManager::getSingleton().closeFile(_fileName);


		XMLDocument* pDocument = XMLParser::getSingleton().parse(temp, false);
		XMLDocument* pChild = pDocument->getFirstChild();
		XMLDocument* m_pCurrentNode = NULL;

		while(pChild)
		{
			string nodeName = pChild->getTagName();

			if(nodeName == "asset")
			{
				loadAsset(pChild->getFirstChild());
			}
			else if(nodeName == "library_geometries")
			{
				loadGeometry(pChild->getFirstChild(), vPolicySurfaces);
			}
			else if(nodeName == "library_images")
			{
				loadImages(pChild->getFirstChild());
			}
			else if(nodeName == "library_materials")
			{
				loadMaterials(pChild->getFirstChild());
			}
			else if(nodeName == "library_visual_scenes")
			{
				loadScenes(pChild->getFirstChild(), vPolicySurfaces);
			}
			
			m_pCurrentNode = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(m_pCurrentNode);
		}

		NGENE_DELETE(pDocument);
		
		return vPolicySurfaces;
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::loadScene(XMLDocument* _node, vector <Surface>& _surfaces)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = _node;

		while(pNode) 
		{
			string tagName = pNode->getTagName();

			if(tagName == "node")
			{
				string stNodeId = pNode->getAttribute("id");
				string stNodeName = pNode->getAttribute("name");
				string stType = pNode->getAttribute("type");

				if(stNodeName == "" && stNodeId != "")
					stNodeName = stNodeId;

				loadNode(pNode->getFirstChild(), stNodeId, _surfaces, stType, "", "", NULL);
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::loadNode(XMLDocument* _node, const string& _nodeName,
		vector <Surface>& _surfaces, const string& _type, const string& _parentName,
		const string& _parentType, const Matrix4x4* _parentTransform)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = _node;

		Matrix4x4 matTrans;
		Matrix4x4 matRot;
		Matrix4x4 matScale;

		Quaternion quatX, quatY, quatZ;

		bool bBakedMatrices = false;

		Vector3 vecEulerAngles;

		SModel* pModel = NULL;

		while(pNode) 
		{
			string tagName = pNode->getTagName();

			// <translate> tells translation of the node
			if(tagName == "translate")
			{
				string trans = pNode->getValue();
				vector <string> splitArray;
				splitStr(splitArray, trans, " \n\r\t", true, true);

				Vector3 vecTrans;
				if(m_Asset.upVector == CUV_Y)				
				{
					vecTrans.set(atof(splitArray[0].c_str()),
								 atof(splitArray[1].c_str()),
								 atof(splitArray[2].c_str()));
				}
				else
				{
					vecTrans.set(atof(splitArray[0].c_str()),
								 atof(splitArray[2].c_str()),
								 atof(splitArray[1].c_str()));
				}
					
				matTrans.translate(vecTrans);
			}
			// <rotate> tells the rotation about x, y and z axes
			else if(tagName == "rotate")
			{
				// X axis
				string sid = pNode->getAttribute("sid");
				if(sid.find("X") != string::npos)
				{
					string rot = pNode->getValue();
					vector <string> splitArray;
					splitStr(splitArray, rot, " \n\r\t", true, true);

					vecEulerAngles.x = Maths::degToRad(atof(splitArray[3].c_str()));

					quatX.set(Vector3::UNIT_X, vecEulerAngles.x);
				}
				// Y axis
				else if(sid.find("Y") != string::npos)
				{
					string rot = pNode->getValue();
					vector <string> splitArray;
					splitStr(splitArray, rot, " \n\r\t", true, true);

					if(m_Asset.upVector == CUV_Y)				
					{
						vecEulerAngles.y = Maths::degToRad(atof(splitArray[3].c_str()));
						quatY.set(Vector3::UNIT_Y, vecEulerAngles.y);
					}
					else
					{
						vecEulerAngles.z = Maths::degToRad(atof(splitArray[3].c_str()));
						quatZ.set(Vector3::UNIT_Z, vecEulerAngles.z);
					}
				}
				// Z axis
				else if(sid.find("Z") != string::npos)
				{
					string rot = pNode->getValue();
					vector <string> splitArray;
					splitStr(splitArray, rot, " \n\r\t", true, true);

					if(m_Asset.upVector == CUV_Y)				
					{
						vecEulerAngles.z = Maths::degToRad(atof(splitArray[3].c_str()));
						quatZ.set(Vector3::UNIT_Z, vecEulerAngles.z);
					}
					else
					{
						vecEulerAngles.y = Maths::degToRad(atof(splitArray[3].c_str()));
						quatY.set(Vector3::UNIT_Y, vecEulerAngles.y);
					}
				}
			}
			// <scale> tag tells scaling of the node
			else if(tagName == "scale")
			{
				string scale = pNode->getValue();
				vector <string> splitArray;
				splitStr(splitArray, scale, " \n\r\t", true, true);

				Vector3 vecScale;
				if(m_Asset.upVector == CUV_Y)				
				{
					vecScale.set(atof(splitArray[0].c_str()),
								 atof(splitArray[1].c_str()),
								 atof(splitArray[2].c_str()));
				}
				else
				{
					vecScale.set(atof(splitArray[0].c_str()),
								 atof(splitArray[2].c_str()),
								 atof(splitArray[1].c_str()));
				}

				matScale.scale(vecScale);
			}
			// <matrix> have transformation data baked in
			else if(tagName == "matrix")
			{
				string matrix = pNode->getValue();
				vector <string> splitArray;
				splitStr(splitArray, matrix, " \n\r\t", true, true);

				for(uint i = 0; i < 16; ++i)
					matTrans.m[i] = atof(splitArray[i].c_str());

				if(m_Asset.upVector == CUV_Z)
				{
					float fTmp = matTrans._m42;
					matTrans._m42 = matTrans._m43;
					matTrans._m43 = fTmp;
				}

				matTrans.transpose();

				bBakedMatrices = true;
			}
			// <instance_geometry> binds this node with existing mesh
			else if(tagName == "instance_geometry")
			{
				string name = pNode->getAttribute("url");
				name = name.substr(1, name.size() - 1);

				pModel = &m_Models[name];
			}
			// <node> can have child nodes
			else if(tagName == "node")
			{
				string stNodeId = pNode->getAttribute("id");
				string stNodeName = pNode->getAttribute("name");
				string stType = pNode->getAttribute("type");

				if(stNodeName == "" && stNodeId != "")
					stNodeName = stNodeId;

				matRot.rotateYawPitchRoll(vecEulerAngles);

				Matrix4x4 matTransform = matScale * matRot * matTrans;

				loadNode(pNode->getFirstChild(), stNodeId, _surfaces, stType, _nodeName, _type,
					&matTransform);
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}

		Quaternion quatRot = quatZ * quatY * quatX;//quatZ * quatY * quatX;
		matRot = quatRot.toMatrix();
		//matRot.rotateYawPitchRoll(vecEulerAngles);

		Matrix4x4 matTransform;
		if(bBakedMatrices)
			matTransform = matTrans;
		else
			matTransform = matScale * matRot * matTrans;


		// Create Collada node
		processNodeType(_type, pModel, matTransform, _surfaces, _nodeName, _parentName, _parentType,
			_parentTransform);
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::loadAsset(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = _node;
		
		while(pNode) 
		{
			string tagName = pNode->getTagName();

			if(tagName == "contributor")
			{
				SAssetContributor tempContributor;

				pTemp = pNode->getFirstChild();
				while(pTemp)
				{
					if(pTemp->getTagName() == "author")
						tempContributor.stAuthor = pTemp->getValue();
					if(pTemp->getTagName() == "authoring_tool")
						tempContributor.stAuthoringTool  = pTemp->getValue();
					if(pTemp->getTagName() == "comments")
						tempContributor.stComments  = pTemp->getValue();
					if(pTemp->getTagName() == "copyright")
						tempContributor.stCopyright = pTemp->getValue();
					if(pTemp->getTagName() == "source_data")
						tempContributor.stSourceData = pTemp->getValue();

					pTemp = pTemp->getNextSibling();						
				}
				

				m_Asset.pContributor.push_back(tempContributor);
			}
			else if(tagName == "created")			
				m_Asset.stCreated = pNode->getValue();			
			else if(tagName == "modified")			
				m_Asset.stModified = pNode->getValue();			
			else if(tagName == "unit")
			{
				m_Asset.stUnitMeter = pNode->getAttribute("meter");
				m_Asset.stUnitName = pNode->getAttribute("name");
			}
			else if(tagName == "up_axis")
			{
				string upAxis = pNode->getValue();
				if(upAxis == "Y_UP")
					m_Asset.upVector = CUV_Y;
				else
					m_Asset.upVector = CUV_Z;
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::loadImages(XMLDocument* _node)
	{

	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::loadScenes(XMLDocument* _node, vector <Surface>& _surfaces)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = _node;

		while(pNode) 
		{
			string tagName = pNode->getTagName();

			if(tagName == "visual_scene")
			{
				loadScene(pNode->getFirstChild(), _surfaces);
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::loadMaterials(XMLDocument* _node)
	{	
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::loadGeometry(XMLDocument* _node, vector <Surface>& _surfaces)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = _node;		

		while(pNode)
		{
			if(pNode->getTagName() == "geometry")
			{
				stMeshId = pNode->getAttribute("id");
				stMeshName = pNode->getAttribute("name");

				if(stMeshName == "" && stMeshId != "")
					stMeshName = stMeshId;
				else if(stMeshName == "" && stMeshId == "")
				{
					stringstream buffer;
					buffer << "Surface_" << _surfaces.size();
					stMeshName = buffer.str();
				}

				pTemp = pNode->getFirstChild();

				while(pTemp)
				{
					if(pTemp->getTagName() == "mesh")
					{
						//<mesh><source id="" name=""> name: position, normal, map1
						processMeshNode(pTemp->getFirstChild(), _surfaces);
					}

					if(pTemp->getTagName() == "extra")
					{
						bDoubleSided = true;
					}

					pTemp = pTemp->getNextSibling();
				}
			}			

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::processMeshNode(XMLDocument* _node, vector<Surface>& _surfaces)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;


		// Create model object
		SModel model;

	
		while(pNode)
		{	
			string tagName = pNode->getTagName();

			if(tagName == "source")				
			{
				string id = pNode->getAttribute("id");
				string name = pNode->getAttribute("name");
			
				processSourceNode(pNode->getFirstChild(), id, name);
			}
			else if(tagName == "vertices")
			{
				VertexPositionSource = pNode->getFirstChild()->getNextSibling()->getAttribute("source");
				VertexPositionSource = VertexPositionSource.substr(1, VertexPositionSource.size()); // We delete the first '#'

				model.vertexSource = VertexPositionSource;
			}
			else if(tagName == "triangles" || tagName == "polygons")	
			{
				Surface surface;	
				wstring stName(stMeshName.begin(), stMeshName.end());
				uint index = stName.find_first_of(L"-");
				stName = stName.substr(0, index);
				wstringstream buffer;
				buffer << stName << "_" << model.surfaces.size();
				surface.setName(buffer.str());

				model.surfaces.push_back(surface);
				index = model.surfaces.size() - 1;

				STriangleSet set;
				memset(&set, 0, sizeof(STriangleSet));

				processTriangles(pNode->getFirstChild(), &set, model.surfaces[index], pNode->getTagName());

				model.sets.push_back(set);
			}
			else if(tagName == "polylist")
			{
				Surface surface;	
				wstring stName(stMeshName.begin(), stMeshName.end());
				uint index = stName.find_first_of(L"-");
				stName = stName.substr(0, index);
				wstringstream buffer;
				buffer << stName << "_" << model.surfaces.size();
				surface.setName(buffer.str());

				model.surfaces.push_back(surface);
				index = model.surfaces.size() - 1;

				STriangleSet set;
				memset(&set, 0, sizeof(STriangleSet));

				processTriangles(pNode->getFirstChild(), &set, model.surfaces[index], pNode->getTagName());

				model.sets.push_back(set);
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}

		if(model.surfaces.size())
			m_Models[stMeshName] = model;
	}
//----------------------------------------------------------------------	
	void MeshLoadPolicyCollada::readArray(XMLDocument* _node, SSource* _source)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;
		
		vector <string> splitArray;		
		string tempValues;
		
		while(pNode)
		{
			string nodeName = pNode->getTagName();

			if(nodeName.find("array") != string::npos) // float_array, int_array,...
			{	
				_source->Array.Id = pNode->getAttribute("id");
				tempValues = pNode->getValue();				
			}
			else if(nodeName == "technique_common")
			{
				if(pNode->getFirstChild()->getNextSibling()->getTagName() == "accessor")
				{
					XMLDocument* accessorNode = pNode->getFirstChild()->getNextSibling();

					_source->Accessors.Count = (uint)atoi(accessorNode->getAttribute("count").c_str());
					_source->Accessors.Stride = (uint)atoi(accessorNode->getAttribute("stride").c_str());

					XMLDocument* parameterNode = accessorNode->getFirstChild();

					while(parameterNode)
					{
						if(parameterNode->getTagName() == "param")
						{
							SColladaParam param;

							param.Name = parameterNode->getAttribute("name");
							param.Type = parameterNode->getAttribute("type");

							_source->Accessors.Parameters.push_back(param);							
						}

						parameterNode = parameterNode->getNextSibling();
					}
				}
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}

		splitStr(splitArray, tempValues, " \n\r\t", true, true);	

		for(uint i = 0; i < splitArray.size(); ++i)
		{
			_source->Array.Data.push_back(atof(splitArray[i].c_str()));
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::processSourceNode(XMLDocument* _node, string id, string name)
	{	
		SSource sourceNode;		

		sourceNode.Id = id;
		sourceNode.Name = name;
		
		readArray(_node, &sourceNode);

		m_ColladaSource[sourceNode.Id] = sourceNode;
	}	
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::processTriangles(XMLDocument* _node, STriangleSet* _model,
		Surface& _surface, string geometryType)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		string& indicesArray = _model->indicesArray;
		vector <string> pVertex;
		vector <string> verticesPerPolygon;

		uint attributesNum = 0;
		
		while(pNode)
		{
			string tagName = pNode->getTagName();

			// <input> information about data sources, their type, offset etc.
			if(tagName == "input")
			{
				SColladaSemantic tempSemantic;

				tempSemantic.offset = (unsigned)atoi(pNode->getAttribute("offset").c_str());
				tempSemantic.source = pNode->getAttribute("source");
				tempSemantic.source = tempSemantic.source.substr(1, tempSemantic.source.size()); // We delete the first '#'
				tempSemantic.set	= (unsigned)atoi(pNode->getAttribute("set").c_str());

				string semantic = pNode->getAttribute("semantic");

				if(semantic == "VERTEX")
					tempSemantic.type = COLLADA_SEMANTIC_VERTEX;
				else if(semantic == "NORMAL")
					tempSemantic.type = COLLADA_SEMANTIC_NORMAL;
				else if(semantic == "TEXCOORD")
					tempSemantic.type = COLLADA_SEMANTIC_TEXCOORD;
				else if(semantic == "TEXTANGENT")
					tempSemantic.type = COLLADA_SEMANTIC_TEXTANGENT;
				else if(semantic == "TEXBINORMAL")
					tempSemantic.type = COLLADA_SEMANTIC_TEXBINORMAL;

				_model->semanticMapping.push_back(tempSemantic);

				++attributesNum;
			}
			// <vcount> tag specifies number of vertices for each of the polygons
			else if(tagName == "vcount")
			{
				FuncSplit <string> tokenizer;
				tokenizer(pNode->getValue(), verticesPerPolygon, " ");
			}
			// <p> tag stands for primitives
			else if(tagName == "p")
			{	
				if(geometryType == "triangles")
					indicesArray.append(pNode->getValue());
				else if(geometryType == "polylist" || geometryType == "polygons")
				{
					vector <string> tempPrimitive;
					splitStr(tempPrimitive, pNode->getValue(), " \n\r\t", true, true);


					uint triangleIndex = 0;
					uint startIndex = 0;
					for(uint j = 0; j < verticesPerPolygon.size(); ++j)
					{
						uint vertices = (uint)atoi(verticesPerPolygon[triangleIndex++].c_str());
						if(vertices == 3)
						{
							for(uint i = 0; i < vertices * attributesNum; ++i)
							{
								indicesArray.append(tempPrimitive[startIndex++]);
								indicesArray.append(" ");
							}
						}
						else if(vertices == 4)
						{
							for(uint attr = 0; attr < attributesNum; ++attr)
							{
								indicesArray.append(tempPrimitive[startIndex + attr]);
								indicesArray.append(" ");
							}

							for(uint attr = 0; attr < attributesNum; ++attr)
							{
								indicesArray.append(tempPrimitive[startIndex + attributesNum + attr]);
								indicesArray.append(" ");
							}

							for(uint attr = 0; attr < attributesNum; ++attr)
							{
								indicesArray.append(tempPrimitive[startIndex + 3 * attributesNum + attr]);
								indicesArray.append(" ");
							}

							for(uint attr = 0; attr < attributesNum; ++attr)
							{
								indicesArray.append(tempPrimitive[startIndex + 2 * attributesNum + attr]);
								indicesArray.append(" ");
							}

							for(uint attr = 0; attr < attributesNum; ++attr)
							{
								indicesArray.append(tempPrimitive[startIndex + 3 * attributesNum + attr]);
								indicesArray.append(" ");
							}

							for(uint attr = 0; attr < attributesNum; ++attr)
							{
								indicesArray.append(tempPrimitive[startIndex + attributesNum + attr]);
								indicesArray.append(" ");
							}

							startIndex += 4 * attributesNum;
						}
					}
				}
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::processPrimitives(XMLDocument* _node, vector <ushort> _indicesArray)
	{

	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::createMesh(SModel* _model, STriangleSet* _set, Surface& _surface,
		const Matrix4x4& _transform)
	{
		vector <string> splitArray;	
		uint indicesCount = splitStr(splitArray, _set->indicesArray, " \n\r\t", true, true);		
		_set->nIndicesCount = indicesCount / 3;
		vector <string>::const_iterator iter = splitArray.begin();

		HashTable <string, SVertexKey> key;
		
		// We need to build a new list of vertices and indices
		// for this we set some variables to 0
		ushort indexIncrementer = 0;
		_set->nVerticesCount = 0;
		_set->nIndicesCount = 0;

		vector <SColladaSemantic>& semanticMapping = _set->semanticMapping;

		string positionSource = _model->vertexSource;

		for(uint i = 0; i < indicesCount;)
		{
			// Build the single SVertex
			SVertexKey tempKey;
			memset(&tempKey, 0, sizeof(SVertexKey));
			string tempKeyString = "";

			for(uint ii = 0; ii < _set->semanticMapping.size(); ++ii, i++)
			{
				nGENE_COLLADA_SEMANTIC_TYPE type = semanticMapping[ii].type;

				switch(type)
				{
				case COLLADA_SEMANTIC_VERTEX:
					tempKey.nVertexIndex = (ushort)atoi(splitArray[i].c_str());
					break;

				case COLLADA_SEMANTIC_NORMAL:
					tempKey.nNormalIndex = (ushort)atoi(splitArray[i].c_str());
					break;

				case COLLADA_SEMANTIC_TEXCOORD:
					tempKey.nTextureIndex = (ushort)atoi(splitArray[i].c_str());
					break;

				case COLLADA_SEMANTIC_TEXTANGENT:
					tempKey.nTextureTangent = (ushort)atoi(splitArray[i].c_str());
					break;
				case COLLADA_SEMANTIC_TEXBINORMAL:
					tempKey.nTextureBinormal = (ushort)atoi(splitArray[i].c_str());
					break;
				}

				tempKeyString.append(splitArray[i]);
			}

			if(key.find(tempKeyString) != key.end()) 
			{
				// Already in the list, retrieve the value and set the index				
				_set->pIndices.push_back(key[tempKeyString].index);	
				_set->nIndicesCount++;				
			}
			else
			{
				// The key is unique, so we need to insert it

				tempKey.index = indexIncrementer;	// We set a new unique index value, afterwards we increment it				
				key[tempKeyString] = tempKey;								
				indexIncrementer++;

				SVertex tempSurface;
				memset(&tempSurface, 0, sizeof(SVertex));

				for(uint ii = 0; ii < semanticMapping.size(); ++ii)
				{
					nGENE_COLLADA_SEMANTIC_TYPE type = semanticMapping[ii].type;

					// Position
					if(type == COLLADA_SEMANTIC_VERTEX)
					{
						uint position = tempKey.nVertexIndex * m_ColladaSource[positionSource].Accessors.Stride;
						
						Vector4 pos;

						if(m_Asset.upVector == CUV_Y)				
						{
							//-x,y,z							
							pos.x = -m_ColladaSource[positionSource].Array.Data[position];
							pos.y = m_ColladaSource[positionSource].Array.Data[position + 1];
							pos.z = m_ColladaSource[positionSource].Array.Data[position + 2];
						}
						else
						{
							pos.x = m_ColladaSource[positionSource].Array.Data[position];
							pos.z = m_ColladaSource[positionSource].Array.Data[position + 1];
							pos.y = m_ColladaSource[positionSource].Array.Data[position + 2];
						}

						pos.w = 1.0f;
						pos *= _transform;

						tempSurface.vecPosition = Vector3(pos.x, pos.y, pos.z);

						if(!m_matTransform.isIdentity())
						{
							Vector4 vecPos(tempSurface.vecPosition, 0.0f);
							vecPos.multiply(m_matTransform);
							tempSurface.vecPosition.set(vecPos.x, vecPos.y, vecPos.z);
						}
					}
					// Normal vector
					else if(type == COLLADA_SEMANTIC_NORMAL)
					{
						uint position = tempKey.nNormalIndex * m_ColladaSource[semanticMapping[ii].source].Accessors.Stride;
					
						tempSurface.vecNormal.x = m_ColladaSource[semanticMapping[ii].source].Array.Data[position];
						tempSurface.vecNormal.y = m_ColladaSource[semanticMapping[ii].source].Array.Data[position + 1];
						tempSurface.vecNormal.z = m_ColladaSource[semanticMapping[ii].source].Array.Data[position + 2];
					}
					// Texture coordinate
					else if(type == COLLADA_SEMANTIC_TEXCOORD)
					{
						uint position = tempKey.nTextureIndex * m_ColladaSource[semanticMapping[ii].source].Accessors.Stride;

						tempSurface.vecTexCoord.x = m_ColladaSource[semanticMapping[ii].source].Array.Data[position];
						tempSurface.vecTexCoord.y = 1.0f - m_ColladaSource[semanticMapping[ii].source].Array.Data[position + 1];			
					}
				}

				_set->pSurface.push_back(tempSurface);				
				_set->pIndices.push_back(tempKey.index);	// We set the old value of the index so we can get it back later and build the indices

				_set->nIndicesCount++;
				_set->nVerticesCount++;
			}
		}

		SVertexSkinning* pArray = &(*_set->pSurface2.begin());
		if(m_bMoveToCentre)
		{
			AABB box;
			dword dwVerticesCount = _set->pSurface2.size();
			box.construct(pArray, dwVerticesCount, sizeof(SVertex), 0);
			Vector3 vecCentre = box.getCentre();
			for(uint i = 0; i < dwVerticesCount; ++i)
			{
				pArray[i].position -= vecCentre;
			}
		}
				
		IndexedBuffer* ivb;
		VertexBuffer* vb;

		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = pArray;
		vbDesc.verticesNum = _set->nVerticesCount;
		vbDesc.primitivesNum = _set->nIndicesCount / 3;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		vb = manager.createVertexBuffer(vbDesc);

		_surface.setVertexBuffer(vb);
		if(_set->nIndicesCount)
		{
			INDEXEDBUFFER_DESC ivbDesc;
			ivbDesc.indices =  &(*_set->pIndices.begin());
			ivbDesc.indicesNum = _set->nIndicesCount;			
			ivb = manager.createIndexedBuffer(ivbDesc);
			vb->setIndexedBuffer(ivb);
		}
	}
//----------------------------------------------------------------------
	int MeshLoadPolicyCollada::splitStr(vector <string>& L, const string& seq,
		const string& _1cdelim, bool keeptoken, bool _removews)
	{
		typedef string::size_type ST;

		string delims = _1cdelim;
		string STR;

		if(delims.empty()) 
			delims = "\n\r";

		ST pos = 0, LEN = seq.size();

		while(pos < LEN)
		{
			STR = ""; // Init/clear the STR token buffer

			// remove any delimiters including optional (white)spaces
			while((delims.find(seq[pos]) != string::npos) && (pos < LEN)) 
				++pos;

			// leave if @eos
			if(pos == LEN) 
				return L.size();

			// Save token data
			while((delims.find(seq[pos]) == string::npos) && (pos < LEN)) 
				STR += seq[pos++];

			if(!STR.empty()) 
				L.push_back(STR);
		}

		return L.size();
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::processNodeType(const string& _type, SModel* _model,
		const Matrix4x4& _transform, vector <Surface>& _surfaces, const string& _nodeName,
		const string& _parentName, const string& _parentType, const Matrix4x4* _parentTransform)
	{
		if(_type == "" || _type == "NODE")
			processNodeTypeNode(_model, _transform, _surfaces);
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyCollada::processNodeTypeNode(SModel* _model, const Matrix4x4& _transform,
		vector <Surface>& _surfaces)
	{
		if(!_model)
		{
			HashTable <string, SModel>::iterator iter;
			for(iter = m_Models.begin(); iter != m_Models.end(); ++iter)
			{
				_model = &iter->second;
				break;
			}
		}

		if(_model && _model->surfaces.size())
		{
			uint surfacesNum = _model->surfaces.size();
			for(uint i = 0; i < surfacesNum; ++i)
			{
				createMesh(_model, &_model->sets[i], _model->surfaces[i], _transform);
				_surfaces.push_back(_model->surfaces[i]);

				_model->sets[i].semanticMapping.clear();
			}

			_model->surfaces.clear();
		}
	}
//----------------------------------------------------------------------
	MeshLoadPolicyCollada::MeshLoadPolicyCollada():
		m_pDocument(NULL)
	{
	}
//----------------------------------------------------------------------
}
