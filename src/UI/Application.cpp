// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <QLabel>
#include <SFML/Graphics/Texture.hpp>
#include "Application.hpp"

Application::Application( int& argc, char* argv[] ) :
	QApplication( argc, argv ) {
	// Do the setup!
	setupMainWindow();
}

void Application::generateButtonPressed( bool ) {
	// @todo Set all the parameters  of the world generator
	// @todo Generate the world
}

void Application::setupMainWindow() {
	// Settings dock
	setupSettingsDock();
	m_main_window.addDockWidget( Qt::LeftDockWidgetArea, m_settings_dock );

	// Viewer tabs
	setupMapViewer();
	m_main_window.setCentralWidget( m_viewer_tabs );

	// Make the window visible
	m_main_window.show();
	m_main_window.setWindowState( Qt::WindowMaximized );
}

void Application::setupMapViewer() {
	// Create the tab container
	m_viewer_tabs = new QTabWidget;
}

void Application::setupSettingsDock() {
	// Create the dock
	m_settings_dock = new QDockWidget( "Settings" );
	m_settings_dock->setFeatures( QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable );

	// Create the main settings box and layout
	m_settings_box = new QWidget;
	m_settings_main_layout = new QVBoxLayout;

	// Create the settings tabs
	setupSettingsTabs();
	m_settings_main_layout->addWidget( m_settings_tabs );

	// Create the "Generate" button
	m_settings_generate_button = new QPushButton( "Generate World" );
	m_settings_main_layout->addWidget( m_settings_generate_button );

	// Attach the "Generate" button to the proper slot
	QObject::connect( m_settings_generate_button, &QPushButton::clicked, this, &Application::generateButtonPressed );

	// Finish the main settings box and layout
	m_settings_main_layout->setSizeConstraint( QLayout::SetMinimumSize );
	m_settings_box->setLayout( m_settings_main_layout );

	// Finish by attaching the main box to the dock
	m_settings_dock->setWidget( m_settings_box );
}

void Application::setupSettingsTabs() {
	// Create the tab container
	m_settings_tabs = new QTabWidget;

	// Create the general tab
	m_settings_general_tab = new QWidget;
	m_settings_general_layout = new QGridLayout;
	m_settings_general_width = new QSpinBox;
	m_settings_general_height = new QSpinBox;
	m_settings_general_seed = new QSpinBox;
	m_settings_general_persistence = new QDoubleSpinBox;
	m_settings_general_octaves = new QSpinBox;

	// Setup the general input fields
	m_settings_general_width->setRange( 1, static_cast< int >( sf::Texture::getMaximumSize() ) );
	m_settings_general_height->setRange( 1, static_cast< int >( sf::Texture::getMaximumSize() ) );
	m_settings_general_seed->setMinimum( 0 );
	m_settings_general_persistence->setRange( 0., 1. );
	m_settings_general_persistence->setSingleStep( 0.05 );
	m_settings_general_octaves->setRange( 1, 128 );

	// Attach the general inputs
	m_settings_general_layout->addWidget( new QLabel( "Width:" ), 1, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_width, 1, 2, 1, 1 );
	m_settings_general_layout->addWidget( new QLabel( "Height:" ), 2, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_height, 2, 2, 1, 1 );
	m_settings_general_layout->addWidget( new QLabel( "Seed:" ), 3, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_seed, 3, 2, 1, 1 );
	m_settings_general_layout->addWidget( new QLabel( "Persistence:" ), 4, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_persistence, 4, 2, 1, 1 );
	m_settings_general_layout->addWidget( new QLabel( "Octaves:" ), 5, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_octaves, 5, 2, 1, 1 );

	// Attach the general tab to the tab container
	m_settings_general_layout->setSizeConstraint( QLayout::SetMinimumSize );
	m_settings_general_tab->setLayout( m_settings_general_layout );
	m_settings_tabs->addTab( m_settings_general_tab, "General" );

	// Create the weather tab
	m_settings_weather_tab = new QWidget;

	// Attach the weather tab to the tab container
	m_settings_tabs->addTab( m_settings_weather_tab, "Weather" );
}
