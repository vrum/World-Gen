// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <vector>

template< class T, std::size_t D > class Array {
	private:
		std::size_t m_size;

		std::vector< Array< T, D-1 > > m_array;

	public:
		template< typename... Args > Array( std::size_t size, Args... args ) :
			m_size( size ),
			m_array( size, Array< T, D-1 >( std::forward< Args >( args )... ) ) {
		}

		Array< T, D-1 >& operator[]( std::size_t index ) {
			return m_array[ index ];
		}
};

template< class T > class Array< T, 1 > {
	private:
		std::size_t m_size;

		std::vector< T > m_array;

	public:
		Array( std::size_t size ) :
			m_size( size ),
			m_array( size ){
		}

		T& operator[]( std::size_t index ) {
			return m_array[ index ];
		}
};

template< class T > using Array2D = Array< T, 2 >;

#endif // ARRAY_HPP
