// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef MAPWRITER_HPP
#define MAPWRITER_HPP

#include <QImage>
#include "World.hpp"

class MapWriter {
	private:
		World m_world;

	public:
		void setWorld( World& world );

		QImage writeHeightMapToImage();
		QImage writeHeatMapToImage();

		static QImage writeHeightMapToImage( World& world );
		static QImage writeHeatMapToImage( World& world );

		void writeHeightMapToFile( std::string filename );
		void writeHeatMapToFile( std::string filename );

		static void writeHeightMapToFile( World& world, std::string filename );
		static void writeHeatMapToFile( World& world, std::string filename );
};

#endif // MAPWRITER_HPP
