// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef RR_BEST_TIMES_HPP_INCLUDE_GUARD_
#define RR_BEST_TIMES_HPP_INCLUDE_GUARD_

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "RaceTracks.hpp"

#include "miniblocxx/IntrusiveCountableBase.hpp"
#include <string>
#include <map>

namespace rr
{
	class BestTimes: public virtual IntrusiveCountableBase
	{
	public:
		BestTimes(const std::string& dataFilePath);

		int getTime(RaceTracks::Races id) const;
		// returns true if the reported time is lower than the previous time
		bool reportTime(RaceTracks::Races id, int time);

	private:
		void load();
		void save();

	private:
		std::string m_dataFilePath;
		typedef std::map<RaceTracks::Races, int> map_t;
		map_t m_times;
		typedef map_t::value_type TimesPair;
	};
} // namespace rr

#endif
