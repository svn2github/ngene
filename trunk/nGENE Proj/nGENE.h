/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		nGENE.h
Version:	0.38
---------------------------------------------------------------------------
*/

#include "AABB.h"
#include "AIManager.h"
#include "AnimatedMeshLoadPolicyCollada.h"
#include "AnimatedMeshLoadPolicyXFile.h"
#include "Billboard.h"
#include "BWTEncoder.h"
#include "Camera.h"
#include "Camera2D.h"
#include "Camera3D.h"
#include "CameraFirstPerson.h"
#include "CameraThirdPerson.h"
#include "CatmullRomInterpolator.h"
#include "CharacterController.h"
#include "CollisionListener.h"
#include "DefaultFileArchive.h"
#include "DeviceKeyboard.h"
#include "DeviceMouse.h"
#include "DeviceRender.h"
#include "Engine.h"
#include "Factory.h"
#include "File.h"
#include "FileArchive.h"
#include "FileManager.h"
#include "ForceField.h"
#include "ForceFieldShape.h"
#include "ForceFieldShapesGroup.h"
#include "ForceFieldTypes.h"
#include "Frustum.h"
#include "FuncFilterBox.h"
#include "FuncSplit.h"
#include "GUIButton.h"
#include "GUICheckBox.h"
#include "GUIComboBox.h"
#include "GUIFont.h"
#include "GUIGroupBox.h"
#include "GUIImage.h"
#include "GUIListBox.h"
#include "GUIManager.h"
#include "GUIRadioButton.h"
#include "GUISlider.h"
#include "GUIStatic.h"
#include "GUITextField.h"
#include "GUIWindow.h"
#include "HashTable.h"
#include "HuffmanEncoder.h"
#include "IAnimationController.h"
#include "InputSystem.h"
#include "InterpolatedController.h"
#include "ISound.h"
#include "ISoundManager.h"
#include "ITexture.h"
#include "Joint.h"
#include "JointCylinder.h"
#include "JointDistance.h"
#include "JointFixed.h"
#include "JointHinge.h"
#include "JointSphere.h"
#include "LightStage.h"
#include "LinearInterpolator.h"
#include "Listener.h"
#include "ListenerRegistry.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "MeshLoadPolicy3dsFile.h"
#include "MeshLoadPolicyCollada.h"
#include "MeshLoadPolicyXFile.h"
#include "MTFEncoder.h"
#include "NodeLight.h"
#include "NodeMesh.h"
#include "NodeParticleSystem.h"
#include "NodeSound.h"
#include "NodeTerrain.h"
#include "NodeVisible.h"
#include "OrientationController.h"
#include "PerlinNoise.h"
#include "PhysicsMaterial.h"
#include "PhysicsWorld.h"
#include "PositionController.h"
#include "PostStage.h"
#include "PrefabBox.h"
#include "PrefabCapsule.h"
#include "PrefabCylinder.h"
#include "PrefabPlane.h"
#include "PrefabPyramid.h"
#include "PrefabSphere.h"
#include "PrefabTeapot.h"
#include "PrefabTorus.h"
#include "Quaternion.h"
#include "QuaternionInterpolator.h"
#include "Ray.h"
#include "RendererDX9.h"
#include "RenderPass.h"
#include "RenderStage.h"
#include "PhysicsActor.h"
#include "RLEEncoder.h"
#include "ScaleController.h"
#include "SceneManager.h"
#include "SceneManagerFactory.h"
#include "ScenePartitioner.h"
#include "ScenePartitionerQuadTree.h"
#include "ScenePartitionerOctTree.h"
#include "ScriptSystemLua.h"
#include "ScriptRuby.h"
#include "Shader.h"
#include "ShaderConstant.h"
#include "ShaderConstantTypes.h"
#include "ShaderInstance.h"
#include "ShaderManager.h"
#include "Singleton.h"
#include "ShapeEnumerator.h"
#include "TaskManager.h"
#include "TextureDX9.h"
#include "TextureManager.h"
#include "TextureSampler.h"
#include "TimeDate.h"
#include "Timer.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "VertexBufferManager.h"
#include "VertexDeclaration.h"
#include "WindowWin32.h"
#include "XMLNode.h"
#include "XMLParser.h"
#include "XMLSerializer.h"