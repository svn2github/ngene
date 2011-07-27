/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		frmMain.h
Version:	0.08
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRMMAIN_H_
#define __INC_FRMMAIN_H_


#include "frmAbout.h"
#include "frmErrorList.h"
#include "frmHeightMapEditor.h"
#include "frmMaterialEditor.h"
#include "frmMaterialLib.h"
#include "frmOutput.h"
#include "frmPropertyManager.h"
#include "frmRenderTarget.h"
#include "frmSceneGraph.h"
#include "frmScriptEditor.h"
#include "ShaderEditor.h"
#include "frmSolutionExplorer.h"
#include "frmToolbox.h"
#include "frmWebBrowser.h"
#include "frmFXs.h"

#include "MaterialWrapper.h"

#include "CameraWrapper.h"
#include "LightWrapper.h"
#include "EngineWrapper.h"
#include "FurWrapper.h"
#include "LightningWrapper.h"
#include "MeshWrapper.h"
#include "PrefabBoxWrapper.h"
#include "PrefabCapsuleWrapper.h"
#include "PrefabCylinderWrapper.h"
#include "PrefabPlaneWrapper.h"
#include "PrefabPyramidWrapper.h"
#include "PrefabSphereWrapper.h"
#include "PrefabTeapotWrapper.h"
#include "PrefabTorusWrapper.h"
#include "RigidBodyWrapper.h"
#include "SkyWrapper.h"
#include "SoundWrapper.h"
#include "CloudsWrapper.h"
#include "TerrainWrapper.h"
#include "WaterWrapper.h"
#include "ParticleSystemWrapper.h"


namespace nGENEToolset
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace WeifenLuo::WinFormsUI::Docking;
	using WeifenLuo::WinFormsUI::Docking::DockPanel;

	/** Application MDI main form.
	*/
	public ref class frmMain: public System::Windows::Forms::Form
	{
	private:
		nGENEToolset::frmRenderTarget^ target;					///< Form to which rendering is done.
	private: System::Windows::Forms::ToolStripButton^  tbsNewProject;



	private: System::Windows::Forms::ToolStripButton^  tsbOpen;

	private: System::Windows::Forms::ToolStripButton^  tsbSave;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator8;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton4;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton5;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton6;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator9;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton7;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton8;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator10;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuContentAuthoringShaderEditor;

	private: System::Windows::Forms::ToolStripMenuItem^  mnuMaterialEditor;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator11;
	private: System::Windows::Forms::ToolStripMenuItem^  animationEditorToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  textureEditorToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  texturedToolStripMenuItem;




	private: System::Windows::Forms::ToolStripMenuItem^  exportSelectedToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator12;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuFileSave;

	private: System::Windows::Forms::ToolStripMenuItem^  saveAsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator13;
	private: System::Windows::Forms::ToolStripMenuItem^  simulationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  pausedToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuLighting;

	private: System::Windows::Forms::ToolStripMenuItem^  modesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aIToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuSimulationModesPhysics;

	private: System::Windows::Forms::ToolStripMenuItem^  gameplayToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  materialLibraryToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  lightToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertLightDirectional;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertLightOmni;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertLightSpot;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertMesh;




	private: System::Windows::Forms::ToolStripMenuItem^  particleSystemToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  prefabToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPrefabBox;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPrefabSphere;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPrefabPlane;



			 nGENEToolset::frmAbout^ frmAboutBox;					///< About box
			 nGENEToolset::frmMaterialLib^ frmMatLib;				///< Material library window
			 nGENEToolset::frmOutput^ frmOut;						///< Output window
			 nGENEToolset::frmErrorList^ frmError;					///< Error list
			 nGENEToolset::frmPropertyManager^ frmProperty;			///< Properties window
			 nGENEToolset::frmMaterialEditor^ frmMatEdit;			///< Material editor window
			 nGENEToolset::frmToolbox^ frmTools;					///< Toolbox
			 nGENEToolset::frmFXs^ frmFX;							///< Special effects
			 nGENEToolset::frmSceneGraph^ frmGraph;					///< Scene graph viewer
			 nGENEToolset::frmSolutionExplorer^ frmExplorer;		///< Solution explorer
			 nGENEToolset::frmHeightMapEditor^ frmHeight;			///< Height-map editor
			 nGENEToolset::ShaderEditor^ frmShader;					///< Shader editor
			 nGENEToolset::frmScriptEditor^ frmScript;				///< Script editor


	private: System::Windows::Forms::ToolStripMenuItem^  scenegraphEditorToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator14;
			 nGENEToolset::frmWebBrowser^ frmBrowser;				///< Built-in web browser
	private: System::Windows::Forms::ToolStripStatusLabel^  tssLabel;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator15;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuScriptEditor;

	private: System::Windows::Forms::ToolStripMenuItem^  mnuNewProject;

	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  projectToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuFileOpenFile;

	private: System::Windows::Forms::ToolStripMenuItem^  saveAllToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator16;
	private: System::Windows::Forms::ToolStripMenuItem^  closeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  cameraToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertCameraFPP;


			 nGENEToolset::LightWrapper^ light;						///< @todo Remove it
			 nGENEToolset::TerrainWrapper^ terrain;					///< @todo Remove it
			 nGENEToolset::SkyWrapper^ sky;							///< @todo Remove it
			 nGENEToolset::SceneNode^ nodeObj;

	private: System::Windows::Forms::ToolStripMenuItem^  windowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuWindowSaveLayout;


	private: System::Windows::Forms::ToolStripMenuItem^  outdoorToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertOutdoorSky;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertOutdoorTerrain;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertOutdoorClouds;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInstertOutdoorWater;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuViewMaterialLibrary;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuViewOutput;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuViewSolutionExplorer;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuViewToolbox;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPSParticleSystem;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPSParticleEmitter;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPSParticleColour;
private: System::Windows::Forms::ToolStripMenuItem^  mnuViewModeHDR;
private: System::Windows::Forms::ToolStripButton^  tbsRemoveNode;

private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator17;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator18;
private: System::Windows::Forms::ToolStripMenuItem^  mnuViewModeShadows;
private: System::Windows::Forms::ToolStripMenuItem^  mnuViewModeReflections;
private: System::Windows::Forms::ToolStripMenuItem^  mnuHelpTutorials;
private: System::Windows::Forms::ToolStripMenuItem^  mnuViewModeDebugInfo;
private: System::Windows::Forms::ToolStripButton^  tsbLockX;
private: System::Windows::Forms::ToolStripButton^  tsbLockY;
private: System::Windows::Forms::ToolStripButton^  tsbLockZ;



private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator19;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator20;
private: System::Windows::Forms::ToolStripMenuItem^  lockAxesToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  xAxisToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  yAxisToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  zAxisToolStripMenuItem;
private: System::Windows::Forms::ToolStrip^  toolStrip2;
private: System::Windows::Forms::ToolStripButton^  tbsLight;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator21;
private: System::Windows::Forms::ToolStripButton^  tbsBox;
private: System::Windows::Forms::ToolStripButton^  tbsSphere;
private: System::Windows::Forms::ToolStripButton^  tbsPlane;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator22;
private: System::Windows::Forms::ToolStripButton^  tbsCamera;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator23;
private: System::Windows::Forms::ToolStripButton^  tbsTerrain;
private: System::Windows::Forms::ToolStripButton^  tbsSky;
private: System::Windows::Forms::ToolStripButton^  tbsClouds;
private: System::Windows::Forms::ToolStripButton^  tbsWater;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator24;
private: System::Windows::Forms::ToolStripButton^  tbsParticleSystem;
private: System::Windows::Forms::ToolStripButton^  tbsParticleEmitter;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator25;
private: System::Windows::Forms::ToolStripButton^  tbsMesh;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator26;
private: System::Windows::Forms::ToolStripMenuItem^  otherToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertOtherFur;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator27;
private: System::Windows::Forms::ToolStripMenuItem^  mnuHeightMapEditor;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPSParticleForce;
private: System::Windows::Forms::ToolStripMenuItem^  mnuErrorList;
private: System::Windows::Forms::ToolStripMenuItem^  physicsToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  rigidBodyToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  softBodyToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  clothToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  controllerToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPhysicsRigidActor;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator28;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPhysicsRigidBox;
private: System::Windows::Forms::ToolStripMenuItem^  boxToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  capsuleToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  planeToolStripMenuItem1;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator29;
private: System::Windows::Forms::ToolStripMenuItem^  meshToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  terrainToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  mnuFXs;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertLightVolumetric;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertSound;
private: System::Windows::Forms::ToolStripButton^  tbsSound;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator30;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertLightning;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPrefabCapsule;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPrefabTeapot;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPrefabTorus;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPrefabCylinder;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPrefabPyramid;
private: System::Windows::Forms::ToolStripMenuItem^  mnuInsertPSParticleDeflector;
private: System::Windows::Forms::ToolStripStatusLabel^  tssFPS;
private: WeifenLuo::WinFormsUI::Docking::DockPanel^  dockManager;
private: System::Windows::Forms::ToolStripMenuItem^  renderingToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  mnuSimulationRenderingContinuous;
private: System::Windows::Forms::ToolStripMenuItem^  mnuSimulationRenderingRequestRender;










			 nGENEToolset::CameraWrapper^ camera;

	public:
		frmMain(void)
		{
			InitializeComponent();
		}

	private:
		System::Windows::Forms::MenuStrip^  mnsMainMenu;
		System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;

		System::Windows::Forms::ToolStripMenuItem^  mnuAbout;
		System::Windows::Forms::ToolStripMenuItem^  newToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  undoToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  redoToolStripMenuItem;
		System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
		System::Windows::Forms::ToolStrip^  toolStrip1;
private: System::Windows::Forms::StatusStrip^  stsStrip;

		System::Windows::Forms::ToolStripMenuItem^  insertToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  searchToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  contentsToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  indexToolStripMenuItem;
		System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  mnuNGENEOnTheWeb;

		System::Windows::Forms::ToolStripMenuItem^  technicalSupportToolStripMenuItem;
		System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
		System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
		System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
		System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  customizeToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  mnuToolsOptions;

		System::Windows::Forms::ToolStripMenuItem^  cutToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  copyToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  pasteToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  mnuEditDelete;

		System::Windows::Forms::ToolStripMenuItem^  materialLibraryToolStripMenuItem;
		System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;
		System::Windows::Forms::ToolStripMenuItem^  toolbarsToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  standardToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  fullToolStripMenuItem;
		System::Windows::Forms::ToolStripSeparator^  toolStripSeparator7;
		System::Windows::Forms::ToolStripMenuItem^  modeToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  solidToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  mnuWireframe;

		 WeifenLuo::WinFormsUI::Docking::DeserializeDockContent^ deserializeDockContent;

private: System::Windows::Forms::ToolStripMenuItem^  mnuViewPropertyManager;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;

		System::Windows::Forms::ToolStripMenuItem^  propertiesToolStripMenuItem;

	protected:
		//FrameworkWin32* app;
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmMain()
		{
			if(components)
				delete components;

			//if(app)
			//	delete app;
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frmMain::typeid));
			this->mnsMainMenu = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->newToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuNewProject = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fileToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->projectToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuFileOpenFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mnuFileSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAllToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator16 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->closeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator13 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exportSelectedToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator12 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->undoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->redoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->copyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pasteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator18 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mnuEditDelete = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator20 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->lockAxesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->xAxisToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->yAxisToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->zAxisToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->insertToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->lightToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertLightDirectional = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertLightOmni = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertLightSpot = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertLightVolumetric = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertMesh = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->particleSystemToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPSParticleSystem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPSParticleEmitter = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPSParticleColour = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPSParticleForce = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPSParticleDeflector = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->prefabToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPrefabBox = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPrefabSphere = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPrefabCapsule = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPrefabCylinder = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPrefabPlane = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPrefabPyramid = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPrefabTeapot = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPrefabTorus = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cameraToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertCameraFPP = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->outdoorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertOutdoorSky = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertOutdoorTerrain = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertOutdoorClouds = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInstertOutdoorWater = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertLightning = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->otherToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertOtherFur = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->physicsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->rigidBodyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertPhysicsRigidActor = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator28 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mnuInsertPhysicsRigidBox = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->boxToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->capsuleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->planeToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator29 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->meshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->terrainToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->softBodyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->clothToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->controllerToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuInsertSound = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuViewMaterialLibrary = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuViewOutput = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuErrorList = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuViewPropertyManager = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuViewSolutionExplorer = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuViewToolbox = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuFXs = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolbarsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->standardToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->solidToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->texturedToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuWireframe = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuLighting = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuViewModeHDR = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuViewModeShadows = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuViewModeReflections = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuViewModeDebugInfo = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fullToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->propertiesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->materialLibraryToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuContentAuthoringShaderEditor = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuMaterialEditor = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->materialLibraryToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator11 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->animationEditorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->textureEditorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator15 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mnuScriptEditor = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator27 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mnuHeightMapEditor = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->scenegraphEditorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator14 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->customizeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuToolsOptions = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->simulationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pausedToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->modesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aIToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuSimulationModesPhysics = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gameplayToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->windowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuWindowSaveLayout = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->searchToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contentsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->indexToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mnuNGENEOnTheWeb = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuHelpTutorials = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->technicalSupportToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mnuAbout = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->tbsNewProject = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbOpen = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbSave = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator8 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripButton4 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton5 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton6 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator9 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tbsRemoveNode = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator17 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripButton7 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton8 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator10 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tsbLockX = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbLockY = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbLockZ = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator19 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->stsStrip = (gcnew System::Windows::Forms::StatusStrip());
			this->tssLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->tssFPS = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->dockManager = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanel());
			this->toolStrip2 = (gcnew System::Windows::Forms::ToolStrip());
			this->tbsLight = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator21 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tbsBox = (gcnew System::Windows::Forms::ToolStripButton());
			this->tbsSphere = (gcnew System::Windows::Forms::ToolStripButton());
			this->tbsPlane = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator22 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tbsCamera = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator23 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tbsTerrain = (gcnew System::Windows::Forms::ToolStripButton());
			this->tbsSky = (gcnew System::Windows::Forms::ToolStripButton());
			this->tbsClouds = (gcnew System::Windows::Forms::ToolStripButton());
			this->tbsWater = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator24 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tbsParticleSystem = (gcnew System::Windows::Forms::ToolStripButton());
			this->tbsParticleEmitter = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator25 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tbsMesh = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator26 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tbsSound = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator30 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->renderingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuSimulationRenderingContinuous = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnuSimulationRenderingRequestRender = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnsMainMenu->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->stsStrip->SuspendLayout();
			this->toolStrip2->SuspendLayout();
			this->SuspendLayout();
			// 
			// mnsMainMenu
			// 
			this->mnsMainMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->fileToolStripMenuItem, 
				this->editToolStripMenuItem, this->insertToolStripMenuItem, this->viewToolStripMenuItem, this->toolsToolStripMenuItem, this->simulationToolStripMenuItem, 
				this->windowToolStripMenuItem, this->helpToolStripMenuItem});
			this->mnsMainMenu->Location = System::Drawing::Point(0, 0);
			this->mnsMainMenu->Name = L"mnsMainMenu";
			this->mnsMainMenu->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->mnsMainMenu->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->mnsMainMenu->Size = System::Drawing::Size(1158, 24);
			this->mnsMainMenu->TabIndex = 0;
			this->mnsMainMenu->Text = L"mnsMainMenu";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {this->newToolStripMenuItem, 
				this->openToolStripMenuItem, this->toolStripSeparator5, this->mnuFileSave, this->saveAsToolStripMenuItem, this->saveAllToolStripMenuItem, 
				this->toolStripSeparator16, this->closeToolStripMenuItem, this->toolStripSeparator13, this->exportSelectedToolStripMenuItem, 
				this->toolStripSeparator12, this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// newToolStripMenuItem
			// 
			this->newToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->mnuNewProject, 
				this->fileToolStripMenuItem1});
			this->newToolStripMenuItem->Name = L"newToolStripMenuItem";
			this->newToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->newToolStripMenuItem->Text = L"New";
			this->newToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::newToolStripMenuItem_Click);
			// 
			// mnuNewProject
			// 
			this->mnuNewProject->Name = L"mnuNewProject";
			this->mnuNewProject->Size = System::Drawing::Size(120, 22);
			this->mnuNewProject->Text = L"Project...";
			// 
			// fileToolStripMenuItem1
			// 
			this->fileToolStripMenuItem1->Name = L"fileToolStripMenuItem1";
			this->fileToolStripMenuItem1->Size = System::Drawing::Size(120, 22);
			this->fileToolStripMenuItem1->Text = L"File...";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->projectToolStripMenuItem1, 
				this->mnuFileOpenFile});
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->openToolStripMenuItem->Text = L"Open";
			// 
			// projectToolStripMenuItem1
			// 
			this->projectToolStripMenuItem1->Name = L"projectToolStripMenuItem1";
			this->projectToolStripMenuItem1->Size = System::Drawing::Size(120, 22);
			this->projectToolStripMenuItem1->Text = L"Project...";
			// 
			// mnuFileOpenFile
			// 
			this->mnuFileOpenFile->Name = L"mnuFileOpenFile";
			this->mnuFileOpenFile->Size = System::Drawing::Size(120, 22);
			this->mnuFileOpenFile->Text = L"File...";
			this->mnuFileOpenFile->Click += gcnew System::EventHandler(this, &frmMain::mnuFileOpenFile_Click);
			// 
			// toolStripSeparator5
			// 
			this->toolStripSeparator5->Name = L"toolStripSeparator5";
			this->toolStripSeparator5->Size = System::Drawing::Size(159, 6);
			// 
			// mnuFileSave
			// 
			this->mnuFileSave->Name = L"mnuFileSave";
			this->mnuFileSave->Size = System::Drawing::Size(162, 22);
			this->mnuFileSave->Text = L"Save";
			this->mnuFileSave->Click += gcnew System::EventHandler(this, &frmMain::mnuFileSave_Click);
			// 
			// saveAsToolStripMenuItem
			// 
			this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
			this->saveAsToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->saveAsToolStripMenuItem->Text = L"Save as...";
			// 
			// saveAllToolStripMenuItem
			// 
			this->saveAllToolStripMenuItem->Name = L"saveAllToolStripMenuItem";
			this->saveAllToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->saveAllToolStripMenuItem->Text = L"Save all";
			// 
			// toolStripSeparator16
			// 
			this->toolStripSeparator16->Name = L"toolStripSeparator16";
			this->toolStripSeparator16->Size = System::Drawing::Size(159, 6);
			// 
			// closeToolStripMenuItem
			// 
			this->closeToolStripMenuItem->Name = L"closeToolStripMenuItem";
			this->closeToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->closeToolStripMenuItem->Text = L"Close";
			// 
			// toolStripSeparator13
			// 
			this->toolStripSeparator13->Name = L"toolStripSeparator13";
			this->toolStripSeparator13->Size = System::Drawing::Size(159, 6);
			// 
			// exportSelectedToolStripMenuItem
			// 
			this->exportSelectedToolStripMenuItem->Name = L"exportSelectedToolStripMenuItem";
			this->exportSelectedToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->exportSelectedToolStripMenuItem->Text = L"Export selected...";
			// 
			// toolStripSeparator12
			// 
			this->toolStripSeparator12->Name = L"toolStripSeparator12";
			this->toolStripSeparator12->Size = System::Drawing::Size(159, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::exitToolStripMenuItem_Click);
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->undoToolStripMenuItem, 
				this->redoToolStripMenuItem, this->toolStripSeparator2, this->cutToolStripMenuItem, this->copyToolStripMenuItem, this->pasteToolStripMenuItem, 
				this->toolStripSeparator18, this->mnuEditDelete, this->toolStripSeparator20, this->lockAxesToolStripMenuItem});
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(39, 20);
			this->editToolStripMenuItem->Text = L"Edit";
			// 
			// undoToolStripMenuItem
			// 
			this->undoToolStripMenuItem->Name = L"undoToolStripMenuItem";
			this->undoToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->undoToolStripMenuItem->Text = L"Undo";
			// 
			// redoToolStripMenuItem
			// 
			this->redoToolStripMenuItem->Name = L"redoToolStripMenuItem";
			this->redoToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->redoToolStripMenuItem->Text = L"Redo";
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(121, 6);
			// 
			// cutToolStripMenuItem
			// 
			this->cutToolStripMenuItem->Name = L"cutToolStripMenuItem";
			this->cutToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->cutToolStripMenuItem->Text = L"Cut";
			// 
			// copyToolStripMenuItem
			// 
			this->copyToolStripMenuItem->Name = L"copyToolStripMenuItem";
			this->copyToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->copyToolStripMenuItem->Text = L"Copy";
			// 
			// pasteToolStripMenuItem
			// 
			this->pasteToolStripMenuItem->Name = L"pasteToolStripMenuItem";
			this->pasteToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->pasteToolStripMenuItem->Text = L"Paste";
			// 
			// toolStripSeparator18
			// 
			this->toolStripSeparator18->Name = L"toolStripSeparator18";
			this->toolStripSeparator18->Size = System::Drawing::Size(121, 6);
			// 
			// mnuEditDelete
			// 
			this->mnuEditDelete->Name = L"mnuEditDelete";
			this->mnuEditDelete->Size = System::Drawing::Size(124, 22);
			this->mnuEditDelete->Text = L"Delete";
			this->mnuEditDelete->Click += gcnew System::EventHandler(this, &frmMain::mnuEditDelete_Click);
			// 
			// toolStripSeparator20
			// 
			this->toolStripSeparator20->Name = L"toolStripSeparator20";
			this->toolStripSeparator20->Size = System::Drawing::Size(121, 6);
			// 
			// lockAxesToolStripMenuItem
			// 
			this->lockAxesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->xAxisToolStripMenuItem, 
				this->yAxisToolStripMenuItem, this->zAxisToolStripMenuItem});
			this->lockAxesToolStripMenuItem->Name = L"lockAxesToolStripMenuItem";
			this->lockAxesToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->lockAxesToolStripMenuItem->Text = L"Lock axes";
			// 
			// xAxisToolStripMenuItem
			// 
			this->xAxisToolStripMenuItem->Checked = true;
			this->xAxisToolStripMenuItem->CheckOnClick = true;
			this->xAxisToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->xAxisToolStripMenuItem->Name = L"xAxisToolStripMenuItem";
			this->xAxisToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::Shift) 
				| System::Windows::Forms::Keys::X));
			this->xAxisToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->xAxisToolStripMenuItem->Text = L"X axis";
			this->xAxisToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::xAxisToolStripMenuItem_Click);
			// 
			// yAxisToolStripMenuItem
			// 
			this->yAxisToolStripMenuItem->Checked = true;
			this->yAxisToolStripMenuItem->CheckOnClick = true;
			this->yAxisToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->yAxisToolStripMenuItem->Name = L"yAxisToolStripMenuItem";
			this->yAxisToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::Shift) 
				| System::Windows::Forms::Keys::Y));
			this->yAxisToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->yAxisToolStripMenuItem->Text = L"Y axis";
			this->yAxisToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::yAxisToolStripMenuItem_Click);
			// 
			// zAxisToolStripMenuItem
			// 
			this->zAxisToolStripMenuItem->Checked = true;
			this->zAxisToolStripMenuItem->CheckOnClick = true;
			this->zAxisToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->zAxisToolStripMenuItem->Name = L"zAxisToolStripMenuItem";
			this->zAxisToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::Shift) 
				| System::Windows::Forms::Keys::Z));
			this->zAxisToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->zAxisToolStripMenuItem->Text = L"Z axis";
			this->zAxisToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::zAxisToolStripMenuItem_Click);
			// 
			// insertToolStripMenuItem
			// 
			this->insertToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->lightToolStripMenuItem, 
				this->mnuInsertMesh, this->particleSystemToolStripMenuItem, this->prefabToolStripMenuItem, this->cameraToolStripMenuItem, this->outdoorToolStripMenuItem, 
				this->otherToolStripMenuItem, this->physicsToolStripMenuItem, this->mnuInsertSound});
			this->insertToolStripMenuItem->Name = L"insertToolStripMenuItem";
			this->insertToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->insertToolStripMenuItem->Text = L"Insert";
			this->insertToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::insertToolStripMenuItem_Click);
			// 
			// lightToolStripMenuItem
			// 
			this->lightToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->mnuInsertLightDirectional, 
				this->mnuInsertLightOmni, this->mnuInsertLightSpot, this->mnuInsertLightVolumetric});
			this->lightToolStripMenuItem->Name = L"lightToolStripMenuItem";
			this->lightToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->lightToolStripMenuItem->Text = L"Light";
			// 
			// mnuInsertLightDirectional
			// 
			this->mnuInsertLightDirectional->Name = L"mnuInsertLightDirectional";
			this->mnuInsertLightDirectional->Size = System::Drawing::Size(132, 22);
			this->mnuInsertLightDirectional->Text = L"Directional";
			this->mnuInsertLightDirectional->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertLightDirectional_Click);
			// 
			// mnuInsertLightOmni
			// 
			this->mnuInsertLightOmni->Name = L"mnuInsertLightOmni";
			this->mnuInsertLightOmni->Size = System::Drawing::Size(132, 22);
			this->mnuInsertLightOmni->Text = L"Omni";
			this->mnuInsertLightOmni->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertLightOmni_Click);
			// 
			// mnuInsertLightSpot
			// 
			this->mnuInsertLightSpot->Name = L"mnuInsertLightSpot";
			this->mnuInsertLightSpot->Size = System::Drawing::Size(132, 22);
			this->mnuInsertLightSpot->Text = L"Spot";
			this->mnuInsertLightSpot->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertLightSpot_Click);
			// 
			// mnuInsertLightVolumetric
			// 
			this->mnuInsertLightVolumetric->Name = L"mnuInsertLightVolumetric";
			this->mnuInsertLightVolumetric->Size = System::Drawing::Size(132, 22);
			this->mnuInsertLightVolumetric->Text = L"Volumetric";
			this->mnuInsertLightVolumetric->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertLightVolumetric_Click);
			// 
			// mnuInsertMesh
			// 
			this->mnuInsertMesh->Name = L"mnuInsertMesh";
			this->mnuInsertMesh->Size = System::Drawing::Size(154, 22);
			this->mnuInsertMesh->Text = L"Mesh";
			this->mnuInsertMesh->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertMesh_Click);
			// 
			// particleSystemToolStripMenuItem
			// 
			this->particleSystemToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->mnuInsertPSParticleSystem, 
				this->mnuInsertPSParticleEmitter, this->mnuInsertPSParticleColour, this->mnuInsertPSParticleForce, this->mnuInsertPSParticleDeflector});
			this->particleSystemToolStripMenuItem->Name = L"particleSystemToolStripMenuItem";
			this->particleSystemToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->particleSystemToolStripMenuItem->Text = L"Particle System";
			// 
			// mnuInsertPSParticleSystem
			// 
			this->mnuInsertPSParticleSystem->Name = L"mnuInsertPSParticleSystem";
			this->mnuInsertPSParticleSystem->Size = System::Drawing::Size(163, 22);
			this->mnuInsertPSParticleSystem->Text = L"Particle system";
			this->mnuInsertPSParticleSystem->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPSParticleSystem_Click);
			// 
			// mnuInsertPSParticleEmitter
			// 
			this->mnuInsertPSParticleEmitter->Name = L"mnuInsertPSParticleEmitter";
			this->mnuInsertPSParticleEmitter->Size = System::Drawing::Size(163, 22);
			this->mnuInsertPSParticleEmitter->Text = L"Particle emitter";
			this->mnuInsertPSParticleEmitter->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPSParticleEmitter_Click);
			// 
			// mnuInsertPSParticleColour
			// 
			this->mnuInsertPSParticleColour->Name = L"mnuInsertPSParticleColour";
			this->mnuInsertPSParticleColour->Size = System::Drawing::Size(163, 22);
			this->mnuInsertPSParticleColour->Text = L"Particle colour";
			this->mnuInsertPSParticleColour->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPSParticleColour_Click);
			// 
			// mnuInsertPSParticleForce
			// 
			this->mnuInsertPSParticleForce->Name = L"mnuInsertPSParticleForce";
			this->mnuInsertPSParticleForce->Size = System::Drawing::Size(163, 22);
			this->mnuInsertPSParticleForce->Text = L"Particle force";
			this->mnuInsertPSParticleForce->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPSParticleForce_Click);
			// 
			// mnuInsertPSParticleDeflector
			// 
			this->mnuInsertPSParticleDeflector->Name = L"mnuInsertPSParticleDeflector";
			this->mnuInsertPSParticleDeflector->Size = System::Drawing::Size(163, 22);
			this->mnuInsertPSParticleDeflector->Text = L"Particle deflector";
			this->mnuInsertPSParticleDeflector->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPSParticleDeflector_Click);
			// 
			// prefabToolStripMenuItem
			// 
			this->prefabToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->mnuInsertPrefabBox, 
				this->mnuInsertPrefabSphere, this->mnuInsertPrefabCapsule, this->mnuInsertPrefabCylinder, this->mnuInsertPrefabPlane, this->mnuInsertPrefabPyramid, 
				this->mnuInsertPrefabTeapot, this->mnuInsertPrefabTorus});
			this->prefabToolStripMenuItem->Name = L"prefabToolStripMenuItem";
			this->prefabToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->prefabToolStripMenuItem->Text = L"Prefab";
			// 
			// mnuInsertPrefabBox
			// 
			this->mnuInsertPrefabBox->Name = L"mnuInsertPrefabBox";
			this->mnuInsertPrefabBox->Size = System::Drawing::Size(118, 22);
			this->mnuInsertPrefabBox->Text = L"Box";
			this->mnuInsertPrefabBox->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPrefabBox_Click);
			// 
			// mnuInsertPrefabSphere
			// 
			this->mnuInsertPrefabSphere->Name = L"mnuInsertPrefabSphere";
			this->mnuInsertPrefabSphere->Size = System::Drawing::Size(118, 22);
			this->mnuInsertPrefabSphere->Text = L"Sphere";
			this->mnuInsertPrefabSphere->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPrefabSphere_Click);
			// 
			// mnuInsertPrefabCapsule
			// 
			this->mnuInsertPrefabCapsule->Name = L"mnuInsertPrefabCapsule";
			this->mnuInsertPrefabCapsule->Size = System::Drawing::Size(118, 22);
			this->mnuInsertPrefabCapsule->Text = L"Capsule";
			this->mnuInsertPrefabCapsule->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPrefabCapsule_Click);
			// 
			// mnuInsertPrefabCylinder
			// 
			this->mnuInsertPrefabCylinder->Name = L"mnuInsertPrefabCylinder";
			this->mnuInsertPrefabCylinder->Size = System::Drawing::Size(118, 22);
			this->mnuInsertPrefabCylinder->Text = L"Cylinder";
			this->mnuInsertPrefabCylinder->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPrefabCylinder_Click);
			// 
			// mnuInsertPrefabPlane
			// 
			this->mnuInsertPrefabPlane->Name = L"mnuInsertPrefabPlane";
			this->mnuInsertPrefabPlane->Size = System::Drawing::Size(118, 22);
			this->mnuInsertPrefabPlane->Text = L"Plane";
			this->mnuInsertPrefabPlane->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPrefabPlane_Click);
			// 
			// mnuInsertPrefabPyramid
			// 
			this->mnuInsertPrefabPyramid->Name = L"mnuInsertPrefabPyramid";
			this->mnuInsertPrefabPyramid->Size = System::Drawing::Size(118, 22);
			this->mnuInsertPrefabPyramid->Text = L"Pyramid";
			this->mnuInsertPrefabPyramid->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPrefabPyramid_Click);
			// 
			// mnuInsertPrefabTeapot
			// 
			this->mnuInsertPrefabTeapot->Name = L"mnuInsertPrefabTeapot";
			this->mnuInsertPrefabTeapot->Size = System::Drawing::Size(118, 22);
			this->mnuInsertPrefabTeapot->Text = L"Teapot";
			this->mnuInsertPrefabTeapot->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPrefabTeapot_Click);
			// 
			// mnuInsertPrefabTorus
			// 
			this->mnuInsertPrefabTorus->Name = L"mnuInsertPrefabTorus";
			this->mnuInsertPrefabTorus->Size = System::Drawing::Size(118, 22);
			this->mnuInsertPrefabTorus->Text = L"Torus";
			this->mnuInsertPrefabTorus->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPrefabTorus_Click);
			// 
			// cameraToolStripMenuItem
			// 
			this->cameraToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->mnuInsertCameraFPP});
			this->cameraToolStripMenuItem->Name = L"cameraToolStripMenuItem";
			this->cameraToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->cameraToolStripMenuItem->Text = L"Camera";
			// 
			// mnuInsertCameraFPP
			// 
			this->mnuInsertCameraFPP->Name = L"mnuInsertCameraFPP";
			this->mnuInsertCameraFPP->Size = System::Drawing::Size(137, 22);
			this->mnuInsertCameraFPP->Text = L"First-person";
			this->mnuInsertCameraFPP->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertCameraFPP_Click);
			// 
			// outdoorToolStripMenuItem
			// 
			this->outdoorToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->mnuInsertOutdoorSky, 
				this->mnuInsertOutdoorTerrain, this->mnuInsertOutdoorClouds, this->mnuInstertOutdoorWater, this->mnuInsertLightning});
			this->outdoorToolStripMenuItem->Name = L"outdoorToolStripMenuItem";
			this->outdoorToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->outdoorToolStripMenuItem->Text = L"Outdoor";
			this->outdoorToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::outdoorToolStripMenuItem_Click);
			// 
			// mnuInsertOutdoorSky
			// 
			this->mnuInsertOutdoorSky->Name = L"mnuInsertOutdoorSky";
			this->mnuInsertOutdoorSky->Size = System::Drawing::Size(125, 22);
			this->mnuInsertOutdoorSky->Text = L"Sky";
			this->mnuInsertOutdoorSky->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertOutdoorSky_Click);
			// 
			// mnuInsertOutdoorTerrain
			// 
			this->mnuInsertOutdoorTerrain->Name = L"mnuInsertOutdoorTerrain";
			this->mnuInsertOutdoorTerrain->Size = System::Drawing::Size(125, 22);
			this->mnuInsertOutdoorTerrain->Text = L"Terrain";
			this->mnuInsertOutdoorTerrain->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertOutdoorTerrain_Click);
			// 
			// mnuInsertOutdoorClouds
			// 
			this->mnuInsertOutdoorClouds->Name = L"mnuInsertOutdoorClouds";
			this->mnuInsertOutdoorClouds->Size = System::Drawing::Size(125, 22);
			this->mnuInsertOutdoorClouds->Text = L"Clouds";
			this->mnuInsertOutdoorClouds->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertOutdoorClouds_Click);
			// 
			// mnuInstertOutdoorWater
			// 
			this->mnuInstertOutdoorWater->Name = L"mnuInstertOutdoorWater";
			this->mnuInstertOutdoorWater->Size = System::Drawing::Size(125, 22);
			this->mnuInstertOutdoorWater->Text = L"Water";
			this->mnuInstertOutdoorWater->Click += gcnew System::EventHandler(this, &frmMain::mnuInstertOutdoorWater_Click);
			// 
			// mnuInsertLightning
			// 
			this->mnuInsertLightning->Name = L"mnuInsertLightning";
			this->mnuInsertLightning->Size = System::Drawing::Size(125, 22);
			this->mnuInsertLightning->Text = L"Lightning";
			this->mnuInsertLightning->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertLightning_Click);
			// 
			// otherToolStripMenuItem
			// 
			this->otherToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->mnuInsertOtherFur});
			this->otherToolStripMenuItem->Name = L"otherToolStripMenuItem";
			this->otherToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->otherToolStripMenuItem->Text = L"Other";
			// 
			// mnuInsertOtherFur
			// 
			this->mnuInsertOtherFur->Name = L"mnuInsertOtherFur";
			this->mnuInsertOtherFur->Size = System::Drawing::Size(91, 22);
			this->mnuInsertOtherFur->Text = L"Fur";
			this->mnuInsertOtherFur->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertOtherFur_Click);
			// 
			// physicsToolStripMenuItem
			// 
			this->physicsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->rigidBodyToolStripMenuItem, 
				this->softBodyToolStripMenuItem, this->clothToolStripMenuItem, this->controllerToolStripMenuItem});
			this->physicsToolStripMenuItem->Name = L"physicsToolStripMenuItem";
			this->physicsToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->physicsToolStripMenuItem->Text = L"Physics";
			// 
			// rigidBodyToolStripMenuItem
			// 
			this->rigidBodyToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->mnuInsertPhysicsRigidActor, 
				this->toolStripSeparator28, this->mnuInsertPhysicsRigidBox, this->boxToolStripMenuItem, this->capsuleToolStripMenuItem, this->planeToolStripMenuItem1, 
				this->toolStripSeparator29, this->meshToolStripMenuItem, this->terrainToolStripMenuItem});
			this->rigidBodyToolStripMenuItem->Name = L"rigidBodyToolStripMenuItem";
			this->rigidBodyToolStripMenuItem->Size = System::Drawing::Size(181, 22);
			this->rigidBodyToolStripMenuItem->Text = L"Rigid Body";
			// 
			// mnuInsertPhysicsRigidActor
			// 
			this->mnuInsertPhysicsRigidActor->Name = L"mnuInsertPhysicsRigidActor";
			this->mnuInsertPhysicsRigidActor->Size = System::Drawing::Size(116, 22);
			this->mnuInsertPhysicsRigidActor->Text = L"Actor";
			// 
			// toolStripSeparator28
			// 
			this->toolStripSeparator28->Name = L"toolStripSeparator28";
			this->toolStripSeparator28->Size = System::Drawing::Size(113, 6);
			// 
			// mnuInsertPhysicsRigidBox
			// 
			this->mnuInsertPhysicsRigidBox->Name = L"mnuInsertPhysicsRigidBox";
			this->mnuInsertPhysicsRigidBox->Size = System::Drawing::Size(116, 22);
			this->mnuInsertPhysicsRigidBox->Text = L"Box";
			this->mnuInsertPhysicsRigidBox->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertPhysicsRigidBox_Click);
			// 
			// boxToolStripMenuItem
			// 
			this->boxToolStripMenuItem->Name = L"boxToolStripMenuItem";
			this->boxToolStripMenuItem->Size = System::Drawing::Size(116, 22);
			this->boxToolStripMenuItem->Text = L"Sphere";
			// 
			// capsuleToolStripMenuItem
			// 
			this->capsuleToolStripMenuItem->Name = L"capsuleToolStripMenuItem";
			this->capsuleToolStripMenuItem->Size = System::Drawing::Size(116, 22);
			this->capsuleToolStripMenuItem->Text = L"Capsule";
			// 
			// planeToolStripMenuItem1
			// 
			this->planeToolStripMenuItem1->Name = L"planeToolStripMenuItem1";
			this->planeToolStripMenuItem1->Size = System::Drawing::Size(116, 22);
			this->planeToolStripMenuItem1->Text = L"Plane";
			this->planeToolStripMenuItem1->Click += gcnew System::EventHandler(this, &frmMain::planeToolStripMenuItem1_Click);
			// 
			// toolStripSeparator29
			// 
			this->toolStripSeparator29->Name = L"toolStripSeparator29";
			this->toolStripSeparator29->Size = System::Drawing::Size(113, 6);
			// 
			// meshToolStripMenuItem
			// 
			this->meshToolStripMenuItem->Name = L"meshToolStripMenuItem";
			this->meshToolStripMenuItem->Size = System::Drawing::Size(116, 22);
			this->meshToolStripMenuItem->Text = L"Mesh";
			// 
			// terrainToolStripMenuItem
			// 
			this->terrainToolStripMenuItem->Name = L"terrainToolStripMenuItem";
			this->terrainToolStripMenuItem->Size = System::Drawing::Size(116, 22);
			this->terrainToolStripMenuItem->Text = L"Terrain";
			// 
			// softBodyToolStripMenuItem
			// 
			this->softBodyToolStripMenuItem->Name = L"softBodyToolStripMenuItem";
			this->softBodyToolStripMenuItem->Size = System::Drawing::Size(181, 22);
			this->softBodyToolStripMenuItem->Text = L"Soft Body";
			// 
			// clothToolStripMenuItem
			// 
			this->clothToolStripMenuItem->Name = L"clothToolStripMenuItem";
			this->clothToolStripMenuItem->Size = System::Drawing::Size(181, 22);
			this->clothToolStripMenuItem->Text = L"Cloth";
			// 
			// controllerToolStripMenuItem
			// 
			this->controllerToolStripMenuItem->Name = L"controllerToolStripMenuItem";
			this->controllerToolStripMenuItem->Size = System::Drawing::Size(181, 22);
			this->controllerToolStripMenuItem->Text = L"Character Controller";
			// 
			// mnuInsertSound
			// 
			this->mnuInsertSound->Name = L"mnuInsertSound";
			this->mnuInsertSound->Size = System::Drawing::Size(154, 22);
			this->mnuInsertSound->Text = L"Sound";
			this->mnuInsertSound->Click += gcnew System::EventHandler(this, &frmMain::mnuInsertSound_Click);
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(13) {this->mnuViewMaterialLibrary, 
				this->mnuViewOutput, this->mnuErrorList, this->mnuViewPropertyManager, this->mnuViewSolutionExplorer, this->mnuViewToolbox, this->mnuFXs, 
				this->toolStripSeparator1, this->toolbarsToolStripMenuItem, this->modeToolStripMenuItem, this->fullToolStripMenuItem, this->toolStripSeparator7, 
				this->propertiesToolStripMenuItem});
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->viewToolStripMenuItem->Text = L"View";
			// 
			// mnuViewMaterialLibrary
			// 
			this->mnuViewMaterialLibrary->Name = L"mnuViewMaterialLibrary";
			this->mnuViewMaterialLibrary->Size = System::Drawing::Size(168, 22);
			this->mnuViewMaterialLibrary->Text = L"Material Library";
			this->mnuViewMaterialLibrary->Click += gcnew System::EventHandler(this, &frmMain::mnuViewMaterialLibrary_Click);
			// 
			// mnuViewOutput
			// 
			this->mnuViewOutput->Name = L"mnuViewOutput";
			this->mnuViewOutput->Size = System::Drawing::Size(168, 22);
			this->mnuViewOutput->Text = L"Output";
			this->mnuViewOutput->Click += gcnew System::EventHandler(this, &frmMain::mnuViewOutput_Click);
			// 
			// mnuErrorList
			// 
			this->mnuErrorList->Name = L"mnuErrorList";
			this->mnuErrorList->Size = System::Drawing::Size(168, 22);
			this->mnuErrorList->Text = L"Error List";
			this->mnuErrorList->Click += gcnew System::EventHandler(this, &frmMain::mnuErrorList_Click);
			// 
			// mnuViewPropertyManager
			// 
			this->mnuViewPropertyManager->Name = L"mnuViewPropertyManager";
			this->mnuViewPropertyManager->Size = System::Drawing::Size(168, 22);
			this->mnuViewPropertyManager->Text = L"Properties";
			this->mnuViewPropertyManager->Click += gcnew System::EventHandler(this, &frmMain::mnuPropertyManager_Click);
			// 
			// mnuViewSolutionExplorer
			// 
			this->mnuViewSolutionExplorer->Name = L"mnuViewSolutionExplorer";
			this->mnuViewSolutionExplorer->Size = System::Drawing::Size(168, 22);
			this->mnuViewSolutionExplorer->Text = L"Solution Explorer";
			this->mnuViewSolutionExplorer->Click += gcnew System::EventHandler(this, &frmMain::mnuViewSolutionExplorer_Click);
			// 
			// mnuViewToolbox
			// 
			this->mnuViewToolbox->Name = L"mnuViewToolbox";
			this->mnuViewToolbox->Size = System::Drawing::Size(168, 22);
			this->mnuViewToolbox->Text = L"Toolbox";
			this->mnuViewToolbox->Click += gcnew System::EventHandler(this, &frmMain::mnuViewToolbox_Click);
			// 
			// mnuFXs
			// 
			this->mnuFXs->Name = L"mnuFXs";
			this->mnuFXs->Size = System::Drawing::Size(168, 22);
			this->mnuFXs->Text = L"Scene Effects";
			this->mnuFXs->Click += gcnew System::EventHandler(this, &frmMain::mnuFXs_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(165, 6);
			// 
			// toolbarsToolStripMenuItem
			// 
			this->toolbarsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->standardToolStripMenuItem});
			this->toolbarsToolStripMenuItem->Name = L"toolbarsToolStripMenuItem";
			this->toolbarsToolStripMenuItem->Size = System::Drawing::Size(168, 22);
			this->toolbarsToolStripMenuItem->Text = L"Toolbars";
			// 
			// standardToolStripMenuItem
			// 
			this->standardToolStripMenuItem->Checked = true;
			this->standardToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->standardToolStripMenuItem->Name = L"standardToolStripMenuItem";
			this->standardToolStripMenuItem->Size = System::Drawing::Size(121, 22);
			this->standardToolStripMenuItem->Text = L"Standard";
			// 
			// modeToolStripMenuItem
			// 
			this->modeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->solidToolStripMenuItem, 
				this->texturedToolStripMenuItem, this->mnuWireframe, this->mnuLighting, this->mnuViewModeHDR, this->mnuViewModeShadows, this->mnuViewModeReflections, 
				this->mnuViewModeDebugInfo});
			this->modeToolStripMenuItem->Name = L"modeToolStripMenuItem";
			this->modeToolStripMenuItem->Size = System::Drawing::Size(168, 22);
			this->modeToolStripMenuItem->Text = L"Mode";
			// 
			// solidToolStripMenuItem
			// 
			this->solidToolStripMenuItem->Checked = true;
			this->solidToolStripMenuItem->CheckOnClick = true;
			this->solidToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->solidToolStripMenuItem->Name = L"solidToolStripMenuItem";
			this->solidToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->solidToolStripMenuItem->Text = L"Solid";
			// 
			// texturedToolStripMenuItem
			// 
			this->texturedToolStripMenuItem->Checked = true;
			this->texturedToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->texturedToolStripMenuItem->Name = L"texturedToolStripMenuItem";
			this->texturedToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->texturedToolStripMenuItem->Text = L"Textured";
			this->texturedToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::texturedToolStripMenuItem_Click);
			// 
			// mnuWireframe
			// 
			this->mnuWireframe->CheckOnClick = true;
			this->mnuWireframe->Name = L"mnuWireframe";
			this->mnuWireframe->Size = System::Drawing::Size(162, 22);
			this->mnuWireframe->Text = L"Wire-frame";
			this->mnuWireframe->Click += gcnew System::EventHandler(this, &frmMain::mnuWireframe_Click);
			// 
			// mnuLighting
			// 
			this->mnuLighting->Checked = true;
			this->mnuLighting->CheckOnClick = true;
			this->mnuLighting->CheckState = System::Windows::Forms::CheckState::Checked;
			this->mnuLighting->Name = L"mnuLighting";
			this->mnuLighting->Size = System::Drawing::Size(162, 22);
			this->mnuLighting->Text = L"Lighting";
			this->mnuLighting->Click += gcnew System::EventHandler(this, &frmMain::mnuLighting_Click);
			// 
			// mnuViewModeHDR
			// 
			this->mnuViewModeHDR->CheckOnClick = true;
			this->mnuViewModeHDR->Name = L"mnuViewModeHDR";
			this->mnuViewModeHDR->Size = System::Drawing::Size(162, 22);
			this->mnuViewModeHDR->Text = L"HDR";
			this->mnuViewModeHDR->Click += gcnew System::EventHandler(this, &frmMain::mnuViewModeHDR_Click);
			// 
			// mnuViewModeShadows
			// 
			this->mnuViewModeShadows->Checked = true;
			this->mnuViewModeShadows->CheckOnClick = true;
			this->mnuViewModeShadows->CheckState = System::Windows::Forms::CheckState::Checked;
			this->mnuViewModeShadows->Name = L"mnuViewModeShadows";
			this->mnuViewModeShadows->Size = System::Drawing::Size(162, 22);
			this->mnuViewModeShadows->Text = L"Shadows";
			this->mnuViewModeShadows->Click += gcnew System::EventHandler(this, &frmMain::mnuViewModeShadows_Click);
			// 
			// mnuViewModeReflections
			// 
			this->mnuViewModeReflections->Checked = true;
			this->mnuViewModeReflections->CheckOnClick = true;
			this->mnuViewModeReflections->CheckState = System::Windows::Forms::CheckState::Checked;
			this->mnuViewModeReflections->Name = L"mnuViewModeReflections";
			this->mnuViewModeReflections->Size = System::Drawing::Size(162, 22);
			this->mnuViewModeReflections->Text = L"Reflections";
			this->mnuViewModeReflections->Click += gcnew System::EventHandler(this, &frmMain::mnuViewModeReflections_Click);
			// 
			// mnuViewModeDebugInfo
			// 
			this->mnuViewModeDebugInfo->Checked = true;
			this->mnuViewModeDebugInfo->CheckOnClick = true;
			this->mnuViewModeDebugInfo->CheckState = System::Windows::Forms::CheckState::Checked;
			this->mnuViewModeDebugInfo->Name = L"mnuViewModeDebugInfo";
			this->mnuViewModeDebugInfo->Size = System::Drawing::Size(162, 22);
			this->mnuViewModeDebugInfo->Text = L"Draw debug info";
			this->mnuViewModeDebugInfo->Click += gcnew System::EventHandler(this, &frmMain::mnuViewModeDebugInfo_Click);
			// 
			// fullToolStripMenuItem
			// 
			this->fullToolStripMenuItem->CheckOnClick = true;
			this->fullToolStripMenuItem->Name = L"fullToolStripMenuItem";
			this->fullToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::X));
			this->fullToolStripMenuItem->Size = System::Drawing::Size(168, 22);
			this->fullToolStripMenuItem->Text = L"Full Screen";
			this->fullToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::fullToolStripMenuItem_Click);
			// 
			// toolStripSeparator7
			// 
			this->toolStripSeparator7->Name = L"toolStripSeparator7";
			this->toolStripSeparator7->Size = System::Drawing::Size(165, 6);
			// 
			// propertiesToolStripMenuItem
			// 
			this->propertiesToolStripMenuItem->Name = L"propertiesToolStripMenuItem";
			this->propertiesToolStripMenuItem->Size = System::Drawing::Size(168, 22);
			this->propertiesToolStripMenuItem->Text = L"Properties";
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->materialLibraryToolStripMenuItem, 
				this->toolStripSeparator6, this->scenegraphEditorToolStripMenuItem, this->toolStripSeparator14, this->customizeToolStripMenuItem, 
				this->mnuToolsOptions});
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->toolsToolStripMenuItem->Text = L"Tools";
			// 
			// materialLibraryToolStripMenuItem
			// 
			this->materialLibraryToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->mnuContentAuthoringShaderEditor, 
				this->mnuMaterialEditor, this->materialLibraryToolStripMenuItem1, this->toolStripSeparator11, this->animationEditorToolStripMenuItem, 
				this->textureEditorToolStripMenuItem, this->toolStripSeparator15, this->mnuScriptEditor, this->toolStripSeparator27, this->mnuHeightMapEditor});
			this->materialLibraryToolStripMenuItem->Name = L"materialLibraryToolStripMenuItem";
			this->materialLibraryToolStripMenuItem->Size = System::Drawing::Size(175, 22);
			this->materialLibraryToolStripMenuItem->Text = L"Content Authoring";
			// 
			// mnuContentAuthoringShaderEditor
			// 
			this->mnuContentAuthoringShaderEditor->Name = L"mnuContentAuthoringShaderEditor";
			this->mnuContentAuthoringShaderEditor->Size = System::Drawing::Size(173, 22);
			this->mnuContentAuthoringShaderEditor->Text = L"Shader Editor";
			this->mnuContentAuthoringShaderEditor->Click += gcnew System::EventHandler(this, &frmMain::mnuContentAuthoringShaderEditor_Click);
			// 
			// mnuMaterialEditor
			// 
			this->mnuMaterialEditor->Name = L"mnuMaterialEditor";
			this->mnuMaterialEditor->Size = System::Drawing::Size(173, 22);
			this->mnuMaterialEditor->Text = L"Material Editor";
			this->mnuMaterialEditor->Click += gcnew System::EventHandler(this, &frmMain::mnuMaterialEditor_Click);
			// 
			// materialLibraryToolStripMenuItem1
			// 
			this->materialLibraryToolStripMenuItem1->Name = L"materialLibraryToolStripMenuItem1";
			this->materialLibraryToolStripMenuItem1->Size = System::Drawing::Size(173, 22);
			this->materialLibraryToolStripMenuItem1->Text = L"Material Library";
			// 
			// toolStripSeparator11
			// 
			this->toolStripSeparator11->Name = L"toolStripSeparator11";
			this->toolStripSeparator11->Size = System::Drawing::Size(170, 6);
			// 
			// animationEditorToolStripMenuItem
			// 
			this->animationEditorToolStripMenuItem->Name = L"animationEditorToolStripMenuItem";
			this->animationEditorToolStripMenuItem->Size = System::Drawing::Size(173, 22);
			this->animationEditorToolStripMenuItem->Text = L"Animation Editor";
			// 
			// textureEditorToolStripMenuItem
			// 
			this->textureEditorToolStripMenuItem->Name = L"textureEditorToolStripMenuItem";
			this->textureEditorToolStripMenuItem->Size = System::Drawing::Size(173, 22);
			this->textureEditorToolStripMenuItem->Text = L"Texture Editor";
			// 
			// toolStripSeparator15
			// 
			this->toolStripSeparator15->Name = L"toolStripSeparator15";
			this->toolStripSeparator15->Size = System::Drawing::Size(170, 6);
			// 
			// mnuScriptEditor
			// 
			this->mnuScriptEditor->Name = L"mnuScriptEditor";
			this->mnuScriptEditor->Size = System::Drawing::Size(173, 22);
			this->mnuScriptEditor->Text = L"Script Editor";
			this->mnuScriptEditor->Click += gcnew System::EventHandler(this, &frmMain::mnuScriptEditor_Click);
			// 
			// toolStripSeparator27
			// 
			this->toolStripSeparator27->Name = L"toolStripSeparator27";
			this->toolStripSeparator27->Size = System::Drawing::Size(170, 6);
			// 
			// mnuHeightMapEditor
			// 
			this->mnuHeightMapEditor->Name = L"mnuHeightMapEditor";
			this->mnuHeightMapEditor->Size = System::Drawing::Size(173, 22);
			this->mnuHeightMapEditor->Text = L"Height-map Editor";
			this->mnuHeightMapEditor->Click += gcnew System::EventHandler(this, &frmMain::mnuHeightMapEditor_Click);
			// 
			// toolStripSeparator6
			// 
			this->toolStripSeparator6->Name = L"toolStripSeparator6";
			this->toolStripSeparator6->Size = System::Drawing::Size(172, 6);
			// 
			// scenegraphEditorToolStripMenuItem
			// 
			this->scenegraphEditorToolStripMenuItem->Name = L"scenegraphEditorToolStripMenuItem";
			this->scenegraphEditorToolStripMenuItem->Size = System::Drawing::Size(175, 22);
			this->scenegraphEditorToolStripMenuItem->Text = L"Scene-graph Editor";
			// 
			// toolStripSeparator14
			// 
			this->toolStripSeparator14->Name = L"toolStripSeparator14";
			this->toolStripSeparator14->Size = System::Drawing::Size(172, 6);
			// 
			// customizeToolStripMenuItem
			// 
			this->customizeToolStripMenuItem->Name = L"customizeToolStripMenuItem";
			this->customizeToolStripMenuItem->Size = System::Drawing::Size(175, 22);
			this->customizeToolStripMenuItem->Text = L"Customize...";
			// 
			// mnuToolsOptions
			// 
			this->mnuToolsOptions->Name = L"mnuToolsOptions";
			this->mnuToolsOptions->Size = System::Drawing::Size(175, 22);
			this->mnuToolsOptions->Text = L"Options...";
			this->mnuToolsOptions->Click += gcnew System::EventHandler(this, &frmMain::mnuToolsOptions_Click);
			// 
			// simulationToolStripMenuItem
			// 
			this->simulationToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->pausedToolStripMenuItem, 
				this->modesToolStripMenuItem, this->renderingToolStripMenuItem});
			this->simulationToolStripMenuItem->Name = L"simulationToolStripMenuItem";
			this->simulationToolStripMenuItem->Size = System::Drawing::Size(76, 20);
			this->simulationToolStripMenuItem->Text = L"Simulation";
			// 
			// pausedToolStripMenuItem
			// 
			this->pausedToolStripMenuItem->Checked = true;
			this->pausedToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->pausedToolStripMenuItem->Name = L"pausedToolStripMenuItem";
			this->pausedToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->pausedToolStripMenuItem->Text = L"Paused";
			// 
			// modesToolStripMenuItem
			// 
			this->modesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->aIToolStripMenuItem, 
				this->mnuSimulationModesPhysics, this->gameplayToolStripMenuItem});
			this->modesToolStripMenuItem->Name = L"modesToolStripMenuItem";
			this->modesToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->modesToolStripMenuItem->Text = L"Modes";
			// 
			// aIToolStripMenuItem
			// 
			this->aIToolStripMenuItem->Checked = true;
			this->aIToolStripMenuItem->CheckOnClick = true;
			this->aIToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->aIToolStripMenuItem->Name = L"aIToolStripMenuItem";
			this->aIToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->aIToolStripMenuItem->Text = L"AI";
			// 
			// mnuSimulationModesPhysics
			// 
			this->mnuSimulationModesPhysics->Checked = true;
			this->mnuSimulationModesPhysics->CheckOnClick = true;
			this->mnuSimulationModesPhysics->CheckState = System::Windows::Forms::CheckState::Checked;
			this->mnuSimulationModesPhysics->Name = L"mnuSimulationModesPhysics";
			this->mnuSimulationModesPhysics->Size = System::Drawing::Size(152, 22);
			this->mnuSimulationModesPhysics->Text = L"Physics";
			this->mnuSimulationModesPhysics->Click += gcnew System::EventHandler(this, &frmMain::mnuSimulationModesPhysics_Click);
			// 
			// gameplayToolStripMenuItem
			// 
			this->gameplayToolStripMenuItem->Checked = true;
			this->gameplayToolStripMenuItem->CheckOnClick = true;
			this->gameplayToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->gameplayToolStripMenuItem->Name = L"gameplayToolStripMenuItem";
			this->gameplayToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->gameplayToolStripMenuItem->Text = L"Gameplay";
			// 
			// windowToolStripMenuItem
			// 
			this->windowToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->mnuWindowSaveLayout});
			this->windowToolStripMenuItem->Name = L"windowToolStripMenuItem";
			this->windowToolStripMenuItem->Size = System::Drawing::Size(63, 20);
			this->windowToolStripMenuItem->Text = L"Window";
			// 
			// mnuWindowSaveLayout
			// 
			this->mnuWindowSaveLayout->Name = L"mnuWindowSaveLayout";
			this->mnuWindowSaveLayout->Size = System::Drawing::Size(134, 22);
			this->mnuWindowSaveLayout->Text = L"Save layout";
			this->mnuWindowSaveLayout->Click += gcnew System::EventHandler(this, &frmMain::mnuWindowSaveLayout_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->searchToolStripMenuItem, 
				this->contentsToolStripMenuItem, this->indexToolStripMenuItem, this->toolStripSeparator3, this->mnuNGENEOnTheWeb, this->mnuHelpTutorials, 
				this->technicalSupportToolStripMenuItem, this->toolStripSeparator4, this->mnuAbout});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			this->helpToolStripMenuItem->Click += gcnew System::EventHandler(this, &frmMain::helpToolStripMenuItem_Click);
			// 
			// searchToolStripMenuItem
			// 
			this->searchToolStripMenuItem->Name = L"searchToolStripMenuItem";
			this->searchToolStripMenuItem->Size = System::Drawing::Size(174, 22);
			this->searchToolStripMenuItem->Text = L"Search";
			// 
			// contentsToolStripMenuItem
			// 
			this->contentsToolStripMenuItem->Name = L"contentsToolStripMenuItem";
			this->contentsToolStripMenuItem->Size = System::Drawing::Size(174, 22);
			this->contentsToolStripMenuItem->Text = L"Contents";
			// 
			// indexToolStripMenuItem
			// 
			this->indexToolStripMenuItem->Name = L"indexToolStripMenuItem";
			this->indexToolStripMenuItem->Size = System::Drawing::Size(174, 22);
			this->indexToolStripMenuItem->Text = L"Index";
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(171, 6);
			// 
			// mnuNGENEOnTheWeb
			// 
			this->mnuNGENEOnTheWeb->Name = L"mnuNGENEOnTheWeb";
			this->mnuNGENEOnTheWeb->Size = System::Drawing::Size(174, 22);
			this->mnuNGENEOnTheWeb->Text = L"nGENE on the Web";
			this->mnuNGENEOnTheWeb->Click += gcnew System::EventHandler(this, &frmMain::mnuNGENEOnTheWeb_Click);
			// 
			// mnuHelpTutorials
			// 
			this->mnuHelpTutorials->Name = L"mnuHelpTutorials";
			this->mnuHelpTutorials->Size = System::Drawing::Size(174, 22);
			this->mnuHelpTutorials->Text = L"Tutorials";
			this->mnuHelpTutorials->Click += gcnew System::EventHandler(this, &frmMain::mnuHelpTutorials_Click);
			// 
			// technicalSupportToolStripMenuItem
			// 
			this->technicalSupportToolStripMenuItem->Name = L"technicalSupportToolStripMenuItem";
			this->technicalSupportToolStripMenuItem->Size = System::Drawing::Size(174, 22);
			this->technicalSupportToolStripMenuItem->Text = L"Technical Support";
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(171, 6);
			// 
			// mnuAbout
			// 
			this->mnuAbout->Name = L"mnuAbout";
			this->mnuAbout->Size = System::Drawing::Size(174, 22);
			this->mnuAbout->Text = L"About";
			this->mnuAbout->Click += gcnew System::EventHandler(this, &frmMain::mnuAbout_Click);
			// 
			// toolStrip1
			// 
			this->toolStrip1->AllowItemReorder = true;
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(17) {this->tbsNewProject, this->tsbOpen, 
				this->tsbSave, this->toolStripSeparator8, this->toolStripButton4, this->toolStripButton5, this->toolStripButton6, this->toolStripSeparator9, 
				this->tbsRemoveNode, this->toolStripSeparator17, this->toolStripButton7, this->toolStripButton8, this->toolStripSeparator10, 
				this->tsbLockX, this->tsbLockY, this->tsbLockZ, this->toolStripSeparator19});
			this->toolStrip1->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::HorizontalStackWithOverflow;
			this->toolStrip1->Location = System::Drawing::Point(0, 24);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(1158, 25);
			this->toolStrip1->TabIndex = 1;
			this->toolStrip1->Text = L"toolStrip1";
			this->toolStrip1->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &frmMain::toolStrip1_ItemClicked);
			// 
			// tbsNewProject
			// 
			this->tbsNewProject->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsNewProject->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsNewProject.Image")));
			this->tbsNewProject->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsNewProject->Name = L"tbsNewProject";
			this->tbsNewProject->Size = System::Drawing::Size(23, 22);
			this->tbsNewProject->Text = L"Create new world";
			this->tbsNewProject->Click += gcnew System::EventHandler(this, &frmMain::tbsNewProject_Click);
			// 
			// tsbOpen
			// 
			this->tsbOpen->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbOpen->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbOpen.Image")));
			this->tsbOpen->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbOpen->Name = L"tsbOpen";
			this->tsbOpen->Size = System::Drawing::Size(23, 22);
			this->tsbOpen->Text = L"toolStripButton2";
			this->tsbOpen->ToolTipText = L"Open existing map file";
			this->tsbOpen->Click += gcnew System::EventHandler(this, &frmMain::tsbOpen_Click);
			// 
			// tsbSave
			// 
			this->tsbSave->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbSave->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbSave.Image")));
			this->tsbSave->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbSave->Name = L"tsbSave";
			this->tsbSave->Size = System::Drawing::Size(23, 22);
			this->tsbSave->Text = L"toolStripButton3";
			this->tsbSave->ToolTipText = L"Save world";
			this->tsbSave->Click += gcnew System::EventHandler(this, &frmMain::tsbSave_Click);
			// 
			// toolStripSeparator8
			// 
			this->toolStripSeparator8->Name = L"toolStripSeparator8";
			this->toolStripSeparator8->Size = System::Drawing::Size(6, 25);
			// 
			// toolStripButton4
			// 
			this->toolStripButton4->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton4->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton4.Image")));
			this->toolStripButton4->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton4->Name = L"toolStripButton4";
			this->toolStripButton4->Size = System::Drawing::Size(23, 22);
			this->toolStripButton4->Text = L"toolStripButton4";
			// 
			// toolStripButton5
			// 
			this->toolStripButton5->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton5->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton5.Image")));
			this->toolStripButton5->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton5->Name = L"toolStripButton5";
			this->toolStripButton5->Size = System::Drawing::Size(23, 22);
			this->toolStripButton5->Text = L"toolStripButton5";
			// 
			// toolStripButton6
			// 
			this->toolStripButton6->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton6->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton6.Image")));
			this->toolStripButton6->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton6->Name = L"toolStripButton6";
			this->toolStripButton6->Size = System::Drawing::Size(23, 22);
			this->toolStripButton6->Text = L"toolStripButton6";
			// 
			// toolStripSeparator9
			// 
			this->toolStripSeparator9->Name = L"toolStripSeparator9";
			this->toolStripSeparator9->Size = System::Drawing::Size(6, 25);
			// 
			// tbsRemoveNode
			// 
			this->tbsRemoveNode->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsRemoveNode->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsRemoveNode.Image")));
			this->tbsRemoveNode->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsRemoveNode->Name = L"tbsRemoveNode";
			this->tbsRemoveNode->Size = System::Drawing::Size(23, 22);
			this->tbsRemoveNode->Text = L"toolStripButton9";
			this->tbsRemoveNode->ToolTipText = L"Delete selected node";
			this->tbsRemoveNode->Click += gcnew System::EventHandler(this, &frmMain::tbsRemoveNode_Click);
			// 
			// toolStripSeparator17
			// 
			this->toolStripSeparator17->Name = L"toolStripSeparator17";
			this->toolStripSeparator17->Size = System::Drawing::Size(6, 25);
			// 
			// toolStripButton7
			// 
			this->toolStripButton7->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton7->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton7.Image")));
			this->toolStripButton7->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton7->Name = L"toolStripButton7";
			this->toolStripButton7->Size = System::Drawing::Size(23, 22);
			this->toolStripButton7->Text = L"toolStripButton7";
			// 
			// toolStripButton8
			// 
			this->toolStripButton8->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton8->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton8.Image")));
			this->toolStripButton8->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton8->Name = L"toolStripButton8";
			this->toolStripButton8->Size = System::Drawing::Size(23, 22);
			this->toolStripButton8->Text = L"toolStripButton8";
			// 
			// toolStripSeparator10
			// 
			this->toolStripSeparator10->Name = L"toolStripSeparator10";
			this->toolStripSeparator10->Size = System::Drawing::Size(6, 25);
			// 
			// tsbLockX
			// 
			this->tsbLockX->Checked = true;
			this->tsbLockX->CheckOnClick = true;
			this->tsbLockX->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tsbLockX->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->tsbLockX->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbLockX.Image")));
			this->tsbLockX->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbLockX->Name = L"tsbLockX";
			this->tsbLockX->Size = System::Drawing::Size(23, 22);
			this->tsbLockX->Text = L"X";
			this->tsbLockX->ToolTipText = L"Lock x axis";
			this->tsbLockX->Click += gcnew System::EventHandler(this, &frmMain::tsbLockX_Click);
			// 
			// tsbLockY
			// 
			this->tsbLockY->Checked = true;
			this->tsbLockY->CheckOnClick = true;
			this->tsbLockY->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tsbLockY->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->tsbLockY->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbLockY.Image")));
			this->tsbLockY->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbLockY->Name = L"tsbLockY";
			this->tsbLockY->Size = System::Drawing::Size(23, 22);
			this->tsbLockY->Text = L"Y";
			this->tsbLockY->ToolTipText = L"Lock y axis";
			this->tsbLockY->Click += gcnew System::EventHandler(this, &frmMain::tsbLockY_Click);
			// 
			// tsbLockZ
			// 
			this->tsbLockZ->Checked = true;
			this->tsbLockZ->CheckOnClick = true;
			this->tsbLockZ->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tsbLockZ->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->tsbLockZ->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbLockZ.Image")));
			this->tsbLockZ->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbLockZ->Name = L"tsbLockZ";
			this->tsbLockZ->Size = System::Drawing::Size(23, 22);
			this->tsbLockZ->Text = L"Z";
			this->tsbLockZ->ToolTipText = L"Lock z axis";
			this->tsbLockZ->Click += gcnew System::EventHandler(this, &frmMain::tsbLockZ_Click);
			// 
			// toolStripSeparator19
			// 
			this->toolStripSeparator19->Name = L"toolStripSeparator19";
			this->toolStripSeparator19->Size = System::Drawing::Size(6, 25);
			// 
			// stsStrip
			// 
			this->stsStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->tssLabel, this->tssFPS});
			this->stsStrip->Location = System::Drawing::Point(0, 530);
			this->stsStrip->Name = L"stsStrip";
			this->stsStrip->Padding = System::Windows::Forms::Padding(1, 0, 10, 0);
			this->stsStrip->Size = System::Drawing::Size(1158, 22);
			this->stsStrip->TabIndex = 2;
			this->stsStrip->Text = L"statusStrip1";
			// 
			// tssLabel
			// 
			this->tssLabel->Name = L"tssLabel";
			this->tssLabel->Size = System::Drawing::Size(39, 17);
			this->tssLabel->Text = L"Ready";
			// 
			// tssFPS
			// 
			this->tssFPS->Name = L"tssFPS";
			this->tssFPS->Size = System::Drawing::Size(38, 17);
			this->tssFPS->Text = L"FPS: 0";
			// 
			// toolStrip2
			// 
			this->toolStrip2->AllowItemReorder = true;
			this->toolStrip2->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(20) {this->tbsLight, this->toolStripSeparator21, 
				this->tbsBox, this->tbsSphere, this->tbsPlane, this->toolStripSeparator22, this->tbsCamera, this->toolStripSeparator23, this->tbsTerrain, 
				this->tbsSky, this->tbsClouds, this->tbsWater, this->toolStripSeparator24, this->tbsParticleSystem, this->tbsParticleEmitter, 
				this->toolStripSeparator25, this->tbsMesh, this->toolStripSeparator26, this->tbsSound, this->toolStripSeparator30});
			this->toolStrip2->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::HorizontalStackWithOverflow;
			this->toolStrip2->Location = System::Drawing::Point(0, 49);
			this->toolStrip2->Name = L"toolStrip2";
			this->toolStrip2->Size = System::Drawing::Size(1158, 25);
			this->toolStrip2->TabIndex = 2;
			this->toolStrip2->Text = L"toolStrip2";
			// 
			// tbsLight
			// 
			this->tbsLight->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsLight->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsLight.Image")));
			this->tbsLight->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsLight->Name = L"tbsLight";
			this->tbsLight->Size = System::Drawing::Size(23, 22);
			this->tbsLight->Text = L"toolStripButton1";
			this->tbsLight->ToolTipText = L"Add new light object";
			this->tbsLight->Click += gcnew System::EventHandler(this, &frmMain::tbsLight_Click);
			// 
			// toolStripSeparator21
			// 
			this->toolStripSeparator21->Name = L"toolStripSeparator21";
			this->toolStripSeparator21->Size = System::Drawing::Size(6, 25);
			// 
			// tbsBox
			// 
			this->tbsBox->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsBox->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsBox.Image")));
			this->tbsBox->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsBox->Name = L"tbsBox";
			this->tbsBox->Size = System::Drawing::Size(23, 22);
			this->tbsBox->Text = L"toolStripButton1";
			this->tbsBox->ToolTipText = L"Add new box object";
			this->tbsBox->Click += gcnew System::EventHandler(this, &frmMain::tbsBox_Click);
			// 
			// tbsSphere
			// 
			this->tbsSphere->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsSphere->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsSphere.Image")));
			this->tbsSphere->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsSphere->Name = L"tbsSphere";
			this->tbsSphere->Size = System::Drawing::Size(23, 22);
			this->tbsSphere->Text = L"toolStripButton1";
			this->tbsSphere->ToolTipText = L"Add new sphere object";
			this->tbsSphere->Click += gcnew System::EventHandler(this, &frmMain::tbsSphere_Click);
			// 
			// tbsPlane
			// 
			this->tbsPlane->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsPlane->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsPlane.Image")));
			this->tbsPlane->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsPlane->Name = L"tbsPlane";
			this->tbsPlane->Size = System::Drawing::Size(23, 22);
			this->tbsPlane->Text = L"toolStripButton1";
			this->tbsPlane->ToolTipText = L"Add new plane object";
			this->tbsPlane->Click += gcnew System::EventHandler(this, &frmMain::tbsPlane_Click);
			// 
			// toolStripSeparator22
			// 
			this->toolStripSeparator22->Name = L"toolStripSeparator22";
			this->toolStripSeparator22->Size = System::Drawing::Size(6, 25);
			// 
			// tbsCamera
			// 
			this->tbsCamera->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsCamera->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsCamera.Image")));
			this->tbsCamera->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsCamera->Name = L"tbsCamera";
			this->tbsCamera->Size = System::Drawing::Size(23, 22);
			this->tbsCamera->Text = L"toolStripButton1";
			this->tbsCamera->ToolTipText = L"Add new camera object";
			this->tbsCamera->Click += gcnew System::EventHandler(this, &frmMain::tbsCamera_Click);
			// 
			// toolStripSeparator23
			// 
			this->toolStripSeparator23->Name = L"toolStripSeparator23";
			this->toolStripSeparator23->Size = System::Drawing::Size(6, 25);
			// 
			// tbsTerrain
			// 
			this->tbsTerrain->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsTerrain->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsTerrain.Image")));
			this->tbsTerrain->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsTerrain->Name = L"tbsTerrain";
			this->tbsTerrain->Size = System::Drawing::Size(23, 22);
			this->tbsTerrain->Text = L"toolStripButton1";
			this->tbsTerrain->ToolTipText = L"Add new terrain object";
			this->tbsTerrain->Click += gcnew System::EventHandler(this, &frmMain::tbsTerrain_Click);
			// 
			// tbsSky
			// 
			this->tbsSky->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsSky->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsSky.Image")));
			this->tbsSky->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsSky->Name = L"tbsSky";
			this->tbsSky->Size = System::Drawing::Size(23, 22);
			this->tbsSky->Text = L"toolStripButton1";
			this->tbsSky->ToolTipText = L"Add new sky object";
			this->tbsSky->Click += gcnew System::EventHandler(this, &frmMain::tbsSky_Click);
			// 
			// tbsClouds
			// 
			this->tbsClouds->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsClouds->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsClouds.Image")));
			this->tbsClouds->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsClouds->Name = L"tbsClouds";
			this->tbsClouds->Size = System::Drawing::Size(23, 22);
			this->tbsClouds->Text = L"toolStripButton2";
			this->tbsClouds->ToolTipText = L"Add new clouds object";
			this->tbsClouds->Click += gcnew System::EventHandler(this, &frmMain::tbsClouds_Click);
			// 
			// tbsWater
			// 
			this->tbsWater->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsWater->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsWater.Image")));
			this->tbsWater->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsWater->Name = L"tbsWater";
			this->tbsWater->Size = System::Drawing::Size(23, 22);
			this->tbsWater->Text = L"toolStripButton1";
			this->tbsWater->ToolTipText = L"Add new water object";
			this->tbsWater->Click += gcnew System::EventHandler(this, &frmMain::tbsWater_Click);
			// 
			// toolStripSeparator24
			// 
			this->toolStripSeparator24->Name = L"toolStripSeparator24";
			this->toolStripSeparator24->Size = System::Drawing::Size(6, 25);
			// 
			// tbsParticleSystem
			// 
			this->tbsParticleSystem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsParticleSystem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsParticleSystem.Image")));
			this->tbsParticleSystem->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsParticleSystem->Name = L"tbsParticleSystem";
			this->tbsParticleSystem->Size = System::Drawing::Size(23, 22);
			this->tbsParticleSystem->Text = L"toolStripButton1";
			this->tbsParticleSystem->ToolTipText = L"Add new particle system";
			this->tbsParticleSystem->Click += gcnew System::EventHandler(this, &frmMain::tbsParticleSystem_Click);
			// 
			// tbsParticleEmitter
			// 
			this->tbsParticleEmitter->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsParticleEmitter->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsParticleEmitter.Image")));
			this->tbsParticleEmitter->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsParticleEmitter->Name = L"tbsParticleEmitter";
			this->tbsParticleEmitter->Size = System::Drawing::Size(23, 22);
			this->tbsParticleEmitter->Text = L"toolStripButton2";
			this->tbsParticleEmitter->ToolTipText = L"Add new particle emitter object";
			this->tbsParticleEmitter->Click += gcnew System::EventHandler(this, &frmMain::tbsParticleEmitter_Click);
			// 
			// toolStripSeparator25
			// 
			this->toolStripSeparator25->Name = L"toolStripSeparator25";
			this->toolStripSeparator25->Size = System::Drawing::Size(6, 25);
			// 
			// tbsMesh
			// 
			this->tbsMesh->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsMesh->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsMesh.Image")));
			this->tbsMesh->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsMesh->Name = L"tbsMesh";
			this->tbsMesh->Size = System::Drawing::Size(23, 22);
			this->tbsMesh->Text = L"toolStripButton1";
			this->tbsMesh->ToolTipText = L"Add new mesh object";
			this->tbsMesh->Click += gcnew System::EventHandler(this, &frmMain::tbsMesh_Click);
			// 
			// toolStripSeparator26
			// 
			this->toolStripSeparator26->Name = L"toolStripSeparator26";
			this->toolStripSeparator26->Size = System::Drawing::Size(6, 25);
			// 
			// tbsSound
			// 
			this->tbsSound->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbsSound->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tbsSound.Image")));
			this->tbsSound->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbsSound->Name = L"tbsSound";
			this->tbsSound->Size = System::Drawing::Size(23, 22);
			this->tbsSound->Text = L"Add new sound object";
			this->tbsSound->Click += gcnew System::EventHandler(this, &frmMain::tbsSound_Click);
			// 
			// toolStripSeparator30
			// 
			this->toolStripSeparator30->Name = L"toolStripSeparator30";
			this->toolStripSeparator30->Size = System::Drawing::Size(6, 25);
			// 
			// dockManager
			// 
			this->dockManager->ActiveAutoHideContent = nullptr;
			this->dockManager->AllowEndUserDocking = true;
			this->dockManager->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dockManager->Location = System::Drawing::Point(0, 74);
			this->dockManager->Margin = System::Windows::Forms::Padding(2);
			this->dockManager->Name = L"dockManager";
			this->dockManager->Size = System::Drawing::Size(1158, 456);
			this->dockManager->TabIndex = 9;
			// 
			// renderingToolStripMenuItem
			// 
			this->renderingToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->mnuSimulationRenderingContinuous, 
				this->mnuSimulationRenderingRequestRender});
			this->renderingToolStripMenuItem->Name = L"renderingToolStripMenuItem";
			this->renderingToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->renderingToolStripMenuItem->Text = L"Rendering";
			// 
			// mnuSimulationRenderingContinuous
			// 
			this->mnuSimulationRenderingContinuous->Checked = true;
			this->mnuSimulationRenderingContinuous->CheckOnClick = true;
			this->mnuSimulationRenderingContinuous->CheckState = System::Windows::Forms::CheckState::Checked;
			this->mnuSimulationRenderingContinuous->Name = L"mnuSimulationRenderingContinuous";
			this->mnuSimulationRenderingContinuous->Size = System::Drawing::Size(153, 22);
			this->mnuSimulationRenderingContinuous->Text = L"Continuous";
			this->mnuSimulationRenderingContinuous->Click += gcnew System::EventHandler(this, &frmMain::mnuSimulationRenderingContinuous_Click);
			// 
			// mnuSimulationRenderingRequestRender
			// 
			this->mnuSimulationRenderingRequestRender->Enabled = false;
			this->mnuSimulationRenderingRequestRender->Name = L"mnuSimulationRenderingRequestRender";
			this->mnuSimulationRenderingRequestRender->Size = System::Drawing::Size(153, 22);
			this->mnuSimulationRenderingRequestRender->Text = L"Request render";
			this->mnuSimulationRenderingRequestRender->Click += gcnew System::EventHandler(this, &frmMain::mnuSimulationRenderingRequestRender_Click);
			// 
			// frmMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1158, 552);
			this->Controls->Add(this->dockManager);
			this->Controls->Add(this->toolStrip2);
			this->Controls->Add(this->stsStrip);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->mnsMainMenu);
			this->IsMdiContainer = true;
			this->KeyPreview = true;
			this->Location = System::Drawing::Point(0, 28);
			this->MainMenuStrip = this->mnsMainMenu;
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"frmMain";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"nGENE Tech Toolset";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &frmMain::frmMain_Load);
			this->mnsMainMenu->ResumeLayout(false);
			this->mnsMainMenu->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->stsStrip->ResumeLayout(false);
			this->stsStrip->PerformLayout();
			this->toolStrip2->ResumeLayout(false);
			this->toolStrip2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		WeifenLuo::WinFormsUI::Docking::IDockContent^ GetContentFromPersistString(System::String^ persistString)
		{
			if (persistString == frmRenderTarget::typeid->ToString())
				return target;
			else if (persistString == frmPropertyManager::typeid->ToString())
				return frmProperty;
			else if (persistString == frmToolbox::typeid->ToString())
				return frmTools;
			else if (persistString == frmFXs::typeid->ToString())
				return frmFX;
			else if (persistString == frmSolutionExplorer::typeid->ToString())
				return frmExplorer;
			else if (persistString == frmMaterialLib::typeid->ToString())
				return frmMatLib;
			else if (persistString == frmSceneGraph::typeid->ToString())
				return frmGraph;
			else if (persistString == frmOutput::typeid->ToString())
				return frmOut;
			else if (persistString == frmErrorList::typeid->ToString())
				return frmError;
			else
				return nullptr;
		}

	private:
		System::Void frmMain_Load(System::Object^  sender, System::EventArgs^  e)
		{
			// Create windows
			frmAboutBox = gcnew frmAbout();
			frmTools = gcnew frmToolbox();
			frmOut = gcnew frmOutput();
			target = gcnew frmRenderTarget(tssFPS);
			frmProperty = gcnew frmPropertyManager();			
			frmFX = gcnew frmFXs();
			frmBrowser = gcnew frmWebBrowser();
			frmExplorer = gcnew frmSolutionExplorer();
			frmMatLib = gcnew frmMaterialLib(frmProperty, frmFX);
			frmGraph = gcnew frmSceneGraph(frmProperty);
			frmError = gcnew frmErrorList();

			frmHeight = gcnew frmHeightMapEditor(tssLabel);
			frmShader = gcnew ShaderEditor();
			frmScript = gcnew frmScriptEditor();

			// Load layout from file
			deserializeDockContent = gcnew DeserializeDockContent(this,
				&frmMain::GetContentFromPersistString);
			if(System::IO::File::Exists("layout.xml"))
				dockManager->LoadFromXml("layout.xml", deserializeDockContent);

			frmProperty->Show(dockManager);

			frmTools->Show(dockManager);
			frmExplorer->Show(dockManager);
			frmMatLib->Show(dockManager);
			frmFX->Show(dockManager);
			frmGraph->Show(dockManager);
			frmError->Show(dockManager);
			frmOut->Show(dockManager);

			//frmBrowser->Show(dockManager, DockState::Document);
			target->Show(dockManager, DockState::Document);

			this->Invalidate();
			this->Update();

			target->init();

			frmMatLib->addLibrary(L"default");

			frmProperty->SelectedObject = frmRenderTarget::engine;

			// Get scene manager
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			NodeWrapper^ root = gcnew NodeWrapper(sm->getRootNode(), "Scene Root", 0);
			camera = gcnew CameraWrapper();
			camera->Aspect = (float)target->Width / (float)target->Height;
	
			// Create partitioner
			QUAD_TREE_DESC quad_tree;
			quad_tree.bounds.left = -5000.0f;
			quad_tree.bounds.right= 5000.0f;
			quad_tree.bounds.top = -5000.0f;
			quad_tree.bounds.bottom = 5000.0f;
			quad_tree.minimumNodeSize = 50.0f;
			Assert(quad_tree.isValid(), "Invalid quad tree description!");
			ScenePartitioner* m_pPartitioner = new ScenePartitionerQuadTree(quad_tree);
			sm->setScenePartitioner(m_pPartitioner);

			Log::log(LET_EVENT, L"Editor", __WFILE__, __WFUNCTION__, __LINE__,
				L"-----------------------------------------------------------------------------------------");
			Log::log(LET_EVENT, L"Editor", __WFILE__, __WFUNCTION__, __LINE__,
				L"nGENE Tech Editor loaded");
		}

		System::Void helpToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
		}

		System::Void insertToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
		}

		System::Void newToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
		}
private: System::Void mnuPropertyManager_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmProperty->Visible)
				 frmProperty->Hide();
			 else
				 frmProperty->Show(dockManager);
		 }
private: System::Void texturedToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void fullToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void mnuMaterialEditor_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmMatEdit != nullptr)
			 {
				 delete frmMatEdit;
			 }

			 frmMatEdit = gcnew frmMaterialEditor();
			 frmMatEdit->Show(dockManager);
		 }
private: System::Void mnuNGENEOnTheWeb_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmBrowser->Visible)
				 frmBrowser->Hide();
			 else
			 {
				 frmBrowser->setURL("http://ngene.wikidot.com");
				 frmBrowser->Show(dockManager, DockState::Document);
			 }
		 }

private: System::Void mnuWireframe_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->Wireframe = mnuWireframe->Checked;
			 }
		 }
private: System::Void mnuLighting_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->Lighting = mnuLighting->Checked;
			 }
		 }
private: System::Void mnuInsertLightOmni_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 light = gcnew LightWrapper();
					 light->Type = LightType::Point;
					 frmProperty->SelectedObject = light;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }

private: System::Void mnuInsertLightDirectional_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 light = gcnew LightWrapper();
					 light->Type = LightType::Directional;
					 frmProperty->SelectedObject = light;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuSimulationModesPhysics_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->PhysicsEnabled = mnuSimulationModesPhysics->Checked;
			 }
		 }
private: System::Void mnuInsertCameraFPP_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 camera = gcnew CameraWrapper();
					 camera->Aspect = (float)target->Width / (float)target->Height;
					 frmProperty->SelectedObject = camera;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertPrefabBox_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew PrefabBoxWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertLightSpot_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 light = gcnew LightWrapper();
					 light->Type = LightType::Spot;
					 frmProperty->SelectedObject = light;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertTerrain_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 OpenFileDialog^ dlgOpenFile = gcnew OpenFileDialog();
					 dlgOpenFile->Title = "Choose height map file";
					 dlgOpenFile->Filter = ".bmp files|*.bmp|.jpg files|*.jpg|.gif files|*.gif|.png files|*.png|All files (*.*)|*.*";
					 dlgOpenFile->FilterIndex = 1;

					 if(dlgOpenFile->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					 {
						 pin_ptr <const wchar_t> str1 = PtrToStringChars(dlgOpenFile->FileName);
						 wstring fileName(str1);

						 ITexture* pTexture = TextureManager::getSingleton().createTextureFromFile(L"Terrain",
										fileName,
										TT_2D,
										TU_NORMAL,
										TF_A8R8G8B8);

						 terrain = gcnew TerrainWrapper(pTexture);
						 frmProperty->SelectedObject = terrain;

						 TextureManager::getSingleton().removeTexture(L"Terrain");
					 }
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void loadLayoutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void outdoorToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void mnuInsertOutdoorTerrain_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 OpenFileDialog^ dlgOpenFile = gcnew OpenFileDialog();
					 dlgOpenFile->Title = "Choose height map file";
					 dlgOpenFile->Filter = ".bmp files|*.bmp|.jpg files|*.jpg|.gif files|*.gif|.png files|*.png|All files (*.*)|*.*";
					 dlgOpenFile->FilterIndex = 1;

					 if(dlgOpenFile->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					 {
						 pin_ptr <const wchar_t> str1 = PtrToStringChars(dlgOpenFile->FileName);
						 wstring fileName(str1);

						 ITexture* pTexture = TextureManager::getSingleton().createTextureFromFile(L"Terrain",
										fileName,
										TT_2D,
										TU_NORMAL,
										TF_A8R8G8B8);

						 terrain = gcnew TerrainWrapper(pTexture);
						 frmProperty->SelectedObject = terrain;

						 TextureManager::getSingleton().removeTexture(L"Terrain");
					 }
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertOutdoorSky_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew SkyWrapper();
					 frmProperty->SelectedObject = nodeObj;

				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertOutdoorClouds_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew CloudsWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInstertOutdoorWater_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew WaterWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuWindowSaveLayout_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 FileManager::getSingleton().changeDir("../release");

			 dockManager->SaveAsXml("layout.xml");

			 FileManager::getSingleton().changeDir("../media");
		 }
private: System::Void mnuAbout_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 frmAboutBox->Show(nullptr);
		 }
private: System::Void mnuViewMaterialLibrary_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmMatLib->Visible)
				 frmMatLib->Hide();
			 else
				 frmMatLib->Show(dockManager);
		 }
private: System::Void mnuViewOutput_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmOut->Visible)
				 frmOut->Hide();
			 else
				 frmOut->Show(dockManager);
		 }
private: System::Void mnuViewSolutionExplorer_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmExplorer->Visible)
				 frmExplorer->Hide();
			 else
				 frmExplorer->Show(dockManager);
		 }
private: System::Void mnuViewToolbox_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmTools->Visible)
				 frmTools->Hide();
			 else
				 frmTools->Show(dockManager);
		 }
private: System::Void mnuInsertPSParticleSystem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew ParticleSystemWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertPSParticleEmitter_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 ParticleSystemWrapper^ ps = safe_cast<ParticleSystemWrapper^>(frmGraph->graph->getActive());

					 nodeObj = gcnew ParticleEmitterWrapper();
					 ps->addEmitter(safe_cast<ParticleEmitterWrapper^>(nodeObj));
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select node of particle system type");
				 }
			 }
		 }
private: System::Void mnuInsertPSParticleColour_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 ParticleEmitterWrapper^ emitter = safe_cast<ParticleEmitterWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew ParticleColourWrapper();
					 emitter->addColour(safe_cast<ParticleColourWrapper^>(nodeObj));
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select node of particle emitter type");
				 }
			 }
		 }

private: System::Void mnuViewModeHDR_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->HDR = mnuViewModeHDR->Checked;
			 }
		 }
private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

private: System::Void mnuFileSave_Click(System::Object^ sender, System::EventArgs^ e)
		 {
			 saveWorld();
		 }

private: System::Void mnuInsertMesh_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());
					 OpenFileDialog^ dlgOpenFile = gcnew OpenFileDialog();
					 dlgOpenFile->Filter = ".x files|*.x|.3ds files|*.3ds|Collada files|*.xml|All files (*.*)|*.*";
					 dlgOpenFile->FilterIndex = 1;

					 if(dlgOpenFile->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					 {
						 String^ ext = IO::Path::GetExtension(dlgOpenFile->FileName);
						 if(ext->ToLower() == ".x")
							nodeObj = gcnew MeshWrapperXFile(dlgOpenFile->FileName);
						 else if(ext->ToLower() == ".3ds")
							 nodeObj = gcnew MeshWrapper3dsFile(dlgOpenFile->FileName);
						 else if(ext->ToLower() == ".xml")
							 nodeObj = gcnew MeshWrapperColladaFile(dlgOpenFile->FileName);
						 else
						 {
							 MessageBox::Show("File format is not supported", "Invalid format",
											  System::Windows::Forms::MessageBoxButtons::OK,
											  System::Windows::Forms::MessageBoxIcon::Error);
							 Log::log(LET_ERROR, L"Editor", __WFILE__, __WFUNCTION__, __LINE__,
								 L"Mesh couldn't be loaded. Unsupported file format");
							 return;
						 }
						 frmProperty->SelectedObject = nodeObj;
					 }
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void tsbSave_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 saveWorld();
		 }

private: System::Void saveWorld()
		 {
			 tssLabel->Text = "Saving map...";

			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->save();
			 }

			 Log::log(LET_EVENT, L"Editor", __WFILE__, __WFUNCTION__, __LINE__,
				 L"Map saved successfully.");

			 tssLabel->Text = "Ready";
		 }
private: System::Void mnuFileOpenFile_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 loadWorld();
		 }
private: System::Void tsbOpen_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 loadWorld();
		 }
private: System::Void loadWorld()
		 {
			 tssLabel->Text = "Loading map...";

			 if(target != nullptr)
			 {
				 bool result = frmRenderTarget::engine->load();
				 if(result)
				 {
					 // Remove all post processing materials
					 if(frmFX != nullptr)
						frmFX->cleanup();
				 }

				 Log::log(LET_EVENT, L"Editor", __WFILE__, __WFUNCTION__, __LINE__,
					 L"Map loaded successfully.");
			 }

			 tssLabel->Text = "Ready";
		 }
private: System::Void mnuInsertPrefabSphere_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew PrefabSphereWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertPrefabPlane_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew PrefabPlaneWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuViewModeShadows_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->Shadows = mnuViewModeShadows->Checked;
			 }
		 }
private: System::Void mnuViewModeReflections_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->Reflections = mnuViewModeReflections->Checked;
			 }
		 }
private: System::Void mnuHelpTutorials_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if(frmBrowser->Visible)
				frmBrowser->Hide();
			else
			{
				frmBrowser->setURL("http://ngene.wikidot.com/tutorials");
				frmBrowser->Show(dockManager, DockState::Document);
			}
		 }
private: System::Void mnuViewModeDebugInfo_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->DebugAABB = mnuViewModeDebugInfo->Checked;
			 }
		 }
private: System::Void toolStrip1_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
		 }
private: System::Void tsbLockX_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->LockX = tsbLockX->Checked;
			 }
		 }
private: System::Void tsbLockY_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->LockY = tsbLockY->Checked;
			 }
		 }
private: System::Void tsbLockZ_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->LockZ = tsbLockZ->Checked;
			 }
		 }
private: System::Void xAxisToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 tsbLockX->Checked = xAxisToolStripMenuItem->Checked;
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->LockX = tsbLockX->Checked;
			 }
		 }
private: System::Void yAxisToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 tsbLockY->Checked = yAxisToolStripMenuItem->Checked;
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->LockY = tsbLockY->Checked;
			 }
		 }
private: System::Void zAxisToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 tsbLockZ->Checked = zAxisToolStripMenuItem->Checked;
			 if(target != nullptr)
			 {
				 frmRenderTarget::engine->LockZ = tsbLockZ->Checked;
			 }
		 }
private: System::Void tbsNewProject_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 createNewProject();
		 }

private:
		System::Void createNewProject()
		{
			System::Windows::Forms::DialogResult result;
			result = MessageBox::Show("Creating new project. All unsaved data will be lost. Continue?",
									  "Creating new project",
									  System::Windows::Forms::MessageBoxButtons::OKCancel,
									  System::Windows::Forms::MessageBoxIcon::Question);

			if(result == System::Windows::Forms::DialogResult::OK)
			{
				frmRenderTarget::engine->reset(target->Width, target->Height);

				if(frmFX != nullptr)
				{
					frmFX->cleanup();
				}
			}
		}
private: System::Void tbsRemoveNode_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			System::Windows::Forms::DialogResult result;
			result = MessageBox::Show("Are you sure you want to delete this node? All child nodes will be removed as well.",
									  "Deleting node",
									  System::Windows::Forms::MessageBoxButtons::YesNo,
									  System::Windows::Forms::MessageBoxIcon::Question);

			if(result == System::Windows::Forms::DialogResult::Yes)
			{
				if(target != nullptr)
				{
					frmRenderTarget::engine->removeSelected();
				}
			}
		 }
private: System::Void tbsLight_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertLightOmni_Click(sender, e);
		 }
private: System::Void tbsBox_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertPrefabBox_Click(sender, e);
		 }
private: System::Void tbsSphere_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertPrefabSphere_Click(sender, e);
		 }
private: System::Void tbsPlane_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertPrefabPlane_Click(sender, e);
		 }
private: System::Void tbsCamera_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertCameraFPP_Click(sender, e);
		 }
private: System::Void tbsTerrain_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertTerrain_Click(sender, e);
		 }
private: System::Void tbsSky_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertOutdoorSky_Click(sender, e);
		 }
private: System::Void tbsClouds_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertOutdoorClouds_Click(sender, e);
		 }
private: System::Void tbsWater_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInstertOutdoorWater_Click(sender, e);
		 }
private: System::Void tbsParticleSystem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertPSParticleSystem_Click(sender, e);
		 }
private: System::Void tbsParticleEmitter_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertPSParticleEmitter_Click(sender, e);
		 }
private: System::Void tbsMesh_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertMesh_Click(sender, e);
		 }
private: System::Void mnuEditDelete_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 tbsRemoveNode_Click(sender, e);
		 }
private: System::Void mnuInsertOtherFur_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 SurfaceWrapper^ surf = safe_cast<SurfaceWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew FurWrapper(surf);
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select surface to use as fur base",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuToolsOptions_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 frmProperty->SelectedObject = frmRenderTarget::engine;
			 }
		 }
private: System::Void mnuHeightMapEditor_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmHeight->Visible)
				 frmHeight->Hide();
			 else
				 frmHeight->Show(dockManager);
		 }
private: System::Void mnuContentAuthoringShaderEditor_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmShader->Visible)
				 frmShader->Hide();
			 else
				 frmShader->Show(dockManager);
		 }
private: System::Void mnuInsertPSParticleForce_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 ParticleEmitterWrapper^ emitter = safe_cast<ParticleEmitterWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew ParticleForceWrapper();
					 emitter->addForce(safe_cast<ParticleForceWrapper^>(nodeObj));
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select node of particle emitter type",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuErrorList_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmError->Visible)
				 frmError->Hide();
			 else
				 frmError->Show(dockManager);
		 }
private: System::Void mnuInsertPhysicsRigidBox_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew PhysicsActorWrapper(node->getnGENENode(), L"Box", true);
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node of node type",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void planeToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew PhysicsActorWrapper(node->getnGENENode(), L"Plane", false);
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node of node type",
									  "Select different node",
									  System::Windows::Forms::MessageBoxButtons::OK,
									  System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuScriptEditor_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmScript->Visible)
				 frmScript->Hide();
			 else
				 frmScript->Show(dockManager);
		 }
private: System::Void mnuFXs_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(frmFX->Visible)
				 frmFX->Hide();
			 else
				 frmFX->Show(dockManager);
		 }
private: System::Void mnuInsertLightVolumetric_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 LightWrapper^ lw = safe_cast<LightWrapper^>(frmGraph->graph->getActive());

					 nodeObj = gcnew VolumetricLightWrapper(lw->getLight());
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select light node",
						 "Select different node",
						 System::Windows::Forms::MessageBoxButtons::OK,
						 System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertSound_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 OpenFileDialog^ dlgOpenFile = gcnew OpenFileDialog();
					 dlgOpenFile->Filter = ".wav files|*.wav|.ogg files|*.ogg|*.mp3 files|*.mp3|All files (*.*)|*.*";
					 dlgOpenFile->FilterIndex = 1;

					 if(dlgOpenFile->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					 {
						 nodeObj = gcnew SoundWrapper(dlgOpenFile->FileName);
						 frmProperty->SelectedObject = nodeObj;
					 }

					
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
						 "Select different node",
						 System::Windows::Forms::MessageBoxButtons::OK,
						 System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void tbsSound_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 mnuInsertSound_Click(sender, e);
		 }
private: System::Void mnuInsertLightning_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew LightningWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
						 "Select different node",
						 System::Windows::Forms::MessageBoxButtons::OK,
						 System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertPrefabCapsule_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew PrefabCapsuleWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
						 "Select different node",
						 System::Windows::Forms::MessageBoxButtons::OK,
						 System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertPrefabTeapot_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew PrefabTeapotWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
						 "Select different node",
						 System::Windows::Forms::MessageBoxButtons::OK,
						 System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertPrefabTorus_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew PrefabTorusWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
						 "Select different node",
						 System::Windows::Forms::MessageBoxButtons::OK,
						 System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertPrefabCylinder_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew PrefabCylinderWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
						 "Select different node",
						 System::Windows::Forms::MessageBoxButtons::OK,
						 System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertPrefabPyramid_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 NodeWrapper^ node = safe_cast<NodeWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew PrefabPyramidWrapper();
					 frmProperty->SelectedObject = nodeObj;
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select scene node",
						 "Select different node",
						 System::Windows::Forms::MessageBoxButtons::OK,
						 System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuInsertPSParticleDeflector_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(target != nullptr)
			 {
				 try {
					 ParticleEmitterWrapper^ emitter = safe_cast<ParticleEmitterWrapper^>(frmGraph->graph->getActiveSceneNode());

					 nodeObj = gcnew ParticleDeflectorWrapper();
					 emitter->addDeflector(safe_cast<ParticleDeflectorWrapper^>(nodeObj));
				 } catch(InvalidCastException^) {
					 MessageBox::Show("Please select node of particle emitter type",
						 "Select different node",
						 System::Windows::Forms::MessageBoxButtons::OK,
						 System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
			 }
		 }
private: System::Void mnuSimulationRenderingContinuous_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 bool bRender = mnuSimulationRenderingContinuous->Checked;

			 mnuSimulationRenderingRequestRender->Enabled = !bRender;
			 target->setContinuousRendering(bRender);
		 }

private: System::Void mnuSimulationRenderingRequestRender_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 target->requestRender();
		 }
};
}


#endif