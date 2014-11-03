// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef NOISE_HPP
#define NOISE_HPP

#include "Array.hpp"
#include "Vector.hpp"

double noiseFunc2D( unsigned int seed, unsigned int x, unsigned int y );
double noiseFunc4D( unsigned int seed, unsigned int x, unsigned int y, unsigned int u, unsigned int v );

Array< double, 2 > generateNoiseOctaveUntiled( Vector2ui size, unsigned int seed, double scaling );
Array< double, 2 > generateNoiseOctaveTiled( Vector2ui size, unsigned int seed, double scaling );

Array< double, 2 > generateNoise( Vector2ui size, unsigned int octaves = 8, double dropoff = 0.5, bool tile = true );
Array< double, 2 > generateNoise( Vector2ui size, unsigned int seed, unsigned int octaves = 8, double dropoff = 0.5,
								  bool tile = true );

Array< double, 2 > normalizeNoise( Array< double, 2 > noise, bool extend = false );

#endif // NOISE_HPP
