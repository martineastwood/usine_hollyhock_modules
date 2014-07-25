//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May 31 08:54:58 GMT-0800 1997
// Last Modified: Sat May 31 08:54:58 GMT-0800 1997
// Filename:      ...sig/maint/code/base/Distribution/Distribution.cpp
// Web Address:   http://sig.sapp.org/src/sig/Distribution.cpp
// Documentation: http://sig.sapp.org/doc/classes/Distribution
// Syntax:        C++ 
// References:    Dodge and Jerse, "Computer Music", pp. 269-277
//                Winsor and DeLisa, "Computer Music in C", pp. 177-205
//

#include "Distributions.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#ifndef OLDCPP
   #include <iostream>
   using namespace std;
#else
   #include <iostream.h>
#endif


//////////////////////////////
//
// Distribution::Distribution --
//

Distribution::Distribution(void) {
   seedValue = time(NULL);   // time in seconds since 00:00:00 GMT, 1 Jan 1970
   srand(seedValue);
   doBeta(0.5, 0.5);
   doCauchy(1.0, 0.0);
   doExponential(1.0);
   doBilateral(1.0);
   doTriangular(0.0, 0.5, 1.0);
   doUniform(0.0, 1.0);
   distributionType = 0;
}

Distribution::Distribution(int aSeed) {
   seedValue = aSeed;
   srand(seedValue);
   doBeta(0.5, 0.5);
   doCauchy(1.0, 0.0);
   doExponential(1.0);
   doBilateral(1.0);
   doTriangular(0.0, 0.5, 1.0);
   doUniform(0.0, 1.0);
}



//////////////////////////////
//
// Distribution::~Distribution --
//

Distribution::~Distribution() { }



//////////////////////////////
//
// Distribution::doDistributions --
//

void Distribution::doBeta(void)        { setType(BETA       ); }
void Distribution::doBeta(float Avalue, float Bvalue) { 
   if (Avalue <= 0) {
      cerr << "Beta distribution parameter a = " << Avalue 
           << " cannot be negative or zero." << endl;
      exit(1);
   }
   if (Bvalue <= 0) {
      cerr << "Beta distribution parameter b = " << Bvalue 
           << " cannot be negative or zero." << endl;
      exit(1);
   }
   
   beta_inv_a = 1.0/Avalue;
   beta_inv_b = 1.0/Bvalue;
   doBeta();
}

void Distribution::doBilateral(void)   { setType(BILATERAL  ); }
void Distribution::doBilateral(float aLambda) {
   bilateral_lambda = aLambda;
   doBilateral();
}

void Distribution::doCauchy(void)      { setType(CAUCHY     ); }
void Distribution::doCauchy(float aSpread, float aMean) { 
   cauchy_spread = aSpread;
   cauchy_mean = aMean;
   doCauchy();
}

void Distribution::doExponential(void) { setType(EXPONENTIAL); }
void Distribution::doExponential(float aLambda) { 
   exponential_lambda = aLambda;
   doExponential();
}

void Distribution::doGaussian(void)    { setType(GAUSSIAN   ); }
void Distribution::doGaussian(float aDeviation, float aMean) {
   gaussian_deviation = aDeviation;
   gaussian_mean = aMean;
   doGaussian();
}

void Distribution::doLinear(void)      { setType(LINEAR     ); }
void Distribution::doPoisson(void)     { setType(POISSON    ); }

void Distribution::doTriangular(void)  { setType(TRIANGULAR ); }
void Distribution::doTriangular(float aLowerLimit, float aMax,
  float anUpperLimit) { 

   if (aMax < aLowerLimit) {
      cerr << "Cannot have a max less than a lower limit in a" 
           << " triangular distribution: " << endl 
           << "   lower limit = " << aLowerLimit 
           << "  max = " << aMax << endl;
      exit(1);
   }
   if (anUpperLimit < aMax) {
      cerr << "Cannot have a max greater than the upper limit in a" 
           << " triangular distribution: " << endl 
           << "   upper limit = " << anUpperLimit 
           << "  max = " << aMax << endl;
      exit(1);
   }

   triangular_max = aMax;
   triangular_diffl = aMax - aLowerLimit;
   triangular_diffu = anUpperLimit - aMax;
   doTriangular();
}

void Distribution::doUniform(void)     { setType(UNIFORM    ); }
void Distribution::doUniform(float aLowerLimit, float anUpperLimit) {
   if (aLowerLimit > anUpperLimit) {
      cerr << "Cannot invert max and min in range: [" 
           << aLowerLimit << ", " << anUpperLimit << "]." << endl;
      exit(1);
   }

   uniform_low = aLowerLimit;
   uniform_range  = anUpperLimit - aLowerLimit;
   doUniform();
}

void Distribution::doWeibull(void)     { setType(WEIBULL    ); }
void Distribution::doWeibull(float t, float s) { 
   if (t <= 0) {
      cerr << "t value: " << t << " of Weibull distribution must be > 0." 
           << endl;
      exit(1);
   }
   weibull_inv_t = 1.0/t;
   weibull_s = s;
   doWeibull();
}




//////////////////////////////
//
// Distribution::getType --
//

int Distribution::getType(void) {
   return distributionType;
}



//////////////////////////////
//
// Distribution::seed --
//

void Distribution::seed(int aSeed) {
   seedValue = aSeed;
   srand(seedValue);
}



//////////////////////////////
//
// Distribution::setType --
//

int Distribution::setType(int newDistributionType) {
   int oldType = distributionType;
   distributionType = newDistributionType;
   return oldType;
}



//////////////////////////////
//
// Distribution::value --
//

float Distribution::value(void) {
   switch (getType()) {
      case BETA:
         return BetaDistribution();
      case BILATERAL:
         return BilateralDistribution();
      case CAUCHY:
         return CauchyDistribution();
      case EXPONENTIAL:
         return ExponentialDistribution();
      case GAUSSIAN:
         return GaussianDistribution();
      case LINEAR:
         return LinearDistribution();
      case TRIANGULAR:
         return TriangularDistribution();
      case WEIBULL:
         return WeibullDistribution();
      case UNIFORM:
      default: 
         return UniformDistribution();
   }
}

float Distribution::value(float min, float max, int numAttempts) {
   if (min >= max) {
      cerr << "Cannot invert the selection range: [" << min << ", "
           << max << "]" << endl;
      exit(1);
   }
   static float number;
   static int counter;
   
   counter = 0;
   number = value();
   while (number < min || number > max) {
      number = value();
      counter++;
      if (counter > numAttempts) {
         return 0.0;
      }
   }
   
   return number;
}
      



///////////////////////////////////////////////////////////////////////
//
// Private functions:
//


//////////////////////////////
//
// Distribution::BetaDistribution --
//

float Distribution::BetaDistribution(void) {
   static float pow1, pow2, sum, num1, num2;

   while((num1 = myrand()) == 0.0) /* set int1 to be nonzero */ ;
   while((num2 = myrand()) == 0.0) /* set int2 to be nonzero */ ;

   pow1 = pow(int1, beta_inv_a);
   pow2 = pow(int2, beta_inv_b);
   sum  = pow1 + pow2;

   if (sum > 1.0) {
      return BetaDistribution();
   } else {
      return pow1/sum;
   }
}
   


//////////////////////////////
//
// Distribution::BilateralDistribution --
//

float Distribution::BilateralDistribution(void) {
   static double number;
   number = 2.0 * myrand();
   if (number == 0.0 || number == 2.0) {
      return BilateralDistribution();
   }
   if (number <= 1.0) {
      return log(number) / bilateral_lambda;
   } else {
      return -log((2.0-number) / bilateral_lambda);
   }
}
   


//////////////////////////////
//
// Distribution::CauchyDistribution --
//

#ifndef PI
   #define PI (3.1415927)
#endif

float Distribution::CauchyDistribution(void) {
  static double number;
  while ((number = myrand()) == 0.5) {};
  return cauchy_spread * tan(PI*number) + cauchy_mean;
}



//////////////////////////////
//
// Distribution::ExponentialDistribution --
//

float Distribution::ExponentialDistribution(void) {
   static double temp;
   while ((temp = myrand()) == 0.0) {};
   return -log(temp)/exponential_lambda;
}



//////////////////////////////
//
// Distribution::GaussianDistribution --
//

float Distribution::GaussianDistribution(void) {
   static int i;
   static int n = 12;
   static double scale = 1.0 / sqrt(n/12.0);
   static double sum;

   sum = 0.0;
   for (i=0; i<n; i++) {
      sum += rand();
   }
   sum /= RAND_MAX;
   return gaussian_deviation * scale * (sum-n/2.0) + gaussian_mean;
}



//////////////////////////////
//
// Distribution::LinearDistribution --
//

float Distribution::LinearDistribution(void) {
   static int int1, int2;
   int1 = rand();
   int2 = rand();

   if (int2 < int1) {
      int1 = int2;
   }
   return (float)int1/RAND_MAX;
}
   

//////////////////////////////
//
// Distribution::TriangularDistribution --
//

float Distribution::TriangularDistribution(void) {
   static float normed_value;
   normed_value = myrand()+myrand();
   static float output;

   // scale each side of the triangle to the requested range:
   if (normed_value < 1.0) {
      output = (normed_value - 1.0) * triangular_diffl + triangular_max;
   } else {
      output = (normed_value - 1.0) * triangular_diffu + triangular_max;
   }
  
   // normalize the sides of the triangle so that the probability
   // densities matchup at the maximum:
   if (triangular_diffl == triangular_diffu) {
      return output;
   } else if (output < triangular_max && triangular_diffl < triangular_diffu) {
      if (myrand() < (float)triangular_diffl/triangular_diffu) {
         return output;
      } else {
         return TriangularDistribution();
      }
   } else if (output > triangular_max && triangular_diffu < triangular_diffl) {
      if (myrand() < (float)triangular_diffu/triangular_diffl) {
         return output;
      } else {
         return TriangularDistribution();
      }
   }

   return output;
}
   


//////////////////////////////
//
// Distribution::UniformDistribution --
//

float Distribution::UniformDistribution(void) {
   return myrand() * uniform_range + uniform_low;
}



//////////////////////////////
//
// Distribution::WeibullDistribution --
//

float Distribution::WeibullDistribution(void) {
   static double number;   

   number = myrand();
   if (number == 0.0 || number == 1.0) {
      return WeibullDistribution();
   }

   return weibull_s * pow(log(1.0/(1.0-number)), weibull_inv_t);
}



//////////////////////////////
//
// Distribution::myrand -- returns a number between 0 and 1.
//

float Distribution::myrand(void) {
   return (float)rand()/RAND_MAX;
}



// md5sum: 974c485a59cb3f75858a811d8ad206cf Distribution.cpp [20050403]