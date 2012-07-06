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

#ifndef __DRAWABLE_RECTANGLE_HPP__
#define __DRAWABLE_RECTANGLE_HPP__

#include "EngineConfig.hpp"
#include "Drawable.hpp"
#include <vector>
#include "GL.hpp"

namespace engine
{

	class DrawableRectangle: public Drawable
	{
	public:
		DrawableRectangle(float width, float height);
		void setColor(float r, float g, float b, float a);
		void setBorderColor(float r, float g, float b, float a);
		void setBorderWidth(float width);
		virtual void draw(const Rectangle& screen);

	private:
		void fillColorArray(GLfloat* destColorArray, int destSize, float r, float g, float b, float a);
		void setVertexes(GLfloat width, GLfloat height);

		GLfloat barWidth;
		GLfloat barHeight;

		GLfloat vertexes[8];
		GLfloat color[32];
		
		GLfloat borderVertexes[8];
		GLfloat borderColor[32];
		GLfloat borderWidth;

	};

} // namespace engine

#endif // __DRAWABLE_RECTANGLE_HPP__
