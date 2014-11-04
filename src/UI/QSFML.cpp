// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information
// Adapted from http://becomingindiedev.blogspot.com/2013/10/qt-5-and-sfml-20-integration.html
// Thanks to the author of that blog!

#include "QSFML.hpp"
#ifdef Q_WS_X11
#include <Qt/qx11info_x11.h>
#include <X11/Xlib.h>
#endif

QSFML::QSFML( QWidget* parent, const QPoint& position, const QSize& size ) :
	QWidget( parent ),
	m_initialized( false ) {
	// Setup Qt attributes for rendering
	setAttribute( Qt::WA_PaintOnScreen );
	setAttribute( Qt::WA_OpaquePaintEvent );
	setAttribute( Qt::WA_NoSystemBackground );

	// Set strong focus to enable keyboard events to be received
	setFocusPolicy( Qt::StrongFocus );

	// Setup the widget geometry
	move( position );
	resize( size );
}

void QSFML::showEvent( QShowEvent* ) {
	if ( !m_initialized ) {
		// Flush events in X11
		#ifdef Q_WS_X11
		XFlush( QX11Info::display() );
		#endif

		// Create the SFML window with the widget handle
		// Handles vary in type from system to system
		// Some are pointers, others are integers
		// Hence, old-style casts are needed
		sf::RenderWindow::create( ( sf::WindowHandle )( winId() ) );

		// Let the derived class do its thing
		onInit();

		// Finish initialization
		m_initialized = true;
	}
}

QPaintEngine* QSFML::paintEngine() const {
	return nullptr;
}

void QSFML::paintEvent(QPaintEvent*) {
	// Let the derived class do its thing
	onUpdate();

	// Display
	sf::RenderWindow::display();
}

void QSFML::onInit() {
}

void QSFML::onUpdate() {
}

QSFML::~QSFML() {
}
