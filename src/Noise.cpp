// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <cmath>
#include <random>
#include "Noise.hpp"

#include <iostream>

static double pi = 3.14159265358979;

Array< double, 2 > generateNoiseBase2D( Vector2ui size, unsigned int seed ) {
	Array< double, 2 > noise( size.x, size.y );
	std::mt19937_64 prng_engine( seed );
	std::uniform_real_distribution< double > prng;

	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			noise[ x ][ y ] = prng( prng_engine );
		}
	}

	return noise;
}

Array< double, 4 > generateNoiseBase4D( Vector2ui size, unsigned int seed ) {
	Array< double, 4 > noise( size.x, size.y, size.x, size.y );
	std::mt19937_64 prng_engine( seed );
	std::uniform_real_distribution< double > prng;

	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			for( unsigned int u = 0; u < size.x; ++u ) {
				for( unsigned int v = 0; v < size.y; ++v ) {
					noise[ x ][ y ][ u ][ v ] = prng( prng_engine );
				}
			}
		}
	}

	return noise;
}

Array< double, 2 > generateNoiseOctaveUntiled( Vector2ui size, unsigned int seed, double scaling ) {
	// Initialize the octave
	Array< double, 2 > octave = generateNoiseBase2D( size, seed );

	// Scale the octave
	Array< double, 2 > octave_scaled( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			// Recalculate the position
			double x_scaled = static_cast< double >( x ) / scaling;
			double y_scaled = static_cast< double >( y ) / scaling;

			// Calculate the sides
			unsigned int left = static_cast< unsigned int >( floor( x_scaled ) );
			unsigned int right = static_cast< unsigned int >( ceil( x_scaled ) );
			unsigned int top = static_cast< unsigned int >( floor( y_scaled ) );
			unsigned int bottom = static_cast< unsigned int >( ceil( y_scaled ) );

			// Calculate interpolation
			double x_interp = x_scaled - floor( x_scaled );
			double y_interp = y_scaled - floor( y_scaled );
			double t_x = x_interp * x_interp * ( 3 - 2 * x_interp );
			double t_y = y_interp * y_interp * ( 3 - 2 * y_interp );

			// Calculate the side values
			double topleft = octave[ left ][ top ];
			double topright = octave[ right ][ top ];
			double bottomleft = octave[ left ][ bottom ];
			double bottomright = octave[ right ][ bottom ];

			// Interpolate down to one value
			double left_value = ( 1 - t_y ) * topleft + t_y * bottomleft;
			double right_value = ( 1 - t_y ) * topright + t_y * bottomright;
			double value = ( 1 - t_x ) * left_value + t_x * right_value;

			octave_scaled[ x ][ y ] = value;
		}
	}

	return octave_scaled;
}

Array< double, 2 > generateNoiseOctaveTiled( Vector2ui size, unsigned int seed, double scaling ) {
	// Initialize the octave
	std::cout << "Generating 4D base\n";
	Array< double, 4 > octave = generateNoiseBase4D( size, seed );
	std::cout << "4D base generated\n";

	// Scale the octave
	Array< double, 2 > octave_scaled( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			std::cout << "Position: " << x << ", " << y << '\n';
			// Recalculate the position
			double x_scaled = static_cast< double >( x ) / ( scaling * size.x );
			double y_scaled = static_cast< double >( y ) / ( scaling * size.y );

			// Calculate the 4D parameters
			double nx = ( size.x - 1 ) * ( 1 + cos( x_scaled / ( 2 * pi ) ) ) / 2;
			double ny = ( size.y - 1 ) * ( 1 + cos( y_scaled / ( 2 * pi ) ) ) / 2;
			double nu = ( size.x - 1 ) * ( 1 + sin( x_scaled / ( 2 * pi ) ) ) / 2;
			double nv = ( size.y - 1 ) * ( 1 + sin( y_scaled / ( 2 * pi ) ) ) / 2;

			// Calculate the sides
			unsigned int x1 = static_cast< unsigned int >( floor( nx ) );
			unsigned int x2 = static_cast< unsigned int >( ceil( nx ) );
			unsigned int y1 = static_cast< unsigned int >( floor( ny ) );
			unsigned int y2 = static_cast< unsigned int >( ceil( ny ) );
			unsigned int u1 = static_cast< unsigned int >( floor( nu ) );
			unsigned int u2 = static_cast< unsigned int >( ceil( nu ) );
			unsigned int v1 = static_cast< unsigned int >( floor( nv ) );
			unsigned int v2 = static_cast< unsigned int >( ceil( nv ) );

			// Calculate the interpolations
			double x_interp = nx - floor( nx );
			double y_interp = ny - floor( ny );
			double u_interp = nu - floor( nu );
			double v_interp = nv - floor( nv );
			double t_x = x_interp * x_interp * ( 3 - 2 * x_interp );
			double t_y = y_interp * y_interp * ( 3 - 2 * y_interp );
			double t_u = u_interp * u_interp * ( 3 - 2 * u_interp );
			double t_v = v_interp * v_interp * ( 3 - 2 * v_interp );

			// Calculate the side values
			double val_1111 = octave[ x1 ][ y1 ][ u1 ][ v1 ];
			double val_1112 = octave[ x1 ][ y1 ][ u1 ][ v2 ];
			double val_1121 = octave[ x1 ][ y1 ][ u2 ][ v1 ];
			double val_1122 = octave[ x1 ][ y1 ][ u2 ][ v2 ];
			double val_1211 = octave[ x1 ][ y2 ][ u1 ][ v1 ];
			double val_1212 = octave[ x1 ][ y2 ][ u1 ][ v2 ];
			double val_1221 = octave[ x1 ][ y2 ][ u2 ][ v1 ];
			double val_1222 = octave[ x1 ][ y2 ][ u2 ][ v2 ];
			double val_2111 = octave[ x2 ][ y1 ][ u1 ][ v1 ];
			double val_2112 = octave[ x2 ][ y1 ][ u1 ][ v2 ];
			double val_2121 = octave[ x2 ][ y1 ][ u2 ][ v1 ];
			double val_2122 = octave[ x2 ][ y1 ][ u2 ][ v2 ];
			double val_2211 = octave[ x2 ][ y2 ][ u1 ][ v1 ];
			double val_2212 = octave[ x2 ][ y2 ][ u1 ][ v2 ];
			double val_2221 = octave[ x2 ][ y2 ][ u2 ][ v1 ];
			double val_2222 = octave[ x2 ][ y2 ][ u2 ][ v2 ];

			// Interpolation over v
			double val_111 = ( 1 - t_v ) * val_1111 + t_v * val_1112;
			double val_112 = ( 1 - t_v ) * val_1121 + t_v * val_1122;
			double val_121 = ( 1 - t_v ) * val_1211 + t_v * val_1212;
			double val_122 = ( 1 - t_v ) * val_1221 + t_v * val_1222;
			double val_211 = ( 1 - t_v ) * val_2111 + t_v * val_2112;
			double val_212 = ( 1 - t_v ) * val_2121 + t_v * val_2122;
			double val_221 = ( 1 - t_v ) * val_2211 + t_v * val_2212;
			double val_222 = ( 1 - t_v ) * val_2221 + t_v * val_2222;

			// Interpolation over u
			double val_11 = ( 1 - t_u ) * val_111 + t_u * val_112;
			double val_12 = ( 1 - t_u ) * val_121 + t_u * val_122;
			double val_21 = ( 1 - t_u ) * val_211 + t_u * val_212;
			double val_22 = ( 1 - t_u ) * val_221 + t_u * val_222;

			// Interpolation over y
			double val_1 = ( 1 - t_y ) * val_11 + t_y * val_12;
			double val_2 = ( 1 - t_y ) * val_21 + t_y * val_22;

			// Interpolation over x
			double value = ( 1 - t_x ) * val_1 + t_x * val_2;

			octave_scaled[ x ][ y ] = value;
		}
	}

	return octave_scaled;
}

Array< double, 2 > generateNoise( Vector2ui size, unsigned int octaves, double dropoff, bool tile ) {
	std::random_device entropy;
	unsigned int seed = entropy();
	return generateNoise( size, seed, octaves, dropoff, tile );
}

Array< double, 2 > generateNoise( Vector2ui size, unsigned int seed, unsigned int octaves, double dropoff, bool tile ) {
	// Initialize the noise
	std::cout << "Initializing noise\n";
	Array< double, 2 > noise( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			noise[ x ][ y ] = 0;
		}
	}
	std::cout << "Noise initialized\n";

	// Loop through the octaves
	double scaling = 1;
	double limit = 0;
	std::cout << "Octave Generation Start\n";
	for( unsigned int i = 0; i < octaves; ++i ) {
		std::cout << "Octave #" << i << '\n';
		Array< double, 2 > octave( size.x, size.y );

		// Untiled octave
		if( !tile ) {
			std::cout << "Generating untiled Octave\n";
			octave = generateNoiseOctaveUntiled( size, seed, scaling );
			std::cout << "Untiled Octave generated\n";
		}

		// Tiled octave
		else {
			std::cout << "Generating tiled Octave\n";
			octave = generateNoiseOctaveTiled( size, seed, scaling );
			std::cout << "Tiled Octave generated\n";
		}

		// Combine octaves
		for( unsigned int x = 0; x < size.x; ++x ) {
			for( unsigned int y = 0; y < size.y; ++y ) {
				noise[ x ][ y ] = noise[ x ][ y ] * dropoff + octave[ x ][ y ];
			}
		}

		// Update octave parameters
		scaling *= 2;
		limit = limit * dropoff + 1;
	}
	std::cout << "Octave Generation End\n";

	// Rescale the noise
	std::cout << "Rescaling Start\n";
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			noise[ x ][ y ] /= limit;
		}
	}
	std::cout << "Rescaling End\n";

	return noise;
}
