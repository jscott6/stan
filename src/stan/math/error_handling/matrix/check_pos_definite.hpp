#ifndef __STAN__MATH__ERROR_HANDLING__MATRIX__CHECK_POS_DEFINITE_HPP__
#define __STAN__MATH__ERROR_HANDLING__MATRIX__CHECK_POS_DEFINITE_HPP__

#include <sstream>
#include <stan/math/matrix/Eigen.hpp>
#include <stan/math/error_handling/default_policy.hpp>
#include <stan/math/error_handling/raise_domain_error.hpp>
#include <stan/math/error_handling/matrix/constraint_tolerance.hpp>

namespace stan {
  namespace math {

    /**
     * Return <code>true</code> if the specified matrix is positive definite
     *
     * NOTE: symmetry is NOT checked by this function
     * 
     * @param function
     * @param y Matrix to test.
     * @param name
     * @param result
     * @return <code>true</code> if the matrix is positive definite.
     * @tparam T Type of scalar.
     */
    // FIXME: update warnings (message has (0,0) item)
    template <typename T_y, typename T_result, class Policy>
    inline bool check_pos_definite(const char* function,
                  const Eigen::Matrix<T_y,Eigen::Dynamic,Eigen::Dynamic>& y,
                  const char* name,
                  T_result* result,
                  const Policy&) {
      typedef 
        typename Eigen::Matrix<T_y,Eigen::Dynamic,Eigen::Dynamic>::size_type 
        size_type;
      if (y.rows() == 1 && y(0,0) <= CONSTRAINT_TOLERANCE) {
        std::ostringstream message;
        message << name << " is not positive definite. " 
                << name << "(0,0) is %1%.";
        T_result tmp = policies::raise_domain_error<T_y>(function,
                                                         message.str().c_str(),
                                                         y(0,0), Policy());
        if (result != 0)
          *result = tmp;
        return false;
      }
      Eigen::LDLT< Eigen::Matrix<T_y,Eigen::Dynamic,Eigen::Dynamic> > cholesky 
        = y.ldlt();
      if((cholesky.vectorD().array() <= CONSTRAINT_TOLERANCE).any())  {
        std::ostringstream message;
        message << name << " is not positive definite. " 
                << name << "(0,0) is %1%.";
        T_result tmp = policies::raise_domain_error<T_y>(function,
                                                         message.str().c_str(),
                                                         y(0,0), Policy());
        if (result != 0)
          *result = tmp;
        return false;
      }
      return true;
    }


    template <typename T_y, typename T_result>
    inline bool check_pos_definite(const char* function,
                  const Eigen::Matrix<T_y,Eigen::Dynamic,Eigen::Dynamic>& y,
                  const char* name,
                  T_result* result) {
      return check_pos_definite(function,y,name,result,default_policy());
    }


    template <typename T>
    inline bool check_pos_definite(const char* function,
                  const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>& y,
                  const char* name,
                  T* result = 0) {
      return check_pos_definite(function,y,name,result,default_policy());
    }

  }
}
#endif