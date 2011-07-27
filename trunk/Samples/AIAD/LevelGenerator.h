/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LevelGenerator.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once

#include "FrameworkWin32.h"


/// Type of the cell
enum LEVEL_CELL_TYPE
{
	LCT_UNKNOWN = 0,
	LCT_FLOOR = 1,
	LCT_WALL = 2,
	LCT_DOORS = 3,
	LCT_STAIRS = 4
};


/// Descriptor of the level
typedef struct SLevelDesc
{
	uint width;
	uint length;

	Vector2 startPoint;

	vector <LEVEL_CELL_TYPE> cells;
} LEVEL_DESC;

enum POSSIBLE_DIR
{
	PD_NORTH = (1<<0),
	PD_SOUTH = (1<<1),
	PD_WEST = (1<<2),
	PD_EAST = (1<<3)
};


/** Class used to generates maze-like levels.
*/
class LevelGenerator
{
private:
	SLevelDesc m_Level;


	void generate();

public:
	LevelGenerator(const LEVEL_DESC& _desc);
	virtual ~LevelGenerator();

	void saveLevelAsBitmap(const string& _file);

	LEVEL_DESC& getLevel();
};



inline LEVEL_DESC& LevelGenerator::getLevel()
{
	return m_Level;
}
//----------------------------------------------------------------------