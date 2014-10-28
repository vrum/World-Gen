// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <SFML/Graphics/Image.hpp>
#include "Noise.hpp"

int main( int argc, char* argv[] ) {
	Vector2ui size;
	size.x = 512;
	size.y = 512;

	// Generate noise
	Array2D< double > noise = generateNoise( size, 8, 2, 0.8, true );

	// Write the noise to an image
	sf::Image noisy_image;
	noisy_image.create( size.x, size.y );
	for( unsigned int x = 0; x < size.x; ++x ) {
		for( unsigned int y = 0; y < size.y; ++y ) {
			sf::Uint8 gray = static_cast< sf::Uint8 >( ( noise[ x ][ y ] + 1. ) / 2. * 255. );
			noisy_image.setPixel( x, y, sf::Color( gray, gray, gray ) );
		}
	}

	// Save the image
	noisy_image.saveToFile( "Noise.png" );

	return 0;
}
