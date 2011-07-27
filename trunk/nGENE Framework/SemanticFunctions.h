/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SemanticFunctions.h
Version:	0.05
Info:		Default semantic functions declarations.
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SEMANTICFUNCTIONS_H_
#define __INC_SEMANTICFUNCTIONS_H_


#include "Prerequisities.h"


namespace nGENE
{
namespace Application
{
	void worldViewProj(void* _values, uint _count);
	void viewProj(void* _values, uint _count);
	void viewInverse(void* _values, uint _count);
	void viewInverseTranspose(void* _values, uint _count);
	void worldView(void* _values, uint _count);
	void world(void* _values, uint _count);
	void view(void* _values, uint _count);
	void proj(void* _values, uint _count);
	void projInverse(void* _values, uint _count);

	void camera(void* _values, uint _count);
	void cameraInverse(void* _values, uint _count);
	void eyePosition(void* _values, uint _count);

	void nearPlane(void* _values, uint _count);
	void farPlane(void* _values, uint _count);

	void lightDirection(void* _values, uint _count);
	void lightPosition(void* _values, uint _count);
	void lightColour(void* _values, uint _count);
	void lightIntensity(void* _values, uint _count);
	void lightRadius(void* _values, uint _count);
	void lightFov(void* _value, uint _count);
	void lightSpotExponent(void* _value, uint _count);
	void lightConstantAttenuation(void* _value, uint _count);
	void lightLinearAttenuation(void* _value, uint _count);
	void lightQuadraticAttenuation(void* _value, uint _count);

	void shadowDensity(void* _value, uint _count);

	void invTanHalfFov(void* _values, uint _count);
	void aspect(void* _values, uint _count);
	void pixelSize(void* _values, uint _count);

	void ambient(void* _values, uint _count);
	void diffuse(void* _values, uint _count);
	void emissive(void* _values, uint _count);

	void time(void* _values, uint _count);
}
}


#endif