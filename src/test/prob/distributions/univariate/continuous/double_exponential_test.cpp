#include <stan/prob/distributions/univariate/continuous/double_exponential.hpp>
#include <gtest/gtest.h>
#include <boost/random/mersenne_twister.hpp>

TEST(ProbDistributionsDoubleExponential, random) {
  boost::random::mt19937 rng;
  EXPECT_NO_THROW(stan::prob::double_exponential_rng(2.0,1.0,rng));
}
