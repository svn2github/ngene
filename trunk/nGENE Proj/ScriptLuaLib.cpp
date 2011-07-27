/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScriptLuaLib.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "ScriptLua.h"


#include "AABB.h"
#include "Camera.h"
#include "CameraFirstPerson.h"
#include "CameraThirdPerson.h"
#include "CharacterController.h"
#include "Engine.h"
#include "Frustum.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "Matrix4x4.h"
#include "NodeLight.h"
#include "PrefabBox.h"
#include "Renderer.h"
#include "Singleton.h"


namespace nGENE
{
namespace Script
{
namespace Lua
{
	static ScriptLua* getScriptObject(lua_State* _state);
	static int luaBeep(lua_State* _state);
	static int luaWaitFrame(lua_State* _state);
	static int luaWaitMilis(lua_State* _state);
	static int luaWaitTime(lua_State* _state);

	/// nGENE Lua scripting library
	static const luaL_reg ngeneLib[] =
    {
		{"Beep",         luaBeep},
		{"WaitFrame",    luaWaitFrame},
		{"WaitMilis",    luaWaitMilis},
		{"WaitTime",	 luaWaitTime},
		{NULL, NULL}
    };



	void registerNGENELib(lua_State* _state)
	{
		luaL_openlib(_state, "nGENE", ngeneLib, 0);


		// Register nGENE Tech classes and functions
		luabind::module(_state) [
			luabind::class_<AABB>("AABB")
				.def(luabind::constructor <const Vector3, const Vector3&> ())
				.def("getCentre", &AABB::getCentre)
				.def("getMaximum", &AABB::getMaximum)
				.def("getMinimum", &AABB::getMinimum)
				.def("setAABB", &AABB::setAABB)
				.def("setMaximum", &AABB::setMaximum)
				.def("setMinimum", &AABB::setMinimum),

			luabind::class_<Node>("Node")
				.def("addChild", (void(Node::*)(const wstring&, Node&))&Node::addChild)
				.def("addChildPtr", (void(Node::*)(const wstring&, Node*))&Node::addChild)
				.def("getCreator", &Node::getCreator)
				.def("getPositionLocal", &Node::getPositionLocal)
				.def("getPositionWorld", &Node::getPositionWorld)
				.def("setPositionLocal", (void(Node::*)(Real, Real, Real))&Node::setPosition)
				.def("getRotation", &Node::getRotation)
				.def("getScale", &Node::getScale)
				.def("getTransform", &Node::getTransform)
				.def("getEnabled", &Node::isEnabled)
				.def("setEnabled", &Node::setEnabled)
				.def("setPosition", (void(Node::*)(const Vector3&))&Node::setPosition)
				.def("setPosition", (void(Node::*)(Real, Real, Real))&Node::setPosition)
				.def("setRotation", (void(Node::*)(const Quaternion&))&Node::setRotation)
				.def("setScale", (void(Node::*)(const Vector3&))&Node::setScale)
				.def("setScale", (void(Node::*)(Real, Real, Real))&Node::setScale)
				.def("setScale", (void(Node::*)(Real))&Node::setScale)
				.def("translate", (void(Node::*)(const Vector3&))&Node::setScale)
				.def("translate", (void(Node::*)(Real, Real, Real))&Node::setScale),

			luabind::class_<Camera, Node>("Camera")
				.def("getViewMatrix", &Camera::getViewMatrix)
				.def("getFrustum", &Camera::getFrustumPtr)
				.def("getForward", &Camera::getForward)
				.def("getRight", &Camera::getRight)
				.def("getUp", &Camera::getUp)
				.def("requestUpdate", &Camera::requestUpdate)
				.def("setForward", &Camera::setForward)
				.def("setUp", &Camera::setUp)
				.def("updateCamera", &Camera::updateCamera),

			luabind::class_<Camera3D, Camera>("Camera3D")
				.def("getVelocity", &Camera3D::getVelocity)
				.def("setVelocity", &Camera3D::setVelocity),

			luabind::class_<CameraFirstPerson, Camera3D>("CameraFirstPerson"),

			luabind::class_<CameraThirdPerson, Camera3D>("CameraThirdPerson")
				.def("setTarget", &CameraThirdPerson::setTarget)
				.def("setDistanceFromTarget", &CameraThirdPerson::setDistanceFromTarget),

			luabind::class_<CharacterController>("CharacterController")
				.def("getVelocity", &CharacterController::getVelocity)
				.def("getRotation", &CharacterController::getRotation)
				.def("jump", &CharacterController::jump)
				.def("move", &CharacterController::move)
				.def("setRotation", &CharacterController::setRotation)
				.def("setVelocity", &CharacterController::setVelocity),

			luabind::class_<Colour>("Colour")
				.def(luabind::constructor <byte, byte, byte, byte> ())
				.def(luabind::constructor <dword> ())
				.def("getRed", &Colour::getRed)
				.def("getGreen", &Colour::getGreen)
				.def("getBlue", &Colour::getBlue)
				.def("getAlpha", &Colour::getAlpha)
				.def("getARGB", &Colour::getDwordARGB)
				.def("inverse", &Colour::inverse)
				.def("saturate", &Colour::saturate)
				.def("setRed", &Colour::setRed)
				.def("setGreen", &Colour::setGreen)
				.def("setBlue", &Colour::setBlue)
				.def("setAlpha", &Colour::setAlpha),

			luabind::class_<Engine>("Engine")
				.def("getActiveCamera", &Engine::getActiveCamera)
				.def("getTimer", &Engine::getTimer)
				.def("getRenderer", &Engine::getRenderer)
				.def("getRenderWindowsNum", &Engine::getRenderWindowsNum)
				.def("getSceneManager", &Engine::getSceneManager)
				.def("getSceneManagersNum", &Engine::getSceneManagersNum)
				.def("setActiveCamera", &Engine::setActiveCamera),

			luabind::class_<Frustum>("Frustum")
				.def("getAspect", &Frustum::getAspect)
				.def("getEyePos", &Frustum::getEyePos)
				.def("getNear", &Frustum::getNear)
				.def("getFar", &Frustum::getFar)
				.def("getForward", &Frustum::getForward)
				.def("getFov", &Frustum::getFOV)
				.def("getProjectionMatrix", &Frustum::getProjectionMatrix)
				.def("getRight", &Frustum::getRight)
				.def("getUp", &Frustum::getUp)
				.def("reset", &Frustum::reset)
				.def("setAspect", &Frustum::setAspect)
				.def("setNear", &Frustum::setNear)
				.def("setFar", &Frustum::setFar)
				.def("setFov", &Frustum::setFOV)
				.def("setVectors", &Frustum::setVectors)
				.def("setPerspective", &Frustum::setPerspective)
				.def("testAABB", &Frustum::testAABB)
				.def("testPoint", &Frustum::testPoint)
				.def("testSphere", &Frustum::testSphere),

			luabind::class_<Material>("Material")
				.def("getAmbient", &Material::getAmbient)
				.def("getDiffuse", &Material::getDiffuse)
				.def("getSpecular", &Material::getSpecular)
				.def("getEmissive", &Material::getEmissive)
				.def("getSpecularPower", &Material::getSpecular)
				.def("getZBias", &Material::getZBias)
				.def("getAlphaTestRef", &Material::getAlphaTestReference)
				.def("getSpecularPower", &Material::setSpecular)
				.def("getTwoSided", &Material::isTwoSided)
				.def("getZWrite", &Material::isZWrite)
				.def("getCastShadow", &Material::isCastingShadow)
				.def("getTransparent", &Material::isTransparent)
				.def("getAlphaTest", &Material::isAlphaTest)
				.def("getLightable", &Material::isLightable)
				.def("getEnabled", &Material::isEnabled)
				.def("setZBias", &Material::setZBias)
				.def("setAlphaTestRef", &Material::setAlphaTestReference)
				.def("setTwoSided", &Material::setTwoSided)
				.def("setZWrite", &Material::setZWrite)
				.def("setCastShadow", &Material::setCastShadow)
				.def("setTransparent", &Material::setTransparent)
				.def("setAlphaTest", &Material::setAlphaTest)
				.def("setLightable", &Material::setLightable)
				.def("setEnabled", &Material::setEnabled),

			luabind::class_<MaterialLibrary>("MaterialLibrary")
				.def("getMaterialByName", (Material*(MaterialLibrary::*)(const wstring&))&MaterialLibrary::getMaterial)
				.def("getMaterialByID", (Material*(MaterialLibrary::*)(uint))&MaterialLibrary::getMaterial),
			
			luabind::class_<MaterialManager>("MaterialManager")
				.def("getLibraryByName", (MaterialLibrary*(MaterialManager::*)(const wstring&))&MaterialManager::getLibrary)
				.def("getLibraryByID", (MaterialLibrary*(MaterialManager::*)(uint))&MaterialManager::getLibrary),

			luabind::class_<Matrix4x4>("Matrix4x4")
				.def(luabind::constructor <> ())
				.def("invert", &Matrix4x4::invert)
				.def_readwrite("_11", &Matrix4x4::_m11)
				.def_readwrite("_12", &Matrix4x4::_m12)
				.def_readwrite("_13", &Matrix4x4::_m13)
				.def_readwrite("_14", &Matrix4x4::_m14)
				.def_readwrite("_21", &Matrix4x4::_m21)
				.def_readwrite("_22", &Matrix4x4::_m22)
				.def_readwrite("_23", &Matrix4x4::_m23)
				.def_readwrite("_24", &Matrix4x4::_m24)
				.def_readwrite("_31", &Matrix4x4::_m31)
				.def_readwrite("_32", &Matrix4x4::_m32)
				.def_readwrite("_33", &Matrix4x4::_m33)
				.def_readwrite("_34", &Matrix4x4::_m34)
				.def_readwrite("_41", &Matrix4x4::_m41)
				.def_readwrite("_42", &Matrix4x4::_m42)
				.def_readwrite("_43", &Matrix4x4::_m43)
				.def_readwrite("_44", &Matrix4x4::_m44)
				.def("getTranslation", &Matrix4x4::getTranslation)
				.def("identity", &Matrix4x4::identity)
				.def("isIdentity", &Matrix4x4::isIdentity)
				.def("isZero", &Matrix4x4::isZero)
				.def("lookAt", &Matrix4x4::lookAt)
				.def("negate", &Matrix4x4::negate)
				.def("orthogonal", &Matrix4x4::orthogonal)
				.def("perspective", &Matrix4x4::perspective)
				.def("rotate", &Matrix4x4::rotate)
				.def("rotateAroundXAxis", &Matrix4x4::rotateAroundXAxis)
				.def("rotateAroundYAxis", &Matrix4x4::rotateAroundYAxis)
				.def("rotateAroundZAxis", &Matrix4x4::rotateAroundZAxis)
				.def("rotateYawPitchRoll", (void(Matrix4x4::*)(const Vector3&))&Matrix4x4::rotateYawPitchRoll)
				.def("rotateYawPitchRoll", (void(Matrix4x4::*)(Real, Real, Real))&Matrix4x4::rotateYawPitchRoll)
				.def("scale", (void(Matrix4x4::*)(Real, Real, Real))&Matrix4x4::scale)
				.def("scale", (void(Matrix4x4::*)(Real))&Matrix4x4::scale)
				.def("scale", (void(Matrix4x4::*)(const Vector3&))&Matrix4x4::scale)
				.def("setTranslation", (void(Matrix4x4::*)(Real, Real, Real))&Matrix4x4::setTranslation)
				.def("setTranslation", (void(Matrix4x4::*)(const Vector3&))&Matrix4x4::setTranslation)
				.def("subtract", &Matrix4x4::subtract)
				.def("translate", (void(Matrix4x4::*)(Real, Real, Real))&Matrix4x4::translate)
				.def("translate", (void(Matrix4x4::*)(const Vector3&))&Matrix4x4::translate)
				.def("transpose", &Matrix4x4::transpose)
				.def("zero", &Matrix4x4::zero)
				.def(luabind::const_self * luabind::other<Matrix4x4>())
				.def(luabind::const_self + luabind::other<Matrix4x4>()),

			luabind::class_<NodeLight, Node>("NodeLight")
				.def("getDirection", &NodeLight::getDirection)
				.def("getColour", &NodeLight::getColour)
				.def("getIntensity", &NodeLight::getIntensity)
				.def("getRadius", &NodeLight::getRadius)
				.def("getFov", &NodeLight::getFOV)
				.def("getSpotExponent", &NodeLight::getSpotExponent),

			luabind::class_<PrefabBox>("PrefabBox")
				.def(luabind::constructor<AABB&, const Vector2&> ())
				.def(luabind::constructor<> ()),

			luabind::class_<Quaternion>("Quaternion")
				.def(luabind::constructor <Vector3, Real> ())
				.def(luabind::self * luabind::other<Quaternion>())
				.def("conjugate", &Quaternion::conjugate)
				.def("identity", &Quaternion::identity)
				.def("invert", &Quaternion::invert)
				.def("getLength", &Quaternion::getLength)
				.def("normalize", &Quaternion::normalize)
				.def("toMatrix", &Quaternion::toMatrix),

			luabind::class_<Renderer>("Rendererer")
				.def("getActiveLight", &Renderer::getActiveLight)
				.def("getWorldMatrix", &Renderer::getWorldMatrix)
				.def("getViewMatrix", &Renderer::getViewMatrix)
				.def("getProjectionMatrix", &Renderer::getProjectionMatrix)
				.def("getActiveMaterial", &Renderer::getActiveMaterial),

			luabind::class_<SceneManager>("SceneManager")
				.def("createBox", (PrefabBox* (SceneManager::*)(Real, Real, Real, const Vector2&))&SceneManager::createBox, luabind::adopt(luabind::result))
				.def("createBox", (PrefabBox* (SceneManager::*)(Vector3&, const Vector2&))&SceneManager::createBox, luabind::adopt(luabind::result)),

			luabind::class_<Timer>("Timer")
				.def("getMiliseconds", &Timer::getMilliseconds),

			luabind::class_<Vector2>("Vector2")
				.def(luabind::constructor <Real, Real> ())
				.def_readwrite("x", &Vector2::x)
				.def_readwrite("y", &Vector2::y),

			luabind::class_<Vector3>("Vector3")
				.def(luabind::constructor <Real, Real, Real> ())
				.def_readwrite("x", &Vector3::x)
				.def_readwrite("y", &Vector3::y)
				.def_readwrite("z", &Vector3::z),

			luabind::class_<Vector4>("Vector4")
				.def(luabind::constructor <Real, Real, Real, Real> ())
				.def_readwrite("x", &Vector4::x)
				.def_readwrite("y", &Vector4::y)
				.def_readwrite("z", &Vector4::z)
				.def_readwrite("w", &Vector4::w),

			luabind::def("getEngine", &Engine::getSingletonPtr),
			luabind::def("getRenderer", &Renderer::getSingletonPtr),
			luabind::def("getMaterialManager", &MaterialManager::getSingletonPtr)
		];
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	static int luaBeep(lua_State* _state)
    {
		Beep(static_cast<dword>(luaL_checknumber(_state, 1)),
			 static_cast<dword>(luaL_checknumber(_state, 2)));

		return 0;
    }
//----------------------------------------------------------------------
	static int luaWaitFrame(lua_State* _state)
    {
		ScriptLua* pScript = getScriptObject(_state);

		pScript->setWaitFrames(luaL_checknumber(_state, 1));
		
		return (lua_yield(_state, 1));
    }
//----------------------------------------------------------------------
	static int luaWaitTime(lua_State* _state)
    {
		ScriptLua* pScript = getScriptObject(_state);

		pScript->setWaitTime(static_cast<float>(luaL_checknumber(_state, 1)));
		
		return (lua_yield(_state, 1));
    }
//----------------------------------------------------------------------
	static int luaWaitMilis(lua_State* _state)
    {
		ScriptLua* pScript = getScriptObject(_state);

		uint timeStamp = luaL_checknumber(_state, 1) + pScript->getTime();
		pScript->setWaitTime(static_cast<float>(timeStamp));
		
		return (lua_yield(_state, 1));
    }
//----------------------------------------------------------------------
	static ScriptLua* getScriptObject(lua_State* _state)
    {
		lua_pushlightuserdata(_state, _state);
		lua_gettable(_state, LUA_GLOBALSINDEX);

		return (static_cast<ScriptLua*>(lua_touserdata(_state, -1)));
    }
//----------------------------------------------------------------------
}
}
}