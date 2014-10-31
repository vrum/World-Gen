// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef WORLD_HPP
#define WORLD_HPP

#include "Array.hpp"
#include "Vector.hpp"

class World {
	private:
		Array< double, 2 > m_height_map;
		Array< double, 2 > m_temperature_map;
		Array< double, 2 > m_wind_map_x;
		Array< double, 2 > m_wind_map_y;

		Array< double, 2 > m_temperature_base_map;

		void updateMaps();

	public:
		World();

		void generateWorld();

		Array< double, 2 > getHeightMap() const;
		Array< double, 2 > getTemperatureMap() const;
		Array< double, 2 > getWindMapX() const;
		Array< double, 2 > getWindMapY() const;

		unsigned int iterations;
		unsigned int octaves;
		double       persistence;
		double       sea_level;
		unsigned int seed;
		Vector2ui    size;
		double       temperature_noise;
		double       wind_noise;
};

#endif // WORLD_HPP
