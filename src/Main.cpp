// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <iostream>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include "Noise.hpp"

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

	// Generate noise
	Array2D< double > noise = generateNoise( size, layers, 2, persistence, true );

	// Write the noise to an image
	sf::Image noisy_image;
	noisy_image.create( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			sf::Color pixel_color;

			// Land
			if( noise[ x ][ y ] > 0.03 ) {
				pixel_color = sf::Color::Green;
			}
			// Water
			else {
				pixel_color = sf::Color::Blue;
			}

			noisy_image.setPixel( x, y, pixel_color );
		}
	}

	// Save the image
	noisy_image.saveToFile( std::string( argv[ 3 ] ) );

	return 0;
}
