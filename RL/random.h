#ifndef __RANDOM_H__
#define __RANDOM_H__

/* Note: This uses the multiply-with-carry method invented by George Marsaglia. */

class CRandom
{
	unsigned int m_iSeed1;
	unsigned int m_iSeed2;

	unsigned int m_iNoiseSeed1;
	unsigned int m_iNoiseSeed2;

	float Noise(int iX, int iY);
	float SmoothNoise(int iX, int iY);
	/* Interpolate between values A and B at position Mu (0-1.0f) along this ray, where 0 is A and 1.0 is B */
	float Interpolate( float fA, float fB, float fMu );
	

public:
	CRandom();

	/* Initialise generator provided seeds */
	void Init( int Seed1, int Seed2 );
	/* Initialise using curtime */
	void Init();

	/* Returns an int from 0x0 to 0xFFFFFFFF */
	unsigned int NextRandomNumber();

	/* Random float from 0 to 1*/
	float RandFloat();

	/* Random float between min and max inclusive */
	float RandFloat( float Min, float Max  );

	/* Random int between Min and max inclusive */
	int RandInt( int Min, int Max );

	/* Return 2D perlin noise */
	float PerlinNoise( float fX, float fY, int iOctaves, float fPersistance = 0.25f);

	float InterpolatedNoise( float fX, float fY );
};
#endif