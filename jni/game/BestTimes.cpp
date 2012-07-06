// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "BestTimes.hpp"
#include <fstream>
#include "boost/lexical_cast.hpp"
#include "boost/foreach.hpp"

#define foreach BOOST_FOREACH

#include "engine/Log.hpp"

namespace rr
{

	BestTimes::BestTimes(const std::string& dataFilePath)
		: m_dataFilePath(dataFilePath)
	{
		load();
	}

	int BestTimes::getTime(RaceTracks::Races id) const
	{
		map_t::const_iterator it = m_times.find(id);
		if (it == m_times.end())
		{
			return INT_MAX;
		}
		return it->second;
	}

	bool BestTimes::reportTime(RaceTracks::Races id, int time)
	{
		if (time < getTime(id))
		{
			m_times[id] = time;
			save();
			return true;
		}
		return false;
	}

	void BestTimes::load()
	{
		std::ifstream ifs(m_dataFilePath.c_str());
		while (ifs)
		{
			std::string curLine;
			getline(ifs, curLine);

			if (!ifs)
				break;

			size_t idx = curLine.find(':');
			if (idx == std::string::npos)
				continue;
			
			try 
			{
				LOGD("BT load id: %s", curLine.substr(0, idx).c_str());
				int id = boost::lexical_cast<int>(curLine.substr(0, idx));
				LOGD("BT load time: %s", curLine.substr(idx+1).c_str());
				int time = boost::lexical_cast<int>(curLine.substr(idx + 1));
				m_times[static_cast<RaceTracks::Races>(id)] = time;
			}
			catch (const std::exception&) 
			{
				continue;
			}
		}
	}

	void BestTimes::save()
	{
		std::ofstream ofs(m_dataFilePath.c_str());
		LOGASSERT(ofs, "BestTimes::save() failed to open %s", m_dataFilePath.c_str());
		foreach(TimesPair item, m_times)
		{
			ofs << item.first << ':' << item.second << '\n';
		}
	}

} // namespace rr
