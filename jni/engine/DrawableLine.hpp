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

#ifndef __DRAWABLE_LINE_HPP__
#define __DRAWABLE_LINE_HPP__

#include "EngineConfig.hpp"
#include "Drawable.hpp"
#include <vector>
#include "GL.hpp"

namespace engine
{
	class DrawableLine: public Drawable
	{
	public:
		DrawableLine(std::vector<Point> lineVertexes);
		void setColor(float r, float g, float b, float a);
		void setWidth(float width);
		virtual void draw(const Rectangle& screen);

	private:
		void fillColorArray(GLfloat* destColorArray, int destSize, float r, float g, float b, float a);
		void setVertexes(std::vector<Point> & lineVertexes);

		GLfloat width;
		int vertexNum;
		
		GLfloat vertexes[8];
		GLfloat color[32];

	};

} // namespace engine

#endif // __DRAWABLE_LINE_HPP__
