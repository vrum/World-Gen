// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QApplication>
#include "MainWindow.hpp"

class Application : public QApplication {
	public:
		MainWindow main_window;

		Application( int& argc, char* argv[] );
};

#endif // APPLICATION_HPP
