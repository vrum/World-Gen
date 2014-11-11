// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QApplication>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include "../World.hpp"

//////////////////////////////////////////////////////////////////////
/// @todo Finish fixing viewer (QGraphicsScene and QGraphicsViews) ///
//////////////////////////////////////////////////////////////////////

class Application : public QApplication {
	Q_OBJECT

	private:
		World m_world_generator;

		///////////////
		/// Widgets ///
		///////////////
		QMainWindow m_main_window;

		QDockWidget* m_settings_dock;
		QWidget*     m_settings_box;
		QVBoxLayout* m_settings_main_layout;
		QTabWidget*  m_settings_tabs;
		QPushButton* m_settings_generate_button;

		QWidget*        m_settings_general_tab;
		QGridLayout*    m_settings_general_layout;
		QSpinBox*       m_settings_general_width;
		QSpinBox*       m_settings_general_height;
		QSpinBox*       m_settings_general_seed;
		QDoubleSpinBox* m_settings_general_persistence;
		QSpinBox*       m_settings_general_octaves;

		QWidget*        m_settings_weather_tab;
		QGridLayout*    m_settings_weather_layout;
		QDoubleSpinBox* m_settings_weather_land_heat;
		QDoubleSpinBox* m_settings_weather_sea_heat;

		QTabWidget*    m_viewer_tabs;
		QGraphicsScene m_viewer_height;
		QGraphicsItem* m_viewer_height_item;
		QGraphicsView* m_viewer_height_display;
		QGraphicsScene m_viewer_heat;
		QGraphicsItem* m_viewer_heat_item;
		QGraphicsView* m_viewer_heat_display;

		///////////////////////
		/// Setup Functions ///
		///////////////////////
		void setupMainWindow();
		void setupMapViewer();
		void setupSettingsDock();
		void setupSettingsGeneral();
		void setupSettingsTabs();
		void setupSettingsWeather();

	public:
		Application( int& argc, char* argv[] );

	private slots:
		void generateButtonPressed( bool );
};

#endif // APPLICATION_HPP
