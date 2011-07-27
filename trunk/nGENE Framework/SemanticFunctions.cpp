/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SemanticFunctions.cpp
Version:	0.07
Info:		Default semantic functions definitions.
---------------------------------------------------------------------------
*/

#include "Camera.h"
#include "Frustum.h"
#include "LightStage.h"
#include "Material.h"
#include "NodeLight.h"
#include "Renderer.h"
#include "SemanticFunctions.h"
#include "Timer.h"


namespace nGENE
{
namespace Application
{
	void worldViewProj(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		Matrix4x4 matWorld= pRenderer->getWorldMatrix();
		Matrix4x4 matView = pRenderer->getViewMatrix();
		Matrix4x4 matProj = pRenderer->getProjectionMatrix();

		Matrix4x4 matWVP = matWorld * matView * matProj;
		memcpy(_values, &matWVP, _count);
	}
//----------------------------------------------------------------------
	void viewProj(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		Matrix4x4 matView = pRenderer->getViewMatrix();
		Matrix4x4 matProj = pRenderer->getProjectionMatrix();

		Matrix4x4 matVP = matView * matProj;
		memcpy(_values, &matVP, _count);
	}
//----------------------------------------------------------------------
	void worldView(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		Matrix4x4 matWorld = pRenderer->getWorldMatrix();
		Matrix4x4 matView = pRenderer->getViewMatrix();

		Matrix4x4 matWV = matWorld * matView;
		memcpy(_values, &matWV, _count);
	}
//----------------------------------------------------------------------
	void viewInverse(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();
		Matrix4x4 matViewInverse = pRenderer->getViewMatrix();
		matViewInverse.invert();

		memcpy(_values, &matViewInverse, _count);
	}
//----------------------------------------------------------------------
	void viewInverseTranspose(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();
		Matrix4x4 matViewIT = pRenderer->getViewMatrix();
		matViewIT.invert();
		matViewIT.transpose();

		memcpy(_values, &matViewIT, _count);
	}
//----------------------------------------------------------------------
	void world(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		Matrix4x4 matWorld = pRenderer->getWorldMatrix();
		memcpy(_values, &matWorld, _count);
	}
//----------------------------------------------------------------------
	void view(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		Matrix4x4 matView = pRenderer->getViewMatrix();
		memcpy(_values, &matView, _count);
	}
//----------------------------------------------------------------------
	void proj(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		Matrix4x4 matProj = pRenderer->getProjectionMatrix();
		memcpy(_values, &matProj, _count);
	}
//----------------------------------------------------------------------
	void projInverse(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		Matrix4x4 matProj = pRenderer->getProjectionMatrix();
		matProj.invert();
		memcpy(_values, &matProj, _count);
	}
//----------------------------------------------------------------------
	void camera(void* _values, uint _count)
	{
		Camera* pCamera = Engine::getSingleton().getActiveCamera();
		if(!pCamera)
			return;

		Matrix4x4 matCamera = pCamera->getViewMatrix();
		memcpy(_values, &matCamera, _count);
	}
//----------------------------------------------------------------------
	void cameraInverse(void* _values, uint _count)
	{
		Camera* pCamera = Engine::getSingleton().getActiveCamera();
		if(!pCamera)
			return;

		Matrix4x4 matCameraInverse = pCamera->getViewMatrix();
		matCameraInverse.invert();

		memcpy(_values, &matCameraInverse, _count);
	}
//----------------------------------------------------------------------
	void eyePosition(void* _values, uint _count)
	{
		Camera* pCamera = Engine::getSingleton().getActiveCamera();

		if(pCamera)
		{
			Vector3 vecEyePosition = pCamera->getPositionWorld();
			memcpy(_values, &vecEyePosition, _count);
		}
	}
//----------------------------------------------------------------------
	void nearPlane(void* _values, uint _count)
	{
		Camera* pCamera = Engine::getSingleton().getActiveCamera();

		if(pCamera)
		{
			float nearPlane = pCamera->getFrustum().getNear();
			memcpy(_values, &nearPlane, _count);
		}
		else
		{
			float nearPlane = 1.0f;
			memcpy(_values, &nearPlane, _count);
		}
	}
//----------------------------------------------------------------------
	void farPlane(void* _values, uint _count)
	{
		Camera* pCamera = Engine::getSingleton().getActiveCamera();

		if(pCamera)
		{
			float farPlane = pCamera->getFrustum().getFar();
			memcpy(_values, &farPlane, _count);
		}
		{
			float farPlane = 100.0f;
			memcpy(_values, &farPlane, _count);
		}
	}
//----------------------------------------------------------------------
	void lightDirection(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		Vector3 vecLightDir;
		if(pRenderer->getActiveLight())
			vecLightDir = pRenderer->getActiveLight()->getDirection();
		memcpy(_values, &vecLightDir, _count);
	}
//----------------------------------------------------------------------
	void lightColour(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		Colour diffuse;
		if(pRenderer->getActiveLight())
			diffuse = pRenderer->getActiveLight()->getColour();
		float col[] = {(float)diffuse.getRed() / 255.0f,
					   (float)diffuse.getGreen() / 255.0f,
					   (float)diffuse.getBlue() / 255.0f,
					   1.0f};
		memcpy(_values, col, _count);
	}
//----------------------------------------------------------------------
	void lightPosition(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		Vector3 vecPosition;
		if(pRenderer->getActiveLight())
			vecPosition = pRenderer->getActiveLight()->getPositionLocal();
		memcpy(_values, &vecPosition, _count);
	}
//----------------------------------------------------------------------
	void lightIntensity(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();
		
		float fIntensity;
		if(pRenderer->getActiveLight())
			fIntensity = pRenderer->getActiveLight()->getIntensity();
		memcpy(_values, &fIntensity, _count);
	}
//----------------------------------------------------------------------
	void lightRadius(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();
		
		Vector3 vecRadius;
		if(pRenderer->getActiveLight())
			vecRadius = pRenderer->getActiveLight()->getRadius();
		memcpy(_values, &vecRadius, _count);
	}
//----------------------------------------------------------------------
	void lightFov(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		float fov;
		if(pRenderer->getActiveLight())
			fov = pRenderer->getActiveLight()->getFOV();
		memcpy(_values, &fov, _count);
	}
//----------------------------------------------------------------------
	void lightSpotExponent(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		float exponent;
		if(pRenderer->getActiveLight())
			exponent = pRenderer->getActiveLight()->getSpotExponent();
		memcpy(_values, &exponent, _count);
	}
//----------------------------------------------------------------------
	void lightConstantAttenuation(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		float attenuation;
		if(pRenderer->getActiveLight())
			attenuation = pRenderer->getActiveLight()->getConstantAttenuation();
		memcpy(_values, &attenuation, _count);
	}
//----------------------------------------------------------------------
	void lightLinearAttenuation(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		float attenuation;
		if(pRenderer->getActiveLight())
			attenuation = pRenderer->getActiveLight()->getLinearAttenuation();
		memcpy(_values, &attenuation, _count);
	}
//----------------------------------------------------------------------
	void lightQuadraticAttenuation(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		float attenuation;
		if(pRenderer->getActiveLight())
			attenuation = pRenderer->getActiveLight()->getQuadraticAttenuation();
		memcpy(_values, &attenuation, _count);
	}
//----------------------------------------------------------------------
	void shadowDensity(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();

		float density;
		if(pRenderer->getActiveLight())
			density = pRenderer->getActiveLight()->getShadowDensity();
		memcpy(_values, &density, _count);
	}
//----------------------------------------------------------------------
	void invTanHalfFov(void* _values, uint _count)
	{
		float result = 1.0f;
		if(Engine::getSingleton().hasActiveCamera())
		{
			Camera* pCamera = Engine::getSingleton().getActiveCamera();
			result = 1.0f / tanf(pCamera->getFrustum().getFOV() * 0.5f);
		}
		memcpy(_values, &result, _count);
	}
//----------------------------------------------------------------------
	void aspect(void* _values, uint _count)
	{
		float aspect = 1.0f;
		if(Engine::getSingleton().hasActiveCamera())
		{
			Camera* pCam = Engine::getSingleton().getActiveCamera();
			aspect = pCam->getFrustum().getAspect();
		}
		else
		{
			Renderer* pRenderer = Renderer::getSingletonPtr();
			const SDisplayMode& mode = pRenderer->getDisplayMode();
			aspect = static_cast<float>(mode.width) / static_cast<float>(mode.height);
		}

		memcpy(_values, &aspect, _count);
	}
//----------------------------------------------------------------------
	void pixelSize(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();
		const SDisplayMode& mode = pRenderer->getDisplayMode();

		Vector2 size(1.0f / static_cast<float>(mode.width),
					 1.0f / static_cast<float>(mode.height));
		memcpy(_values, &size.x, _count);
	}
//----------------------------------------------------------------------
	void ambient(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();
		Material* pMaterial = pRenderer->getActiveMaterial();
		Colour& clr = pMaterial->getAmbient();

		float ambient[4] = {clr.getRedF(),
							clr.getGreenF(),
							clr.getBlueF(),
							clr.getAlphaF()};

		memcpy(_values, ambient, _count);
	}
//----------------------------------------------------------------------
	void diffuse(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();
		Material* pMaterial = pRenderer->getActiveMaterial();
		Colour& clr = pMaterial->getDiffuse();

		float diffuse[4] = {clr.getRedF(),
							clr.getGreenF(),
							clr.getBlueF(),
							clr.getAlphaF()};

		memcpy(_values, diffuse, _count);
	}
//----------------------------------------------------------------------
	void emissive(void* _values, uint _count)
	{
		Renderer* pRenderer = Renderer::getSingletonPtr();
		Material* pMaterial = pRenderer->getActiveMaterial();

		// Use emission only for HDR
		LightStage* pStage = (LightStage*)pRenderer->getRenderStage(L"Light");
		float emissiveModifier = 0.0f;
		if(pStage)
		{
			if(pStage->isEnabled())
				emissiveModifier = pStage->isUsingHDR() ? 1.0f : 0.0f;
		}
		float emissive = pMaterial->getEmissive() * emissiveModifier;

		memcpy(_values, &emissive, _count);
	}
//----------------------------------------------------------------------
	void time(void* _values, uint _count)
	{
		Timer& timer = Engine::getSingleton().getTimer();
		
		float time = (float)timer.getMilliseconds();
		memcpy(_values, &time, _count);
	}
//----------------------------------------------------------------------
}
}