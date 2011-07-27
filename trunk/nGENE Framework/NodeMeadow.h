/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeMeadow.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEMEADOW_H_
#define __INC_NODEMEADOW_H_


#include "Node.h"
#include "Prerequisities.h"


namespace nGENE
{
namespace Nature
{
	/// Description of the plant
	typedef struct SPlantDesc
	{
		uint plantsCount;					///< How many plants are in the set
		float width;
		float widthSpread;
		float height;
		float heightSpread;
		float undergroundLevel;				///< How much of the plant should be under the ground

		Material* material;					///< Pointer to the material object
		ITexture* distributionMap;			///< Map of density distribution

		SPlantDesc():
			plantsCount(0),
			width(0.0f),
			widthSpread(0.0f),
			height(0.0f),
			heightSpread(0.0f),
			undergroundLevel(0.0f),
			material(NULL),
			distributionMap(NULL)
		{
		}
	} PLANT_DESC;

	/// Description of the decorator
	/// Description of the plant
	typedef struct SDecoratorDesc
	{
		uint objectsCount;					///< How many objects are in the set
		float undergroundLevel;				///< How much of the plant should be under the ground

		NodeVisible* node;					///< Pointer to the node being distributed
		ITexture* distributionMap;			///< Map of density distribution

		Real scaleSpread;					///< Deviation of scaling

		SDecoratorDesc():
			objectsCount(0),
			undergroundLevel(0.0f),
			node(NULL),
			distributionMap(NULL),
			scaleSpread(0.0f)
		{
		}
	} DECORATOR_DESC;


	/** Object representing meadow.
		@remarks
			Meadow class lets you easily create fields of grass and
			flowers spread over terrain.
	*/
	class NodeMeadow: public Node
	{
	private:
		vector <PLANT_DESC> m_vPlants;
		NodeTerrain* m_pTerrain;

	protected:
		/// Updates meadow.
		void onUpdate();

	public:
		NodeMeadow();
		NodeMeadow(NodeTerrain* _terrain);
		~NodeMeadow();

		/** Adds new plant-set to the meadow.
			@remarks
				This function will fail if terrain has not been set. To
				do this either call setTerrain() method or pass the terrain
				object's pointer at meadow's creation to the constructor.
		*/
		void addPlant(PLANT_DESC& _desc);
		/** Adds new set of objects to the meadow.
			@remarks
				This function will fail if terrain has not been set. To
				do this either call setTerrain() method or pass the terrain
				object's pointer at meadow's creation to the constructor.
		*/
		void addDecorator(DECORATOR_DESC& _desc);

		/// Sets valid pointer to the terrain object.
		void setTerrain(NodeTerrain* _terrain);
	};



	inline void NodeMeadow::setTerrain(NodeTerrain* _terrain)
	{
		m_pTerrain = _terrain;
	}
//----------------------------------------------------------------------
}
}


#endif