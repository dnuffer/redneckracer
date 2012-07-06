// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RRConfig.hpp"
#include "Globals.hpp"

namespace rr
{

namespace
{
	// This is a pointer that is never deleted, so that the destructors don't run. Yes, we are doing this on
	// purpose! The static destructors are called after main() exits, and probably after java has gone
	// away and got rid of the GLSurfaceView. There is nothing that the game really needs to do in the
	// destructors anyway.
	RedneckRacerGame* theGame = new RedneckRacerGame();
}

RedneckRacerGame& game()
{
	return *theGame;
}

}
