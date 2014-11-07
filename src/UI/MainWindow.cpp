// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#include "MainWindow.hpp"

MainWindow::MainWindow() {
	// Set up the map viewer tabs
	createViewerTabs();
	setCentralWidget( map_viewer_tabs );

	// Set up the settings dock
	createSettingsDock();
	addDockWidget( Qt::LeftDockWidgetArea, settings_dock );
}

void MainWindow::createSettingsBox() {
	// Create the box
	m_settings_box = new QGroupBox;
	m_settings_box->setFlat( true );

	// Create the layout
	createSettingsLayout();
	m_settings_box->setLayout( m_settings_layout );
}

void MainWindow::createSettingsDock() {
	settings_dock = new QDockWidget( "Settings" );
	createSettingsBox();
	settings_dock->setWidget( m_settings_box );
}

void MainWindow::createSettingsLayout() {
	m_settings_layout = new QVBoxLayout;

	// Setup the settings layout

	// Setup the size constraints
	m_settings_layout->setSizeConstraint( QLayout::SetMinimumSize );
}

void MainWindow::createViewerTabs() {
	map_viewer_tabs = new QTabWidget;
}
