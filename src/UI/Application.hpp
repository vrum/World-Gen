// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QApplication>
#include <QDockWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>

class Application : public QApplication {
	private:
		QMainWindow m_main_window;

		QGroupBox*   m_settings_box;
		QDockWidget* m_settings_dock;
		QVBoxLayout* m_settings_layout;

		QTabWidget* m_viewer_tabs;

		void createMainWindow();
		void createSettingsBox();
		void createSettingsDock();
		void createSettingsLayout();
		void createViewerTabs();

	public:
		Application( int& argc, char* argv[] );

		~Application();
};

#endif // APPLICATION_HPP
