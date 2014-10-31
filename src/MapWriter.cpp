// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <SFML/Graphics/Image.hpp>
#include "MapWriter.hpp"

void MapWriter::setWorld( World& world ) {
	m_world = world;
}

void MapWriter::writeHeightMap( std::string filename ) {
	Array< double, 2 > heightmap = m_world.getHeightMap();
	Vector2ui size = m_world.size;

	sf::Image image;
	image.create( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			sf::Color pixel_color;

			// Land
			if( heightmap[ x ][ y ] > 0. ) {
				sf::Color mountain( 128, 128, 128 );
				sf::Color land( 0, 255, 0 );
				double mixer = ( heightmap[ x ][ y ] - m_world.sea_level ) / ( 1. - m_world.sea_level );
				pixel_color.r = static_cast< std::uint8_t >( ( 1. - mixer ) * land.r + mixer * mountain.r );
				pixel_color.g = static_cast< std::uint8_t >( ( 1. - mixer ) * land.g + mixer * mountain.g );
				pixel_color.b = static_cast< std::uint8_t >( ( 1. - mixer ) * land.b + mixer * mountain.b );
			}
			// Water
			else {
				sf::Color deep( 0, 0, 128 );
				sf::Color shallow( 0, 0, 255 );
				double mixer = ( m_world.sea_level - heightmap[ x ][ y ] ) / ( 1. + m_world.sea_level );
				pixel_color.r = static_cast< std::uint8_t >( ( 1. - mixer ) * shallow.r + mixer * deep.r );
				pixel_color.g = static_cast< std::uint8_t >( ( 1. - mixer ) * shallow.g + mixer * deep.g );
				pixel_color.b = static_cast< std::uint8_t >( ( 1. - mixer ) * shallow.b + mixer * deep.b );
			}

			image.setPixel( x, y, pixel_color );
		}
	}

	image.saveToFile( filename );
}

void MapWriter::writeTemperatureMap( std::string filename ) {
	Array< double, 2 > tempmap = m_world.getTemperatureMap();
	Vector2ui size = m_world.size;

	sf::Image image;
	image.create( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			sf::Color pixel_color;

			// Hot
			if( tempmap[ x ][ y ] > 0. ) {
				sf::Color hot( 255, 0, 0 );
				sf::Color neutral( 255, 255, 0 );
				double mixer = tempmap[ x ][ y ];
				pixel_color.r = static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.r + mixer * hot.r );
				pixel_color.g = static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.g + mixer * hot.g );
				pixel_color.b = static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.b + mixer * hot.b );
			}
			// Cold
			else {
				sf::Color neutral( 255, 255, 0 );
				sf::Color cold( 0, 0, 255 );
				double mixer = -tempmap[ x ][ y ];
				pixel_color.r = static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.r + mixer * cold.r );
				pixel_color.g = static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.g + mixer * cold.g );
				pixel_color.b = static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.b + mixer * cold.b );
			}

			image.setPixel( x, y, pixel_color );
		}
	}

	image.saveToFile( filename );
}
