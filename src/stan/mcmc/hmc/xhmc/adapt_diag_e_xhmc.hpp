#ifndef STAN_MCMC_HMC_XHMC_ADAPT_DIAG_E_XHMC_HPP
#define STAN_MCMC_HMC_XHMC_ADAPT_DIAG_E_XHMC_HPP

#include <stan/interface_callbacks/writer/base_writer.hpp>
#include <stan/mcmc/stepsize_var_adapter.hpp>
#include <stan/mcmc/hmc/xhmc/diag_e_xhmc.hpp>

namespace stan {
  namespace mcmc {
    /**
     * Exhausive Hamiltonian Monte Carlo (XHMC) with multinomial sampling
     * with a Gaussian-Euclidean disintegration and adaptive
     * diagonal metric and adaptive step size
     */
    template <class Model, class BaseRNG>
    class adapt_diag_e_xhmc: public diag_e_xhmc<Model, BaseRNG>,
                             public stepsize_var_adapter {
    public:
        adapt_diag_e_xhmc(const Model& model, BaseRNG& rng)
          : diag_e_xhmc<Model, BaseRNG>(model, rng),
          stepsize_var_adapter(model.num_params_r()) {}

      ~adapt_diag_e_xhmc() {}

      sample
      transition(sample& init_sample,
                 interface_callbacks::writer::base_writer& info_writer,
                 interface_callbacks::writer::base_writer& error_writer) {
        sample s = diag_e_xhmc<Model, BaseRNG>::transition(init_sample,
                                                           info_writer,
                                                           error_writer);

        if (this->adapt_flag_) {
          this->stepsize_adaptation_.learn_stepsize(this->nom_epsilon_,
                                                    s.accept_stat());

          bool update = this->var_adaptation_.learn_variance(this->z_.mInv,
                                                             this->z_.q);

          if (update) {
            this->init_stepsize(info_writer);

            this->stepsize_adaptation_.set_mu(log(10 * this->nom_epsilon_));
            this->stepsize_adaptation_.restart();
          }
        }
        return s;
      }

      void disengage_adaptation() {
        base_adapter::disengage_adaptation();
        this->stepsize_adaptation_.complete_adaptation(this->nom_epsilon_);
      }
    };

  }  // mcmc
}  // stan
#endif
