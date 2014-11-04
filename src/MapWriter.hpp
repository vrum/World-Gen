// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef MAPWRITER_HPP
#define MAPWRITER_HPP

#include <SFML/Graphics/Image.hpp>
#include "World.hpp"

class MapWriter {
	private:
		World m_world;

	public:
		void setWorld( World& world );

		sf::Image writeHeightMapToImage();
		sf::Image writeHeatMapToImage();

		void writeHeightMapToFile( std::string filename );
		void writeHeatMapToFile( std::string filename );
};

#endif // MAPWRITER_HPP
