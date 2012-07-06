// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef GLOBALS_HPP_INCLUDED
#define GLOBALS_HPP_INCLUDED

#include "RRConfig.hpp"
#include "RedneckRacerGame.hpp"

namespace rr
{
	RedneckRacerGame& game();

	const int DefaultScaleWidth = 480;
	const int DefaultScaleHeight = 800;
	const int DefaultScreenLeft = -DefaultScaleWidth/2;
	const int DefaultScreenRight = DefaultScaleWidth/2;
	const int DefaultScreenBottom = -DefaultScaleHeight/2;
	const int DefaultScreenTop = DefaultScaleHeight/2;
	const int DefaultScreenBorderPad = 10;

	const int DefaultScreenRightPad = DefaultScreenRight - DefaultScreenBorderPad;
	const int DefaultScreenLeftPad = DefaultScreenLeft + DefaultScreenBorderPad;
	const int DefaultScreenTopPad = DefaultScreenTop - DefaultScreenBorderPad;
	const int DefaultScreenBottomPad = DefaultScreenBottom + DefaultScreenBorderPad;

	const int Center = -24;
	const int ColumnOne = DefaultScreenLeftPad / 2;
	const int ColumnOneLeftJustified = DefaultScreenLeft + 60;
	const int ColumnTwo = DefaultScreenRightPad / 2;
	const int ColumnTwoLeftJustified = 60;
	const int RowOne = DefaultScreenTop / 2;
	const int RowTwo = DefaultScreenTop / 3;
	const int RowThree = DefaultScreenTop / 6;
	const int RowFour = 0;
	const int RowFive = DefaultScreenBottom /6;
	const int RowSix = DefaultScreenBottom /3;
}

#endif
