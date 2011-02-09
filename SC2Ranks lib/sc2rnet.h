/*
 * SC2Ranks C++ API Library
 *
 * Copyright (c) 2011 Thomas McNiven (trmcniven@gmail.com)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#ifndef SC2RNET_H
#define SC2RNET_H

#include <iostream>
#include <boost/network/protocol/http/client.hpp>
#include "tinyjson.h"

namespace sc2rnet
{
	using namespace boost::network;

	typedef boost::unordered_map<std::string, std::string> sc2rmap;

	// returns the data from the url
	// example: {"achievement_points":1775,"region":"us","portrait":{"row":4,"icon_id":2,"column":3},
	//			"updated_at":"2011-02-04T02:42:08Z","character_code":568,"name":"Vevix","id":1789644,"bnet_id":2127429}
	std::string get_data(std::string const url)
	{
		http::client client;
		http::client::request request(url);
		request << header("Connection", "close");
		http::client::response response = client.get(request);
		return body(response);
	}

	// Parse the json data
	sc2rmap parse_data(std::string const data)
	{
		// storage
		sc2rmap storage;

		// parse the json data
		json::grammar<char>::variant var = json::parse(data.begin() + data.find_first_of('{'), data.begin() + data.find_last_of('}') + 1);
		if (var->type() != typeid(json::grammar<char>::object))
			throw std::exception("parse_data: bad json format");

		json::grammar<char>::object const& obj = boost::any_cast<json::grammar<char>::object>(*var);
		for (auto i = obj.begin(); i != obj.end(); ++i)
		{
			// fill storage with int values
			if ((*i).second->type() == typeid(int))
				storage.insert(std::pair<std::string, std::string>((*i).first, 
					boost::lexical_cast<std::string>(boost::any_cast<int>(*(*i).second))));

			// fill storage with string values
			if ((*i).second->type() == typeid(std::string))
				storage.insert(std::pair<std::string, std::string>((*i).first,
					boost::any_cast<std::string>(*(*i).second)));

			// object type, this is for portrait
			if ((*i).second->type() == typeid(json::grammar<char>::object))
			{
				std::string tmpdata = "{";
				json::grammar<char>::object const& tmpobj = boost::any_cast<json::grammar<char>::object>(*(*i).second);
				for (json::grammar<char>::object::const_reverse_iterator rit = tmpobj.rbegin(); rit != tmpobj.rend();)
				{
					tmpdata += rit->first + ":" + boost::lexical_cast<std::string>(boost::any_cast<int>(*rit->second));
					if (++rit != tmpobj.rend())
						tmpdata += ",";
				}
				tmpdata += "}";
				
				storage.insert(std::pair<std::string, std::string>((*i).first, tmpdata));
			}

			// array type, this is for teams etc.
			if ((*i).second->type() == typeid(json::grammar<char>::array))
				std::cout << "Ding ding\n";
		}
		
		return storage;
	}
}

#endif