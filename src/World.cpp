// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <cmath>
#include <random>
#include "Noise.hpp"
#include "World.hpp"

World::World() :
	m_height_map( 0u, 0u ),
	m_temperature_map( 0u, 0u ),
	m_wind_map_x( 0u, 0u ),
	m_wind_map_y( 0u, 0u ),
	m_temperature_base_map( 0u, 0u ),
	m_wind_x_base_map( 0u, 0u ) {
	std::random_device entropy;
	elevation_dropoff       = 0.5;
	land_temperature_adjust = 0.05;
	iterations              = 64;
	iteration_mixer         = .01;
	iteration_precision     = .1;
	octaves                 = 8;
	persistence             = 0.5;
	pressure_scale          = 10;
	sea_level               = 0.;
	sea_temperature_adjust  = -0.05;
	seed                    = entropy();
	size                    = { 0, 0 };
	temperature_noise       = 0.;
	wind_noise              = 0.;
	wind_speed              = 0.5;
}

void World::generateWorld() {
	// Start with the height map
	m_height_map = generateNoise( size, seed, octaves, persistence, true );

	// Extend the height map
	m_height_map = normalizeNoise( m_height_map, true );

	// Construct the base maps
	m_temperature_base_map = Array< double, 2 >( size.x, size.y );
	m_wind_x_base_map = Array< double, 2 >( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			double y_norm = static_cast< double >( y ) / static_cast< double >( size.y );
			double eq_dist = 1. - 4. * std::abs( y_norm - 0.5 );
			double elevation = m_height_map[ x ][ y ] - sea_level;
			double location_adjust = ( elevation > 0. ?
									   land_temperature_adjust - elevation_dropoff * elevation :
									   sea_temperature_adjust );
			m_temperature_base_map[ x ][ y ] = eq_dist + location_adjust;
			m_wind_x_base_map[ x ][ y ] = wind_speed * eq_dist;
		}
	}
	m_temperature_base_map = normalizeNoise( m_temperature_base_map );
	m_wind_x_base_map = normalizeNoise( m_wind_x_base_map );

	// Initialize the other maps
	m_temperature_map = generateNoise( size, seed + 1, octaves, persistence, true ) * temperature_noise;
	m_wind_map_x = generateNoise( size, seed + 2, octaves, persistence, true ) * wind_noise;
	m_wind_map_y = generateNoise( size, seed + 3, octaves, persistence, true ) * wind_noise;

	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			m_temperature_map[ x ][ y ] += m_temperature_base_map[ x ][ y ];
			m_wind_map_x[ x ][ y ] += m_wind_x_base_map[ x ][ y ];
		}
	}

	m_temperature_map = normalizeNoise( m_temperature_map );
	m_wind_map_x = normalizeNoise( m_wind_map_x );

	// Feed the other maps into each other
	for( unsigned int i = 1; i < iterations; ++i ) {
		updateMaps();
	}
}

Array< double, 2 > World::getHeightMap() const {
	return m_height_map;
}

Array< double, 2 > World::getTemperatureMap() const {
	return m_temperature_map;
}

Array< double, 2 > World::getWindMapX() const {
	return m_wind_map_x;
}

Array< double, 2 > World::getWindMapY() const {
	return m_wind_map_y;
}

void World::updateMaps() {
	// Create copies of the maps
	Array< double, 2 > temp_map_old = m_temperature_map;
	Array< double, 2 > wind_map_x_old = m_wind_map_x;
	Array< double, 2 > wind_map_y_old = m_wind_map_y;

	// Do the calculations
	Array< double, 2 > temp_grad_x( size.x, size.y );
	Array< double, 2 > temp_grad_y( size.x, size.y );
	Array< double, 2 > wind_x_grad_x( size.x, size.y );
	Array< double, 2 > wind_x_grad_y( size.x, size.y );
	Array< double, 2 > wind_y_grad_x( size.x, size.y );
	Array< double, 2 > wind_y_grad_y( size.x, size.y );
	Array< double, 2 > wind_divergence( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			// Gradients
			// Left edge
			if( x == 0 ) {
				temp_grad_x[ x ][ y ] = ( temp_map_old[ 1 ][ y ] - temp_map_old[ size.x - 1 ][ y ] ) / 2.;
				wind_x_grad_x[ x ][ y ] = ( wind_map_x_old[ 1 ][ y ] - wind_map_x_old[ size.x - 1 ][ y ] ) / 2.;
				wind_y_grad_x[ x ][ y ] = ( wind_map_y_old[ 1 ][ y ] - wind_map_y_old[ size.x - 1 ][ y ] ) / 2.;
			}
			// Right edge
			else if( x == size.x - 1 ) {
				temp_grad_x[ x ][ y ] = ( temp_map_old[ 0 ][ y ] - temp_map_old[ size.x - 2 ][ y ] ) / 2.;
				wind_x_grad_x[ x ][ y ] = ( wind_map_x_old[ 0 ][ y ] - wind_map_x_old[ size.x - 2 ][ y ] ) / 2.;
				wind_y_grad_x[ x ][ y ] = ( wind_map_y_old[ 0 ][ y ] - wind_map_y_old[ size.x - 2 ][ y ] ) / 2.;
			}
			// Middle
			else {
				temp_grad_x[ x ][ y ] = ( temp_map_old[ x + 1 ][ y ] - temp_map_old[ x - 1 ][ y ] ) / 2.;
				wind_x_grad_x[ x ][ y ] = ( wind_map_x_old[ x + 1 ][ y ] - wind_map_x_old[ x - 1 ][ y ] ) / 2.;
				wind_y_grad_x[ x ][ y ] = ( wind_map_y_old[ x + 1 ][ y ] - wind_map_y_old[ x - 1 ][ y ] ) / 2.;
			}

			// Top edge
			if( y == 0 ) {
				temp_grad_y[ x ][ y ] = ( temp_map_old[ x ][ 1 ] - temp_map_old[ x ][ size.y - 1 ] ) / 2.;
				wind_x_grad_y[ x ][ y ] = ( wind_map_x_old[ x ][ 1 ] - wind_map_x_old[ x ][ size.y - 1 ] ) / 2.;
				wind_y_grad_y[ x ][ y ] = ( wind_map_y_old[ x ][ 1 ] - wind_map_y_old[ x ][ size.y - 1 ] ) / 2.;
			}
			// Bottom edge
			else if( y == size.y - 1 ) {
				temp_grad_y[ x ][ y ] = ( temp_map_old[ x ][ 0 ] - temp_map_old[ x ][ size.y - 2 ] ) / 2.;
				wind_x_grad_y[ x ][ y ] = ( wind_map_x_old[ x ][ 0 ] - wind_map_x_old[ x ][ size.y - 2 ] ) / 2.;
				wind_y_grad_y[ x ][ y ] = ( wind_map_y_old[ x ][ 0 ] - wind_map_y_old[ x ][ size.y - 2 ] ) / 2.;
			}
			// Middle
			else {
				temp_grad_y[ x ][ y ] = ( temp_map_old[ x ][ y + 1 ] - temp_map_old[ x ][ y - 1 ] ) / 2.;
				wind_x_grad_y[ x ][ y ] = ( wind_map_x_old[ x ][ y + 1 ] - wind_map_x_old[ x ][ y - 1 ] ) / 2.;
				wind_y_grad_y[ x ][ y ] = ( wind_map_y_old[ x ][ y + 1 ] - wind_map_y_old[ x ][ y - 1 ] ) / 2.;
			}

			// Wind divergence
			wind_divergence[ x ][ y ] = wind_x_grad_x[ x ][ y ] + wind_y_grad_y[ x ][ y ];

			// Update temperature using wind flow and wind flow using temperature gradients
			double delta_temp = temp_map_old[ x ][ y ] * wind_divergence[ x ][ y ] +
								wind_map_x_old[ x ][ y ] * temp_grad_x[ x ][ y ] +
								wind_map_y_old[ x ][ y ] * temp_grad_y[ x ][ y ];
			double delta_wind_x = pressure_scale * temp_grad_x[ x ][ y ] +
								  wind_map_x_old[ x ][ y ] * wind_x_grad_x[ x ][ y ] +
								  wind_map_y_old[ x ][ y ] * wind_x_grad_y[ x ][ y ];
			double delta_wind_y = pressure_scale * temp_grad_y[ x ][ y ] +
								  wind_map_x_old[ x ][ y ] * wind_y_grad_x[ x ][ y ] +
								  wind_map_y_old[ x ][ y ] * wind_y_grad_y[ x ][ y ];
			m_temperature_map[ x ][ y ] -= iteration_precision * delta_temp;
			m_wind_map_x[ x ][ y ] -= iteration_precision * delta_wind_x;
			m_wind_map_y[ x ][ y ] -= iteration_precision * delta_wind_y;

			// Average with the base maps (since they represent sources)
			m_temperature_map[ x ][ y ] = iteration_mixer * m_temperature_map[ x ][ y ] +
										  ( 1. - iteration_mixer ) * m_temperature_base_map[ x ][ y ];
			m_wind_map_x[ x ][ y ] = iteration_mixer * m_wind_map_x[ x ][ y ] +
									 ( 1. - iteration_mixer ) * m_wind_x_base_map[ x ][ y ];
		}
	}

	// Renormalize the maps
	m_temperature_map = normalizeNoise( m_temperature_map );
	m_wind_map_x = normalizeNoise( m_wind_map_x );
	m_wind_map_y = normalizeNoise( m_wind_map_y );
}
