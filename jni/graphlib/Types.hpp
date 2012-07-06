/*
 * $Id: Types.hpp,v 1.1.2.8 2010/03/08 03:00:40 kpharris Exp $
 *
 * Part of GraphLib -- A framework for graphics production.
 * Copyright (C) 2010 Kevin Harris
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if       !defined(GRAPHLIB_TYPES_HPP)
#define            GRAPHLIB_TYPES_HPP

/*
 * @author Kevin Harris
 */

#include "graphlib/GenericCoord2.hpp"
#include "graphlib/GenericCoord3.hpp"
#include "graphlib/GenericVector2.hpp"
#include "graphlib/GenericVector3.hpp"
#include "graphlib/GenericPoint2.hpp"
#include "graphlib/GenericPoint3.hpp"
#include "graphlib/GenericSize2.hpp"
#include "graphlib/GenericRGBColor.hpp"
#include "graphlib/GenericRGBAColor.hpp"
#include "graphlib/GenericONB.hpp"
#include "graphlib/GenericFrame.hpp"
#include "graphlib/GenericInterval.hpp"
#include "graphlib/GenericLine.hpp"

namespace graphlib
{
	typedef double NumberType;
	typedef GenericCoord2<NumberType> Coord2;
	typedef GenericCoord3<NumberType> Coord3;
	typedef GenericVector2<NumberType> Vector2;
	typedef GenericVector3<NumberType> Vector3;
	typedef GenericPoint2<NumberType> Point2;
	typedef GenericPoint3<NumberType> Point3;
	typedef GenericSize2<NumberType> Size2;
	typedef GenericONB<NumberType> ONB;
	typedef GenericFrame<NumberType> Frame;
	typedef GenericInterval<NumberType> Interval;
	typedef GenericLine<NumberType, Point2, Vector2> Line2;
	typedef GenericLine<NumberType, Point3, Vector3> Line3;
	typedef GenericRGBColor<unsigned char> RGBColor;
	typedef GenericRGBAColor<unsigned char> RGBAColor;
}

#endif /* !defined(GRAPHLIB_TYPES_HPP) */
