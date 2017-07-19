#include "Tools/Exception/exception.hpp"

#include "Module/Interleaver/Random_column/Interleaver_random_column.hpp"
#include "Module/Interleaver/Row_column/Interleaver_row_column.hpp"
#include "Module/Interleaver/LTE/Interleaver_LTE.hpp"
#include "Module/Interleaver/CCSDS/Interleaver_CCSDS.hpp"
#include "Module/Interleaver/NO/Interleaver_NO.hpp"
#include "Module/Interleaver/Golden/Interleaver_golden.hpp"
#include "Module/Interleaver/Random/Interleaver_random.hpp"
#include "Module/Interleaver/User/Interleaver_user.hpp"

#include "Factory_interleaver.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename T>
Interleaver<T>* Factory_interleaver
::build(const parameters &params)
{
	     if (params.type == "LTE"     ) return new Interleaver_LTE          <T>(params.size,                                             params.n_frames);
	else if (params.type == "CCSDS"   ) return new Interleaver_CCSDS        <T>(params.size,                                             params.n_frames);
	else if (params.type == "RANDOM"  ) return new Interleaver_random       <T>(params.size,                params.seed, params.uniform, params.n_frames);
	else if (params.type == "RAND_COL") return new Interleaver_random_column<T>(params.size, params.n_cols, params.seed, params.uniform, params.n_frames);
	else if (params.type == "ROW_COL" ) return new Interleaver_row_column   <T>(params.size, params.n_cols,                              params.n_frames);
	else if (params.type == "GOLDEN"  ) return new Interleaver_golden       <T>(params.size,                params.seed, params.uniform, params.n_frames);
	else if (params.type == "USER"    ) return new Interleaver_user         <T>(params.size, params.path,                                params.n_frames);
	else if (params.type == "NO"      ) return new Interleaver_NO           <T>(params.size,                                             params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_interleaver
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	req_args[{"itl-size"}] =
		{"positive_int",
		 "number of symbols to interleave."};

	opt_args[{"itl-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{"itl-type"}] =
		{"string",
		 "specify the type of the interleaver.",
		 "LTE, CCSDS, RANDOM, GOLDEN, USER, RAND_COL, ROW_COL, NO"};

	opt_args[{"itl-path"}] =
		{"string",
		 "specify the path to the interleaver file (to use with \"--itl-type USER\")."};

	opt_args[{"itl-cols"}] =
		{"positive_int",
		 "specify the number of columns used for the RAND_COL or ROW_COL interleaver."};

	opt_args[{"itl-uni"}] =
		{"",
		 "enable the regeneration of the interleaver at each new frame."};

	opt_args[{"itl-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};
}

void Factory_interleaver
::store_args(const Arguments_reader& ar, parameters &params)
{
	if(ar.exist_arg({"itl-size"     })) params.size     = ar.get_arg_int({"itl-size"     });
	if(ar.exist_arg({"itl-fra",  "F"})) params.n_frames = ar.get_arg_int({"itl-fra",  "F"});
	if(ar.exist_arg({"itl-type"     })) params.type     = ar.get_arg    ({"itl-type"     });
	if(ar.exist_arg({"itl-path"     })) params.path     = ar.get_arg    ({"itl-path"     });
	if(ar.exist_arg({"itl-cols"     })) params.n_cols   = ar.get_arg_int({"itl-cols"     });
	if(ar.exist_arg({"itl-seed", "S"})) params.seed     = ar.get_arg_int({"itl-seed", "S"});
	if(ar.exist_arg({"itl-uni"      })) params.uniform  = true;
}

void Factory_interleaver
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"itl","Interleaver parameter(s)"});
}

void Factory_interleaver
::header(params_list& head_itl, const parameters& params)
{
	head_itl.push_back(std::make_pair("Type", params.type));
	head_itl.push_back(std::make_pair("Size", std::to_string(params.size)));
	head_itl.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
	if (params.type == "USER")
		head_itl.push_back(std::make_pair("Path", params.path));
	if (params.type == "RAND_COL" || params.type == "ROW_COL")
		head_itl.push_back(std::make_pair("Number of columns", std::to_string(params.n_cols)));
	if (params.type == "RANDOM" || params.type == "GOLDEN" || params.type == "RAND_COL")
	{
		head_itl.push_back(std::make_pair("Seed", std::to_string(params.seed)));
		head_itl.push_back(std::make_pair("Uniform", (params.uniform ? "yes" : "no")));
	}
}

// ==================================================================================== explicit template instantiation 
template aff3ct::module::Interleaver<int16_t>* aff3ct::tools::Factory_interleaver::build<int16_t>(const aff3ct::tools::Factory_interleaver::parameters&);
template aff3ct::module::Interleaver<int32_t>* aff3ct::tools::Factory_interleaver::build<int32_t>(const aff3ct::tools::Factory_interleaver::parameters&);
template aff3ct::module::Interleaver<int64_t>* aff3ct::tools::Factory_interleaver::build<int64_t>(const aff3ct::tools::Factory_interleaver::parameters&);
// ==================================================================================== explicit template instantiation