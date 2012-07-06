// Copyright 2011 Nuffer Brothers Software LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "DrawableLine.hpp"

namespace engine
{
	DrawableLine::DrawableLine(std::vector<Point> lineVertexes):
		width(4)
	{
		for (int i = 0; i<8; ++i)
		{
			vertexes[i] = 0.0;
		}
		setVertexes(lineVertexes);
		setColor(1.0, 1.0, 1.0, 1.0);
		
		vertexNum = lineVertexes.size();
	}
	
	// Draw a simple colored line
	void DrawableLine::draw(const Rectangle& screen)
	{
		using namespace gl;

		Point position = this->getPositionRelativeToOrigin(screen);

		{
			MatrixScope ms;
			disableTexture2D();
			//glEnable(GL_LINE_SMOOTH);
			// Draw border.
			gl::color(4, GL_FLOAT, 0, color);
			vertex(2, GL_FLOAT, 0, vertexes);
			translate(position.x(), position.y(), 0);
			setGLLineWidth(width);
			drawArrays(GL_LINE_STRIP, 0, vertexNum);
		}
	}

	// Color component - from 0.0f to 1.0f.
	void DrawableLine::setColor(float r, float g, float b, float a)
	{
		fillColorArray(color, 32, r, g, b, a);
	}
	
	void DrawableLine::setWidth(float lineWidth)
	{
		width = lineWidth;
	}

	void DrawableLine::fillColorArray(GLfloat* destColorArray, int destSize, float r, float g, float b, float a)
	{
		if ((destSize % 4) != 0) //If destColorArray size is incompatible for 4-elements RGBA color (per vertex).
		{
			for (int i = 0; i < destSize; i++) destColorArray[i] = 1.0;
		}
		else
		{
			for (int i = 0; i < destSize; i += 4)
			{
				destColorArray[i]   = r;
				destColorArray[i+1] = g;
				destColorArray[i+2] = b;
				destColorArray[i+3] = a;
			}
		}
	}
	
	void DrawableLine::setVertexes(vector<Point> & lineVertexes)
	{
		for(unsigned int i = 0, j = 0; i < lineVertexes.size(); i++, j+=2)
		{
			if(i >= 4) //more than 4 line segments are not supported.
				break;
				
			vertexes[j] = lineVertexes[i].x();
			vertexes[j+1] = lineVertexes[i].y();
			
			
			LOGD("Point: %f, %f", lineVertexes[i].x(), lineVertexes[i].y());
			LOGD("Vertex: %f % f", vertexes[j], vertexes[j+1]);
		}
	}

} // namespace engine
