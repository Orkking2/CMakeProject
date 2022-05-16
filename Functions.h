#pragma once

#include "ArrayManager.h"
#include "Optional.h"
#include <string>

#ifndef _NSTD_FUNCTIONS_
#define _NSTD_FUNCTIONS_

_NSTD_BEGIN

typedef unsigned int PARAM;

struct _spParam{
	_spParam(PARAM param, char* str = "NOT_SET") : param(param), str(str) {}
	char* str;
	PARAM param;
};

class _Param {
	static _NSTD vector<_NSTD optional<_spParam>> params_;
public:
	_Param(PARAM lParam, _NSTD optional<PARAM> rParam = _NSTD optional<PARAM>()) : lParam_(lParam), rParam_(rParam) {}

	static bool set_params(char* lParam_str) {
		if(rParam_.is_set())
			_NSTD_FOR(params_.get_len())
				if ((lParam_ == params_[i].get().param) || (rParam.get() == params_[i].get().param))
					return false;
		params_.push_back(_NSTD optional<_spParam> (_spParam(lParam_, lParam_str)));
	}

	PARAM lParam_;
	_NSTD optional<PARAM> rParam_;
};


_NSTD_END
#endif // ifndef _NSTD_FUNCTIONS_