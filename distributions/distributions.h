//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May 31 08:54:58 GMT-0800 1997
// Last Modified: Sat May 31 08:54:58 GMT-0800 1997
// Filename:      ...sig/maint/code/base/Distribution/Distribution.h
// Web Address:   http://sig.sapp.org/include/sigBase/Distribution.h
// Documentation: http://sig.sapp.org/doc/classes/Distribution
// Syntax:        C++ 
//

#ifndef DISTRIBUTION_H_INCLUDED
#define DISTRIBUTION_H_INCLUDED

#define UNIFORM      (0)
#define LINEAR       (1)
#define TRIANGULAR   (2)
#define EXPONENTIAL  (3)
#define BILATERAL    (4)
#define GAUSSIAN     (5)
#define CAUCHY       (6)
#define BETA         (7)
#define WEIBULL      (8)
#define POISSON      (9)


class Distribution {
   public:
                    Distribution  (void);
                    Distribution  (int aSeed);
                   ~Distribution  ();

      void          doBeta        (void);
      void          doBeta        (float Avalue, float Bvalue);
      void          doBilateral   (void);
      void          doBilateral   (float aLambda);
      void          doCauchy      (void);
      void          doCauchy      (float aSpread, float aMean);
      void          doExponential (void);
      void          doExponential (float aLambda);
      void          doGaussian    (void);
      void          doGaussian    (float aDeviation, float aMean);
      void          doLinear      (void);
      void          doPoisson     (void);
      void          doTriangular  (void);
      void          doTriangular  (float alowerLimit, float aMax,
                                   float anUpperLimit);
      void          doUniform     (void);
      void          doUniform   (float aLowerLimit, float anUpperLimit);
      void          doWeibull     (void);
      void          doWeibull     (float t, float s);

      int           getType       (void);
      void          seed          (int seed);
      int           setType       (int newDistributionType);
      float     value         (void);
      float     value         (float min, float max, 
                                     int numAttempts);

   protected:
      int           seedValue;
      int           distributionType;
      int           int1, int2;         // for temporary storage space
      float     beta_inv_a;         // parameters for Beta distribution
      float     beta_inv_b;         // parameters for Beta distribution
      float     exponential_lambda; // parameter for Exponential dist.
      float     bilateral_lambda;   // parameter for Bilateral dist.
      float     gaussian_deviation; // parameter for Gaussian dist.
      float     gaussian_mean;      // parameter for Gaussian dist.
      float     cauchy_spread;      // parameter for Cauchy dist.
      float     cauchy_mean;        // parameter for Cauchy dist.
      float     weibull_inv_t;      // parameter for Weibull dist.
      float     weibull_s;          // parameter for Weibull dist.
      float     triangular_diffu;   // parameter for Triangular dist.
      float     triangular_diffl;   // parameter for Triangular dist.
      float     triangular_max;     // parameter for Triangular dist.
      float     uniform_low;        // parameter for Uniform dist.
      float     uniform_range;      // parameter for Uniform dist.

      // private functions:
      float     BetaDistribution        (void);
      float     BilateralDistribution   (void);
      float     CauchyDistribution      (void);
      float     ExponentialDistribution (void);
      float     GaussianDistribution    (void);
      float     LinearDistribution      (void);
      float     TriangularDistribution  (void);
      float     UniformDistribution     (void);
      float     WeibullDistribution     (void);

      float     myrand                  (void);
};



#endif  /* DISTRIBUTION_H_INCLUDED */



// md5sum: 183e582fc8138fb86e245d8ec44b9223 Distribution.h [20010708]