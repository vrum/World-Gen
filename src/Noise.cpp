// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <cmath>
#include <random>
#include "Noise.hpp"

Array2D< double > generate2DNoise( Vector2ui size ) {
	std::random_device entropy;
	std::mt19937_64 prng( entropy() );
	std::uniform_real_distribution< double > rng( -1.0, 1.0 );
	Array2D< double > noise_map( size.x, size.y );

	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			noise_map[ x ][ y ] = rng( prng );
		}
	}

	return noise_map;
}

Array2D< double > generateNoise( Vector2ui size, unsigned int rounds, unsigned int resolution_tuning, double dropoff,
								 bool tile ) {

	// Prepare for tiling
	if( tile ) {
		size.x *= 2;
		size.y *= 2;
	}

	// Initialize the noise array
	Array2D< double > noise( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			noise[ x ][ y ] = 0.;
		}
	}

	// Loop to calculate each round of noise
	unsigned int spacing = 1;
	double limit = 0.;
	for( unsigned int i = 0; i < rounds; ++i ) {
		// Create and scale the next layer of noise
		Array2D< double > next_noise = generate2DNoise( size );
		Array2D< double > next_noise_scaled( size.x, size.y );
		for( unsigned int x = 0; x < size.x; ++x ) {
			for( unsigned int y = 0; y < size.y; ++y ) {
				// Get the position in the noise
				double x_pos = static_cast< double >( x ) / static_cast< double >( spacing );
				double y_pos = static_cast< double >( y ) / static_cast< double >( spacing );

				// Get the four sides
				unsigned int left_pos = static_cast< unsigned int >( floor( x_pos ) );
				unsigned int right_pos = static_cast< unsigned int >( ceil( x_pos ) );
				unsigned int top_pos = static_cast< unsigned int >( floor( y_pos ) );
				unsigned int bottom_pos = static_cast< unsigned int >( ceil( y_pos ) );

				// Get the interpolation factors
				double x_interp = x_pos - floor( x_pos );
				double y_interp = y_pos - floor( y_pos );

				// Get the four values
				double topleft_value = next_noise[ left_pos ][ top_pos ];
				double topright_value = next_noise[ right_pos ][ top_pos ];
				double bottomleft_value = next_noise[ left_pos ][ bottom_pos ];
				double bottomright_value = next_noise[ right_pos ][ bottom_pos ];

				// Interpolate the four values into one
				double left_value = ( 1 - y_interp ) * topleft_value + y_interp * bottomleft_value;
				double right_value = ( 1 - y_interp ) * topright_value + y_interp * bottomright_value;
				double value = ( 1 - x_interp ) * left_value + x_interp * right_value;

				// Set the noise value
				next_noise_scaled[ x ][ y ] = value;
			}
		}

		// Add the new layer of noise into the overall noise array
		for( unsigned int x = 0; x < size.x; ++x ) {
			for( unsigned int y = 0; y < size.y; ++y ) {
				noise[ x ][ y ] = dropoff * noise[ x ][ y ] + next_noise_scaled[ x ][ y ];
			}
		}

		// Scale the resolution with each step
		spacing *= resolution_tuning;
		limit *= dropoff;
		limit += 1.;
	}

	// Linearly interpolate so that the noise tiles
	if( tile ) {
		Array2D< double > noise_backup = noise;
		for( unsigned int x = size.x / 2; x < size.x; ++x ) {
			for( unsigned int y = size.y / 2; y < size.y; ++y ) {
				double value_a = noise_backup[ x ][ y ];
				double value_b = noise_backup[ x - size.x / 2 ][ y ];
				double value_c = noise_backup[ x ][ y - size.y / 2 ];
				double value_d = noise_backup[ x - size.x / 2 ][ y - size.y / 2 ];
				double x_norm = static_cast< double >( x - size.x / 2 ) / static_cast< double >( size.x / 2 );
				double y_norm = static_cast< double >( y - size.y / 2 ) / static_cast< double >( size.y / 2 );
				double x_interp = x_norm * x_norm * ( 3. - 2. * x_norm );
				double y_interp = y_norm * y_norm * ( 3. - 2. * y_norm );
				double weight_a = ( 1. - x_interp ) * ( 1. - y_interp );
				double weight_b = x_interp * ( 1. - y_interp );
				double weight_c = ( 1. - x_interp ) * y_interp;
				double weight_d = x_interp * y_interp;
				double new_value = weight_a * value_a + weight_b * value_b + weight_c * value_c + weight_d * value_d;
				noise[ x ][ y ] = new_value;
			}
		}
	}

	// Finish by rescaling the noise
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			noise[ x ][ y ] /= limit;
		}
	}

	// Trim it down to just the bottom-right corner if tiling
	if( tile ) {
		Array2D< double > final_noise( size.x / 2, size.y / 2 );
		for( unsigned int x = size.x / 2; x < size.x; ++x ) {
			for( unsigned int y = size.y / 2; y < size.y; ++y ) {
				final_noise[ x - size.x / 2 ][ y - size.y / 2 ] = noise[ x ][ y ];
			}
		}

		return final_noise;
	}

	return noise;
}
