/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PreethamScatter.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "Maths.h"
#include "PreethamScatter.h"


#if _MSC_VER < 1500
	#include "omp.h"
#endif


namespace nGENE
{
namespace Nature
{
	// Initialize static members
	const double PreethamScatter::s_xDC[5][2] =
	{
		{ -0.0193, -0.2592 },
		{ -0.0665,  0.0008 },
		{ -0.0004,  0.2125 },
		{ -0.0641, -0.8989 },
		{ -0.0033,  0.0452 }
	};

	const double PreethamScatter::s_yDC[5][2] =
	{
		{ -0.0167, -0.2608 },
		{ -0.0950,  0.0092 },
		{ -0.0079,  0.2102 },
		{ -0.0441, -1.6537 },
		{ -0.0109,  0.0529 }
	};

	const double PreethamScatter::s_YDC[5][2] =
	{
		{  0.1787, -1.4630 },
		{ -0.3554,  0.4275 },
		{ -0.0227,  5.3251 },
		{  0.1206, -2.5771 },
		{ -0.0670,  0.3703 }
	};

	const double PreethamScatter::s_xZC[3][4] =
	{
		{  0.00166, -0.00375,  0.00209, 0.00000 },
		{ -0.02903,  0.06377, -0.03203, 0.00394 },
		{  0.11693, -0.21196,  0.06052, 0.25886 }
	};  
	  
	const double PreethamScatter::s_yZC[3][4] =
	{
		{  0.00275, -0.00610,  0.00317, 0.00000 },
		{ -0.04214,  0.08970, -0.04153, 0.00516 },
		{  0.15346, -0.26756,  0.06670, 0.26688 }
	};


	const double PreethamScatter::s_CIE_300_800[107][3] =
	{
		#include "Scattering/ciexyz31_300_800.table"
	};

	const double PreethamScatter::s_CIE_350_800[97][3] =
	{
		#include "Scattering/ciexyz31_350_800.table"
	};

	const double PreethamScatter::s_neta[] =
	{
		#include "Scattering/neta.table"
	};

	const double PreethamScatter::s_K[] =
	{
		#include "Scattering/K.table"
	};

	const double PreethamScatter::s_N21[] =
	{
		#include "Scattering/N21.table"
	};

	const double PreethamScatter::s_Ko[] =
	{
		#include "Scattering/Ko.table"
	};

	const double PreethamScatter::s_Kg[] =
	{
		#include "Scattering/Kg.table"
	};

	const double PreethamScatter::s_Kwa[] =
	{
		#include "Scattering/Kwa.table"
	};

	const double PreethamScatter::s_sol[] =
	{
		#include "Scattering/Sol.table"
	};


	const double PreethamScatter::s_V       = 4.0;
	const double PreethamScatter::s_anisCor = 1.06;
	const double PreethamScatter::s_N       = 2.545e25;
	const double PreethamScatter::s_dAlpha1  = 0.83331810215394e-3;
	const double PreethamScatter::s_dAlpha2  = 0.11360016092149e-3;


	PreethamScatter::PreethamScatter():
		ThetaBins(5),
		PhiBins(5)
	{
		m_Sun.theta = -1.0f;
		m_Sun.phi = -1.0f;
		m_dSunSpectralRad = 0.25 * Maths::PI * 1.39 * 1.39 / (150.0 * 150.0);

		setTurbidity(2.0f);

		for(uint i = 0; i < 1801; ++i)
		{
			m_aNetaTable[i].set(static_cast<float>(s_neta[i * 3 + 0]),
                               static_cast<float>(s_neta[i * 3 + 1]),
                               static_cast<float>(s_neta[i * 3 + 2]));
		}

		m_fExposure = 0.1f;
		m_fInscatter = 0.007f;
	}
//----------------------------------------------------------------------
	PreethamScatter::~PreethamScatter()
	{
	}
//----------------------------------------------------------------------
	void PreethamScatter::setTurbidity(double _turbidity)
	{
		m_fTurbidity = _turbidity;


		m_Ax = s_xDC[0][0] * m_fTurbidity + s_xDC[0][1];
		m_Bx = s_xDC[1][0] * m_fTurbidity + s_xDC[1][1];
		m_Cx = s_xDC[2][0] * m_fTurbidity + s_xDC[2][1];
		m_Dx = s_xDC[3][0] * m_fTurbidity + s_xDC[3][1];
		m_Ex = s_xDC[4][0] * m_fTurbidity + s_xDC[4][1];

		m_Ay = s_yDC[0][0] * m_fTurbidity + s_yDC[0][1];
		m_By = s_yDC[1][0] * m_fTurbidity + s_yDC[1][1];
		m_Cy = s_yDC[2][0] * m_fTurbidity + s_yDC[2][1];
		m_Dy = s_yDC[3][0] * m_fTurbidity + s_yDC[3][1];
		m_Ey = s_yDC[4][0] * m_fTurbidity + s_yDC[4][1];

		m_AY = s_YDC[0][0] * m_fTurbidity + s_YDC[0][1];
		m_BY = s_YDC[1][0] * m_fTurbidity + s_YDC[1][1];
		m_CY = s_YDC[2][0] * m_fTurbidity + s_YDC[2][1];
		m_DY = s_YDC[3][0] * m_fTurbidity + s_YDC[3][1];
		m_EY = s_YDC[4][0] * m_fTurbidity + s_YDC[4][1];

		// Calculate Mie and Rayleigh scattering coefficients

		double b_m[101], b_p[101];
		double b_m_ang_prefix[101];
		double b_p_ang_prefix[101];

		double c = (0.06544204545455 * m_fTurbidity - 0.06509886363636) * 1e-15;

		m_vecRayleighFactor.set(0.0f, 0.0f, 0.0f);
		m_vecMieFactor.set(0.0f, 0.0f, 0.0f);
		m_vecRayleighFactorAng.set(0.0f, 0.0f, 0.0f);
		m_vecMieFactorAng.set(0.0f, 0.0f, 0.0f);

		double lambda = 0.0f;
		int i = 0;

		for(lambda = 300.0, i = 0; lambda <= 800.0; lambda += 5.0, ++i)
		{
			double lambdasi = lambda * 1e-9;
			double Nlambda4 = s_N * lambdasi * lambdasi * lambdasi * lambdasi;

			double n2_1 = s_N21[i]; 
			double K = s_K[i];

			b_m[i] = 8.0 * Maths::PI * Maths::PI * Maths::PI * n2_1 * s_anisCor / (3.0 * Nlambda4);
			b_p[i] = 0.434 * Maths::PI * c * pow(2.0 * Maths::PI / lambdasi, s_V - 2.0) * K * pow(0.01, s_V - 3.0);

			b_m_ang_prefix[i] = 2.0 * Maths::PI * Maths::PI * n2_1 * s_anisCor * 0.7629 / (3.0 * Nlambda4);
			b_p_ang_prefix[i] = 0.217 * c * pow(2.0 * Maths::PI / lambdasi, s_V - 2.0) * pow(0.01,s_V - 3.0);

			// Convert spectral curves to CIE XYZ

			m_vecRayleighFactor.x += static_cast<double>(b_m[i] * s_CIE_300_800[i][0]);
			m_vecRayleighFactor.y += static_cast<double>(b_m[i] * s_CIE_300_800[i][1]);
			m_vecRayleighFactor.z += static_cast<double>(b_m[i] * s_CIE_300_800[i][2]);

			m_vecMieFactor.x += static_cast<double>(b_p[i] * s_CIE_300_800[i][0]);
			m_vecMieFactor.y += static_cast<double>(b_p[i] * s_CIE_300_800[i][1]);
			m_vecMieFactor.z += static_cast<double>(b_p[i] * s_CIE_300_800[i][2]);

			m_vecRayleighFactorAng.x += static_cast<double>(b_m_ang_prefix[i] * s_CIE_300_800[i][0]);
			m_vecRayleighFactorAng.y += static_cast<double>(b_m_ang_prefix[i] * s_CIE_300_800[i][1]);
			m_vecRayleighFactorAng.z += static_cast<double>(b_m_ang_prefix[i] * s_CIE_300_800[i][2]);

			m_vecMieFactorAng.x += static_cast<double>(b_p_ang_prefix[i] * s_CIE_300_800[i][0]);
			m_vecMieFactorAng.y += static_cast<double>(b_p_ang_prefix[i] * s_CIE_300_800[i][1]);
			m_vecMieFactorAng.z += static_cast<double>(b_p_ang_prefix[i] * s_CIE_300_800[i][2]);
		}

		computeAttenuatedSunlight();
	}
//----------------------------------------------------------------------
	double PreethamScatter::computeChromaticity(const double m[][4]) const
	{
		double T2 = m_fTurbidity * m_fTurbidity;
		double t2 = m_Sun.theta * m_Sun.theta;
		double t3 = t2 * m_Sun.theta;

		return (T2 * m[0][0] + m_fTurbidity * m[1][0] + m[2][0]) * t3 +
            (T2 * m[0][1] + m_fTurbidity * m[1][1] + m[2][1]) * t2 +
			(T2 * m[0][2] + m_fTurbidity * m[1][2] + m[2][2]) * m_Sun.theta +
            (T2 * m[0][3] + m_fTurbidity * m[1][3] + m[2][3]);
	}
//----------------------------------------------------------------------
	double PreethamScatter::perezFunction(const double _A, const double _B, const double _C, const double _D, const double _E,
		const double _theta, const double _gamma) const
	{
		double e0 = _B / cos(_theta);
		double e1 = _D * _gamma;
		double e2 = cos(_gamma);
		e2 *= e2;

		double f1 = (1 + _A * exp(e0)) * (1 + _C * exp(e1) + _E * e2); 

		e0 = _B;
		e1 = _D * m_Sun.theta;
		e2 = cos(m_Sun.theta);
		e2 *= e2;

		double f2 = (1 + _A * exp( e0 )) * (1 + _C * exp(e1) + _E * e2); 

		return f1 / f2;
	}
//----------------------------------------------------------------------
	void PreethamScatter::computeAttenuatedSunlight()
	{
		double data[91];
		double beta = 0.04608365822050 * m_fTurbidity - 0.04586025928522;

		double m = 1.0 / (cos(m_Sun.theta) + 0.15 * pow(93.885 - Maths::radToDeg(m_Sun.theta), -1.253));
		double lambda;

		m_vecSunSpectralRad.set(0.0f, 0.0f, 0.0f);
		uint i = 0;

		for(i = 0, lambda = 350.0; i < 91; ++i, lambda += 5.0)
		{
			double tauR = exp(-m * 0.008735 * pow(lambda / 1000.0, -4.08));
			double alpha = 1.3;
			double tauA = exp(-m * beta * pow(lambda / 1000.0, -alpha));
			double ozone = 0.35;
			double tauO = exp(-m * s_Ko[i] * ozone);
			double tauG = exp(-1.41 * s_Kg[i] * m / pow( 1.0 + 118.93 * s_Kg[i] * m, 0.45));
			double w = 2.0;
			double tauWA = exp(-0.2385 * s_Kwa[i] * w * m / pow(1.0 + 20.07 * s_Kwa[i] * w * m, 0.45));

			data[i] = 100.0 * s_sol[i] * tauR * tauA * tauO * tauG * tauWA;  // 100 comes from solCurve being in wrong units. 

			// Convert spectral curve to CIE XYZ

			m_vecSunSpectralRad.x += static_cast<float>(data[i] * s_CIE_350_800[i][0]);
			m_vecSunSpectralRad.y += static_cast<float>(data[i] * s_CIE_350_800[i][1]);
			m_vecSunSpectralRad.z += static_cast<float>(data[i] * s_CIE_350_800[i][2]);
		}
	}
//----------------------------------------------------------------------
	void PreethamScatter::setSunAngle(float _angle)
	{
		m_Sun.theta = acos(sin(_angle));
		m_Sun.phi = atan2((double)0.0, (double)cos(_angle));
		m_Sun.direction.set(cos(_angle), sin(_angle), 0.0f);

		updateSun();
	}
//----------------------------------------------------------------------
	void PreethamScatter::updateSun()
	{
		// Calculate sun luminance and chromaticity values
		double dChi = (4.0 / 9.0 - m_fTurbidity / 120.0) * (Maths::PI - 2.0 * m_Sun.theta);

		m_YZenith = (4.0453 * m_fTurbidity - 4.9710) * tan( dChi ) - 0.2155 * m_fTurbidity + 2.4192;

		if(m_YZenith < 0.0)
			m_YZenith = -m_YZenith;

		m_xZenith = computeChromaticity(s_xZC);
		m_yZenith = computeChromaticity(s_yZC);

		computeAttenuatedSunlight();

		double theta, phi;

		double delTheta = Maths::PI / ThetaBins;
		double delPhi   = 2.0 * Maths::PI / PhiBins;

		uint i, j;
		for(i = 0, theta = 0; theta <= Maths::PI; theta += delTheta, ++i)
		{
			for(j = 0, phi = 0; phi <= Maths::PI_DOUBLE; phi+= delPhi, ++j)
			{
				computeS0(theta, phi, m_aS0Mie[i * (ThetaBins + 1) + j], m_aS0Rayleigh[i * (ThetaBins + 1) + j]);
			}
		}
	}
//----------------------------------------------------------------------
	void PreethamScatter::getNeta(const double _theta, const double _v, Vector3& _neta) const
	{
		double theta = Maths::radToDeg(_theta) * 10.0;
		double u = theta - (int)theta;

		if(theta < 0.0 || theta > 1801.0)
		{
			_neta.set(0.0f, 0.0f, 0.0f);
			return;
		}

		if(theta > 1800.0f)
		{
			_neta = m_aNetaTable[1800];
			return;
		}

		_neta = (1.0f - (float)u) * m_aNetaTable[FastFloat::floatToInt(theta)] +
			(float)u * m_aNetaTable[FastFloat::floatToInt(theta) + 1];
	}
//----------------------------------------------------------------------
	void PreethamScatter::computeS0(const double _thetav, const double _phiv, Vector3& _mie, Vector3& _ray) const
	{
		double phiDelta   = Maths::PI / 20.0;
		double thetaDelta = Maths::PI / 2.0 / 20.0;
		double thetaUpper = Maths::PI / 2.0;
		double psi = 0.0f;

		Vector3 skyRad;
		Vector3 beta_ang_1, beta_ang_2;
		Vector3 neta;

		int countTheta = FastFloat::floatToInt(thetaUpper / thetaDelta);
		int countPhi = FastFloat::floatToInt(Maths::PI_DOUBLE / phiDelta);
		double theta = 0.0f;
		double phi = 0.0f;

		int i = 0;
		int j = 0;
		float skyAmb_1_x = 0.0f, skyAmb_1_y = 0.0f, skyAmb_1_z = 0.0f;
		float skyAmb_2_x = 0.0f, skyAmb_2_y = 0.0f, skyAmb_2_z = 0.0f;

		#pragma omp parallel for firstprivate(j) lastprivate(i) private(theta) private(phi) private(skyRad) private(beta_ang_1) private(beta_ang_2) private(neta) private(psi) reduction(+: skyAmb_1_x) reduction(+: skyAmb_1_y) reduction(+: skyAmb_1_z) reduction(+: skyAmb_2_x) reduction(+: skyAmb_2_y) reduction(+: skyAmb_2_z)
		for(i = 0; i < countTheta; ++i)
		{
			for(j = 0; j < countPhi; ++j)
			{
				theta = (double)i * thetaDelta;
				phi = (double)j * phiDelta;
				getSkySpectralRadiance(skyRad, theta, phi);
				psi = angleBetween(_thetav, _phiv, theta, phi);

				getNeta(psi, s_V, neta);
				beta_ang_1 = Vector3(m_vecMieFactorAng.x * neta.x, m_vecMieFactorAng.y * neta.y, m_vecMieFactorAng.z * neta.z);
				beta_ang_2 = m_vecRayleighFactorAng * static_cast<float>((1.0f + 0.9324f * cos(psi) * cos(psi)));

				Vector3 temp1(Vector3(skyRad.x * beta_ang_1.x, skyRad.y * beta_ang_1.y, skyRad.z * beta_ang_1.z) * 
					static_cast<float>(sin(theta) * thetaDelta * phiDelta));
				Vector3 temp2(Vector3(skyRad.x * beta_ang_2.x, skyRad.y * beta_ang_2.y, skyRad.z * beta_ang_2.z) *
					static_cast<float>(sin(theta) * thetaDelta * phiDelta));

				skyAmb_1_x += temp1.x;
				skyAmb_1_y += temp1.y;
				skyAmb_1_z += temp1.z;

				skyAmb_2_x += temp2.x;
				skyAmb_2_y += temp2.y;
				skyAmb_2_z += temp2.z;
			}
		}

		Vector3 skyAmb_1(skyAmb_1_x, skyAmb_1_y, skyAmb_1_z);
		Vector3 skyAmb_2(skyAmb_2_x, skyAmb_2_y, skyAmb_2_z);

		psi = angleBetween(_thetav, _phiv, m_Sun.theta, m_Sun.phi);

		getNeta(psi, s_V, neta);
		beta_ang_1 = Vector3(m_vecMieFactorAng.x * neta.x, m_vecMieFactorAng.y * neta.y, m_vecMieFactorAng.z * neta.z);
		beta_ang_2 = m_vecRayleighFactorAng * static_cast<float>((1.0f + 0.9324f * cos(psi) * cos(psi)));

		Vector3 sunAmb_1 = Vector3(m_vecSunSpectralRad.x * beta_ang_1.x,
								   m_vecSunSpectralRad.y * beta_ang_1.y,
								   m_vecSunSpectralRad.z * beta_ang_1.z) * static_cast<float>(m_dSunSpectralRad);
		Vector3 sunAmb_2 = Vector3(m_vecSunSpectralRad.x * beta_ang_2.x,
								   m_vecSunSpectralRad.y * beta_ang_2.y,
								   m_vecSunSpectralRad.z * beta_ang_2.z) * static_cast<float>(m_dSunSpectralRad);

		_mie = sunAmb_1 + skyAmb_1;
		_ray = sunAmb_2 + skyAmb_2;
	}
//----------------------------------------------------------------------
	void PreethamScatter::getSkySpectralRadiance(Vector3& _color, const double _theta, const double _phi) const
	{
		double gamma = angleBetween(_theta, _phi, m_Sun.theta, m_Sun.phi);
		double x = 0.0f;
		double y = 0.0f;
		double Y = 0.0f;

		//#pragma omp parallel sections
		{
			//#pragma omp section
			x = m_xZenith * perezFunction(m_Ax, m_Bx, m_Cx, m_Dx, m_Ex, _theta, gamma);
			//#pragma omp section
			y = m_yZenith * perezFunction(m_Ay, m_By, m_Cy, m_Dy, m_Ey, _theta, gamma);
			//#pragma omp section
			Y = m_YZenith * perezFunction(m_AY, m_BY, m_CY, m_DY, m_EY, _theta, gamma);
		}

		double X = (x / y) * Y;
		double Z = ((1.0 - x - y) / y) * Y;

		_color.set(static_cast<float>(X),
				   static_cast<float>(Y),
				   static_cast<float>(Z));
	}
//----------------------------------------------------------------------
	double PreethamScatter::angleBetween(const double& _thetav, const double& _phiv, const double& _theta, const double& _phi) const
	{
		double cospsi = sin(_thetav ) * sin(_theta ) * cos(_phi - _phiv) + cos(_thetav) * cos(_theta);

		if(cospsi > 1.0)
			return 0.0;

		if(cospsi < -1.0)
			return Maths::PI;

		return acos(cospsi);
	}
//----------------------------------------------------------------------
}
}
