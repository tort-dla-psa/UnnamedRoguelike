// FastNoise.h
//
// MIT License
//
// Copyright(c) 2017 Jordan Peck
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The developer's email is jorzixdan.me2@gzixmail.com (for great email, take
// off every 'zix'.)
//

// VERSION: 0.4.1

#ifndef FASTNOISE_H
#define FASTNOISE_H

// Uncomment the line below to use doubles throughout FastNoise instead of floats
#define FN_CELLULAR_INDEX_MAX 3
class FastNoise
{
public:
	explicit FastNoise(int seed = 1337) { SetSeed(seed); CalculateFractalBounding(); }

	enum NoiseType { Perlin, PerlinFractal, Simplex, SimplexFractal};

	void SetSeed(int seed);
	int GetSeed() const { return m_seed; }

	void SetFrequency(double frequency) { m_frequency = frequency; }							// Default: 0.01
	double GetFrequency() const { return m_frequency; }
	
	void SetNoiseType(NoiseType noiseType) { m_noiseType = noiseType; }							// Default: Simplex
	NoiseType GetNoiseType() const { return m_noiseType; }

	void SetFractalOctaves(int octaves) { m_octaves = octaves; CalculateFractalBounding(); }	// Default: 3
	int GetFractalOctaves() const { return m_octaves; }
	
	void SetFractalLacunarity(double lacunarity) { m_lacunarity = lacunarity; }					// Default: 2.0
	double GetFractalLacunarity() const { return m_lacunarity; }

	void SetFractalGain(double gain) { m_gain = gain; CalculateFractalBounding(); }				// Default: 0.5
	double GetFractalGain() const { return m_gain; }

	//2D
	double GetPerlin(double x, double y) const;
	double GetPerlinFractal(double x, double y) const;
	double GetSimplex(double x, double y) const;
	double GetSimplexFractal(double x, double y) const;
	double GetNoise(double x, double y) const;
	//3D
	double GetPerlin(double x, double y, double z) const;
	double GetPerlinFractal(double x, double y, double z) const;
	double GetSimplex(double x, double y, double z) const;
	double GetSimplexFractal(double x, double y, double z) const;
	double GetNoise(double x, double y, double z) const;

private:
	unsigned char m_perm[512];
	unsigned char m_perm12[512];

	int m_seed = 1337;
	double m_frequency = double(0.01);
	NoiseType m_noiseType = Simplex;
	int m_octaves = 3;
	double m_lacunarity = double(2);
	double m_gain = double(0.5);
	double m_fractalBounding;

	void CalculateFractalBounding();

	//2D
	double SinglePerlinFractalFBM(double x, double y) const;
	double SinglePerlin(unsigned char offset, double x, double y) const;
	double SingleSimplexFractalFBM(double x, double y) const;
	double SingleSimplex(unsigned char offset, double x, double y) const;
	//3D
	double SinglePerlinFractalFBM(double x, double y, double z) const;
	double SinglePerlin(unsigned char offset, double x, double y, double z) const;
	double SingleSimplexFractalFBM(double x, double y, double z) const;
	double SingleSimplex(unsigned char offset, double x, double y, double z) const;

	inline unsigned char Index2D_12(unsigned char offset, int x, int y) const;
	inline unsigned char Index3D_12(unsigned char offset, int x, int y, int z) const;
	inline unsigned char Index2D_256(unsigned char offset, int x, int y) const;
	inline unsigned char Index3D_256(unsigned char offset, int x, int y, int z) const;
	inline double ValCoord2DFast(unsigned char offset, int x, int y) const;
	inline double ValCoord3DFast(unsigned char offset, int x, int y, int z) const;
	inline double GradCoord2D(unsigned char offset, int x, int y, double xd, double yd) const;
	inline double GradCoord3D(unsigned char offset, int x, int y, int z, double xd, double yd, double zd) const;
};
#endif