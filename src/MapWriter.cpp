// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <QColor>
#include "MapWriter.hpp"

void MapWriter::setWorld( World& world ) {
	m_world = world;
}

void MapWriter::writeHeatMapToFile( std::string filename ) {
	writeHeatMapToFile( m_world, filename );
}

void MapWriter::writeHeatMapToFile( World& world, std::string filename ) {
	QImage image = writeHeatMapToImage( world );

	image.save( filename.c_str() );
}

QImage MapWriter::writeHeatMapToImage() {
	return MapWriter::writeHeatMapToImage( m_world );
}

QImage MapWriter::writeHeatMapToImage( World& world ) {
	Array< double, 2 > heatmap = world.getHeatMap();
	Vector2ui size = world.getSize();

	QImage image( static_cast< int >( size.x ), static_cast< int >( size.y ), QImage::Format_ARGB32 );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			QColor pixel_color;

			// Hot
			if( heatmap[ x ][ y ] > 0. ) {
				QColor hot( 255, 0, 0 );
				QColor neutral( 255, 255, 0 );
				double mixer = heatmap[ x ][ y ];
				pixel_color.setRed( static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.red() + mixer * hot.red() ) );
				pixel_color.setGreen( static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.green() + mixer * hot.green() ) );
				pixel_color.setBlue( static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.blue() + mixer * hot.blue() ) );
			}
			// Cold
			else {
				QColor neutral( 255, 255, 0 );
				QColor cold( 0, 0, 255 );
				double mixer = -heatmap[ x ][ y ];
				pixel_color.setRed( static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.red() + mixer * cold.red() ) );
				pixel_color.setGreen( static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.green() + mixer * cold.green() ) );
				pixel_color.setBlue( static_cast< std::uint8_t >( ( 1. - mixer ) * neutral.blue() + mixer * cold.blue() ) );
			}

			image.setPixel( static_cast< int >( x ), static_cast< int >( y ), pixel_color.rgba() );
		}
	}

	return image;
}

void MapWriter::writeHeightMapToFile( std::string filename ) {
	writeHeightMapToFile( m_world, filename );
}

void MapWriter::writeHeightMapToFile( World& world, std::string filename ) {
	QImage image = writeHeightMapToImage( world );

	image.save( filename.c_str() );
}

QImage MapWriter::writeHeightMapToImage() {
	return MapWriter::writeHeightMapToImage( m_world );
}

QImage MapWriter::writeHeightMapToImage( World& world ) {
	Array< double, 2 > heightmap = world.getHeightMap();
	Vector2ui size = world.getSize();
	double sea_level = world.getSeaLevel();

	QImage image( static_cast< int >( size.x ), static_cast< int >( size.y ), QImage::Format_ARGB32 );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			QColor pixel_color;

			// Land
			if( heightmap[ x ][ y ] > sea_level ) {
				QColor mountain( 128, 128, 128 );
				QColor land( 0, 255, 0 );
				double mixer = ( heightmap[ x ][ y ] - sea_level ) / ( 1. - sea_level );
				pixel_color.setRed( static_cast< std::uint8_t >( ( 1. - mixer ) * land.red() + mixer * mountain.red() ) );
				pixel_color.setGreen( static_cast< std::uint8_t >( ( 1. - mixer ) * land.green() + mixer * mountain.green() ) );
				pixel_color.setBlue( static_cast< std::uint8_t >( ( 1. - mixer ) * land.blue() + mixer * mountain.blue() ) );
			}
			// Water
			else {
				QColor deep( 0, 0, 128 );
				QColor shallow( 0, 0, 255 );
				double mixer = ( sea_level - heightmap[ x ][ y ] ) / ( 1. + sea_level );
				pixel_color.setRed( static_cast< std::uint8_t >( ( 1. - mixer ) * shallow.red() + mixer * deep.red() ) );
				pixel_color.setGreen( static_cast< std::uint8_t >( ( 1. - mixer ) * shallow.green() + mixer * deep.green() ) );
				pixel_color.setBlue( static_cast< std::uint8_t >( ( 1. - mixer ) * shallow.blue() + mixer * deep.blue() ) );
			}

			image.setPixel( static_cast< int >( x ), static_cast< int >( y ), pixel_color.rgba() );
		}
	}

	return image;
}
