//  file: bessel.cpp 
//
//  Spherical Bessel functions via up and down recursion      
//                                                                     
//
//  Programmer:  Dick Furnstahl  furnstahl.1@osu.edu
//
//  Revision history:
//      02-Jan-2011  new version, for 780.20 Computational Physics
//
//  Notes:  
//   * compile with:  "g++ -o bessel bessel.cpp"
//   * adapted from: "Projects in Computational Physics" by Landau and Paez  
//             copyrighted by John Wiley and Sons, New York               
//             code copyrighted by RH Landau  
//   * data saved as: x y1 y2  --- should print column headings!!                        
// 
//  Interpreting the Plot:
//
//  up until the log(x) ~ 1/2 the relative error stays fairly high. It then quickly
//  jumps down to where log(rel_error) ~ -35. The slope of this line is about -7.5.
//  Then between log(x) = 2 and log(x) = 3.5 it stays at that low error until it jumps
//  up again, this time with a slope of something like 60 (basically straight up).
//  The high error is maintained until about 4.5 where our data ends. This Tells me
//  that there can be specific regions where our methods for computing the bessel
//  functions works very well at a specific x-range. 
//
//
//************************************************************************

// include files
#include <iostream>		// note that .h is omitted
#include <iomanip>		// note that .h is omitted
#include <fstream>		// note that .h is omitted
#include <cmath>
#include <gsl/gsl_sf_bessel.h>
using namespace std;		// we need this when .h is omitted

// function prototypes 
double down_recursion (double x, int n, int m);	// downward algorithm 
double up_recursion (double x, int n);	        // upward algorithm 

// global constants  
const double xmax = 100.0;	// max of x  
const double xmin = 0.1;	// min of x >0  
const double step = 0.1;	// delta x  
const int order = 10;		// order of Bessel function 
const int start = 50;		// used for downward algorithm 

//********************************************************************
int
main ()
{
  double ans_down, ans_up, gsl_bes, rel_dif, rel_dif_gsl;

  // open an output file stream
  ofstream my_out ("bessel.dat");

  my_out << "# Spherical Bessel functions via up and down recursion" 
         << endl;
  my_out << "log(x)   ans_down   ans_up   gsl_bes   log(rel_dif)   rel_dif_gsl" << endl;
  // step through different x values
  for (double x = xmin; x <= xmax; x += step)
    {
      ans_down = down_recursion (x, order, start);
      ans_up = up_recursion (x, order);
      rel_dif = (abs(ans_up - ans_down)/(abs(ans_up)+abs(ans_down)));
      gsl_bes = gsl_sf_bessel_jl(order, x);
      rel_dif_gsl = (abs(gsl_bes - ans_down)/(abs(gsl_bes)+abs(ans_down)));

      my_out << fixed << setprecision (12) << setw (8) << log(x) << " "
	<< scientific << setprecision (12)
	<< setw (13) << ans_down << " "
	<< setw (13) << ans_up << " " 
  << setw (13) << gsl_bes << " "
  << setw (13) << log(rel_dif) << " "
  << setw (13) << rel_dif_gsl << " "
        << endl;
    }
  cout << "data stored in bessel.dat." << endl;

  // close the output file
  my_out.close ();
  return (0);
}


//------------------------end of main program----------------------- 

// function using downward recursion  
double
down_recursion (double x, int n, int m)
{
  double j[start + 2];		// array to store Bessel functions 
  j[m + 1] = j[m] = 1.;		// start with "something" (choose 1 here) 
  for (int k = m; k > 0; k--)
    {
      j[k - 1] = ((2.* double(k) + 1.) / x) * j[k] - j[k + 1];  // recur. rel.
    }
  double scale = (sin (x) / x) / j[0];	// scale the result 
  return (j[n] * scale);
}


//------------------------------------------------------------------ 

// function using upward recursion  
double
up_recursion (double x, int n)
{
  double term_three = 0.;
  double term_one = (sin (x)) / x;	// start with lowest order 
  double term_two = (sin (x) - x * cos (x)) / (x * x);	// next order
  for (int k = 1; k < n; k += 1)	// loop for order of function     
    { // recurrence relation
      term_three = ((2.*double(k) + 1.) / x) * term_two - term_one;	       
      term_one = term_two;
      term_two = term_three;
    }
  return (term_three);
}
