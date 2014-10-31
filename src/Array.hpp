// Copyright (c) 2014 Kody Kurtz
// See the LICENSE file in the root directory of the repository for licensing information

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <vector>

template< class T, std::size_t D > class Array {
	private:
		std::size_t m_size[ D ];

		std::vector< Array< T, D-1 > > m_array;

		template< typename... Args > void setSizes( std::size_t index, std::size_t size, Args... args ) {
			setSizes( index, size );

			setSizes( index, args... );
		}

		void setSizes( std::size_t index, std::size_t size ) {
			if( index < D ) {
				m_size[ index ] = size;
			}
		}

	public:
		template< typename... Args > Array( std::size_t size, Args... args ) :
			m_array( size, Array< T, D-1 >( std::forward< Args >( args )... ) ) {
			setSizes( 0, size, args... );
		}

		Array< T, D-1 >& operator[]( std::size_t index ) {
			return m_array[ index ];
		}

		Array< T, D > operator*( const T& scalar ) const {
			auto new_array = *this;
			for( unsigned int i = 0; i < new_array.m_array.size(); ++i ) {
				new_array.m_array[ i ] = new_array.m_array[ i ] * scalar;
			}

			return new_array;
		}

		std::size_t size( std::size_t index ) {
			if( index < D ) {
				return m_size[ index ];
			}

			return 0;
		}
};

template< class T > class Array< T, 1 > {
	private:
		std::size_t m_size;

		std::vector< T > m_array;

		void setSizes( std::size_t index, std::size_t size ) {
			if( index < 1 ) {
				m_size = size;
			}
		}

	public:
		Array( std::size_t size ) :
			m_size( size ),
			m_array( size ){
		}

		T& operator[]( std::size_t index ) {
			return m_array[ index ];
		}

		Array< T, 1 > operator*( const T& scalar ) const {
			auto new_array = *this;
			for( unsigned int i = 0; i < new_array.m_array.size(); ++i ) {
				new_array.m_array[ i ] = new_array.m_array[ i ] * scalar;
			}

			return new_array;
		}

		std::size_t size( std::size_t index ) {
			if( index < 1 ) {
				return m_size;
			}

			return 0;
		}
};

template< class T > using Array2D = Array< T, 2 >;

#endif // ARRAY_HPP
