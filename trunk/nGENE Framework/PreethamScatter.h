/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PreethamScatter.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREETHAMSCATTER_H_
#define __INC_PREETHAMSCATTER_H_


#include "Prerequisities.h"
#include "Vector3.h"
#include "Vector4.h"


namespace nGENE
{
namespace Nature
{
	/** Class implementing Preetham scattering method.
		@remarks
			You can conveniently control preetham scattering by calling
			setSunAngle() method and passing to it angle the Sun is above
			horizon.
		@par
			This code is tightly based on the Marco Spoerl article from
			ShaderX 3. What's more fragments of it are exactly his work.
	*/
	class PreethamScatter
	{
	private:
		struct Sun
		{
			double theta;								///< Sun's angle from zenith
			double phi;									///< Sun's angle from horizon
			Vector3 direction;							///< Sun-light direction
		} m_Sun;

		double m_fTurbidity;							///< Sky's turbidity factor
		double m_fExposure;								///< Sky's exposure
		double m_fInscatter;							///< Sky's in-scattering factor

		double m_Ax, m_Bx, m_Cx, m_Dx, m_Ex;			///< Perez function parameter for chromaticity
		double m_Ay, m_By, m_Cy, m_Dy, m_Ey;			///< Perez function parameter for chromaticity
		double m_AY, m_BY, m_CY, m_DY, m_EY;			///< Perez function parameter for luminance

		double m_YZenith;								///< Zenith luminance
		double m_xZenith;								///< Zenith chromaticity
		double m_yZenith;								///< Zenith chromaticity


		Vector3 m_vecSunSpectralRad;					///< Sun's spectral radiance
		Vector3 m_vecRayleighFactor;					///< Rayleigh total scattering coefficient
		Vector3 m_vecMieFactor;							///< Mie total scattering coefficient
		Vector3 m_vecRayleighFactorAng;					///< Rayleigh angular scattering coefficient
		Vector3 m_vecMieFactorAng;						///< Mie angular scattering coefficient


		const uint ThetaBins;
		const uint PhiBins;


		double m_dSunSpectralRad;						///< Sun's solid angle w


		static const double s_xDC[5][2];				///< Chromaticity distribution coefficients
		static const double s_yDC[5][2];				///< Chromaticity distribution coefficients
		static const double s_YDC[5][2];				///< Luminance distribution coefficients
		static const double s_xZC[3][4];				///< Chromaticity distribution coefficients for zenith
		static const double s_yZC[3][4];				///< Chromaticity distribution coefficients for zenith

		static const double s_CIE_300_800[107][3];		///< CIE 1931 2-deg, XYZ Color Matching Functions (300nm - 800nm)
		static const double s_CIE_350_800[97][3];		///< CIE 1931 2-deg, XYZ Color Matching Functions (350nm - 800nm)

		static const double s_neta[];
		static const double s_K[];
		static const double s_N21[];
		static const double s_Ko[];
		static const double s_Kg[];
		static const double s_Kwa[];
		static const double s_sol[];

		Vector3 m_aNetaTable[1801];
		Vector3 m_aS0Mie[36];
		Vector3 m_aS0Rayleigh[36];


		static const double s_V;
		static const double s_anisCor;
		static const double s_N;


		static const double s_dAlpha1;
		static const double s_dAlpha2;
		

		double angleBetween(const double& _thetav, const double& _phiv, const double& _theta, const double& _phi) const;
		
		double computeChromaticity(const double m[][4]) const;
		void computeAttenuatedSunlight();
		void computeS0(const double _thetav, const double _phiv, Vector3& _mie, Vector3& _ray) const;

		double perezFunction(const double _A, const double _B, const double _C, const double _D,
			const double _E, const double _theta, const double _gamma) const;
		void getNeta(const double _theta, const double _v, Vector3& _neta) const;
		void getSkySpectralRadiance(Vector3& _color, const double _theta, const double _phi) const;


		PreethamScatter(const PreethamScatter& src);
		PreethamScatter& operator=(const PreethamScatter& rhs);

	public:
		PreethamScatter();
		~PreethamScatter();

		/// Sets turbidity factor and performs all necessary computations.
		void setTurbidity(double _turbidity);
		double getTurbidity() const;

		void setExposure(float _exposure);
		float getExposure() const;

		void setInscattering(float _inscatter);
		float getInscattering() const;

		/// Update sun's parameters
		void updateSun();

		/** Sets sun angle above horizon.
			@remarks
				This method internally call setSun() to perform all necessary
				computations.
		*/
		void setSunAngle(float _angle);

		Vector3 getZenithColor() const;
		Vector2 getAlphaFactors() const;
		Vector3& getMieFactor();
		Vector3& getRayleighFactor();
		Vector3& getSunLightDirection();

		Vector3* getS0Mie();
		Vector3* getS0Rayleigh();
	};



	inline double PreethamScatter::getTurbidity() const
	{
		return m_fTurbidity;
	}
//----------------------------------------------------------------------
	inline void PreethamScatter::setExposure(float _exposure)
	{
		m_fExposure = _exposure;
	}
//----------------------------------------------------------------------
	inline float PreethamScatter::getExposure() const
	{
		return m_fExposure;
	}
//----------------------------------------------------------------------
	inline void PreethamScatter::setInscattering(float _inscatter)
	{
		m_fInscatter = _inscatter;
	}
//----------------------------------------------------------------------
	inline float PreethamScatter::getInscattering() const
	{
		return m_fInscatter;
	}
//----------------------------------------------------------------------
	inline Vector3 PreethamScatter::getZenithColor() const
	{
		return Vector3(m_xZenith, m_yZenith, m_YZenith);
	}
//----------------------------------------------------------------------
	inline Vector2 PreethamScatter::getAlphaFactors() const
	{
		return Vector2(s_dAlpha1, s_dAlpha2);
	}
//----------------------------------------------------------------------
	inline Vector3& PreethamScatter::getMieFactor()
	{
		return m_vecMieFactor;
	}
//----------------------------------------------------------------------
	inline Vector3& PreethamScatter::getRayleighFactor()
	{
		return m_vecRayleighFactor;
	}
//----------------------------------------------------------------------
	inline Vector3& PreethamScatter::getSunLightDirection()
	{
		return m_Sun.direction;
	}
//----------------------------------------------------------------------
	inline Vector3* PreethamScatter::getS0Mie()
	{
		return &m_aS0Mie[0];
	}
//----------------------------------------------------------------------
	inline Vector3* PreethamScatter::getS0Rayleigh()
	{
		return &m_aS0Rayleigh[0];
	}
//----------------------------------------------------------------------
}
}


#endif