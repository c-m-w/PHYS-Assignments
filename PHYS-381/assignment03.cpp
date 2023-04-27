/// main.cpp

#include <iostream>
#include <iomanip>
#include <cmath>
#include <functional>
#include <vector>

using ldbl_t = long double;

constexpr ldbl_t PI = 3.14159265358979323846;
constexpr auto NSIGMA = 5;
constexpr ldbl_t SIGMA = 1e-2;
constexpr auto INTEGRATE_PRECISION = 1e4;

ldbl_t f_M_derivative( ldbl_t x,
						 ldbl_t const x_0,
						 ldbl_t const sigma );
ldbl_t __stdcall f( ldbl_t x );
ldbl_t integrate( ldbl_t const xmin,
				  ldbl_t const xmax,
				  std::function< ldbl_t( __stdcall )( ldbl_t ) > fn );
void print_points( std::vector< std::pair< ldbl_t, ldbl_t > > & const p );

void main( )
{
	auto const integrand_func = [ ]( ldbl_t const x_0 )
	{
		return [ x_0 ]( ldbl_t x )
		{
			return f( x ) * f_M_derivative( x,
											x_0,
											SIGMA );
		};
	};

	auto constexpr NPOINTS = 101;
	ldbl_t constexpr XMIN = 0;
	ldbl_t constexpr XMAX = PI / 2;
	ldbl_t constexpr STEP = ( XMAX - XMIN ) / ( NPOINTS - 1 );

	std::vector< std::pair< ldbl_t, ldbl_t > > p { };

	for ( ldbl_t s = XMIN;
		  s <= XMAX;
		  s += STEP ) 
	{
		auto const p_y = integrate( s - NSIGMA * SIGMA,
									s + NSIGMA * SIGMA,
									integrand_func( s ) );

		p.emplace_back( std::make_pair( s,
										p_y ) );
	}

	print_points( p );
}

ldbl_t f_M_derivative( ldbl_t x,
						 ldbl_t const x_0,
						 ldbl_t const sigma )
{
	auto const norm = 1 / std::sqrt( 2 * PI * std::pow( sigma, 2 ) );

	return ( x - x_0 ) / std::pow( sigma, 2 ) 
		* std::exp( -std::pow( x - x_0, 2 ) 
					/ ( 2 * std::pow( sigma, 2 ) ) ) 
		* norm;
}

ldbl_t __stdcall f( ldbl_t x )
{
	return std::sin( x );
}

ldbl_t integrate( ldbl_t const xmin,
				  ldbl_t const xmax,
				  std::function< ldbl_t( __stdcall )( ldbl_t ) > fn )
{
	auto const step = ( xmax - xmin ) / INTEGRATE_PRECISION;
	ldbl_t t = xmin;
	ldbl_t sum = 0;

	for ( auto i = 0;
		  i < INTEGRATE_PRECISION;
		  i++ )
	{
		sum += step * fn( t ); // left-handed reimann sum
		t += step;
	}

	return sum;
}

void print_points( std::vector< std::pair< ldbl_t, ldbl_t > > & const p )
{
	std::cout << "x,\t y" << std::endl;

	for ( auto const [ x, y ]: p )
		std::cout << std::fixed << std::setprecision( 8 ) 
			<< "" << x << ",\t" 
			<< y << "" << std::endl;
}
