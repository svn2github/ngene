/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AnimatedMeshLoadPolicyCollada.cpp
Version:	0.05

Author:		Wojciech Toman
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "AnimatedMeshLoadPolicyCollada.h"
#include "AABB.h"
#include "FileNarrow.h"
#include "FuncSplit.h"
#include "Point.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "VertexBufferManager.h"
#include "XMLDocument.h"
#include "XMLNode.h"
#include "XMLParser.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo AnimatedMeshLoadPolicyCollada::Type(L"AnimatedMeshLoadPolicyCollada", NULL);


	vector <Surface> AnimatedMeshLoadPolicyCollada::loadMesh(const wstring& _fileName,
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
			else if(nodeName == "library_controllers")
			{
				loadControllers(pChild->getFirstChild(), vPolicySurfaces);
			}
			else if(nodeName == "library_animations")
			{
				loadAnimations(pChild->getFirstChild(), vPolicySurfaces);
			}

			m_pCurrentNode = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(m_pCurrentNode);
		}


		// Create animations finally
		createAnimations();


		NGENE_DELETE(pDocument);
		
		return vPolicySurfaces;
	}
//----------------------------------------------------------------------
	AnimatedMeshLoadPolicyCollada::AnimatedMeshLoadPolicyCollada():
		m_bMoveToCentre(false)
	{
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::loadAnimations(XMLDocument* _node, vector <Surface>& _surfaces)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		while(pNode)
		{
			string tagName = pNode->getTagName();

			// <animation> tag holds information about single animation curve
			if(tagName == "animation")
			{
				processAnimation(pNode->getFirstChild());
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::createAnimations()
	{
		KeyFrameAnimationSet animationSet;
		HashTable <string, KeyFrameAnimation> animations;

		string prev;
		vector <Real> x, y, z;

		string prevBone = "";

		for(uint i = 0; i < m_Channels.size(); ++i)
		{
			SAnimationChannel& channel = m_Channels[i];
			SAnimationSampler& sampler = m_Samplers[channel.source];

			if(animations.find(channel.targetBone) == animations.end())
			{
				KeyFrameAnimation animation;
				animation.setAnimationLength(500.0f);
				animations[channel.targetBone] = animation;
			}

			KeyFrameAnimation& animation = animations[channel.targetBone];


			vector <Real> times;
			vector <Real> values;

			uint stride = 0;

			for(uint j = 0; j < sampler.semantics.size(); ++j)
			{
				SColladaSemantic& semantic = sampler.semantics[j];
				switch(semantic.type)
				{
				case COLLADA_SEMANTIC_INPUT:
					times = m_ColladaSource[semantic.source].Array.Data;
					break;
				case COLLADA_SEMANTIC_OUTPUT:
					values = m_ColladaSource[semantic.source].Array.Data;
					stride = m_ColladaSource[semantic.source].Accessors.Stride;
					break;
				}
			}
			
			KeyFrame frame;
			if(channel.targetValue.find("X.ANGLE") != string::npos)
			{
				x = values;
			}
			else if(channel.targetValue.find("Y.ANGLE") != string::npos)
			{
				y = values;
			}
			else if(channel.targetValue.find("Z.ANGLE") != string::npos)
			{
				z = values;

				for(uint key = 0; key < values.size(); key += stride)
				{
					Quaternion quatRot;
					Vector3 euler;
					if(m_Asset.upVector == CUV_Y)
					{
						euler.set(Maths::degToRad(z[key]),
								  Maths::degToRad(y[key]),
								  Maths::degToRad(x[key]));
					}
					else
					{
						euler.set(Maths::degToRad(y[key]),
								  Maths::degToRad(z[key]),
								  Maths::degToRad(x[key]));
					}

					quatRot.fromEulerAngles(euler);
					frame.setRotation(quatRot);
					
					uint index = key / stride;
					if(index)
						frame.setStage(times[key / stride]);
					else
						frame.setStage(0.0f);

					animation.addRotationKey(frame);
					if(!index)
					{
						frame.setStage(1.0f);
						animation.addRotationKey(frame);
					}
				}

			}
			else if(channel.targetValue.find("scale") != string::npos)
			{
				for(uint key = 0; key < values.size(); key += stride)
				{
					float scale = values[key];
					frame.setScale(Vector3(scale, scale, scale));
					
					uint index = key / stride;
					if(index)
						frame.setStage(times[key / stride]);
					else
						frame.setStage(0.0f);

					animation.addScaleKey(frame);
					if(!index)
					{
						frame.setStage(1.0f);
						animation.addScaleKey(frame);
					}
				}
			}
			else if(channel.targetValue.find("location") != string::npos ||
					channel.targetValue.find("translate") != string::npos)
			{
				for(uint key = 0; key < values.size(); key += stride)
				{
					if(m_Asset.upVector == CUV_Y)				
					{
						Vector3 pos(values[key],
									values[key + 1],
									values[key + 2]);
						frame.setPosition(pos);
					}
					else
					{
						Vector3 pos(values[key],
									values[key + 2],
									values[key + 1]);
						frame.setPosition(pos);
					}
					
					uint index = key / stride;
					if(index)
						frame.setStage(times[key / stride]);
					else
						frame.setStage(0.0f);

					animation.addPositionKey(frame);
					if(!index)
					{
						frame.setStage(1.0f);
						animation.addPositionKey(frame);
					}
				}
			}
		}

		HashTable <string, KeyFrameAnimation>::iterator iter;
		for(iter = animations.begin(); iter != animations.end(); ++iter)
		{
			BONE* pBone = m_Skeleton.getBone(iter->first);
			animationSet.addAnimation(pBone, iter->second);
		}

		animationSet.setName(L"animation");
		m_Sets.push_back(animationSet);
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::processSkin(XMLDocument* _node, const string& _id)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		Matrix4x4 matBindShape;

		while(pNode)
		{
			string tagName = pNode->getTagName();

			// <bind_shape_matrix> tag holds information about mesh matrix in bind pose
			if(tagName == "bind_shape_matrix")
			{
				string matrix = pNode->getValue();
				vector <string> splitArray;
				splitStr(splitArray, matrix, " \n\r\t", true, true);
				if(splitArray.size() == 16)
				{
					for(uint i = 0; i < 16; ++i)
						matBindShape.m[i] = atof(splitArray[i].c_str());
					matBindShape.transpose();
				}
			}
			else if(tagName == "source")
			{
				string id = pNode->getID();

				if(id.find("joints") != string::npos)
				{
					processJoints(pNode->getFirstChild());
				}
				else if(id.find("poses") != string::npos)
				{
					processBindPoses(pNode->getFirstChild());
				}
				else if(id.find("weights") != string::npos)
				{
					processWeightsArray(pNode->getFirstChild());
				}
			}
			else if(tagName == "vertex_weights")
			{
				processWeights(pNode->getFirstChild());
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::processBindPoses(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		while(pNode)
		{
			string tagName = pNode->getTagName();

			if(tagName == "float_array")
			{
				string bindPoses = pNode->getValue();
				vector <string> splitArray;
				splitStr(splitArray, bindPoses, " \n\r\t", true, true);

				if(!(splitArray.size() % m_Skeleton.getBonesNum()))
				{
					uint bonesNum = m_Skeleton.getBonesNum();
					for(uint i = 0; i < bonesNum; ++i)
					{
						uint start = i << 4;
						for(uint j = start; j < (start + 16); ++j)
						{
							m_Skeleton.getBone(i)->offsetTransform.m[j - start] = atof(splitArray[j].c_str());
						}

						Matrix4x4& matOffset = m_Skeleton.getBone(i)->offsetTransform;
						matOffset.transpose();

						if(m_Asset.upVector == CUV_Z)
						{
							float fTmp = matOffset._m42;
							matOffset._m42 = matOffset._m43;
							matOffset._m43 = fTmp;
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
	void AnimatedMeshLoadPolicyCollada::processWeightsArray(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		while(pNode)
		{
			string tagName = pNode->getTagName();

			if(tagName == "float_array")
			{
				string weights = pNode->getValue();
				vector <string> splitArray;
				splitStr(splitArray, weights, " \n\r\t", true, true);

				for(uint i = 0; i < splitArray.size(); ++i)
				{
					m_vWeights.push_back(atof(splitArray[i].c_str()));
				}
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::processWeights(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		while(pNode)
		{
			string tagName = pNode->getTagName();

			// <vcount> tag tells how many bones influence each vertex
			if(tagName == "vcount")
			{
				string weights = pNode->getValue();
				vector <string> splitArray;
				splitStr(splitArray, weights, " \n\r\t", true, true);

				for(uint i = 0; i < splitArray.size(); ++i)
				{
					SVertexInfluence influence;
					memset(&influence, 0, sizeof(SVertexInfluence));
					influence.bonesNum = atoi(splitArray[i].c_str());

					m_Influences.push_back(influence);
				}
			}
			// <v> tag binds weights with the bones
			else if(tagName == "v")
			{
				string weights = pNode->getValue();
				vector <string> splitArray;
				splitStr(splitArray, weights, " \n\r\t", true, true);

				uint start = 0;

				for(uint i = 0; i < m_Influences.size(); ++i)
				{
					SVertexInfluence& influence = m_Influences[i];
					uint add = influence.bonesNum << 1;
					float weightTotal = 0.0f;

					for(uint j = start; j < start + add; j += 2)
					{
						influence.bonesIndices.push_back(atoi(splitArray[j].c_str()));

						float weight = m_vWeights[atoi(splitArray[j + 1].c_str())];
						influence.weights.push_back(weight);
						weightTotal += weight;
					}

					for(uint weight = 0; weight < influence.weights.size(); ++weight)
					{
						influence.weights[weight] /= weightTotal;
					}

					start += (influence.bonesNum << 1);
				}
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::processJoints(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		while(pNode)
		{
			string tagName = pNode->getTagName();
			//string id = pNode->getID();

			if(tagName.find("array") != string::npos)
			{
				string bones = pNode->getValue();
				vector <string> splitArray;
				splitStr(splitArray, bones, " \n\r\t", true, true);

				for(uint i = 0; i < splitArray.size(); ++i)
				{
					BONE* pBone = new BONE();
					pBone->name = splitArray[i];
					m_Skeleton.addBone(pBone);
				}
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::processController(XMLDocument* _node, const string& _id)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		while(pNode)
		{
			string tagName = pNode->getTagName();

			// <animation> tag holds information about single animation curve
			if(tagName == "skin")
			{
				processSkin(pNode->getFirstChild(), _id);
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::loadControllers(XMLDocument* _node, vector<Surface>& _surfaces)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		while(pNode)
		{
			string tagName = pNode->getTagName();

			// <animation> tag holds information about single animation curve
			if(tagName == "controller")
			{
				string id = pNode->getID();

				processController(pNode->getFirstChild(), id);
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::processAnimation(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		while(pNode)
		{
			string tagName = pNode->getTagName();

			// <source> tag holds information about data
			if(tagName == "source")
			{
				string id = pNode->getAttribute("id");
				string name = pNode->getAttribute("name");		

				processSourceNode(pNode->getFirstChild(), id, name);
			}
			// <sampler> tag binds sources and interpolation together
			else if(tagName == "sampler")
			{
				string id = pNode->getAttribute("id");

				processSampler(pNode->getFirstChild(), id);
			}
			// <channel> tag binds source with the target joint
			else if(tagName == "channel")
			{
				SAnimationChannel channel;

				channel.source = pNode->getAttribute("source");
				channel.source = channel.source.substr(1, channel.source.size()); // We delete the first '#'

				vector <string> targets;
				FuncSplit <string> tokenizer;
				tokenizer(pNode->getAttribute("target"), targets, "/");
				channel.targetBone = targets[0];
				channel.targetValue= targets[1];

				m_Channels.push_back(channel);
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::processSampler(XMLDocument* _node, const string& _id)
	{
		XMLDocument* pNode = _node;
		XMLDocument* pTemp = NULL;

		SAnimationSampler sampler;

		while(pNode)
		{
			string tagName = pNode->getTagName();

			if(tagName == "input")
			{
				SColladaSemantic tempSemantic;

				tempSemantic.offset = (unsigned)atoi(pNode->getAttribute("offset").c_str());
				tempSemantic.source = pNode->getAttribute("source");
				tempSemantic.source = tempSemantic.source.substr(1, tempSemantic.source.size()); // We delete the first '#'
				tempSemantic.set	= 0;

				if(pNode->getAttribute("semantic") == "INPUT")				
					tempSemantic.type = COLLADA_SEMANTIC_INPUT;				
				if(pNode->getAttribute("semantic") == "OUTPUT")				
					tempSemantic.type = COLLADA_SEMANTIC_OUTPUT;
				if(pNode->getAttribute("semantic") == "INTERPOLATION")
					tempSemantic.type = COLLADA_SEMANTIC_INTERPOLATION;

				sampler.semantics.push_back(tempSemantic);
			}

			pTemp = pNode;
			pNode = pNode->getNextSibling();
			NGENE_DELETE(pTemp);
		}

		m_Samplers[_id] = sampler;
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyCollada::createMesh(SModel* _model, STriangleSet* _set,
		Surface& _surface, const Matrix4x4& _transform)
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
			// Build the single SVertexSkinning
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

				SVertexSkinning tempSurface;
				memset(&tempSurface, 0, sizeof(SVertexSkinning));

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

						tempSurface.position = Vector3(pos.x, pos.y, pos.z);

						if(!m_matTransform.isIdentity())
						{
							Vector4 vecPos(tempSurface.position, 0.0f);
							vecPos.multiply(m_matTransform);
							tempSurface.position.set(vecPos.x, vecPos.y, vecPos.z);
						}
					}
					// Normal vector
					else if(type == COLLADA_SEMANTIC_NORMAL)
					{
						uint position = tempKey.nNormalIndex * m_ColladaSource[semanticMapping[ii].source].Accessors.Stride;

						tempSurface.normal.x = m_ColladaSource[semanticMapping[ii].source].Array.Data[position];
						tempSurface.normal.y = m_ColladaSource[semanticMapping[ii].source].Array.Data[position + 1];
						tempSurface.normal.z = m_ColladaSource[semanticMapping[ii].source].Array.Data[position + 2];
					}
					// Texture coordinate
					else if(type == COLLADA_SEMANTIC_TEXCOORD)
					{
						uint position = tempKey.nTextureIndex * m_ColladaSource[semanticMapping[ii].source].Accessors.Stride;

						tempSurface.texCoord.x = m_ColladaSource[semanticMapping[ii].source].Array.Data[position];
						tempSurface.texCoord.y = 1.0f - m_ColladaSource[semanticMapping[ii].source].Array.Data[position + 1];			
					}
				}

				// Set up skinning related stuff
				SVertexInfluence& influence = m_Influences[tempKey.nVertexIndex];
				
				for(uint bone = 0; bone < influence.bonesNum; ++bone)
				{
					tempSurface.boneIndices |= influence.bonesIndices[bone] << (bone << 3);
					tempSurface.boneWeights.vals[bone] = influence.weights[bone];
				}


				_set->pSurface2.push_back(tempSurface);				
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
			box.construct(pArray, dwVerticesCount, sizeof(SVertexSkinning), 0);
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
		vbDesc.vertexSize = sizeof(SVertexSkinning);
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"DeclarationSkinning");
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
	void AnimatedMeshLoadPolicyCollada::processNodeType(const string& _type, SModel* _model,
		const Matrix4x4& _transform, vector <Surface>& _surfaces, const string& _nodeName,
		const string& _parentName, const string& _parentType, const Matrix4x4* _parentTransform)
	{
		if(_parentName == "")
		{
			// Now find root
			uint bonesNum = m_Skeleton.getBonesNum();
			for(uint i = 0; i < bonesNum; ++i)
			{
				BONE* pBone = m_Skeleton.getBone(i);
				if(!pBone->parent)
				{
					m_Skeleton.setRoot(pBone);
					break;
				}
			}
		}


		if(_type == "" || _type == "NODE")
			processNodeTypeNode(_model, _transform, _surfaces);
		else if(_type == "JOINT")
		{
			// Set up hierarchy of bones
			BONE* pBone = m_Skeleton.getBone(_nodeName);
			if(pBone)
			{
				pBone->localTransform = _transform;
				
				BONE* pParent = m_Skeleton.getBone(_parentName);
				// Create dummy parent
				if(!pParent && _parentName != "" && _parentType == "JOINT")
				{
					pParent = new BONE();
					pParent->name = _parentName;

					m_Skeleton.addBone(pParent);
				}

				if(pParent)
				{
					pBone->parent = pParent;

					if(pParent->children)
					{
						BONE* pTemp = pParent->children;
						pParent->children = pBone;
						pBone->nextSibling = pTemp;
						pTemp->prevSibling = pBone;
					}
					else
						pParent->children = pBone;
				}
			}
		}
	}
//----------------------------------------------------------------------
}
