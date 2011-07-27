/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LevelGenerator.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "LevelGenerator.h"
#include "nGENE.h"


LevelGenerator::LevelGenerator(const LEVEL_DESC& _desc):
	m_Level(_desc)
{
	generate();
}
//----------------------------------------------------------------------
LevelGenerator::~LevelGenerator()
{
}
//----------------------------------------------------------------------
void LevelGenerator::generate()
{
	// First mark all cells as impassable
	for(uint i = 0; i < m_Level.width; ++i)
	{
		for(uint j = 0; j < m_Level.length; ++j)
			m_Level.cells.push_back(LCT_WALL);
	}

	m_Level.cells[m_Level.startPoint.y * m_Level.length + m_Level.startPoint.x] = LCT_FLOOR;

	vector <Vector2> vPoints;
	vPoints.push_back(m_Level.startPoint);

	int num = 0;
	while(vPoints.size() && ++num < 150)
	{
		// Decide - room/corridor?
		bool corridor = true;
		/// ...

		Vector2& point = vPoints.front();
		vPoints.erase(vPoints.begin());

		uint dirs = 0;
		uint dirsCount = 0;
		if(point.x > 1 && point.y > 0 && point.y < m_Level.length - 1)
		{
			if(m_Level.cells[point.y * m_Level.length + point.x - 1] == LCT_WALL)
			{
				dirs |= PD_WEST;
				++dirsCount;
			}
		}

		if(point.x < m_Level.width - 2 && point.y > 0 && point.y < m_Level.length - 1)
		{
			if(m_Level.cells[point.y * m_Level.length + point.x + 1] == LCT_WALL)
			{
				dirs |= PD_EAST;
				++dirsCount;
			}
		}

		if(point.y < m_Level.length - 2 && point.x > 0 && point.x < m_Level.width - 1)
		{
			if(m_Level.cells[(point.y + 1) * m_Level.length + point.x] == LCT_WALL)
			{
				dirs |= PD_NORTH;
				++dirsCount;
			}
		}

		if(point.y > 1 && point.x > 0 && point.x < m_Level.width - 1)
		{
			if(m_Level.cells[(point.y - 1) * m_Level.length + point.x] == LCT_WALL)
			{
				dirs |= PD_SOUTH;
				++dirsCount;
			}
		}

		uint newDirs = 0;
		if(dirsCount)
			newDirs = rand() % dirsCount + 1;
		for(uint i = 0; i < newDirs; ++i)
		{
			Vector2 dir;
			if(dirs & PD_WEST)
			{
				dir.set(-1, 0);
				dirs &= ~PD_WEST;
			}
			else if(dirs & PD_EAST)
			{
				dir.set(1, 0);
				dirs &= ~PD_EAST;
			}
			else if(dirs & PD_NORTH)
			{
				dir.set(0, 1);
				dirs &= ~PD_NORTH;
			}
			else if(dirs & PD_SOUTH)
			{
				dir.set(0, -1);
				dirs &= ~PD_SOUTH;
			}


			Vector2 index = point + dir;

			m_Level.cells[index.y * m_Level.length + index.x] = LCT_FLOOR;
			vPoints.push_back(index);
		}
	}
}
//----------------------------------------------------------------------
void LevelGenerator::saveLevelAsBitmap(const string& _file)
{

}
//----------------------------------------------------------------------