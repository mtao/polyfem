#pragma once

#include <polyfem/State.hpp>
#include <polyfem/CompositeFunctional.hpp>
#include <polyfem/MatrixUtils.hpp>

#include <cppoptlib/problem.h>
#include <filesystem>

namespace polyfem
{
	class OptimizationProblem : public cppoptlib::Problem<double>
	{
	public:
		using typename cppoptlib::Problem<double>::Scalar;
		using typename cppoptlib::Problem<double>::TVector;

		OptimizationProblem(State &state_, const std::shared_ptr<CompositeFunctional> j_, const json &args);

		virtual ~OptimizationProblem() = default;

		void solve_pde(const TVector &x);

		virtual void smoothing(const TVector &x, TVector &new_x){};

		virtual void save_to_file(const TVector &x0)
		{
			logger().debug("Save to file {} ...", state.resolve_output_path(fmt::format("opt_{:d}.vtu", iter)));
			state.save_vtu(state.resolve_output_path(fmt::format("opt_{:d}.vtu", iter)), 0.);
		}

		virtual void solution_changed(const TVector &newX) = 0;

		bool stop(const TVector &x) { return false; }

		virtual void post_step(const int iter_num, const TVector &x0) { iter++; }

		virtual void line_search_begin(const TVector &x0, const TVector &x1);

	protected:
		State &state;
		std::string optimization_name = "";

		int iter = 0;
		int save_iter = -1;

		int dim;
		int actual_dim;

		int save_freq = 1;

		json opt_params;

		std::shared_ptr<CompositeFunctional> j;

		TVector descent_direction;

		// better initial guess for forward solves
		Eigen::MatrixXd sol_at_ls_begin;
		TVector x_at_ls_begin;

		// store value and grad of current solution
		double cur_val;
		TVector cur_x, cur_grad;
	};
} // namespace polyfem
