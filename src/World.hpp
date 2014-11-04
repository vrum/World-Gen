// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef WORLD_HPP
#define WORLD_HPP

#include "Array.hpp"
#include "Vector.hpp"

class World {
	private:
		Array< double, 2 > m_height_map;
		Array< double, 2 > m_heat_map;

		Array< double, 2 > m_heat_source_map;

		double       m_elevation_heat_dropoff;
		double       m_heat_noise;
		bool         m_height_map_generated;
		double       m_land_heat_adjust;
		unsigned int m_octaves;
		double       m_persistence;
		double       m_sea_heat_adjust;
		double       m_sea_level;
		unsigned int m_seed;
		Vector2ui    m_size;

		void generateHeatMap();
		void generateHeatSourceMap();
		void updateMaps();

	public:
		World();

		void generateWorld();

		void generateHeightMap();
		void generateAuxillaryMaps();

		Array< double, 2 > getHeightMap() const;
		Array< double, 2 > getHeatMap()   const;

		double       getHeatDropoff() const;
		double       getHeatNoise()   const;
		double       getLandHeat()    const;
		unsigned int getOctaves()     const;
		double       getPersistence() const;
		double       getSeaHeat()     const;
		double       getSeaLevel()    const;
		unsigned int getSeed()        const;
		Vector2ui    getSize()        const;

		void setHeatDropoff( double heat_dropoff );
		void setHeatNoise( double heat_noise );
		void setLandHeat( double land_heat );
		void setOctaves( unsigned int octaves );
		void setPersistence( double persistence );
		void setSeaHeat( double sea_heat );
		void setSeaLevel( double sea_level );
		void setSeed();
		void setSeed( unsigned int seed );
		void setSize( Vector2ui size );
};

#endif // WORLD_HPP
