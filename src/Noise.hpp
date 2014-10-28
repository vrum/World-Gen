// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef NOISE_HPP
#define NOISE_HPP

#include <vector>
#include "Array.hpp"
#include "Vector.hpp"

Array2D< double > generate2DNoise( Vector2ui size );

Array2D< double > generateNoise( Vector2ui size, unsigned int rounds = 8, unsigned int resolution_tuning = 2, double dropoff = 0.5 );

#endif // NOISE_HPP
