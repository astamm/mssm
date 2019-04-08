#ifndef PROBLEM_DATA_H
#define PROBLEM_DATA_H
#include "arma.h"
#include "dists.h"
#include "thread_pool.h"

/* util class to hold information and objects used for the computations */
class control_obj {
  std::unique_ptr<thread_pool> pool;
public:
  /* input needed for proposal distribution */
  const double nu, covar_fac, ftol_rel;
  /* number of particles */
  const arma::uword N_part;
  const comp_out what_stat;

  control_obj
    (const arma::uword, const double, const double, const double,
     const arma::uword, const std::string&);
  control_obj& operator=(const control_obj&) = delete;
  control_obj(const control_obj&) = delete;
  control_obj(control_obj&&) = default;

  thread_pool& get_pool() const;
};

class problem_data {
  using cvec = const arma::vec;
  using cmat = const arma::mat;

  /* objects related to observed outcomes */
  cvec &Y, &cfix, &ws;
  cmat &X, &Z;
  const std::vector<arma::uvec> &time_indices;

  /* objects related to state-space model */
  cmat F, Q, Q0;

  /* objects related to computations */
  const std::unique_ptr<thread_pool> pool;
public:
  cvec mu0;
  const arma::uword n_periods;
  const control_obj ctrl;

  problem_data(
    cvec&, cvec&, cvec&, cmat&, cmat&, const std::vector<arma::uvec>&,
    cmat&, cmat&, cmat&, cvec&, control_obj&&);
  problem_data(const problem_data&) = delete;
  problem_data& operator=(const problem_data&) = delete;
  problem_data(problem_data&&) = default;

  /* returns an object to compute the conditional distribution of the
   * observed outcome at a given time given a state vector */
  std::unique_ptr<cdist> get_obs_dist(const arma::uword) const;
  /* returns an object to compute the conditional distribution of the state
   * at a given time given a state vector at the previous time point */
  std::unique_ptr<cdist> get_sta_dist(const arma::uword) const;
};

#endif
