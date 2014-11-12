// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include <cmath>
#include <cstdint>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include "Application.hpp"
#include "../MapWriter.hpp"

static const unsigned int num_digits = 2;

Application::Application( int& argc, char* argv[] ) :
	QApplication( argc, argv ) {
	// Do the setup!
	setupMainWindow();

	// Rip values out of the world generator and feed them to the UI
	m_settings_general_width->setValue( static_cast< int >( m_world_generator.getSize().x ) );
	m_settings_general_height->setValue( static_cast< int >( m_world_generator.getSize().y ) );
	m_settings_general_seed->setValue( static_cast< int >( m_world_generator.getSeed() ) );
	m_settings_general_persistence->setValue( m_world_generator.getPersistence() );
	m_settings_general_octaves->setValue( static_cast< int >( m_world_generator.getOctaves() ) );
	m_settings_weather_land_heat->setValue( m_world_generator.getLandHeat() );
	m_settings_weather_sea_heat->setValue( m_world_generator.getSeaHeat() );

	// Generate the default world
	m_world_generator.generateWorld();
	QImage heightmap = MapWriter::writeHeightMapToImage( m_world_generator );
	QImage heatmap = MapWriter::writeHeatMapToImage( m_world_generator );
	m_viewer_height_item = m_viewer_height.addPixmap( QPixmap::fromImage( heightmap ) );
	m_viewer_heat_item = m_viewer_heat.addPixmap( QPixmap::fromImage( heatmap ) );
}

void Application::generateButtonPressed( bool ) {
	double epsilon = 0.5 * pow( 10., -static_cast< double >( num_digits ) );

	// Go through each parameter, and if it differs from what's in the world, update it
	if( m_settings_general_width->value() != static_cast< int >( m_world_generator.getSize().x ) ||
		m_settings_general_height->value() != static_cast< int >( m_world_generator.getSize().y ) ) {
		Vector2ui new_size = { static_cast< unsigned int >( m_settings_general_width->value() ),
							   static_cast< unsigned int >( m_settings_general_height->value() ) };
		m_world_generator.setSize( new_size );
	}

	if( std::abs( m_settings_general_sea_level->value() - m_world_generator.getSeaLevel() ) > epsilon ) {
		m_world_generator.setSeaLevel( m_settings_general_sea_level->value() );
	}

	if( static_cast< unsigned int >( m_settings_general_seed->value() ) != m_world_generator.getSeed() ) {
		m_world_generator.setSeed( static_cast< unsigned int >( m_settings_general_seed->value() ) );
	}

	if( std::abs( m_settings_general_persistence->value() - m_world_generator.getPersistence() ) > epsilon ) {
		m_world_generator.setPersistence( m_settings_general_persistence->value() );
	}

	if( m_settings_general_octaves->value() != static_cast< int >( m_world_generator.getOctaves() ) ) {
		m_world_generator.setOctaves( static_cast< unsigned int >( m_settings_general_octaves->value() ) );
	}

	if( std::abs( m_settings_weather_land_heat->value() - m_world_generator.getLandHeat() ) > epsilon ) {
		m_world_generator.setLandHeat( m_settings_weather_land_heat->value() );
	}

	if( std::abs( m_settings_weather_sea_heat->value() - m_world_generator.getSeaHeat() ) > epsilon ) {
		m_world_generator.setSeaHeat( m_settings_weather_sea_heat->value() );
	}

	m_world_generator.generateAuxillaryMaps();
	m_viewer_height.removeItem( m_viewer_height_item );
	m_viewer_heat.removeItem( m_viewer_heat_item );
	QImage heightmap = MapWriter::writeHeightMapToImage( m_world_generator );
	QImage heatmap = MapWriter::writeHeatMapToImage( m_world_generator );
	m_viewer_height_item = m_viewer_height.addPixmap( QPixmap::fromImage( heightmap ) );
	m_viewer_heat_item = m_viewer_heat.addPixmap( QPixmap::fromImage( heatmap ) );
}

void Application::randomizeButtonPressed( bool pass_along ) {
	m_world_generator.setSeed();
	m_settings_general_seed->setValue( static_cast< double >( m_world_generator.getSeed() ) );
	generateButtonPressed( pass_along );
}

void Application::saveHeatMap() {
	QString filename = QFileDialog::getSaveFileName( &m_main_window, "Save Heatmap to...", "./Heatmap.png" );
	MapWriter::writeHeatMapToFile( m_world_generator, filename.toStdString() );
}

void Application::saveHeightMap() {
	QString filename = QFileDialog::getSaveFileName( &m_main_window, "Save Heightmap to...", "./Heightmap.png" );
	MapWriter::writeHeightMapToFile( m_world_generator, filename.toStdString() );
}

void Application::setupMainWindow() {
	// Settings dock
	setupSettingsDock();
	m_main_window.addDockWidget( Qt::LeftDockWidgetArea, m_settings_dock );

	// Viewer tabs
	setupMapViewer();
	m_main_window.setCentralWidget( m_viewer_tabs );

	// File menu
	QMenu* file_menu = new QMenu( "File" );
	QAction* save_height_action = file_menu->addAction( "Save Heightmap" );
	QAction* save_heat_action = file_menu->addAction( "Save Heatmap" );
	connect( save_height_action, SIGNAL( triggered() ), this, SLOT( saveHeightMap() ) );
	connect( save_heat_action, SIGNAL( triggered() ), this, SLOT( saveHeatMap() ) );
	m_main_window.menuBar()->addMenu( file_menu );

	// Make the window visible
	m_main_window.show();
	m_main_window.setWindowState( Qt::WindowMaximized );
}

void Application::setupMapViewer() {
	// Create the tab container
	m_viewer_tabs = new QTabWidget;
	m_viewer_height_display = new QGraphicsView( &m_viewer_height );
	m_viewer_heat_display = new QGraphicsView( &m_viewer_heat );

	// Attach everything properly
	m_viewer_tabs->addTab( m_viewer_height_display, "Height" );
	m_viewer_tabs->addTab( m_viewer_heat_display, "Heat" );
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

	// Free space!
	QWidget* spacer = new QWidget;
	spacer->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	m_settings_main_layout->addWidget( spacer );

	// Create the buttons
	QWidget* button_container = new QWidget;
	QHBoxLayout* button_layout = new QHBoxLayout;
	m_settings_randomize_button = new QPushButton( "Randomize Seed" );
	m_settings_generate_button = new QPushButton( "Generate World" );
	button_layout->addWidget( m_settings_randomize_button );
	button_layout->addWidget( m_settings_generate_button );
	button_layout->setSizeConstraint( QLayout::SetMinimumSize );
	button_container->setLayout( button_layout );
	m_settings_main_layout->addWidget( button_container );

	// Attach the buttons to the proper slots
	QObject::connect( m_settings_randomize_button, &QPushButton::clicked, this, &Application::randomizeButtonPressed );
	QObject::connect( m_settings_generate_button, &QPushButton::clicked, this, &Application::generateButtonPressed );

	// Finish the main settings box and layout
	m_settings_main_layout->setSizeConstraint( QLayout::SetMinimumSize );
	m_settings_box->setLayout( m_settings_main_layout );

	// Finish by attaching the main box to the dock
	m_settings_dock->setWidget( m_settings_box );
}

void Application::setupSettingsGeneral() {
	// Create the general tab
	m_settings_general_tab = new QWidget;
	m_settings_general_layout = new QGridLayout;
	m_settings_general_width = new QSpinBox;
	m_settings_general_height = new QSpinBox;
	m_settings_general_sea_level = new QDoubleSpinBox;
	m_settings_general_seed = new QDoubleSpinBox;
	m_settings_general_persistence = new QDoubleSpinBox;
	m_settings_general_octaves = new QSpinBox;

	// Setup the general input fields
	m_settings_general_width->setRange( 0, 8192 );
	m_settings_general_height->setRange( 0, 8192 );
	m_settings_general_sea_level->setRange( -1., 1. );
	m_settings_general_seed->setRange( 0., static_cast< double >( UINT_MAX ) );
	m_settings_general_seed->setDecimals( 0 );
	m_settings_general_persistence->setRange( 0., 1. );
	m_settings_general_persistence->setSingleStep( 0.05 );
	m_settings_general_persistence->setDecimals( num_digits );
	m_settings_general_octaves->setRange( 1, 128 );

	// Attach the general inputs
	m_settings_general_layout->addWidget( new QLabel( "Width:" ), 1, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_width, 1, 2, 1, 1 );
	m_settings_general_layout->addWidget( new QLabel( "Height:" ), 2, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_height, 2, 2, 1, 1 );
	m_settings_general_layout->addWidget( new QLabel( "Sea Level:" ), 3, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_sea_level, 3, 2, 1, 1 );
	m_settings_general_layout->addWidget( new QLabel( "Seed:" ), 4, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_seed, 4, 2, 1, 1 );
	m_settings_general_layout->addWidget( new QLabel( "Persistence:" ), 5, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_persistence, 5, 2, 1, 1 );
	m_settings_general_layout->addWidget( new QLabel( "Octaves:" ), 6, 1, 1, 1 );
	m_settings_general_layout->addWidget( m_settings_general_octaves, 6, 2, 1, 1 );

	// Adjust the layout
	m_settings_general_layout->setSizeConstraint( QLayout::SetMinimumSize );
	m_settings_general_tab->setLayout( m_settings_general_layout );
}

void Application::setupSettingsTabs() {
	// Create the tab container
	m_settings_tabs = new QTabWidget;

	// Setup the general tab
	setupSettingsGeneral();
	m_settings_tabs->addTab( m_settings_general_tab, "General" );

	// Setup the settings tab
	setupSettingsWeather();
	m_settings_tabs->addTab( m_settings_weather_tab, "Weather" );
}

void Application::setupSettingsWeather() {
	// Create the weather tab
	m_settings_weather_tab = new QWidget;
	m_settings_weather_layout = new QGridLayout;
	m_settings_weather_land_heat = new QDoubleSpinBox;
	m_settings_weather_sea_heat = new QDoubleSpinBox;

	// Setup the weather input fields
	m_settings_weather_land_heat->setRange( -1., 1. );
	m_settings_weather_land_heat->setSingleStep( 0.05 );
	m_settings_weather_land_heat->setDecimals( num_digits );
	m_settings_weather_sea_heat->setRange( -1., 1. );
	m_settings_weather_sea_heat->setSingleStep( 0.05 );
	m_settings_weather_sea_heat->setDecimals( num_digits );

	// Attach the weather inputs
	m_settings_weather_layout->addWidget( new QLabel( "Land Heat:" ), 1, 1, 1, 1 );
	m_settings_weather_layout->addWidget( m_settings_weather_land_heat, 1, 2, 1, 1 );
	m_settings_weather_layout->addWidget( new QLabel( "Sea Heat:" ), 2, 1, 1, 1 );
	m_settings_weather_layout->addWidget( m_settings_weather_sea_heat, 2, 2, 1, 1 );

	// Adjust the layout
	m_settings_weather_layout->setSizeConstraint( QLayout::SetMinimumSize );
	m_settings_weather_tab->setLayout( m_settings_weather_layout );
}
