/*
 * SC2Ranks C++ API Library
 *
 * Copyright (c) 2011 Thomas McNiven (trmcniven@gmail.com)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#ifndef SC2RANKS_H
#define SC2RANKS_H

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/unordered_map.hpp>

#include "sc2rnet.h"

namespace sc2ranks
{
	// appKey for requests
	std::string appKey = "";

	using sc2rnet::sc2rmap;

	// returns base information about a character
	// "Minimum amount of character data, just gives achievement points, character code and battle.net id info."
	sc2rmap get_char_info(std::string const region, std::string const name, long bnetid = 0, int code = 0)
	{
		std::string url = "";
		if (bnetid)
			url = boost::str(boost::format("http://sc2ranks.com/api/base/char/%1%/%2%!%3%.json?appKey=%4%") 
							% region % name % bnetid % appKey);
		else if (code)
			url = boost::str(boost::format("http://sc2ranks.com/api/base/char/%1%/%2%$%3%.json?appKey=%4%") 
							% region % name % code % appKey);
		else
			throw std::exception("get_char_info: bnetid or code must be supplied");

		std::string data = sc2rnet::get_data(url);
		return sc2rnet::parse_data(data);
	}

	// Returns the url of the character in string format
	std::string const get_char_url(std::string const region, std::string const name, long bnetid = 0, int code = 0)
	{
		if (bnetid)
			return boost::str(boost::format("http://sc2ranks.com/char/%1%/%2%/%3%") % region % bnetid % name);
		else if (code)
			return boost::str(boost::format("http://sc2ranks.com/charcode/%1%/%2%/%3%") % region % code % name);
		else
			throw std::exception("get_char_url: bnetid or code must be supplied");
	}

}

#endif // SC2RANKS_H