/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AnimatedMeshLoadPolicyXFile.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "AnimatedMeshLoadPolicyXFile.h"
#include "AABB.h"
#include "DeviceRender.h"
#include "DX9Mapping.h"
#include "KeyFrameAnimationSet.h"
#include "Surface.h"
#include "Vertex.h"
#include "VertexBufferManager.h"
#include "VertexDeclaration.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo AnimatedMeshLoadPolicyXFile::Type(L"AnimatedMeshLoadPolicyXFile", NULL);


	struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9* ppTextures;       // array of textures, entries are NULL if no texture specified    

		// SkinMesh info             
		LPD3DXMESH pOrigMesh;
		LPD3DXATTRIBUTERANGE pAttributeTable;
		DWORD NumAttributeGroups;
		DWORD NumInfl;
		LPD3DXBUFFER pBoneCombinationBuf;
		D3DXMATRIX** ppBoneMatrixPtrs;
		D3DXMATRIX* pBoneOffsetMatrices;
		DWORD NumPaletteEntries;
		bool UseSoftwareVP;
		DWORD iAttributeSW;     // used to denote the split between SW and HW if necessary for non-indexed skinning
	};

	struct D3DXFRAME_DERIVED: public D3DXFRAME
	{
		D3DXFRAME_DERIVED* parent;

		void findParent(D3DXFRAME_DERIVED* _parent=NULL)
		{
			parent = _parent;

			D3DXFRAME_DERIVED* pChild = (D3DXFRAME_DERIVED*)pFrameFirstChild;
			while(pChild)
			{
				pChild->findParent(this);
				pChild = (D3DXFRAME_DERIVED*)pChild->pFrameSibling;
			}
		}

		D3DXFRAME_DERIVED* find(D3DXFRAME_DERIVED* _frame, char* _name)
		{
			if(_frame && _frame->Name && _name)
			{
				if(!strcmp(_frame->Name, _name))
					return _frame;
			}

			if(_frame && _frame->pFrameSibling)
			{
				D3DXFRAME_DERIVED *pFrame = find((D3DXFRAME_DERIVED*)_frame->pFrameSibling, _name);
				if(pFrame)
					return pFrame;
			}
			// Try to find matching name in child frames
			if(_frame && _frame->pFrameFirstChild)
			{
				D3DXFRAME_DERIVED *pFrame = find((D3DXFRAME_DERIVED*)_frame->pFrameFirstChild, _name);
				if(pFrame)
					return pFrame;
			}
			// No matches found, return NULL
			return NULL;
		}
	};


	class nGENEXAllocateHierarchy: public ID3DXAllocateHierarchy
	{
	public:
		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
		{
			HRESULT hr = S_OK;
			D3DXFRAME_DERIVED* pFrame = NULL;

			*ppNewFrame = NULL;
			pFrame = new D3DXFRAME_DERIVED();
			if(!pFrame)
			{
				hr = E_OUTOFMEMORY;
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Creating frame failed while creating .x animated mesh. No memory");

				return hr;
			}

			// Initialize transforms
			D3DXMatrixIdentity(&pFrame->TransformationMatrix);

			if(Name)
			{
				pFrame->Name = new CHAR[strlen(Name) + 1];
				memcpy(pFrame->Name, Name, (strlen(Name) + 1) * sizeof(CHAR));
			}
			else
				pFrame->Name = NULL;
			pFrame->pFrameFirstChild = NULL;
			pFrame->pFrameSibling = NULL;
			pFrame->pMeshContainer = NULL;
			pFrame->parent = NULL;

			*ppNewFrame = pFrame;
			pFrame = NULL;

			return hr;
		}

		STDMETHOD(CreateMeshContainer)(THIS_
			LPCSTR Name,
			CONST D3DXMESHDATA *pMeshData,
			CONST D3DXMATERIAL *pMaterials,
			CONST D3DXEFFECTINSTANCE *pEffectInstances,
			DWORD NumMaterials,
			CONST DWORD *pAdjacency,
			LPD3DXSKININFO pSkinInfo,
			LPD3DXMESHCONTAINER *ppNewMeshContainer)
		{
			HRESULT hr = S_OK;
			D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
			uint nFacesNum;
			uint nMaterial;
			uint nBone, nBones;
			LPDIRECT3DDEVICE9 pd3dDevice = NULL;

			LPD3DXMESH pMesh = NULL;

			*ppNewMeshContainer = NULL;

			// this sample does not handle patch meshes, so fail when one is found
			if(pMeshData->Type != D3DXMESHTYPE_MESH)
			{
				hr = E_FAIL;
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Creating mesh failed while creating .x animated mesh. Not a valid mesh");

				return hr;
				//goto e_Exit;
			}

			// get the pMesh interface pointer out of the mesh data structure
			pMesh = pMeshData->pMesh;

			// this sample does not FVF compatible meshes, so fail when one is found
			if(pMesh->GetFVF() == 0)
			{
				hr = E_FAIL;
				return hr;
				//goto e_Exit;
			}

			// allocate the overloaded structure to return as a D3DXMESHCONTAINER
			pMeshContainer = new D3DXMESHCONTAINER_DERIVED();
			if(pMeshContainer == NULL)
			{
				hr = E_OUTOFMEMORY;
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Creating mesh failed while creating .x animated mesh. No memory");

				return hr;
			}

			memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

			// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
			pMeshContainer->Name = NULL;
//			hr = AllocateName(Name, &pMeshContainer->Name);
			if(FAILED(hr))
				return hr;

			pMesh->GetDevice(&pd3dDevice);
			nFacesNum = pMesh->GetNumFaces();

			// if no normals are in the mesh, add them
			if(!(pMesh->GetFVF() & D3DFVF_NORMAL))
			{
				pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

				// clone the mesh to make room for the normals
				hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
					pMesh->GetFVF() | D3DFVF_NORMAL,
					pd3dDevice, &pMeshContainer->MeshData.pMesh);
				//if(FAILED(hr))
				//	goto e_Exit;

				// get the new pMesh pointer back out of the mesh container to use
				// NOTE: we do not release pMesh because we do not have a reference to it yet
				pMesh = pMeshContainer->MeshData.pMesh;

				// now generate the normals for the pmesh
				D3DXComputeNormals(pMesh, NULL);
			}
			else  // if no normals, just add a reference to the mesh for the mesh container
			{
				pMeshContainer->MeshData.pMesh = pMesh;
				pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

				pMesh->AddRef();
			}

			// allocate memory to contain the material information.  This sample uses
			//   the D3D9 materials and texture names instead of the EffectInstance style materials
			pMeshContainer->NumMaterials = std::max((DWORD)1, NumMaterials);
			pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
			pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
			pMeshContainer->pAdjacency = new DWORD[nFacesNum*3];
			if((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
			{
				hr = E_OUTOFMEMORY;
				return hr;
				//goto e_Exit;
			}

			memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * nFacesNum * 3);
			memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

			// if materials provided, copy them
			if(NumMaterials > 0)
			{
				memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

				for(nMaterial = 0; nMaterial < NumMaterials; nMaterial++)
				{
					if(pMeshContainer->pMaterials[nMaterial].pTextureFilename != NULL)
					{
/*						WCHAR strTexturePath[MAX_PATH];
						WCHAR wszBuf[MAX_PATH];
						MultiByteToWideChar(CP_ACP, 0, pMeshContainer->pMaterials[nMaterial].pTextureFilename, -1, wszBuf, MAX_PATH);
						wszBuf[MAX_PATH - 1] = L'\0';
//						DXUTFindDXSDKMediaFileCch(strTexturePath, MAX_PATH, wszBuf);
/*						if(FAILED(D3DXCreateTextureFromFile(pd3dDevice, strTexturePath,
							&pMeshContainer->ppTextures[nMaterial])))
							pMeshContainer->ppTextures[nMaterial] = NULL;*/

						// don't remember a pointer into the dynamic memory, just forget the name after loading
						//pMeshContainer->pMaterials[nMaterial].pTextureFilename = NULL;
					}
				}
			}
			else // if no materials provided, use a default one
			{
				pMeshContainer->pMaterials[0].pTextureFilename = NULL;
				memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
				pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
				pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
				pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
				pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
			}

			// if there is skinning information, save off the required data and then setup for HW skinning
			if(pSkinInfo != NULL)
			{
				// first save off the SkinInfo and original mesh data
				pMeshContainer->pSkinInfo = pSkinInfo;
				pSkinInfo->AddRef();

				pMeshContainer->pOrigMesh = pMesh;
				pMesh->AddRef();

				// Will need an array of offset matrices to move the vertices from the figure space to the bone's space
				nBones = pSkinInfo->GetNumBones();
				pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[nBones];
				if(pMeshContainer->pBoneOffsetMatrices == NULL)
				{
					hr = E_OUTOFMEMORY;
					return hr;
					//goto e_Exit;
				}

				// get each of the bone offset matrices so that we don't need to get them later
				for(nBone = 0; nBone < nBones; ++nBone)
				{
					pMeshContainer->pBoneOffsetMatrices[nBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(nBone));
				}

				// GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version

				{
					UINT MaxMatrices = 40;
					pMeshContainer->NumPaletteEntries = min((DWORD)MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

					DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
					pMeshContainer->UseSoftwareVP = false;
					Flags |= D3DXMESH_MANAGED;

					NGENE_RELEASE( pMeshContainer->MeshData.pMesh );

					hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh(pMeshContainer->pOrigMesh,
						Flags,
						pMeshContainer->NumPaletteEntries,
						pMeshContainer->pAdjacency,
						NULL, NULL, NULL,
						&pMeshContainer->NumInfl,
						&pMeshContainer->NumAttributeGroups,
						&pMeshContainer->pBoneCombinationBuf,
						&pMeshContainer->MeshData.pMesh );
					if( FAILED( hr ) )
						return hr;


					// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
					DWORD NewFVF = ( pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK ) | D3DFVF_NORMAL |
						D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
					if( NewFVF != pMeshContainer->MeshData.pMesh->GetFVF() )
					{
						LPD3DXMESH pMesh;
						hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF( pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF,
							pd3dDevice, &pMesh );
						if( !FAILED( hr ) )
						{
							pMeshContainer->MeshData.pMesh->Release();
							pMeshContainer->MeshData.pMesh = pMesh;
							pMesh = NULL;
						}
					}

					D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
					LPD3DVERTEXELEMENT9 pDeclCur;
					hr = pMeshContainer->MeshData.pMesh->GetDeclaration( pDecl );
					if( FAILED( hr ) )
						return hr;

					// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
					//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
					//          this is more of a "cast" operation
					pDeclCur = pDecl;
					while( pDeclCur->Stream != 0xff )
					{
						if( ( pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES ) && ( pDeclCur->UsageIndex == 0 ) )
							pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
						pDeclCur++;
					}

					hr = pMeshContainer->MeshData.pMesh->UpdateSemantics( pDecl );
					if( FAILED( hr ) )
						return hr;

					// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
					uint g_NumBoneMatricesMax = 0;
					D3DXMATRIXA16* g_pBoneMatrices = NULL;
					if( g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
					{
						g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

						// Allocate space for blend matrices
						delete[] g_pBoneMatrices;
						g_pBoneMatrices = new D3DXMATRIXA16[g_NumBoneMatricesMax];
						if( g_pBoneMatrices == NULL )
						{
							hr = E_OUTOFMEMORY;
							return hr;
						}
					}
				}

//				hr = GenerateSkinnedMesh(pd3dDevice, pMeshContainer);
				//if(FAILED(hr))
					//goto e_Exit;
			}

			*ppNewMeshContainer = pMeshContainer;
			pMeshContainer = NULL;

			if(pMeshContainer)
				DestroyMeshContainer(pMeshContainer);

			return hr;
		}

		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree)
		{
			NGENE_DELETE_ARRAY(pFrameToFree->Name);
			NGENE_DELETE(pFrameToFree);

			return S_OK;
		}

		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase)
		{
			uint nMaterial;
			D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

			NGENE_DELETE_ARRAY(pMeshContainer->Name);
			NGENE_DELETE_ARRAY(pMeshContainer->pAdjacency);
			NGENE_DELETE_ARRAY(pMeshContainer->pMaterials);
			NGENE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

			// release all the allocated textures
			if(pMeshContainer->ppTextures != NULL)
			{
				for(nMaterial = 0; nMaterial < pMeshContainer->NumMaterials; nMaterial++)
				{
					NGENE_RELEASE(pMeshContainer->ppTextures[nMaterial]);
				}
			}

			NGENE_DELETE_ARRAY(pMeshContainer->ppTextures);
			NGENE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
			NGENE_RELEASE(pMeshContainer->pBoneCombinationBuf);
			NGENE_RELEASE(pMeshContainer->MeshData.pMesh);
			NGENE_RELEASE(pMeshContainer->pSkinInfo);
			NGENE_RELEASE(pMeshContainer->pOrigMesh);
			NGENE_DELETE(pMeshContainer);

			return S_OK;
		}

		nGENEXAllocateHierarchy()
		{
		}
	};


	AnimatedMeshLoadPolicyXFile::AnimatedMeshLoadPolicyXFile():
		m_nStartIndex(0)
	{
	}
//----------------------------------------------------------------------
	vector <Surface> AnimatedMeshLoadPolicyXFile::loadMesh(const wstring& _fileName,
		const Matrix4x4& _transform, bool _moveToCentre)
	{
		m_matTransform = _transform;
		m_bMoveToCentre = _moveToCentre;

		vector <Surface> vPolicySurfaces;		// Mesh surfaces

		ID3DXMesh* pStartMesh = NULL;
		ID3DXMesh* pMesh = NULL;

		dword dwMaterialNum = 0;
		HRESULT hr = 0;
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		// Open file
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(_fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint size = pFile->getSize();
		void* buffer = new char[size];
		pFile->read((char*)buffer, size);
		mgr.closeFile(_fileName);

		nGENEXAllocateHierarchy alloc;
		pRoot = NULL;
		ID3DXAnimationController* pController = NULL;

		// Load mesh and skeleton
		if(FAILED(hr = D3DXLoadMeshHierarchyFromXInMemory(buffer,
													      size,
														  D3DXMESH_MANAGED,
														  pDevice,
														  &alloc,
														  NULL,
														  &pRoot,
														  &pController)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"%ls .x file animated mesh loading failed. Reason: %ls",
				_fileName.c_str(),
				DX9Mapping::getDXErrorDescription(hr).c_str());

			NGENE_DELETE_ARRAY(buffer);

			return vPolicySurfaces;
		}

		((D3DXFRAME_DERIVED*)pRoot)->findParent(NULL);

		// Now recursively process frames
		processFrame((D3DXFRAME_DERIVED*)pRoot, NULL, vPolicySurfaces);

		// Now process animation sets
		processAnimation(pController);


		NGENE_DELETE_ARRAY(buffer);


		return vPolicySurfaces;
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyXFile::processFrame(D3DXFRAME_DERIVED* _frame, BONE* _parent, vector <Surface>& _surfaces)
	{
		// Process this frame mesh
		if(_frame->pMeshContainer)
		{
			if(_frame->pMeshContainer->pSkinInfo)
			{
				uint bonesNum = _frame->pMeshContainer->pSkinInfo->GetNumBones();

				for(uint i = 0; i < bonesNum; ++i)
				{
					LPCSTR boneName = _frame->pMeshContainer->pSkinInfo->GetBoneName(i);
					D3DXFRAME_DERIVED* pFrame = ((D3DXFRAME_DERIVED*)pRoot)->find((D3DXFRAME_DERIVED*)pRoot, (char*)boneName);
					if(!pFrame)
						continue;

					// First process this frame data
					Matrix4x4 mat = DX9Mapping::D3DXMATRIXToMatrix4(pFrame->TransformationMatrix);

					BONE* pBone = new BONE();
					D3DXMATRIX* dxMat = _frame->pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
					pBone->offsetTransform = DX9Mapping::D3DXMATRIXToMatrix4(*dxMat);
					pBone->name = pFrame->Name;
					m_Skeleton.addBone(pBone);
					m_Bones[pFrame] = pBone;
				}

				for(uint i = 0; i < bonesNum; ++i)
				{
					LPCSTR boneName = _frame->pMeshContainer->pSkinInfo->GetBoneName(i);
					D3DXFRAME_DERIVED* pFrame = ((D3DXFRAME_DERIVED*)pRoot)->find((D3DXFRAME_DERIVED*)pRoot, (char*)boneName);
					if(!pFrame)
						continue;

					BONE* pBone = m_Skeleton.getBone(i);

					BONE* pParent = NULL;
					if(pFrame->parent)
					{
						if(m_Bones.find(pFrame->parent) != m_Bones.end())
						{
							pParent = m_Bones.find(pFrame->parent)->second;
							pBone->parent = pParent;
						}
					}
					else
						pBone->parent = NULL;

					if(pParent)
					{
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

				// Now find root
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

			if(_frame->pMeshContainer->MeshData.Type == D3DXMESHTYPE_MESH)
			{
				IDirect3DDevice9* pDevice = NULL;
				_frame->pMeshContainer->MeshData.pMesh->GetDevice(&pDevice);

				// Clone mesh if necessary
				ID3DXMesh* pMesh = cloneMesh(_frame->pMeshContainer->MeshData.pMesh, pDevice);

				// Optimize mesh
				optimizeMesh(pMesh);

				// Get attributes table
				dword dwAttributesNum = 0;
				D3DXATTRIBUTERANGE* pAttrTable = NULL;
				getAttributesTable(pMesh, dwAttributesNum, &pAttrTable);

				// Process attributes table and mesh itself
				processMesh(pMesh, dwAttributesNum, pAttrTable, _surfaces);	


				NGENE_RELEASE(pMesh);
			}
		}
		

		// Process children
		if(_frame->pFrameFirstChild)
			processFrame((D3DXFRAME_DERIVED*)_frame->pFrameFirstChild, NULL, _surfaces);

		// Process siblings
		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)_frame->pFrameSibling;
		while(pFrame)
		{
			processFrame(pFrame, _parent, _surfaces);
			pFrame = (D3DXFRAME_DERIVED*)pFrame->pFrameSibling;
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyXFile::processAnimation(ID3DXAnimationController* _controller)
	{
		uint nCount = _controller->GetNumAnimationSets();
		for(uint i = 0; i < nCount; ++i)
		{
			ID3DXAnimationSet* pBaseSet = NULL;
			_controller->GetAnimationSet(i, &pBaseSet);

			ID3DXKeyframedAnimationSet* pSet = (ID3DXKeyframedAnimationSet*)pBaseSet;

			// Now process the set itself
			string stTemp = pSet->GetName();
			wstring stName(stTemp.begin(), stTemp.end());
			uint nAnimationsNum = pSet->GetNumAnimations();

			KeyFrameAnimationSet animationSet;

			for(uint j = 0; j < nAnimationsNum; ++j)
			{
				KeyFrameAnimation animation;

				uint nRotKeysNum = pSet->GetNumRotationKeys(j);
				uint nScaleKeysNum = pSet->GetNumScaleKeys(j);
				uint nTransKeysNum = pSet->GetNumTranslationKeys(j);

				// Try to map this animation to a bone
				LPCSTR animName = new char[255];
				pSet->GetAnimationNameByIndex(j, &animName);

				BONE* pBone = m_Skeleton.getBone(animName);
				// Not possible to map this frame to the bone so lets skip it
				if(!pBone)
					continue;

				D3DXKEY_QUATERNION* pRotKeys = new D3DXKEY_QUATERNION[nRotKeysNum];
				D3DXKEY_VECTOR3* pScaleKeys = new D3DXKEY_VECTOR3[nScaleKeysNum];
				D3DXKEY_VECTOR3* pTransKeys = new D3DXKEY_VECTOR3[nTransKeysNum];

				// Read animation keys
				pSet->GetRotationKeys(j, pRotKeys);
				pSet->GetScaleKeys(j, pScaleKeys);
				pSet->GetTranslationKeys(j, pTransKeys);

				double length = pRotKeys[nRotKeysNum - 1].Time;
				animation.setAnimationLength(length);

				// Process rotation keys
				for(uint k = 0; k < nRotKeysNum; ++k)
				{
					KeyFrame frame;
					frame.setRotation(DX9Mapping::D3DXQUATERNIONToQuaternion(pRotKeys[k].Value));
					frame.setStage(pRotKeys[k].Time / length);

					animation.addRotationKey(frame);
				}

				// Process scale keys
				for(uint k = 0; k < nScaleKeysNum; ++k)
				{
					KeyFrame frame;
					frame.setScale(DX9Mapping::D3DXVECTOR3ToVector3(pScaleKeys[k].Value));
					frame.setStage(pScaleKeys[k].Time / length);

					animation.addScaleKey(frame);
				}

				// Process translation keys
				for(uint k = 0; k < nTransKeysNum; ++k)
				{
					KeyFrame frame;
					frame.setPosition(DX9Mapping::D3DXVECTOR3ToVector3(pTransKeys[k].Value));
					frame.setStage(pTransKeys[k].Time / length);

					animation.addPositionKey(frame);
				}

				animationSet.addAnimation(pBone, animation);
				animationSet.setName(stName);

				NGENE_DELETE_ARRAY(pRotKeys);
				NGENE_DELETE_ARRAY(pTransKeys);
				NGENE_DELETE_ARRAY(pScaleKeys);
			}

			m_Sets.push_back(animationSet);
		}
	}
//----------------------------------------------------------------------
	ID3DXMesh* AnimatedMeshLoadPolicyXFile::cloneMesh(ID3DXMesh* _mesh, IDirect3DDevice9* _device)
	{
		VertexDeclaration* pDecl = VertexBufferManager::getSingleton().getVertexDeclaration(L"DeclarationSkinning");
		D3DVERTEXELEMENT9 elements[MAX_FVF_DECL_SIZE];
		_mesh->GetDeclaration(elements);
		uint count = 0;

		while(elements[count++].Stream != 255);

		bool equal = true;
		if(--count == pDecl->getCount())
		{
			for(uint i = 0; i < pDecl->getCount(); ++i)
			{
				D3DVERTEXELEMENT9 element = DX9Mapping::svertexElementToD3DVERTEXELEMENT9(*pDecl->getVertexElement(i));
				if((elements[i].Method != element.Method) ||
				   (elements[i].Offset != element.Offset) ||
				   (elements[i].Stream != element.Stream) ||
				   (elements[i].Type != element.Type) ||
				   (elements[i].Usage != element.Usage) ||
				   (elements[i].UsageIndex != (elements[i].UsageIndex)))
				{
					equal = false;
					break;
				}
			}
		}
		else
			equal = false;

		ID3DXMesh* pMesh = NULL;
		if(!equal)
		{
			D3DVERTEXELEMENT9* pElements = new D3DVERTEXELEMENT9[pDecl->getCount() + 1];
			for(uint i = 0; i < pDecl->getCount(); ++i)
				pElements[i] = DX9Mapping::svertexElementToD3DVERTEXELEMENT9(*pDecl->getVertexElement(i));
			pElements[pDecl->getCount()].Method = 0;
			pElements[pDecl->getCount()].Offset = 0;
			pElements[pDecl->getCount()].Stream = 0xFF;
			pElements[pDecl->getCount()].Type   = D3DDECLTYPE_UNUSED;
			pElements[pDecl->getCount()].Usage  = 0;
			pElements[pDecl->getCount()].UsageIndex = 0;

			dword flags = D3DXMESH_MANAGED;
			if(_mesh->GetOptions() & D3DXMESH_32BIT)
				flags |= D3DXMESH_32BIT;
			HRESULT hr = _mesh->CloneMesh(flags, pElements, _device, &pMesh);
			if(hr != S_OK)
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Mesh format doesn't match. Reason: %ls",
					DX9Mapping::getDXErrorDescription(hr).c_str());
				return NULL;
			}
			NGENE_RELEASE(_mesh);
		}
		else
			pMesh = _mesh;

		return pMesh;
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyXFile::optimizeMesh(ID3DXMesh* _mesh)
	{
		ID3DXMesh* pMesh = _mesh;
		dword* pAdj = new dword[3 * pMesh->GetNumFaces() * sizeof(dword)];
		HRESULT hr = 0;

		// Generate adjacency information
		if(FAILED(hr = pMesh->GenerateAdjacency(0.00001f, pAdj)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Adjacency information could not be generated. Reason: %ls", DX9Mapping::getDXErrorDescription(hr));
			return;
		}

		// Optimize in-place
		if(FAILED(hr = pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
											  pAdj,
											  NULL,
											  NULL,
											  NULL)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Mesh could not be optimized. Reason: %ls", DX9Mapping::getDXErrorDescription(hr));
			return;
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyXFile::getAttributesTable(ID3DXMesh* _mesh, dword& _numAttrs, D3DXATTRIBUTERANGE** _table)
	{
		ID3DXMesh* pMesh = _mesh;
		HRESULT hr = 0;

		if(FAILED(hr = pMesh->GetAttributeTable(NULL, &_numAttrs)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Attribute table data could not be obtained. Reason: %ls",
				DX9Mapping::getDXErrorDescription(hr));
			return;
		}

		(*_table) = new D3DXATTRIBUTERANGE[_numAttrs * sizeof(D3DXATTRIBUTERANGE)];
		if(FAILED(hr = pMesh->GetAttributeTable((*_table), NULL)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Attribute table data could not be obtained. Reason: %ls",
				DX9Mapping::getDXErrorDescription(hr));
			return;
		}
	}
//----------------------------------------------------------------------
	void AnimatedMeshLoadPolicyXFile::processMesh(ID3DXMesh* _mesh, dword _numAttributes, D3DXATTRIBUTERANGE* _table,
		vector <Surface>& _surfaces)
	{
		ID3DXMesh* pMesh = _mesh;

		IDirect3DVertexBuffer9* pVB = NULL;
		IDirect3DIndexBuffer9* pIVB = NULL;

		SVertexSkinning* pVertices = NULL;
		ushort* pIndices = NULL;


		pMesh->GetVertexBuffer(&pVB);
		pMesh->GetIndexBuffer(&pIVB);


		// Lock the buffers
		dword dwIndexSize = sizeof(ushort);
		if(pMesh->GetOptions() & D3DXMESH_32BIT)
			dwIndexSize = sizeof(dword);

		pVB->Lock(0, pMesh->GetNumVertices() * sizeof(SVertexSkinning), reinterpret_cast<void**>(&pVertices), 0);
		pIVB->Lock(0, pMesh->GetNumFaces() * 3 * dwIndexSize, reinterpret_cast<void**>(&pIndices), 0);


		// Process attributes
		for(uint i = 0; i < _numAttributes; ++i)
		{
			SVertexSkinning* pVertices2 = new SVertexSkinning[_table[i].VertexCount];
			memcpy(pVertices2, pVertices + _table[i].VertexStart, _table[i].VertexCount * sizeof(SVertexSkinning));

			void* pTemp = NULL;
			if(pMesh->GetOptions() & D3DXMESH_32BIT)
			{
				dword* pIndices2 = new dword[3 * _table[i].FaceCount];
				pTemp = new dword[3 * _table[i].FaceCount];
				memcpy(pIndices2, pIndices + 3 * _table[i].FaceStart, 3 * _table[i].FaceCount * dwIndexSize);

				for(dword j = 0; j < _table[i].FaceCount; ++j)
					pIndices2[j] -= _table[i].VertexStart;
				memcpy(pTemp, pIndices2, 3 * _table[i].FaceCount * dwIndexSize);

				NGENE_DELETE_ARRAY(pIndices2);
			}
			else
			{
				ushort* pIndices2 = new ushort[3 * _table[i].FaceCount];
				pTemp = new dword[3 * _table[i].FaceCount];
				memcpy(pIndices2, pIndices + 3 * _table[i].FaceStart, 3 * _table[i].FaceCount * dwIndexSize);

				for(dword j = 0; j < 3 * _table[i].FaceCount; ++j)
					pIndices2[j] -= _table[i].VertexStart;
				memcpy(pTemp, pIndices2, 3 * _table[i].FaceCount * dwIndexSize);

				NGENE_DELETE_ARRAY(pIndices2);
			}

			
			if(!m_matTransform.isIdentity())
			{
				for(uint j = 0; j < _table[i].VertexCount; ++j)
				{	
					Vector4 vecPos(pVertices2[j].position, 0.0f);
					vecPos.multiply(m_matTransform);
					pVertices2[j].position.set(vecPos.x, vecPos.y, vecPos.z);
				}
			}

			if(m_bMoveToCentre)
			{
				AABB box;
				box.construct(pVertices2, _table[i].VertexCount, sizeof(SVertexSkinning), 0);
				Vector3 vecCentre = box.getCentre();
				for(uint j = 0; j < _table[i].VertexCount; ++j)
				{
					pVertices2[j].position -= vecCentre;
				}
			}

			VertexBufferManager& manager = VertexBufferManager::getSingleton();
			VertexBuffer* vb;
			IndexedBuffer* ivb;

			VERTEXBUFFER_DESC vbDesc;
			vbDesc.vertices = pVertices2;
			vbDesc.verticesNum = _table[i].VertexCount;
			vbDesc.primitivesNum = _table[i].FaceCount;
			vbDesc.vertexSize = sizeof(SVertexSkinning);
			vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"DeclarationSkinning");
			vb = manager.createVertexBuffer(vbDesc);

			INDEXEDBUFFER_DESC ivbDesc;
			ivbDesc.indices = pTemp;
			ivbDesc.indicesNum = 3 * _table[i].FaceCount;
			ivb = manager.createIndexedBuffer(ivbDesc);

			vb->setIndexedBuffer(ivb);

			wostringstream buffer;
			buffer << L"surface_" << i + m_nStartIndex;

			Surface surface(buffer.str());
			surface.setVertexBuffer(vb);
			_surfaces.push_back(surface);


			NGENE_DELETE_ARRAY(pVertices2);
			NGENE_DELETE_ARRAY(pTemp);
		}

		m_nStartIndex = _numAttributes;


		pVB->Unlock();
		pIVB->Unlock();


		NGENE_RELEASE(pIVB);
		NGENE_RELEASE(pVB);
	}
//----------------------------------------------------------------------
}
