// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef ARRAY_HPP
#define ARRAY_HPP

template< class T > class Array2D {
	private:
		unsigned int m_width;
		unsigned int m_height;

		T** m_data;

	public:
		Array2D( unsigned int width, unsigned int height ) {
			m_data = new T*[ width ];
			for( unsigned int x = 0; x < width; ++x ) {
				m_data[ x ] = new T[ height ];
			}

			m_width = width;
			m_height = height;
		}

		Array2D( const Array2D< T >& rhs ) {
			m_width = rhs.m_width;
			m_height = rhs.m_height;
			unsigned int width = m_width;
			unsigned int height = m_height;

			m_data = new T*[ width ];
			for( unsigned int x = 0; x < width; ++x ) {
				m_data[ x ] = new T[ height ];
			}

			for( unsigned int x = 0; x < width; ++x ) {
				for( unsigned int y = 0; y < height; ++y ) {
					m_data[ x ][ y ] = rhs.m_data[ x ][ y ];
				}
			}
		}

		Array2D( Array2D< T >&& rhs ) {
			m_width = rhs.m_width;
			m_height = rhs.m_height;
			unsigned int width = m_width;
			unsigned int height = m_height;

			m_data = new T*[ width ];
			for( unsigned int x = 0; x < width; ++x ) {
				m_data[ x ] = new T[ height ];
			}

			for( unsigned int x = 0; x < width; ++x ) {
				for( unsigned int y = 0; y < height; ++y ) {
					m_data[ x ][ y ] = rhs.m_data[ x ][ y ];
				}
			}
		}

		T* operator[]( unsigned int i ) {
			return m_data[ i ];
		}

		~Array2D() {
			for( unsigned int x = 0; x < m_width; ++x ) {
				delete[] m_data[ x ];
			}
			delete[] m_data;
		}
};

#endif // ARRAY_HPP
