// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef _game_RACE_TRACKS_HPP_
#define _game_RACE_TRACKS_HPP_

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include <map>
#include <string>
#include <vector>

namespace rr {

	struct RaceTrack {
		string racename;
		vector<char> racetrack;
		int min_animals;
		int max_animals;
		int min_obstacles;
		int max_obstacles;
	};
	
	class RaceTracks {
	public:
		
		enum Races {
			YANKEESHOT,
			EVERWHICHAWAYS,
			UHMURKIN,
			DADGUMTRAKTURS,
			REALBAMMER,
			WIJADIDJA
		};
		
		RaceTracks();
		virtual ~RaceTracks();

		string getName(Races r);
		vector<char> getRaceTrack(Races r);
		unsigned int getTrackSize(Races r);
		int getMinAnimals(Races r);
		int getMaxAnimals(Races r);
		int getMinObstacles(Races r);
		int getMaxObstacles(Races r);
		void generateRandomTrack(int num_options);

	private:
		map<Races, RaceTrack> races;
	};
}

#endif
