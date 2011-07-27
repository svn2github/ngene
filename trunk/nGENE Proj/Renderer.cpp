/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Renderer.cpp
Version:	0.25
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Renderer.h"
#include "Camera.h"
#include "DeviceRender.h"
#include "Frustum.h"
#include "Material.h"
#include "MaterialManager.h"
#include "NodeLight.h"
#include "RenderPass.h"
#include "RenderStage.h"
#include "ShaderManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	class RenderStagePointerComparator
	{
	public:
		bool operator()(const RenderStage* lhs, const RenderStage* rhs)
		{
			return (lhs->getPriority() < rhs->getPriority());
		}
	};

	class NodeLightPointerComparator
	{
	public:
		bool operator()(const NodeLight* lhs, const NodeLight* rhs)
		{
			return (lhs->getShadowResolution() < rhs->getShadowResolution());
		}
	};


	// Initialize static members
	TypeInfo Renderer::Type(L"Renderer", &BaseClass::Type);


	Renderer::Renderer(bool _multiThreaded):
		m_ClearColour(Colour::COLOUR_BLACK),
		m_pStateManager(new StateManager()),
		m_pVBManager(new VertexBufferManager()),
		m_pShaderManager(new ShaderManager()),
		m_pMaterialManager(new MaterialManager()),
		m_pTextureManager(new TextureManager()),
		m_pActiveLight(NULL),
		m_pActiveMaterial(NULL),
		m_matWorldOld(new Matrix4x4()),
		m_matViewOld(new Matrix4x4()),
		m_matProjOld(new Matrix4x4()),
		m_nClippingFlagOld(0),
		m_bProfile(false),
		m_bMultiThreaded(_multiThreaded),
		onRender(NULL)
	{
	}
//----------------------------------------------------------------------
	Renderer::~Renderer()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Renderer::init()
	{
	}
//----------------------------------------------------------------------
	void Renderer::set2DMode()
	{
		// Do nothing if already in 2D mode
		if(m_DisplayMode.mode == RM_2D)
			return;

		*m_matViewOld  = getViewMatrix();
		*m_matProjOld  = getProjectionMatrix();
		*m_matWorldOld = getWorldMatrix();

		m_nClippingFlagOld = getClippingPlanesFlag();
		if(m_nClippingFlagOld)
			setClippingPlanesFlag(0);

		Matrix4x4 newProjection;
		float fWidth = getDisplayMode().width;
		float fHeight = getDisplayMode().height;
		newProjection.orthogonal(fWidth, fHeight, 0.1f, 1000.0f);
		setProjectionMatrix(newProjection);

		Matrix4x4 newView;
		float tx, ty, tz;
		tx = -fWidth + fWidth * 0.5f;
		ty = fHeight - fHeight * 0.5f;
		tz = 0.1f + 0.1f;

		newView._m22 = -1.0f;
		newView._m41 = tx;
		newView._m42 = ty;
		newView._m43 = tz;
		setViewMatrix(newView);

		m_DisplayMode.mode = RM_2D;
	}
//----------------------------------------------------------------------
	void Renderer::restore3DMode()
	{
		// Do nothing if already in 3D mode
		if(m_DisplayMode.mode == RM_3D)
			return;

		setWorldMatrix(*m_matWorldOld);
		setViewMatrix(*m_matViewOld);
		setProjectionMatrix(*m_matProjOld);

		if(m_nClippingFlagOld)
			setClippingPlanesFlag(m_nClippingFlagOld);

		m_DisplayMode.mode = RM_3D;
	}
//----------------------------------------------------------------------
	void Renderer::setClearColour(dword colour)
	{
		m_ClearColour.setARGB(colour);
	}
//----------------------------------------------------------------------
	void Renderer::setClearColour(byte r, byte g, byte b)
	{
		m_ClearColour.setRGBA(r, g, b);
	}
//----------------------------------------------------------------------
	void Renderer::setClearColour(const Colour& colour)
	{
		m_ClearColour = colour;
	}
//----------------------------------------------------------------------
	void Renderer::cleanup()
	{
		NGENE_DELETE(m_matWorldOld);
		NGENE_DELETE(m_matViewOld);
		NGENE_DELETE(m_matProjOld);
	}
//----------------------------------------------------------------------
	void Renderer::beginRender()
	{
		FrameEvent evt;
		evt.isFrameBegin = true;
		ListenerRegistry <FrameEvent>::handleEvent(evt);

		for(uint i = 0; i < m_vRenderStages.size(); ++i)
		{
			if(m_vRenderStages[i]->isEnabled())
				m_vRenderStages[i]->beginRender();
		}
	}
//----------------------------------------------------------------------
	void Renderer::endRender()
	{
		FrameEvent evt;
		evt.isFrameBegin = false;
		ListenerRegistry <FrameEvent>::handleEvent(evt);

		for(uint i = 0; i < m_vRenderStages.size(); ++i)
		{
			if(m_vRenderStages[i]->isEnabled())
				m_vRenderStages[i]->endRender();
		}
	}
//----------------------------------------------------------------------
	void Renderer::addFrameListener(FrameListener* listener)
	{
		// Add listener to the registry so it
		// could handle upcoming messages.
		AutoPointer <FrameListener> Listener(listener);
		ListenerRegistry <FrameEvent>::addListener(Listener.getPointer());

		m_vFrameListeners.push_back(Listener);
	}
//----------------------------------------------------------------------
	Window* Renderer::getWindow(uint _index) const
	{
		if(_index > m_pWindow.size() - 1)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Window index: %d is too big. Maximum is: %d", _index, m_pWindow.size() - 1);
			_index = m_pWindow.size() - 1;
		}

		return (m_pWindow[_index]);
	}
//----------------------------------------------------------------------
	void Renderer::setMaterial(Material& _material)
	{
		m_pActiveMaterial = &_material;
	}
//----------------------------------------------------------------------
	void Renderer::setTextureFilter(uint _sampler, dword _type, TEXTURE_FILTER _filter)
	{
		if(_type & FILTER_MIN)
			m_DisplayMode.filterMin[_sampler] = _filter;
		if(_type & FILTER_MAG)
			m_DisplayMode.filterMag[_sampler] = _filter;
		if(_type & FILTER_MIP)
			m_DisplayMode.filterMip[_sampler] = _filter;
	}
//----------------------------------------------------------------------
	void Renderer::setTextureAddressingMode(uint _sampler, TEXTURE_ADDRESSING_MODE _mode)
	{
		m_DisplayMode.addressing[_sampler] = _mode;
	}
//----------------------------------------------------------------------
	TEXTURE_ADDRESSING_MODE Renderer::getTextureAddressingMode(uint _sampler) const
	{
		return m_DisplayMode.addressing[_sampler];
	}
//----------------------------------------------------------------------
	TEXTURE_FILTER Renderer::getTextureFilter(uint _sampler, FILTER_TYPE _type) const
	{
		switch(_type)
		{
		case FILTER_MIN: return m_DisplayMode.filterMin[_sampler];
		case FILTER_MAG: return m_DisplayMode.filterMag[_sampler];
		case FILTER_MIP: return m_DisplayMode.filterMip[_sampler];
		default: return static_cast<TEXTURE_FILTER>(0);
		}
	}
//----------------------------------------------------------------------
	void Renderer::setAnisotropyLevel(uint _sampler, uint _anisotropy)
	{
		m_DisplayMode.anisotropyLevel[_sampler] = _anisotropy;
	}
//----------------------------------------------------------------------
	void Renderer::setScissorTestEnabled(bool _value)
	{
		if(!isGlobalState(GS_SCISSORTEST))
			m_DisplayMode.state.scissorTest = _value;
	}
//----------------------------------------------------------------------
	void Renderer::setScissorRectangle(SRect <uint>& _rect)
	{
		m_DisplayMode.scissorRectangle = _rect;
	}
//----------------------------------------------------------------------
	void Renderer::setClippingPlanesFlag(uint _flag)
	{
		m_DisplayMode.clippingPlanesFlag = _flag;
	}
//----------------------------------------------------------------------
	void Renderer::addLight(NodeLight* _light)
	{
		m_vLights.push_back(_light);
		sortLights();
	}
//----------------------------------------------------------------------
	void Renderer::removeLight(NodeLight* _light)
	{
		uint i;
		for(i = 0; i < m_vLights.size(); ++i)
		{
			if(m_vLights[i] == _light)
				break;
		}

		if(i != m_vLights.size())
		{
			m_vLights[i] = NULL;
			m_vLights.erase(m_vLights.begin() + i);
		}
	}
//----------------------------------------------------------------------
	void Renderer::removeLights()
	{
		m_vLights.clear();
	}
//----------------------------------------------------------------------
	void Renderer::resetRenderStates()
	{
		setAlphaBlendEnabled(m_DisplayMode.state.alphaBlend);
		setLightEnabled(m_DisplayMode.state.light);
		setCullingMode(m_DisplayMode.state.culling);
		setFillMode(m_DisplayMode.state.fillMode);
		setSourceBlending(m_DisplayMode.state.source);
		setDestinationBlending(m_DisplayMode.state.destination);
		setZWriteEnabled(m_DisplayMode.state.zWrite);
		setScissorTestEnabled(m_DisplayMode.state.scissorTest);
		setSeparateAlphaBlendEnabled(m_DisplayMode.state.separateAlphaBlend);
		setSeparateAlphaSourceBlending(m_DisplayMode.state.alphaBlendSource);
		setSeparateAlphaDestinationBlending(m_DisplayMode.state.alphaBlendDestination);

		uint nAnisotropy = 0;
		getDeviceRender().testCapabilities(CAPS_MAX_ANISOTROPY, &nAnisotropy);
		for(uint i = 0; i < 8; ++i)
		{
			setTextureFilter(i, FILTER_MIN, m_DisplayMode.filterMin[i]);
			setTextureFilter(i, FILTER_MAG, m_DisplayMode.filterMag[i]);
			setTextureFilter(i, FILTER_MIP, m_DisplayMode.filterMip[i]);

			setTextureAddressingMode(i, m_DisplayMode.addressing[i]);
			
			setAnisotropyLevel(i, m_DisplayMode.anisotropyLevel[i]);

			setTextureAddressingMode(i, m_DisplayMode.addressing[i]);
		}
	}
//----------------------------------------------------------------------
	void Renderer::addRenderStage(const wstring& _name, RenderStage* _stage)
	{
		if(m_RenderStages.find(_name) != m_RenderStages.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Render stage with the name: %ls already exists", _name.c_str());
			return;
		}

		_stage->setRenderer(this);

		m_RenderStages[_name] = _stage;

		m_vRenderStages.push_back(_stage);
		sortStages();
	}
//----------------------------------------------------------------------
	void Renderer::removeRenderStage(const wstring& _name)
	{
		if(m_RenderStages.find(_name) == m_RenderStages.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Render stage with the name: %ls does not exist", _name.c_str());
			return;
		}

		RenderStage* pStage = m_RenderStages[_name];
		uint i = 0;
		for(i = 0; i < m_vRenderStages.size(); ++i)
		{
			if(m_vRenderStages[i] == pStage)
				break;
		}
		if(i != m_vRenderStages.size())
			m_vRenderStages.erase(m_vRenderStages.begin() + i);

		m_RenderStages.erase(_name);
	}
//----------------------------------------------------------------------
	RenderStage* Renderer::getRenderStage(uint _index) const
	{
		if(_index >= m_vRenderStages.size())
			return NULL;

		return m_vRenderStages[_index];
	}
//----------------------------------------------------------------------
	RenderStage* Renderer::getRenderStage(const wstring& _name)
	{
		if(m_RenderStages.find(_name) == m_RenderStages.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Render stage with the name: %ls doesn't exist", _name.c_str());
			return NULL;
		}

		return m_RenderStages[_name];
	}
//----------------------------------------------------------------------
	int Renderer::getRenderStageIndex(const RenderStage* _stage)
	{
		for(uint i = 0; i < m_vRenderStages.size(); ++i)
		{
			if(m_vRenderStages[i] == _stage)
				return i;
		}

		Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Render stage doesn't exist");

		return -1;
	}
//----------------------------------------------------------------------
	uint Renderer::getRenderStagesNum() const
	{
		return m_vRenderStages.size();
	}
//----------------------------------------------------------------------
	void Renderer::sortStages()
	{
		RenderStagePointerComparator comparator;
		std::sort(m_vRenderStages.begin(), m_vRenderStages.end(), comparator);
	}
//----------------------------------------------------------------------
	void Renderer::sortLights()
	{
		NodeLightPointerComparator comparator;
		std::sort(m_vLights.begin(), m_vLights.end(), comparator);
	}
//----------------------------------------------------------------------
	void Renderer::setLightScissor(NodeLight& _light)
	{
		float fRadius = _light.getRadius().x;
		float fRadiusSq = fRadius * fRadius;

		SRect <uint> rect(0, 0, m_DisplayMode.width, m_DisplayMode.height);

		Vector4 l = Vector4(_light.getPositionLocal());
		l.w = 1.0f;
		Matrix4x4 matView = Engine::getSingleton().getActiveCamera()->getViewMatrix();
		matView.transpose();

		l = matView * l;

		Vector3 lightPos = _light.getPositionLocal();
		lightPos = Vector3(l.x, -l.y, l.z);
		Vector3 vecLightPosSq = Vector3(lightPos.x * lightPos.x,
										lightPos.y * lightPos.y,
										lightPos.z * lightPos.z);

		float distance = 1.0f;
		if(Engine::getSingleton().getActiveCamera())
		{
			Frustum& frustum = Engine::getSingleton().getActiveCamera()->getFrustum();
			distance = 1.0f / (Maths::ftan(frustum.getFOV() * 0.5f));
		}

		//distance = 1;
		float D = fRadiusSq * vecLightPosSq.x - (vecLightPosSq.x + vecLightPosSq.z) * (fRadiusSq - vecLightPosSq.z);
		if(D > 0.0f)
		{
			float DSqrt = Maths::fsqrt(D);

			float nx1 = (fRadius * lightPos.x + DSqrt) / (vecLightPosSq.x + vecLightPosSq.z);
			float nx2 = (fRadius * lightPos.x - DSqrt) / (vecLightPosSq.x + vecLightPosSq.z);

			float nz1 = (fRadius - nx1 * lightPos.x) / lightPos.z;
			float nz2 = (fRadius - nx2 * lightPos.x) / lightPos.z;

			float pz1 = (vecLightPosSq.x + vecLightPosSq.z - fRadiusSq) / (lightPos.z - (nz1 / nx1) * lightPos.x);
			float pz2 = (vecLightPosSq.x + vecLightPosSq.z - fRadiusSq) / (lightPos.z - (nz2 / nx2) * lightPos.x);

			float e = distance;
			if(pz1 < 0.0f)
			{
				float x = (nz1 * e) / nx1;
				int xView = FastFloat::floatToInt(((x + 1.0f) * m_DisplayMode.width) * 0.5f);

				float px = -(pz1 * nz1) / nx1;

				if(px < lightPos.x)
					rect.left = max(xView, 0);
				else if(xView > 0 && px > lightPos.x)
					rect.right = min(xView, (int)m_DisplayMode.width);
			}

			if(pz2 < 0.0f)
			{
				float x = (nz2 * e) / nx2;
				int xView = FastFloat::floatToInt(((x + 1.0f) * m_DisplayMode.width) * 0.5f);

				float px = -(pz2 * nz2) / nx2;

				if(px < lightPos.x)
					rect.left = max(xView, 0);
				else if(xView > 0 && px > lightPos.x)
					rect.right = min(xView, (int)m_DisplayMode.width);
			}
		}

		D = fRadiusSq * vecLightPosSq.y - (vecLightPosSq.y + vecLightPosSq.z) * (fRadiusSq - vecLightPosSq.z);
		if(D > 0.0f)
		{
			float DSqrt = Maths::fsqrt(D);

			float ny1 = (fRadius * lightPos.y + DSqrt) / (vecLightPosSq.y + vecLightPosSq.z);
			float ny2 = (fRadius * lightPos.y - DSqrt) / (vecLightPosSq.y + vecLightPosSq.z);

			float nz1 = (fRadius - ny1 * lightPos.y) / lightPos.z;
			float nz2 = (fRadius - ny2 * lightPos.y) / lightPos.z;

			float pz1 = (vecLightPosSq.y + vecLightPosSq.z - fRadiusSq) / (lightPos.z - (nz1 / ny1) * lightPos.y);
			float pz2 = (vecLightPosSq.y + vecLightPosSq.z - fRadiusSq) / (lightPos.z - (nz2 / ny2) * lightPos.y);

			float e = distance * ((float)m_DisplayMode.width / (float)m_DisplayMode.height);
			if(pz1 < 0.0f)
			{
				float y = (nz1 * e) / ny1;
				int yView = FastFloat::floatToInt(((y + 1.0f) * m_DisplayMode.height) * 0.5f);

				float py = -(pz1 * nz1) / ny1;

				if(py < lightPos.y)
					rect.top = max(yView, 0);
				else if(yView > 0 && py > lightPos.y)
					rect.bottom = min(yView, (int)m_DisplayMode.height);
			}

			if(pz2 < 0.0f)
			{
				float y = (nz2 * e) / ny2;
				int yView = FastFloat::floatToInt(((y + 1.0f) * m_DisplayMode.height) * 0.5f);

				float py = -(pz2 * nz2) / ny2;

				if(py < lightPos.y)
					rect.top = max(yView, 0);
				else if(yView > 0 && py > lightPos.y)
					rect.bottom = min(yView, (int)m_DisplayMode.height);
			}
		}

		int square = (rect.bottom - rect.top) * (rect.right - rect.left);
		if(square < (int)(m_DisplayMode.height * m_DisplayMode.width) && square > 0)
		{
			if(rect.left > rect.right || rect.top > rect.bottom)
				return;

			if(!isScissorTestEnabled())
				setScissorTestEnabled(true);
			setScissorRectangle(rect);
		}
		else
		{
			if(isScissorTestEnabled())
				setScissorTestEnabled(false);
		}
	}
//----------------------------------------------------------------------
	void Renderer::setLightEnabled(bool _value)
	{
		m_DisplayMode.state.light=_value;
		for(uint i = 0; i < m_vLights.size(); ++i)
			m_vLights[i]->setDrawDebug(_value);
	}
//----------------------------------------------------------------------
	void Renderer::setProfile(bool _value)
	{
		m_bProfile = _value;
	}
//----------------------------------------------------------------------
	AutoPointer <FrameListener> Renderer::getFrameListener(uint _index) const
	{
		Assert(_index < m_vFrameListeners.size(), "Frame listener index out of bounds");

		return m_vFrameListeners[_index];
	}
//----------------------------------------------------------------------
}