/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeVolumetric.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "NodeVolumetric.h"
#include "Billboard.h"
#include "Camera.h"
#include "ITexture.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "NodeLight.h"
#include "Shader.h"
#include "ShaderConstant.h"
#include "ShaderInstance.h"
#include "TextureManager.h"
#include "Timer.h"
#include "VertexBufferManager.h"


namespace nGENE
{
namespace Nature
{
	// Initialize static members
	TypeInfo NodeVolumetric::Type(L"NodeVolumetric", &NodeVisible::Type);


	NodeVolumetric::NodeVolumetric(NodeLight* _light, uint _slicesNum):
		m_pVB(NULL),
		m_pVolumetricMat(NULL),
		m_pLight(_light),
		m_pDebug(NULL),
		m_Colour(Colour::COLOUR_WHITE),
		m_nMaxSlices(_slicesNum),
		m_fFarPlane(10.0f),
		m_bIsShadowUsed(true),
		m_bIsCookieUsed(true),
		m_bIsNoiseUsed(true),
		m_bShowDebug(true)
	{
		m_matTexture.set(0.5f,	  0.0f,		0.0f, 0.0f,
						 0.0f,	  -0.5f,	0.0f, 0.0f,
						 0.0f,	  0.0f,		1.0f, 0.0f,
						 0.5f,	  0.5f,		0.0f, 1.0f);

		m_pLight->setIndividualShadow(true);
	}
//----------------------------------------------------------------------
	NodeVolumetric::NodeVolumetric(uint _slicesNum):
		m_pVB(NULL),
		m_pVolumetricMat(NULL),
		m_pLight(NULL),
		m_pDebug(NULL),
		m_Colour(Colour::COLOUR_WHITE),
		m_nMaxSlices(_slicesNum),
		m_fFarPlane(10.0f),
		m_bIsShadowUsed(true),
		m_bIsCookieUsed(true),
		m_bIsNoiseUsed(true),
		m_bShowDebug(true)
	{
		m_matTexture.set(0.5f,	  0.0f,		0.0f, 0.0f,
						 0.0f,	  -0.5f,	0.0f, 0.0f,
						 0.0f,	  0.0f,		1.0f, 0.0f,
						 0.5f,	  0.5f,		0.0f, 1.0f);
	}
//----------------------------------------------------------------------
	NodeVolumetric::~NodeVolumetric()
	{
	}
//----------------------------------------------------------------------
	NodeVolumetric::NodeVolumetric(const NodeVolumetric& src):
		NodeVisible(src),
		m_nMaxSlices(src.m_nMaxSlices)
	{
		for(uint i = 0; i < m_vSurfaces.size(); ++i)
		{
			m_vSurfaces[i]->onBeginRender.bind(this, &NodeVolumetric::beginRender);
			m_vSurfaces[i]->onEndRender.bind(this, &NodeVolumetric::endRender);
			m_vSurfaces[i]->onUpdateAABB.bind(this, &NodeVolumetric::updateAABB);
		}
		m_Frustum = src.m_Frustum;
		m_pVB = src.m_pVB;

		m_pLight = src.m_pLight;
		m_pVolumetricMat = src.m_pVolumetricMat;
		m_fFarPlane = src.m_fFarPlane;

		m_Colour = src.m_Colour;

		m_bIsShadowUsed = src.m_bIsShadowUsed;
		m_bIsCookieUsed = src.m_bIsCookieUsed;
		m_bIsNoiseUsed = src.m_bIsNoiseUsed;
		m_bShowDebug = src.m_bShowDebug;

		m_matTexture = src.m_matTexture;

		for(uint i = 0; i < 8; ++i)
			m_ptFrustum[i] = src.m_ptFrustum[i];

		Node::m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	NodeVolumetric& NodeVolumetric::operator=(const NodeVolumetric& rhs)
	{
		if(this == &rhs)
			return (*this);

		NodeVisible::operator=(rhs);

		for(uint i = 0; i < m_vSurfaces.size(); ++i)
		{
			m_vSurfaces[i]->onBeginRender.bind(this, &NodeVolumetric::beginRender);
			m_vSurfaces[i]->onEndRender.bind(this, &NodeVolumetric::endRender);
			m_vSurfaces[i]->onUpdateAABB.bind(this, &NodeVolumetric::updateAABB);
		}
		m_Frustum = rhs.m_Frustum;
		m_pVB = rhs.m_pVB;

		m_pLight = rhs.m_pLight;
		m_pVolumetricMat = rhs.m_pVolumetricMat;
		m_fFarPlane = rhs.m_fFarPlane;

		m_Colour = rhs.m_Colour;

		m_bIsShadowUsed = rhs.m_bIsShadowUsed;
		m_bIsCookieUsed = rhs.m_bIsCookieUsed;
		m_bIsNoiseUsed = rhs.m_bIsNoiseUsed;
		m_bShowDebug = rhs.m_bShowDebug;

		m_matTexture = rhs.m_matTexture;

		for(uint i = 0; i < 8; ++i)
			m_ptFrustum[i] = rhs.m_ptFrustum[i];

		Node::m_bHasChanged = true;


		return (*this);
	}
//----------------------------------------------------------------------
	void NodeVolumetric::init()
	{
		dword dwNumTris = m_nMaxSlices * 2;
		dword dwNumVertices = 4 * m_nMaxSlices;
		dword dwNumIndices = 3 * dwNumTris;

		SVertex* pVertices = new SVertex[dwNumVertices];
		short* pIndices = new short[dwNumIndices];

		memset(pVertices, 0, dwNumVertices * sizeof(SVertex));

		int row, col, shell, curIndex = 0;
		int count = static_cast<int>(m_nMaxSlices);

		for (shell = 0; shell < count; ++shell)
		{
			for (row = 0; row < 1; row++)
			{
				for (col = 0; col < 1; col++)
				{
					// Indices for one triangle
					pIndices[curIndex++] = shell * 4 + (row + 1) * 2 + col;
					pIndices[curIndex++] = shell * 4 + row * 2 + col + 1;
					pIndices[curIndex++] = shell * 4 + row * 2 + col;
					

					// Indices for one triangle
					pIndices[curIndex++] = shell * 4 + (row + 1) * 2 + col + 1;
					pIndices[curIndex++] = shell * 4 + row * 2 + col + 1;
					pIndices[curIndex++] = shell * 4 + (row + 1) * 2 + col;
				}
			}
		}

		row, col, shell = 0;
		int curVertex = 0;
		dword dwCurrentColor;

		for (shell = 0; shell < count; ++shell)
		{
			dwCurrentColor = mapShellToColourChannel(shell);

			for (row = 0; row <= 1; ++row)
			{
				for (col = 0; col <= 1; ++col)
				{
					float fX =         ((float)col)   / ((float)(1));
					float fY = 1.0f - (((float)row)   / ((float)(1)));
					float fZ = (((float)shell) / ((float)(m_nMaxSlices - 1)));

					pVertices[curVertex].vecPosition = Vector3(fX, fY, fZ);
					pVertices[curVertex].dwColour = dwCurrentColor;

					++curVertex;
				}
			}
		}

		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indices = pIndices;
		ivbDesc.indicesNum = dwNumIndices;
		IndexedBuffer* ivb = manager.createIndexedBuffer(ivbDesc);

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = pVertices;
		vbDesc.verticesNum = dwNumVertices;
		vbDesc.primitivesNum = dwNumTris;
		vbDesc.primitivesType = PT_TRIANGLELIST;
		vbDesc.indices = ivb;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");

		m_pVB = manager.createVertexBuffer(vbDesc);

		// Create surface
		Surface temp;
		temp.setVertexBuffer(m_pVB);
		addSurface(L"Base", temp);



		// Create frustum
		m_Frustum.setPerspective(Maths::PI * 0.5f, 1.0f, 1.0f, m_fFarPlane);
		Vector3 pos = Vector3(0, 0, 0);
		Vector3 forward = Vector3::UNIT_Z;
		Vector3 right = Vector3::UNIT_X;
		Vector3 up = Vector3::UNIT_Y;
		m_Frustum.setVectors(pos, forward, right, up);
		m_Frustum.update();

		m_pDebug = new NodeDebugFrustum(&m_Frustum);
		m_pDebug->setCreator(this->getCreator());
		m_pDebug->init();
		this->addChild(L"debug_frustum", m_pDebug);

		setShowDebug(m_bShowDebug);

		// Release memory
		NGENE_DELETE_ARRAY(pVertices);
		NGENE_DELETE_ARRAY(pIndices);

		// Set material
		Surface* pSurface = this->getSurface(L"Base");
		m_pVolumetricMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"light_shaft");
		pSurface->setMaterial(m_pVolumetricMat);

		// Bind delegates
		pSurface->onBeginRender.bind(this, &NodeVolumetric::beginRender);
		pSurface->onEndRender.bind(this, &NodeVolumetric::endRender);
		pSurface->onUpdateAABB.bind(this, &NodeVolumetric::updateAABB);

		m_dwPrevTime = Engine::getSingleton().getTimer().getMilliseconds();
	}
//----------------------------------------------------------------------
	void NodeVolumetric::onUpdate()
	{
		NodeVisible::onUpdate();

		Camera* pActiveCamera = Engine::getSingleton().getActiveCamera();
		// No camera so return
		if(!pActiveCamera)
			return;

		Vector4 pos(this->getPositionLocal());
		pos.w = 1.0f;
		Matrix4x4 matTransform = getTransform();
		matTransform.transpose();
		pos = matTransform.multiply(pos);

		Vector3 dir = m_pLight->getDirection();
		dir.normalize();

		Vector3 up = Vector3::UNIT_Y;
		Vector3 right = dir * up;
		up = right * dir;

		Vector3 vecPos(pos.x, pos.y, pos.z);

		m_Frustum.setVectors(vecPos,
							 dir,
							 right,
							 up);
		m_Frustum.update();

		Vector3 vecMinBounds(Vector3::INFINITY_VECTOR);
		Vector3 vecMaxBounds(Vector3::NEGATIVE_INFINITY_VECTOR);

		if(m_bHasChanged)
		{
			Vector3 eyePos = m_Frustum.getEyePos();
			Vector3 target = m_Frustum.getEyePos() + m_Frustum.getForward();
			up = m_Frustum.getUp();

			m_matLightView.lookAt(eyePos, target, up);

			m_matFrustumWorld = m_matLightView;
			m_matFrustumWorld.invert();

			m_Frustum.setFar(m_pLight->getRadius().x);
			m_Frustum.update();
			m_matLightProj = m_Frustum.getProjectionMatrix();
			m_Frustum.setFar(m_fFarPlane);
			m_Frustum.update();
		}

		Matrix4x4 matFrustumWorldView = m_matFrustumWorld * pActiveCamera->getViewMatrix();

		// Get view space bounds
		
		m_Frustum.getFrustumPointsLocal(m_ptFrustum);

		matFrustumWorldView.transpose();
		Vector4 vert;
		for(uint i = 0; i < 8; ++i)
		{
			vert = matFrustumWorldView.multiply(m_ptFrustum[i]);

			if(vert.x < vecMinBounds.x)
				vecMinBounds.x = vert.x;
			if(vert.y < vecMinBounds.y)
				vecMinBounds.y = vert.y;
			if(vert.z < vecMinBounds.z)
				vecMinBounds.z = vert.z;

			if(vert.x > vecMaxBounds.x)
				vecMaxBounds.x = vert.x;
			if(vert.y > vecMaxBounds.y)
				vecMaxBounds.y = vert.y;
			if(vert.z > vecMaxBounds.z)
				vecMaxBounds.z = vert.z;
		}


		float zBounds = vecMaxBounds.z - vecMinBounds.z;
		float m_fSamplingDelta = 0.01f;


		uint m_numShellsToDraw = (uint)((zBounds / m_fSamplingDelta) + 0.5f);
		m_numShellsToDraw = min(m_nMaxSlices, m_numShellsToDraw);

		// Compute how many triangles need to actually be drawn
		int m_numTrisToDraw = m_numShellsToDraw * 2;

		m_pVB->setPrimitivesNum(m_numTrisToDraw);

		
		RenderPass& pass = m_pVolumetricMat->getActiveRenderTechnique()->getRenderPass(0);
		Shader* pShader = pass.getVertexShader()->getShader();
		ShaderConstant* pConstant = NULL;

		float fractionOfMaxCells = (float)m_numShellsToDraw / (float)m_nMaxSlices;
		pConstant = pShader->getConstant("fractionOfMaxShells");
		if(pConstant)
			pConstant->setDefaultValue(&fractionOfMaxCells);

		pConstant = pShader->getConstant("minBounds");
		if(pConstant)
			pConstant->setDefaultValue(vecMinBounds.getData());
		pConstant = pShader->getConstant("maxBounds");
		if(pConstant)
			pConstant->setDefaultValue(vecMaxBounds.getData());

		Matrix4x4 matInvView = Engine::getSingleton().getActiveCamera()->getViewMatrix();
		matInvView.invert();		

		Matrix4x4 matViewWorldLight = matInvView * m_matLightView;
		pConstant = pShader->getConstant("matViewWorldLight");
		if(pConstant)
			pConstant->setDefaultValue(matViewWorldLight.getData());

		
		Matrix4x4 matWorldLightProj = m_matLightView * m_matLightProj;

		Matrix4x4 matWorldLightProjScroll1;
		Matrix4x4 matWorldLightProjScroll2;
		Matrix4x4 matScroll1;
		Matrix4x4 matScroll2;
		Matrix4x4 matScaleScroll;

		dword now = Engine::getSingleton().getTimer().getMilliseconds();
		dword diff = now - m_dwPrevTime;
		static float fTime;
		fTime += static_cast<float>(diff) * 0.001f;
		matScroll1.translate(fmodf(0.073f * fTime, 1.0f), -fmodf(0.082f * fTime, 1.0f), 0.5f);
		matScroll2.translate(fmodf(0.063f * fTime, 1.0f), fmodf(0.058f * fTime, 1.0f), 0.5f);

		m_dwPrevTime = now;

		matScaleScroll.scale(0.6f, -0.6f, 0.1f);
		matScroll1 = matScaleScroll * matScroll1;
		matScroll2 = matScaleScroll * matScroll2;
		matWorldLightProjScroll1 = matWorldLightProj * matScroll1;
		matWorldLightProjScroll2 = matWorldLightProj * matScroll2;

		Matrix4x4 matViewWorldLightProjScroll1;
		Matrix4x4 matViewWorldLightProjScroll2;
		matViewWorldLightProjScroll1 = matInvView * matWorldLightProjScroll1;
		matViewWorldLightProjScroll2 = matInvView * matWorldLightProjScroll2;

		pConstant = pShader->getConstant("matViewWorldLightProjScroll1");
		if(pConstant)
			pConstant->setDefaultValue(matViewWorldLightProjScroll1.getData());
		pConstant = pShader->getConstant("matViewWorldLightProjScroll2");
		if(pConstant)
			pConstant->setDefaultValue(matViewWorldLightProjScroll2.getData());
		

		Matrix4x4 matWorldLightProjBias = matWorldLightProj * m_matTexture;
		Matrix4x4 matViewWorldLightProjBias = matInvView * matWorldLightProjBias;
		pConstant = pShader->getConstant("matViewWorldLightProjBiasAdj");
		if(pConstant)
			pConstant->setDefaultValue(matViewWorldLightProjBias.getData());


		wstringstream stName;
		stName << L"shadowDepth" << reinterpret_cast<int>((void*)m_pLight) + m_pLight->getShadowResolution();
		ITexture* pShadowMap = TextureManager::getSingleton().getTexture(stName.str());
		if(pShadowMap)
			pass.getTextureSampler(2).setTexture(pShadowMap);

		// Pixel shader
		pShader = pass.getPixelShader()->getShader();
		pConstant = pShader->getConstant("fractionOfMaxShells");
		if(pConstant)
			pConstant->setDefaultValue(&fractionOfMaxCells);

		pConstant = pShader->getConstant("spotExponent");
		float exponent = m_pLight->getSpotExponent();
		if(pConstant)
			pConstant->setDefaultValue(&exponent);

		pConstant = pShader->getConstant("constantAttenuation");
		float atten = m_pLight->getConstantAttenuation();
		if(pConstant)
			pConstant->setDefaultValue(&atten);

		pConstant = pShader->getConstant("linearAttenuation");
		atten = m_pLight->getLinearAttenuation();
		if(pConstant)
			pConstant->setDefaultValue(&atten);

		pConstant = pShader->getConstant("quadraticAttenuation");
		atten = m_pLight->getQuadraticAttenuation();
		if(pConstant)
			pConstant->setDefaultValue(&atten);

		float fov = m_pLight->getFOV();
		pConstant = pShader->getConstant("fov");
		if(pConstant)
			pConstant->setDefaultValue(&fov);

		// Second pass

		RenderPass& pass2 = m_pVolumetricMat->getActiveRenderTechnique()->getRenderPass(1);
		pShader = pass2.getPixelShader()->getShader();
		pConstant = pShader->getConstant("colour");
		if(pConstant)
		{
			float colour[3] = {m_Colour.getRedF(),
							   m_Colour.getGreenF(),
							   m_Colour.getBlueF()};
			pConstant->setDefaultValue(colour);
		}


		// Update bounding volumes
		Matrix4x4 matInvWorldView = pActiveCamera->getViewMatrix();
		matInvWorldView.invert();
		matInvWorldView.transpose();

		Vector4 vecMax(vecMaxBounds, 1.0f);
		Vector4 vecMin(vecMinBounds, 1.0f);

		vecMax = matInvWorldView.multiply(vecMax);
		vecMin = matInvWorldView.multiply(vecMin);

		vecMaxBounds.set(vecMax.x, vecMax.y, vecMax.z);
		vecMinBounds.set(vecMin.x, vecMin.y, vecMin.z);

		m_OBB.setOBB(vecMinBounds, vecMaxBounds);

		m_vSurfaces[0]->updateBoundingBox();
	}
//----------------------------------------------------------------------
	void NodeVolumetric::beginRender()
	{
		Renderer& renderer = Renderer::getSingleton();

		Matrix4x4 matProj = Engine::getSingleton().getActiveCamera()->getFrustum().getProjectionMatrix();
		Matrix4x4 matView = Engine::getSingleton().getActiveCamera()->getViewMatrix();
		Matrix4x4 matViewProj = matView * matProj;

		matViewProj.invert();
		matViewProj.transpose();

		// Setup clipping planes
		Plane planes[6];
		m_Frustum.getFrustumPlanes(planes);

		for(uint i = 0; i < 6; ++i)
		{
			// Create and transform reflection plane to clipping space
			Plane plane = planes[i];
			plane.transform(matViewProj);
			renderer.setUserClipPlane(i, plane);
		}


		// Enable clipping planes
		renderer.setClippingPlanesFlag(PLANE_0_ENABLED |
									   PLANE_1_ENABLED |
									   PLANE_2_ENABLED |
									   PLANE_3_ENABLED |
									   PLANE_4_ENABLED |
									   PLANE_5_ENABLED);

		Shader* pShader = m_pVolumetricMat->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getShader();
		pShader->setBoolean("isShadow", m_bIsShadowUsed);
		pShader->setBoolean("isCookie", m_bIsCookieUsed);
		pShader->setBoolean("isNoise", m_bIsNoiseUsed);
	}
//----------------------------------------------------------------------
	void NodeVolumetric::endRender()
	{
		Renderer::getSingleton().setClippingPlanesFlag(PLANE_NONE);
	}
//----------------------------------------------------------------------
	void NodeVolumetric::updateAABB()
	{
		Surface* pSurface = m_vSurfaces[0];
		if(pSurface)
		{
			Matrix4x4 mat;
			mat.translate(m_vecPosition);
			m_OBB.setTransform(mat);
			m_AABB.construct(m_OBB);

			pSurface->setOBB(m_OBB);
			pSurface->setAABB(m_AABB);
		}
	}
//----------------------------------------------------------------------
	dword NodeVolumetric::mapShellToColourChannel(uint _shell)
	{
		dword dwChannel = _shell % 4;

		switch(dwChannel)
		{
		case 0: return 0xff000000;
		case 1: return 0x00ff0000;
		case 2: return 0x0000ff00;
		case 3: return 0x000000ff;
		}

		return 0xffffffff;
	}
//----------------------------------------------------------------------
	void NodeVolumetric::setFar(Real _far)
	{
		m_fFarPlane = _far;
		m_Frustum.setPerspective(Maths::PI * 0.5f, 1.0f, 1.0f, m_fFarPlane);
		m_Frustum.update();
	}
//----------------------------------------------------------------------
	void NodeVolumetric::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
		NodeVisible::serialize(_serializer, false, true);

		Property <uint> prSlicesNum(m_nMaxSlices);
		_serializer->addProperty("SlicesNum", prSlicesNum);

		Property <bool> prShadowUsed(m_bIsShadowUsed);
		_serializer->addProperty("UseShadow", prShadowUsed);

		Property <bool> prCookieUsed(m_bIsCookieUsed);
		_serializer->addProperty("UseCookie", prCookieUsed);

		Property <bool> prNoiseUsed(m_bIsNoiseUsed);
		_serializer->addProperty("UseNoise", prNoiseUsed);

		Property <bool> prShowDebug(m_bShowDebug);
		_serializer->addProperty("ShowDebug", prShowDebug);

		Property <Real> prFarPlane(m_fFarPlane);
		_serializer->addProperty("Range", prFarPlane);

		Property <Colour> prColour(m_Colour);
		_serializer->addProperty("Colour", prColour);
		
		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeVolumetric::deserialize(ISerializer* _serializer)
	{
		Property <uint> prSlicesNum(m_nMaxSlices);
		_serializer->getProperty("SlicesNum", prSlicesNum);

		Property <Real> prFarPlane(m_fFarPlane);
		_serializer->getProperty("Range", prFarPlane);

		Property <bool> prShadowUsed(m_bIsShadowUsed);
		_serializer->getProperty("UseShadow", prShadowUsed);

		Property <bool> prCookieUsed(m_bIsCookieUsed);
		_serializer->getProperty("UseCookie", prCookieUsed);

		Property <bool> prNoiseUsed(m_bIsNoiseUsed);
		_serializer->getProperty("UseNoise", prNoiseUsed);

		Property <bool> prShowDebug(m_bShowDebug);
		_serializer->getProperty("ShowDebug", prShowDebug);

		Property <Colour> prColour(m_Colour);
		_serializer->getProperty("Colour", prColour);

		m_pLight = (NodeLight*)_serializer->getParent();

		NodeVisible::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	void NodeVolumetric::setLight(NodeLight* _light)
	{
		m_pLight = _light;
		m_pLight->setIndividualShadow(true);
	}
//----------------------------------------------------------------------
	void NodeVolumetric::setShowDebug(bool _value)
	{
		m_pDebug->setEnabled(_value);
		m_bShowDebug = _value;
	}
//----------------------------------------------------------------------
}
}