// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information
// Adapted from http://becomingindiedev.blogspot.com/2013/10/qt-5-and-sfml-20-integration.html
// Thanks to the author of that blog!

#ifndef QSFMLCANVAS_HPP
#define QSFMLCANVAS_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <QWidget>

class QSFMLCanvas : public QWidget, public sf::RenderWindow {
	private:
		bool m_initialized;

	public:
		QSFMLCanvas( QWidget* parent, const QPoint& position, const QSize& size );

		virtual void showEvent(QShowEvent*);
		virtual QPaintEngine* paintEngine() const;
		virtual void paintEvent(QPaintEvent*);

		virtual void onInit();
		virtual void onUpdate();

		virtual ~QSFMLCanvas();
};

#endif // QSFMLCANVAS_HPP
