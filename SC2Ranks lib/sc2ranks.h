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

#include <boost/format.hpp>
#include <boost/any.hpp>
#include <boost/unordered_map.hpp>
#include <boost/network/protocol/http/client.hpp>
#include "tinyjson.h"

typedef json::grammar<char> js;

// Sends a API request to sc2ranks.com
namespace sc2ranks_network
{
	// Cleans up code
	using namespace boost::network;

	// Sends a request to sc2ranks.com url and parses the json response
	js::object parse_data(std::string const& url)
	{
		// make server request
		http::client client;
		http::client::request request(url);
		request << header("Connection", "close");

		// get server response
		http::client::response response = client.get(request);
		std::string data = body(response);

		// parse response data
		js::variant var = json::parse(data.begin(), data.end());
		if (var->type() != typeid(js::object))
			throw std::exception("parse_data: bad json format");

		// return decoded json in form of js::object
		return boost::any_cast<js::object>(*var);
	}
}

// Contains the API functions
namespace sc2ranks
{
	class map
	{
	public:
		// Constructor
		map(js::object const& o)
			: m_object(o)
		{ }

		// Returns the value of the specified key
		// Example:		[Key=Name] [Value=Vevix]
		// Usage:		map.get<type>("name")
		template<typename T>
		T get(std::string const& key)
		{
			// Locate key position
			js::object::const_iterator it = m_object.find(key);
			// Was the key found?
			if (it == m_object.end())
				throw std::exception(("Unable to find key <" + key + ">").c_str());

			// Attempt to cast the value into the specified type and return
			try
			{
				return boost::any_cast<T>(*it->second);
			}
			catch (boost::bad_any_cast const&)
			{
				throw std::exception(("Unable to cast key <" + key + "> to specified type").c_str());
			}
		}

	private:
		js::object m_object;
	};

	// Application keys are required for all API requests, if you do not pass "?appKey=example.com" you will get a no_key error. These are used for statistics on requests.
	std::string appKey = "";

	// Returns the minimum amount of character data, just gives achievement points, character code and battle.net id info.
	// Usage:	get_base_char("us", "Vevix", 2127429);
	//			get_base_char("us", "Vevix", 0, 568);
	map get_base_char(std::string const& region, std::string const& name, int bnetid)
	{
		std::string url = boost::str(boost::format("http://sc2ranks.com/api/base/char/%1%/%2%!%3%.json?appKey=%4%") 
							% region % name % bnetid % appKey);

		// Send request and retrieve data
		js::object obj =  sc2ranks_network::parse_data(url);

		// Check if character exists
		js::object::const_iterator it = obj.find("error");
		if (it != obj.end())
			throw std::exception("get_base_char: Character does not exist");

		return obj;
	}

	// Returns the base character data, as well as base data on all of the players teams.
	// Usage:	get_base_teams("us", "Vevix", 2127429);
	//			get_base_teams("us", "Vevix", 0, 568);
	map get_base_char_team(std::string const& region, std::string const& name, int bnetid)
	{
		std::string url = boost::str(boost::format("http://sc2ranks.com/api/base/teams/%1%/%2%!%3%.json?appKey=%4%") 
			% region % name % bnetid % appKey);

		// Send request and retrieve data
		js::object obj = sc2ranks_network::parse_data(url);

		// Check if character exists
		js::object::const_iterator it = obj.find("error");
		if (it != obj.end())
			throw std::exception("get_base_char_team: Character does not exist");

		return obj;
	}

	// Returns the base character data, and extended team information for the passed bracket.
	map get_char_team(std::string const& region, std::string const& name, int bnetid, int bracket, bool random = false)
	{
		std::string url = boost::str(boost::format("http://sc2ranks.com/api/char/teams/%1%/%2%!%3%/%4%/%5%.json?appKey=%6%") 
			% region % name % bnetid % bracket % random % appKey);

		// Send request and retrieve data
		js::object obj = sc2ranks_network::parse_data(url);

		// Check if character exists
		js::object::const_iterator it = obj.find("error");
		if (it != obj.end())
			throw std::exception("get_char_team: Character does not exist");

		return obj;
	}

	// Returns the link to the specified character on sc2ranks.com
	// Usage:	get_char_url("us", "Vevix", 2127429);
	//			get_char_url("us", "Vevix", 0, 568);
	std::string const get_char_url(std::string const& region, std::string const& name, int bnetid = 0, int code = 0)
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