// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RoadBound.hpp"
#include "engine/Resources.hpp"
#include "engine/Resource.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include "engine/GL.hpp"
#include <algorithm>
#include "boost/random.hpp"

namespace rr
{

	boost::rand48 randEng(::time(NULL));

	RoadBound::RoadBound(float xCenter, float yCenter):
		  lastSectionsHeight(-yCenter)
		, scaleX(1.0f)
		, scaleY(1.0f)
		, centerX(xCenter)
	{

	}

	void RoadBound::loadRoadSectionBorder(const String& name)
	{
		String roadBoundFileName = name;
		ResourcePtr roadBoundResource = Resources::loadResourceFromAssets(roadBoundFileName.c_str());
		String boundStr = String(reinterpret_cast<const char*>(&*roadBoundResource->begin()), distance(roadBoundResource->begin(), roadBoundResource->end()));

		Coord left, right;
		StringArray lines = boundStr.tokenize("\r\n");
		float sectionHeight = 0;
		if(lines.size() > 0)
		{
			StringArray scaleStr = lines.begin()->tokenize(" ");
			scaleX = scaleStr.at(0).toFloat();
			scaleY = scaleStr.at(1).toFloat();
		}

		for (StringArray::const_iterator it = lines.begin() + 1; it != lines.end(); ++it)
		{
			const String& line(*it);
			StringArray toks = line.tokenize(" ");
			sectionHeight = toks.at(0).toFloat() * scaleY; // Y coordinate.
			left = Coord(toks.at(1).toFloat() * scaleX - centerX, lastSectionsHeight + sectionHeight);
			right = Coord(toks.at(2).toFloat() * scaleX - centerX, lastSectionsHeight + sectionHeight);
			roadBorder.push_back(DoubleCoord(left, right));
		}
		lastSectionsHeight += sectionHeight;
		LOGI("Road border size: %zu", roadBorder.size());
		updateBoundingRect();
	}

	Coord RoadBound::getBoundCoordinates(float yCoord) const
	{
		// ? need to optimize ?
		Coord coord(0, yCoord);
		vector<DoubleCoord>::const_iterator borderLine = upper_bound(roadBorder.begin(), roadBorder.end(), DoubleCoord(coord, coord)); // Complexity: At most, logarithmic number of comparisons and linear number of steps in the length of [first,last).
		if (borderLine != roadBorder.end())
		{
			return Coord(borderLine->left.x(), borderLine->right.x());
		}
		else
		{
			//LOGI("Invalid truck position!");
			return Coord(0, 0);
		}
	}

	Point RoadBound::getPointOnRoad(float y) const
	{
		Coord roadCoord = getBoundCoordinates(y);

		boost::uniform_int<unsigned> horizontalRD(
			std::max<int>(0, roadCoord.x() + 10),
			std::max<int>(0, roadCoord.y() - 10));
		return Point(horizontalRD(randEng), y);
	}

	bool RoadBound::isPointOnRoad(float x, float y) const
	{
		Coord bound = getBoundCoordinates(y);
		if (x > bound.x() && x < bound.y())
		{
			return true;
		}
		else return false;
	}

#ifdef TEST_DRAWING_ROAD_BOUND
	unsigned int currentvectpos = 0;
	void RoadBound::draw(const Rectangle& screen)
	{
		using namespace gl;
		vector<GLfloat> vertexes;
		vertexes.reserve(100);
		vector<GLfloat> colors;
		colors.reserve(400);
		bool start = false;

		for(unsigned int i = currentvectpos; i < roadBorder.size(); i++)
		{
			if ((roadBorder[i].left.y() > screen.bottom - 100) && (!start))
			{
				start = true;
				currentvectpos = i;
			}
			if (roadBorder[i].left.y() > screen.top + 100)
			{
				start = false;
				break;
			}
			if(start)
			{
				vertexes.push_back(roadBorder[i].left.x());
				vertexes.push_back(roadBorder[i].left.y());
				vertexes.push_back(roadBorder[i].right.x());
				vertexes.push_back(roadBorder[i].right.y());

				for(int i = 0; i < 8; ++i)
				{
					colors.push_back(0.7);
				}
			}
		}

		GLfloat* vert = new GLfloat[vertexes.size()];
		copy(vertexes.begin(), vertexes.end(), vert);

		GLfloat* col = new GLfloat[colors.size()];
		copy(colors.begin(), colors.end(), col);

		{
			MatrixScope ms;
			disableTexture2D();
			gl::color(4, GL_FLOAT, 0, col);
			vertex(2, GL_FLOAT, 0, vert);
			drawArrays(GL_TRIANGLE_STRIP, 0, vertexes.size()/2);
		}

		delete[] vert;
		delete[] col;
	}
#endif

	void RoadBound::updateBoundingRect()
	{
		LOGASSERT(roadBorder.size() > 0, "Road border size is 0!");
		Rectangle r( -centerX, centerX, roadBorder[roadBorder.size()-1].left.y(), 0);
		LOGD("RoadBound::updateBoundingRect() left: %f, top: %f, right: %f, bottom: %f", r.left, r.top, r.right, r.bottom);
		setBoundingRect(r);
	}

	Coord RoadBound::getLeftBoundByIndex(unsigned int index)
	{
		if (roadBorder.size() > index)
		{
			return Coord(roadBorder[index].left.x(), roadBorder[index].left.y());
		}
		else
		{
			//LOGI("Left index invalid.");
			return Coord(0, 0);
		}
	}

	Coord RoadBound::getRightBoundByIndex(unsigned int index)
	{
		if (roadBorder.size() > index)
		{
			return Coord(roadBorder[index].right.x(), roadBorder[index].right.y());
		}
		else
		{
			//LOGI("Right index invalid.");
			return Coord(0, 0);
		}
	}

} // namespace rr
