/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		BrownianTree.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_BROWNIANTREE_H_
#define __INC_BROWNIANTREE_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Generates 2D brownian tree fractal.
		@par
			It is an implementation of Diffusion Limited Aggregation (DLA)
			algorithm. The code is based on the one found here:
			http://local.wasp.uwa.edu.au/~pbourke/fractals/dla/.
	*/
	class nGENEDLL BrownianTree
	{
	public:
		typedef pair <uint, uint> TREE_SEED;

	public:
		/** Generates tree data.
			@param
				_seeds vector of coordinate pairs specifying starting
				seeds for tree generation.
			@param _width width of the generated fractal.
			@param _height height of the generated fractal.
			@param
				_iterations number of iterations to perform or to
				speak DLA terminology - walkers to put.
			@returns
				bool* dynamically allocated array. It is client
				responsibility to free this. NULL is returned if no
				_seeds where specified (i.e. vector is empty).
		*/
		bool* operator()(const vector <TREE_SEED>& _seeds, uint _width=256, uint _height=256,
			uint _iterations=1000);
	};
}


#endif