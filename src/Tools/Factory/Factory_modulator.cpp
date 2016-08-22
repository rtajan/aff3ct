#include "../Math/max.h"

#include "../../Modulator/BPSK/Modulator_BPSK.hpp"
#include "../../Modulator/BPSK/Modulator_BPSK_fast.hpp"
#include "../../Modulator/PAM/Modulator_PAM.hpp"
#include "../../Modulator/QAM/Modulator_QAM.hpp"
#include "../../Modulator/PSK/Modulator_PSK.hpp"
#include "../../Modulator/GSM/Modulator_GSM.hpp"
#include "../../Modulator/GSM/Modulator_GSM_TBLess.hpp"

#include "Factory_modulator.hpp"

template <typename B, typename R>
Modulator<B,R>* Factory_modulator<B,R>
::build(const t_code_param &code_params, 
        const t_mod_param  &mod_params, 
        const float         sigma)
{
	Modulator<B,R,Q> *modulator = nullptr;

	// build the modulator
	if (mod_params.type == "BPSK")
		modulator = new Modulator_BPSK<B,R>(code_params.N + code_params.tail_length, sigma);
	else if (mod_params.type == "BPSK_FAST")
		modulator = new Modulator_BPSK_fast<B,R>(code_params.N + code_params.tail_length, sigma);
	else if (mod_params.type == "PAM")
	{
		if (mod_params.demod_max == "MAX")
			modulator = new Modulator_PAM<B,R,max<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXL")
			modulator = new Modulator_PAM<B,R,max_linear<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXS")
			modulator = new Modulator_PAM<B,R,max_star<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXSS")
			modulator = new Modulator_PAM<B,R,max_star_safe<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
	}
	else if (mod_params.type == "QAM")
	{
		if (mod_params.demod_max == "MAX")
			modulator = new Modulator_QAM<B,R,max<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXL")
			modulator = new Modulator_QAM<B,R,max_linear<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXS")
			modulator = new Modulator_QAM<B,R,max_star<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXSS")
			modulator = new Modulator_QAM<B,R,max_star_safe<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
	}
	else if (mod_params.type == "PSK")
	{
		if (mod_params.demod_max == "MAX")
			modulator = new Modulator_PSK<B,R,max<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXL")
			modulator = new Modulator_PSK<B,R,max_linear<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXS")
			modulator = new Modulator_PSK<B,R,max_star<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
		else if (mod_params.demod_max == "MAXSS")
			modulator = new Modulator_PSK<B,R,max_star_safe<R>>(code_params.N + code_params.tail_length, mod_params.bits_per_symbol, sigma);
	}

	return modulator;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_modulator<B_8,R_8,R_8>;
template struct Factory_modulator<B_8,R_8,Q_8>;
template struct Factory_modulator<B_16,R_16,R_16>;
template struct Factory_modulator<B_16,R_16,Q_16>;
template struct Factory_modulator<B_32,R_32,R_32>;
template struct Factory_modulator<B_64,R_64,R_64>;
#else
template struct Factory_modulator<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template struct Factory_modulator<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
