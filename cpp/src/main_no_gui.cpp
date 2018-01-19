#include "State.hpp"

#include "CommandLine.hpp"

#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>

using namespace poly_fem;
using namespace Eigen;


/**
* no ui:
* <exec> -mesh <path> -problem <0,1,2,3> -cmd
*
* args:
*   -mesh <path to the mesh>
*   -n_refs <refinements>
*   -problem <0: linear, 1: quadratic, 2: franke, 3: linear elasticity>
*   -quad <quadrature order>
*   -b_samples <number of boundary samples>
*   -spline <use spline basis>
*   -fem <use standard fem with quad/hex meshes>
*   -lambda <first lame parameter>
*   -mu <second lame parameter>
*   -cmd <runs without ui>
*   -ui <runs with ui>
**/
int main(int argc, const char **argv)
{
#ifndef WIN32
    setenv("GEO_NO_SIGNAL_HANDLER", "1", 1);
#endif

    GEO::initialize();

    // Import standard command line arguments, and custom ones
    GEO::CmdLine::import_arg_group("standard");
    GEO::CmdLine::import_arg_group("pre");


    CommandLine command_line;

    std::string path = "";
    std::string output = "";
    int n_refs = 0;
    int problem_num = 0;

    int quadrature_order = 4;
    int discr_order = 1;
    int n_boundary_samples = 10;

    double lambda = 1, mu = 1;

    double refinenemt_location = 0.5;

    bool use_splines = false;


    command_line.add_option("-mesh", path);
    command_line.add_option("-n_refs", n_refs);
    command_line.add_option("-ref_t", refinenemt_location);
    command_line.add_option("-problem", problem_num);


    command_line.add_option("-quad", quadrature_order);
    command_line.add_option("-q", discr_order);
    command_line.add_option("-b_samples", n_boundary_samples);
    command_line.add_option("-spline", "-fem", use_splines);


    command_line.add_option("-lambda", lambda);
    command_line.add_option("-mu", mu);

    command_line.add_option("-output", output);

    command_line.parse(argc, argv);


    State &state = State::state();

    state.quadrature_order = quadrature_order;
    state.use_splines = use_splines;
    state.lambda = lambda;
    state.mu = mu;
    state.discr_order = discr_order;
    state.n_boundary_samples = n_boundary_samples;
    state.refinenemt_location = refinenemt_location;

    state.init(path, n_refs, problem_num);
        // std::cout<<path<<std::endl;
    state.load_mesh();
    state.compute_mesh_stats();
    state.build_basis();
    state.compute_assembly_vals();
    state.assemble_stiffness_mat();
    state.assemble_rhs();
    state.solve_problem();
    state.compute_errors();

    if(!output.empty()){
        std::ofstream out(output);
        state.save_json(out);
    }


    return EXIT_SUCCESS;
}
