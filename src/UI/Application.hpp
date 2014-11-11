// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QApplication>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

class Application : public QApplication {
	Q_OBJECT

	private:
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

		QWidget* m_settings_weather_tab;

		QTabWidget* m_viewer_tabs;

		///////////////////////
		/// Setup Functions ///
		///////////////////////
		void setupMainWindow();
		void setupMapViewer();
		void setupSettingsDock();
		void setupSettingsTabs();

	public:
		Application( int& argc, char* argv[] );

	private slots:
		void generateButtonPressed( bool );
};

#endif // APPLICATION_HPP
