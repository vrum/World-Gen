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
	m_temperature_base_map( 0u, 0u ) {
	std::random_device entropy;
	iterations = 1;
	octaves = 8;
	persistence = 0.5;
	sea_level = 0.;
	seed = entropy();
	size = { 0, 0 };
	temperature_noise = 0.01;
	wind_noise = 0.01;
}

void World::generateWorld() {
	// Start with the height map
	m_height_map = generateNoise( size, seed, octaves, persistence, true );

	// Construct the base maps
	m_temperature_base_map = Array< double, 2 >( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			double y_norm = static_cast< double >( y ) / static_cast< double >( size.y );
			double eq_dist = 1. - 4. * std::abs( y_norm - 0.5 );
			m_temperature_base_map[ x ][ y ] = eq_dist;
		}
	}

	// Initialize the other maps
	m_temperature_map = generateNoise( size, seed + 1, octaves, persistence, true ) * temperature_noise;
	m_wind_map_x = generateNoise( size, seed + 2, octaves, persistence, true ) * wind_noise;
	m_wind_map_y = generateNoise( size, seed + 3, octaves, persistence, true ) * wind_noise;

	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			m_temperature_map[ x ][ y ] += m_temperature_base_map[ x ][ y ];
		}
	}

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

}
