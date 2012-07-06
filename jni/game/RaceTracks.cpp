// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RaceTracks.hpp"
#include "boost/range/algorithm_ext/push_back.hpp"
#include "boost/range/irange.hpp"
#include "boost/range/algorithm/random_shuffle.hpp"
#include <boost/assign/std/vector.hpp>
#include <boost/assert.hpp>

namespace rr {

	// To make defining tracks much easier...
	enum TrackPieceTypes {
		straight,
		leftright,
		rightleft,
		sharpleftback,
		sharprightback,
		squiggle,
		peakleft,
		peakright,
		driftleft,
		driftright
	};
	
	RaceTracks::RaceTracks()
	{
		using namespace boost::assign;
		
		// SHORT TRACK (6 pieces)
		RaceTrack short_track;
		short_track.racename = "Yankee Shot";
		short_track.racetrack += straight, squiggle, driftleft, peakright, leftright, straight;
		short_track.min_animals = 2;
		short_track.max_animals = 5;
		short_track.min_obstacles = 2;
		short_track.max_obstacles = 5;
		races[YANKEESHOT] = short_track;
		
		// MEDIUM TRACK (10 pieces)
		RaceTrack medium_track;
		medium_track.racename = "Everwhichaways";
		medium_track.racetrack += driftleft, driftright, peakleft, peakright, squiggle,
									sharpleftback, squiggle, sharprightback, leftright, rightleft;
		medium_track.min_animals = 5;
		medium_track.max_animals = 8;
		medium_track.min_obstacles = 5;
		medium_track.max_obstacles = 8;
		races[EVERWHICHAWAYS] = medium_track;
		
		// CROWDED TRACK (10 pieces)
		RaceTrack crowded_track;
		crowded_track.racename = "Uhmurkin";
		crowded_track.racetrack += straight, squiggle, peakleft, leftright, peakright,
									driftleft, squiggle, rightleft, peakright, straight;
		crowded_track.min_animals = 10;
		crowded_track.max_animals = 12;
		crowded_track.min_obstacles = 5;
		crowded_track.max_obstacles = 7;
		races[UHMURKIN] = crowded_track;
		
		// LONG TRACK (18 pieces)
		RaceTrack long_track;
		long_track.racename = "Dadgum Trakturs";
		long_track.racetrack += straight, rightleft, sharprightback, peakleft, driftleft, leftright,
								sharpleftback, squiggle, peakright, driftright, squiggle, squiggle,
								peakleft, peakright, driftleft, squiggle, peakright, straight;
		long_track.min_animals = 8;
		long_track.max_animals = 12;
		long_track.min_obstacles = 15;
		long_track.max_obstacles = 18;
		races[DADGUMTRAKTURS] = long_track;
		
		// HEYDAY TRACK (15 pieces)
		RaceTrack heyday_track;
		heyday_track.racename = "Real Bammer";
		heyday_track.racetrack += driftleft, driftright, peakleft, peakright, squiggle,
									leftright, rightleft, sharpleftback, sharprightback, squiggle,
									straight, squiggle, peakleft, peakright, straight;
		heyday_track.min_animals = 15;
		heyday_track.max_animals = 18;
		heyday_track.min_obstacles = 15;
		heyday_track.max_obstacles = 18;
		races[REALBAMMER] = heyday_track;
		
		// RANDOM TRACK (15 pieces)
		RaceTrack random_track;
		random_track.racename = "Wijadidja";
		random_track.racetrack += 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1;
		random_track.min_animals = 3;
		random_track.max_animals = 18;
		random_track.min_obstacles = 3;
		random_track.max_obstacles = 18;
		races[WIJADIDJA] = random_track;
	}
	
	RaceTracks::~RaceTracks()
	{
	}

	string RaceTracks::getName(Races r)
	{
		assert(races.count(r));
		return races.find(r)->second.racename;
	}
	
	vector<char> RaceTracks::getRaceTrack(Races r)
	{
		assert(races.count(r));
		return races.find(r)->second.racetrack;
	}
	
	unsigned int RaceTracks::getTrackSize(Races r)
	{
		assert(races.count(r));
		return races.find(r)->second.racetrack.size();
	}
	
	int RaceTracks::getMinAnimals(Races r)
	{
		assert(races.count(r));
		return races.find(r)->second.min_animals;
	}
	
	int RaceTracks::getMaxAnimals(Races r)
	{
		assert(races.count(r));
		return races.find(r)->second.max_animals;
	}
	
	int RaceTracks::getMinObstacles(Races r)
	{
		assert(races.count(r));
		return races.find(r)->second.min_obstacles;
	}
	
	int RaceTracks::getMaxObstacles(Races r)
	{
		assert(races.count(r));
		return races.find(r)->second.max_obstacles;
	}
	
	void RaceTracks::generateRandomTrack(int num_options)
	{
		assert(races.count(WIJADIDJA));
		races.find(WIJADIDJA)->second.racetrack.clear();
		for (int i = 0; i < 3; ++i) {
			push_back(races.find(WIJADIDJA)->second.racetrack, boost::irange(0u, unsigned(num_options)));
		}
		random_shuffle(races.find(WIJADIDJA)->second.racetrack.begin(), races.find(WIJADIDJA)->second.racetrack.end());
		races.find(WIJADIDJA)->second.racetrack.resize(15);
	}
}
