/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Prerequisities.h
Version:	0.41
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREREQUISITIES_H_
#define __INC_PREREQUISITIES_H_


#include "Assertion.h"
#include "Export.h"
#include "Macros.h"


// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)


#pragma warning (disable: 4786)

// disable: "assignment within conditional expression"
#pragma warning (disable: 4706)

// disable: unreachable code
#pragma warning (disable: 4702)


// disable: "C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc"	
#pragma warning (disable: 4530)
#pragma warning (disable: 4503)

// disable: "truncation from 'double' to 'float'
#pragma warning (disable: 4305)

// disable: "non dll-interface class <type> used as base for dll-interface struct <type2>"
#pragma warning (disable: 4275)

// disable: "conversion from 'size_t' to 'int', possible loss of data"
#pragma warning (disable: 4267)

// disable: "<type>" needs to have dll-interface to be used by clients"
// Occurs if STL object is member non-public variable.
#pragma warning (disable: 4251)

// disable: "conversion from 'double' to 'float', possible loss of data"
#pragma warning (disable: 4244)

// disable: "nonstandard extension used : nameless struct/union"
#pragma warning (disable: 4201)

// disable: "deletion of pointer to incomplete type <type>; no destructor called"
// This warning arises when using AutoPointers.
#pragma warning (disable: 4150)

// disable: "conditional expression is constant"
#pragma warning (disable: 4127)

// disable: "unreferenced formal parameter"
#pragma	warning (disable: 4100)


#include <cassert>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

// Containers
#include <list>
#include <queue>
#include <stack>
#include <string>
#include <vector>

// Algorithms
#include <algorithm>

using namespace std;


namespace nGENE
{
	// Below most of the classes are pre-declared to decrease
	// dependencies between files and this way you can use
	// reference or pointers in header files without including
	// header files containing class definition.
	class AABB;
	class AlphaStage;
	class Camera;
	class CameraEnumerator;
	class CameraFactory;
	class CameraFirstPerson;
	class CameraThirdPerson;
	class CharacterController;
	class CollisionListener;
	class Colour;
	class DeviceKeyboard;
	class DeviceMouse;
	class DeviceRender;
	class DeviceRenderDX9;
	class Engine;
	class ErrorHandler;
	template <typename T>
		class Factory;
	class FastFloat;
	class File;
	class FileArchive;
	class FileArchiveEnumerator;
	class FileManager;
	class FileNarrow;
	class ForceField;
	class ForceFieldShape;
	class ForceFieldShapeEnumerator;
	class ForceFieldShapeFactory;
	class ForceFieldShapesGroup;
	class FrameListener;
	class Frustum;
	class GeometryStage;
	class GUIControl;
	class GUICursor;
	class GUIFont;
	class GUIGroupBox;
	class GUIImage;
	class GUIListBox;
	class GUIManager;
	class GUIRadioButton;
	class GUITextField;
	class GUIWindow;
	template <typename Key, typename T, typename Compare, typename Hash>
		class HashIterator;
	template <typename Key, typename T, typename Compare, typename Hash>
		class HashTable;
	class IAnimationController;
	class ICullable;
	class IFile;
	class IndexedBuffer;
	class IndexedBufferDX9Factory;
	class IndexedBufferFactory;
	class InputDevice;
	class InputDeviceEnumerator;
	class InputDeviceFactory;
	class InputSystem;
	class IProperty;
	class ISerializer;
	class ISerializable;
	class ISound;
	class ISoundInstance;
	class ISoundManager;
	class ITexture;
	class Joint;
	class JointEnumerator;
	class JointFactory;
	class JointSphere;
	class JointSphereFactory;
	class KeyboardFactoryDX9;
	class KeyFrameAnimationSet;
	class LightStage;
	template <typename EventType>
		class Listener;
	class Log;
	class Material;
	class MaterialLibrary;
	class MaterialManager;
	class Matrix3x3;
	class Matrix4x4;
	class MouseFactoryDX9;
	class Node;
	class NodeBillboardSet;
	class NodeLight;
	template <typename LoaderPolicy>
		class NodeMesh;
	class NodeParticleSystem;
	class NodePrefab;
	class NodeSound;
	class NodeTerrain;
	class NodeVisible;
	class OBB;
	class Plane;
	class PhysicsCollisionListener;
	class PhysicsMaterial;
	class PhysicsTriggerListener;
	class PhysicsWorld;
	class PhysXAllocator;
	class Point;
	class PostStage;
	class PrefabArrow;
	class PrefabAxisRods;
	class PrefabBox;
	class PrefabCapsule;
	class PrefabCylinder;
	class PrefabPlane;
	class PrefabPyramid;
	class PrefabSphere;
	class PrefabTeapot;
	class PrefabTorus;
	class Profiler;
	class Quaternion;
	class Renderer;
	class RendererDX9Factory;
	class RendererDX10Factory;
	class RendererEnumerator;
	class RenderPass;
	class RenderStage;
	class RenderQueue;
	class Ray;
	class PhysicsActor;
	class SceneManager;
	class SceneManagerFactory;
	class ScenePartitioner;
	class ScreenOverlay;
	class ScriptLua;
	class ScriptRuby;
	class ScriptSystemLua;
	class Shader;
	class ShaderInstance;
	class ShaderConstant;
	class ShaderManager;
	class Shape;
	class ShapeEnumerator;
	class ShapeFactory;
	class Skeleton;
	class SoundManagerEnumerator;
	class StateManager;
	class Surface;
	class Task;
	class TaskManager;
	class TextureManager;
	class TimeDate;
	class Timer;
	template <typename T>
		class TreeIterator;
	class TriggerListener;
	class Vector2;
	class Vector3;
	class Vector4;
	class Vehicle;
	class VertexBuffer;
	class VertexBufferDX9Factory;
	class VertexBufferFactory;
	class VertexBufferManager;
	class VertexDeclaration;
	class VertexDeclarationDX9Factory;
	class VertexDeclarationFactory;	
	class Window;
	class XMLDocument;
	class XMLNode;
	class XMLParser;
	class XMLSerializer;

	typedef class FileArchiveEntry VFS_ENTRY;


	struct DeviceEvent;
	struct KeyboardEvent;
	struct MouseEvent;

	struct SEngineInfo;
	struct SDisplayMode;
	typedef struct SJointDesc JOINT_DESC;
	template <typename T>
		struct SRect;
	typedef struct SControllerDesc CONTROLLER_DESC;
	typedef struct SForceFieldDesc FORCE_FIELD_DESC;
	typedef struct SShaderConstantSemantic SEMANTIC_DESC;
	typedef struct SShaderConstantType DATATYPE_DESC;
	typedef struct SSkeletonBone BONE;
	typedef struct SSoundDesc SOUND_DESC;
	typedef struct STerrainPatchDescription TERRAIN_DESC;
	typedef struct SWindowDesc WINDOW_DESC;
	struct SVertex;
	struct SVertex2D;
	struct SVertexSkinning;
	typedef struct SVertexElement VERTEX_ELEMENT;


	enum FORCE_FIELD_COORDINATES;
	enum FORCE_FIELD_INTERACTION;
	enum LIGHT_TYPE;
	enum VERTEX_ELEMENT_USAGE;
}


#endif