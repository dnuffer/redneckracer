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

#include "ProgressBar.hpp"

namespace engine
{
	ProgressBar::ProgressBar(float maxVal, float width, float height):
			  maxValue(maxVal)
			, currentValue(0)
			, connectedVariable(&currentValue)
			, barWidth(width)
			, barHeight(height)
			, borderWidth(4)
	{
		for (int i = 0; i<8; ++i)
		{
			vertexes[i] = 0.0;
			borderVertexes[i] = 0.0;
		}
		scaleFactor = width / maxVal;
		setVertexes(width, height);
		setColor(1.0, 1.0, 1.0, 1.0);
		setBorderColor(1.0, 1.0, 1.0, 1.0);
	}

	// Draw a simple colored rectangle without texture.
	void ProgressBar::draw(const Rectangle& screen)
	{
		using namespace gl;

		setVertexes((*connectedVariable) * scaleFactor, barHeight);
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

	void ProgressBar::setVertexes(GLfloat width, GLfloat height)
	{
		// Check for overflow.
		if (width > barWidth)
		{
			width = barWidth;
			(*connectedVariable) = maxValue;
		}
		else if (width < 0)
		{
			width = 0;
			(*connectedVariable) = 0;
		}

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

	float ProgressBar::getCurrentValue() const
	{
		return *connectedVariable;
	}

	void ProgressBar::setBorderWidth(float width)
	{
		borderWidth = width;
	}

	void ProgressBar::setCurrentValue(float value)
	{
		if (value <= maxValue && value >= 0)
		{
			*connectedVariable = value;
		}
		else if (value <= maxValue && value < 0)
		{
			*connectedVariable = 0;
		}
		else
		{
			*connectedVariable = maxValue;
		}
	}

	// Color component - from 0.0f to 1.0f.
	void ProgressBar::setColor(float r, float g, float b, float a)
	{
		fillColorArray(color, 32, r, g, b, a);
	}

	// Color component - from 0.0f to 1.0f.
	void ProgressBar::setBorderColor(float r, float g, float b, float a)
	{
		fillColorArray(borderColor, 32, r, g, b, a);
	}

	void ProgressBar::setPosition(const Point& p)
	{
		Drawable::setPosition(p);
	}

	void ProgressBar::setPositionInterpretation(Drawable::EPositionRelativeToOption positionInterpretation)
	{
		Drawable::setPositionInterpretation(positionInterpretation);
	}

	ProgressBar& ProgressBar::operator +=(float val)
	{
		setCurrentValue((*connectedVariable) + val);
		return *this;
	}

	ProgressBar& ProgressBar::operator -=(float val)
	{
		if (val > (*connectedVariable)) setCurrentValue(0);
		else setCurrentValue((*connectedVariable) - val);
		return *this;
	}

	void ProgressBar::connectVariable(float& variable)
	{
		connectedVariable = &variable;
	}

	void ProgressBar::fillColorArray(GLfloat* destColorArray, int destSize, float r, float g, float b, float a)
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

} // namespace engine
