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

#include "DrawableRectangle.hpp"

namespace engine
{
	DrawableRectangle::DrawableRectangle(float width, float height):
			  barWidth(width)
			, barHeight(height)
			, borderWidth(3.0)
	{
		for (int i = 0; i<8; ++i)
		{
			vertexes[i] = 0.0;
		}
		setVertexes(width, height);
		setColor(1.0, 1.0, 1.0, 1.0);
		setBorderColor(1.0, 1.0, 1.0, 1.0);
	}
	
	// Draw a simple colored rectangle without texture.
	void DrawableRectangle::draw(const Rectangle& screen)
	{
		using namespace gl;

		Point position = this->getPositionRelativeToOrigin(screen);

		{
			MatrixScope ms;
			disableTexture2D();

			// Draw border.
			gl::color(4, GL_FLOAT, 0, borderColor);
			vertex(2, GL_FLOAT, 0, borderVertexes);
			translate(position.x(), position.y(), 0);
			setGLLineWidth(borderWidth);
			drawArrays(GL_LINE_LOOP, 0, 4);
		}
		
		
		{
			MatrixScope ms;
			disableTexture2D();

			// Draw progress.
			gl::color(4, GL_FLOAT, 0, color);
			vertex(2, GL_FLOAT, 0, vertexes);
			translate(position.x(), position.y(), 0);
			drawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}

	// Color component - from 0.0f to 1.0f.
	void DrawableRectangle::setColor(float r, float g, float b, float a)
	{
		fillColorArray(color, 32, r, g, b, a);
	}
	
	void DrawableRectangle::setBorderColor(float r, float g, float b, float a)
	{
		fillColorArray(borderColor, 32, r, g, b, a);
	}
	
	void DrawableRectangle::setBorderWidth(float width)
	{
		borderWidth = width;
	}

	void DrawableRectangle::fillColorArray(GLfloat* destColorArray, int destSize, float r, float g, float b, float a)
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
	
	void DrawableRectangle::setVertexes(GLfloat width, GLfloat height)
	{

		/*	0, 0,  lower left
			0, h,  upper left
			w, 0,  lower right
			w, h   upper right 	*/
		vertexes[3] = vertexes[7] = height;
		vertexes[4] = vertexes[6] = width;
		
		borderVertexes[0] = borderVertexes[1] = borderVertexes[2] = borderVertexes[7] = -borderWidth;
		borderVertexes[3] = borderVertexes[5] = barHeight + borderWidth;
		borderVertexes[4] = borderVertexes[6] = barWidth + borderWidth;
	}

} // namespace engine
