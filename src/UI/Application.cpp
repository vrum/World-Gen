// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include "Application.hpp"

Application::Application( int& argc, char* argv[] ) :
	QApplication( argc, argv ) {
	createMainWindow();
}

void Application::createMainWindow() {
	// Create the main viewer
	createViewerTabs();
	m_main_window.setCentralWidget( m_viewer_tabs );

	// Create the settings dock
	createSettingsDock();
	m_main_window.addDockWidget( Qt::LeftDockWidgetArea, m_settings_dock );

	// Show the window
	m_main_window.show();
	m_main_window.setWindowState( Qt::WindowMaximized );
}

void Application::createSettingsBox() {
	// Create the box
	m_settings_box = new QGroupBox;
	m_settings_box->setFlat( true );

	// Create the layout
	createSettingsLayout();
	m_settings_box->setLayout( m_settings_layout );
}

void Application::createSettingsDock() {
	m_settings_dock = new QDockWidget( "Settings" );
	createSettingsBox();
	m_settings_dock->setWidget( m_settings_box );
}

void Application::createSettingsLayout() {
	m_settings_layout = new QVBoxLayout;

	// Setup the settings layout

	// Setup the size constraints
	m_settings_layout->setSizeConstraint( QLayout::SetMinimumSize );
}

void Application::createViewerTabs() {
	m_viewer_tabs = new QTabWidget;
}

Application::~Application() {
}
