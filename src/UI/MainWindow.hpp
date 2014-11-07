// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDockWidget>
#include <QGroupBox>
#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
	private:
		QGroupBox*   m_settings_box;
		QVBoxLayout* m_settings_layout;

		void createSettingsBox();
		void createSettingsDock();
		void createSettingsLayout();
		void createViewerTabs();

	public:
		QDockWidget* settings_dock;
		QTabWidget*  map_viewer_tabs;

		MainWindow();
};

#endif // MAINWINDOW_HPP
