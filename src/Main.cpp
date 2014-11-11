// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include "UI/Application.hpp"

int main( int argc, char* argv[] ) {
	// Construct the main application
	Application main_app( argc, argv );

	// Execute the app
	return main_app.exec();
}
