// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <cmath>
#include <random>
#include "Noise.hpp"
#include "World.hpp"

static const unsigned int default_width = 256;
static const unsigned int default_height = 256;

World::World() :
	m_height_map( default_width, default_height ),
	m_heat_map( default_width, default_height ),
	m_heat_source_map( default_width, default_height ),
	m_size{ default_width, default_height } {
	std::random_device entropy;
	m_elevation_heat_dropoff = 0.5;
	m_heat_noise             = 0.01;
	m_height_map_generated   = false;
	m_land_heat_adjust       = 0.05;
	m_octaves                = 10;
	m_persistence            = 0.6;
	m_sea_heat_adjust        = -0.05;
	m_sea_level              = 0.;
	m_seed                   = entropy();
}

void World::generateAuxillaryMaps() {
	if( !m_height_map_generated ) {
		generateHeightMap();
	}

	generateHeatSourceMap();
	generateHeatMap();
}

void World::generateHeatMap() {
	// Use the source map to generate the heat map
	m_heat_map = m_heat_source_map;

	// Generate some noise
	Array< double, 2 > heat_noise = generateNoise( m_size, m_seed + 1, m_octaves, m_persistence, true );

	// Then add in the noise
	for( unsigned int x = 0; x < m_size.x; ++x ) {
		for( unsigned int y = 0; y < m_size.y; ++y ) {
			m_heat_map[ x ][ y ] += m_heat_noise * ( heat_noise[ x ][ y ] - m_heat_map[ x ][ y ] );
		}
	}

	// Renormalize the heatmap
	m_heat_map = normalizeNoise( m_heat_map );
}

void World::generateHeatSourceMap() {
	// Iterate the pixels
	m_heat_source_map = Array< double, 2 >( m_size.x, m_size.y );
	for( unsigned int x = 0; x < m_size.x; ++x ) {
		for( unsigned int y = 0; y < m_size.y; ++y ) {
			// Set the temperature based on:
			// -Distance from the equator
			// -Whether the point is land or sea
			// -The elevation if on land
			double y_norm = static_cast< double >( y ) / static_cast< double >( m_size.y );
			double equator_distance = 1. - 4. * std::abs( y_norm - 0.5 );
			double elevation = m_height_map[ x ][ y ] - m_sea_level;
			double location_heat_adjustment = ( elevation > 0. ?
												m_land_heat_adjust - m_elevation_heat_dropoff * elevation :
												m_sea_heat_adjust );
			m_heat_source_map[ x ][ y ] = equator_distance + location_heat_adjustment;
		}
	}

	// Normalize the heat source map
	m_heat_source_map = normalizeNoise( m_heat_source_map );
}

void World::generateHeightMap() {
	// Create the height map
	m_height_map = generateNoise( m_size, m_seed, m_octaves, m_persistence, true );

	// Extend it to the full range
	m_height_map = normalizeNoise( m_height_map, true );

	m_height_map_generated = true;
}

void World::generateWorld() {
	// Generate the height map first
	generateHeightMap();

	// Then construct the auxillary maps
	generateAuxillaryMaps();
}

Array< double, 2 > World::getHeightMap() const {
	return m_height_map;
}

Array< double, 2 > World::getHeatMap() const {
	return m_heat_map;
}

double World::getHeatDropoff() const {
	return m_elevation_heat_dropoff;
}

double World::getHeatNoise() const {
	return m_heat_noise;
}

double World::getLandHeat() const {
	return m_land_heat_adjust;
}

unsigned int World::getOctaves() const {
	return m_octaves;
}

double World::getPersistence() const {
	return m_persistence;
}

double World::getSeaHeat() const {
	return m_sea_heat_adjust;
}

double World::getSeaLevel() const {
	return m_sea_level;
}

unsigned int World::getSeed() const {
	return m_seed;
}

Vector2ui World::getSize() const {
	return m_size;
}

void World::setHeatDropoff( double heat_dropoff ) {
	m_elevation_heat_dropoff = heat_dropoff;
}

void World::setHeatNoise( double heat_noise ) {
	m_heat_noise = heat_noise;
}

void World::setLandHeat( double land_heat ) {
	m_land_heat_adjust = land_heat;
}

void World::setOctaves( unsigned int octaves ) {
	m_octaves = octaves;
	m_height_map_generated = false;
}

void World::setPersistence( double persistence ) {
	m_persistence = persistence;
	m_height_map_generated = false;
}

void World::setSeaHeat( double sea_heat ) {
	m_sea_heat_adjust = sea_heat;
}

void World::setSeaLevel( double sea_level ) {
	m_sea_level = sea_level;
}

void World::setSeed() {
	std::random_device entropy;
	m_seed = entropy();
	m_height_map_generated = false;
}

void World::setSeed( unsigned int seed ) {
	m_seed = seed;
	m_height_map_generated = false;
}

void World::setSize( Vector2ui size ) {
	m_size = size;
	m_height_map_generated = false;
}
