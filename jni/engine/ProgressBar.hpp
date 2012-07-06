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

#ifndef __PROGRESSBAR_HPP__
#define __PROGRESSBAR_HPP__

#include "EngineConfig.hpp"
#include "Drawable.hpp"
#include <vector>
#include "GL.hpp"

namespace engine
{

	class ProgressBar: public Drawable
	{
	public:
		ProgressBar(float maxVal, float width, float height);
		void setCurrentValue(float value);
		float getCurrentValue() const;
		void setColor(float r, float g, float b, float a);
		void setBorderColor(float r, float g, float b, float a);
		virtual void draw(const Rectangle& screen);
		virtual void setPosition(const Point& p);
		virtual void setPositionInterpretation(Drawable::EPositionRelativeToOption positionInterpretation);

		void setBorderWidth(float width);

		// Set external variable for this progressbar (by default progressbar operating with internal value).
		// Changing value of this variable will cause progressbar changing.
		void connectVariable(float& variable);
		ProgressBar& operator+=(float val);
		ProgressBar& operator-=(float val);
		
#if DEBUG // For testing purposes
		GLvoid* getColor() { return color; }
		GLvoid* getVertexes() { return vertexes; }
#endif

	private:
		void fillColorArray(GLfloat* destColorArray, int destSize, float r, float g, float b, float a);
		void setVertexes(GLfloat width, GLfloat height);

		float maxValue;
		float currentValue;
		float* connectedVariable;
		GLfloat barWidth;
		GLfloat barHeight;
		float scaleFactor;

		GLfloat vertexes[8];
		GLfloat color[32];

		GLfloat borderVertexes[8];
		GLfloat borderColor[32];
		GLfloat borderWidth;
	};





} // namespace engine

#endif // __PROGRESSBAR_HPP__
