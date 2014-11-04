// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef MAPWRITER_HPP
#define MAPWRITER_HPP

#include "World.hpp"

class MapWriter {
	private:
		World m_world;

	public:
		void setWorld( World& world );

		void writeHeightMap( std::string filename );
		void writeHeatMap( std::string filename );
};

#endif // MAPWRITER_HPP
