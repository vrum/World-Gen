// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef NOISE_HPP
#define NOISE_HPP

#include "Array.hpp"
#include "Vector.hpp"

Array< double, 2 > generateNoiseBase2D( Vector2ui size, unsigned int seed );
Array< double, 4 > generateNoiseBase4D( Vector2ui size, unsigned int seed );

Array< double, 2 > generateNoiseOctaveUntiled( Vector2ui size, unsigned int seed, double scaling );
Array< double, 2 > generateNoiseOctaveTiled( Vector2ui size, unsigned int seed, double scaling );

Array< double, 2 > generateNoise( Vector2ui size, unsigned int octaves = 8, double dropoff = 0.5, bool tile = true );
Array< double, 2 > generateNoise( Vector2ui size, unsigned int seed, unsigned int octaves = 8, double dropoff = 0.5,
								  bool tile = true );

#endif // NOISE_HPP
