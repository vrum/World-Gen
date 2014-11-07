// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include "Application.hpp"

Application::Application( int& argc, char* argv[] ) :
	QApplication( argc, argv ) {
	main_window.show();
	main_window.setWindowState( Qt::WindowMaximized );
}
