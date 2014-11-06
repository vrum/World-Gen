// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <iostream>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include "MapWriter.hpp"
#include "World.hpp"
#include "UI/Application.hpp"

int main( int argc, char* argv[] ) {
	// Read command-line arguments
	if( argc < 3 + 1 ) {
		std::cout << "Invalid parameters passed. Parameters must be as such:\n";
		std::cout << "<program name> <image width> <image height> <output filename>\n";
		std::cout << "Optional parameters come after in the following order:\n";
		std::cout << "<noise layers> <persistence>";
		return -1;
	}

	Vector2ui size;
	size.x = static_cast< unsigned int >( std::stoul( std::string( argv[ 1 ] ) ) );
	size.y = static_cast< unsigned int >( std::stoul( std::string( argv[ 2 ] ) ) );

	unsigned int layers = 8;
	if( argc >= 4 + 1 ) {
		layers = static_cast< unsigned int >( std::stoul( std::string( argv[ 4 ] ) ) );
	}
	double persistence = 0.8;
	if( argc >= 5 + 1 ) {
		persistence = std::stod( std::string( argv[ 5 ] ) );
	}

	// Generate the world
	World world;
	world.setSize( size );
	world.setOctaves( layers );
	world.setPersistence( persistence );
	world.generateWorld();

	// Write the world to a file
	MapWriter writer;
	writer.setWorld( world );
	writer.writeHeightMapToFile( "Heightmap_" + std::string( argv[ 3 ] ) );
	writer.writeHeatMapToFile( "Heat_" + std::string( argv[ 3 ] ) );

	// Construct the main application
	Application main_app( argc, argv );

	// Execute the app
	return main_app.exec();
}
