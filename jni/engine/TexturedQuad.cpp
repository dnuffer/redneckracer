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

#include "EngineConfig.hpp"
#include "TexturedQuad.hpp"
#include "GL.hpp"

namespace engine
{

const GLfloat TexturedQuad::s_texturedQuadVertexes[8] = {-0.5,-0.5, 0.5,-0.5, -0.5,0.5, 0.5,0.5};
const GLfloat TexturedQuad::s_texturedQuadColorValues[16] = {1.0,1.0,1.0,1.0, 1.0,1.0,1.0,1.0, 1.0,1.0,1.0,1.0, 1.0,1.0,1.0,1.0};

void TexturedQuad::draw(const Rectangle& screen)
{
	using namespace gl;
	if( !texture_ || !texture_->loaded() )
	{
		return;
	}

	vertex(coordsPerVertex_, GL_FLOAT, 0, &vertexes_[0]);
	color(componentsPerColor_, GL_FLOAT, 0, &colors_[0]);

	texture_->draw(screen);

	texCoord(2, GL_FLOAT, 0, uvCoordinates_);

	//render
	drawArrays(renderStyle_, 0, vertexCount());

}

	void TexturedQuad::setFlippedHorizontal(bool x) 
	{ 
		if (_flippedHorizontal == x) return;
		
		// swap uMin and uMax
		swap(uvCoordinates_[0], uvCoordinates_[2]);
		swap(uvCoordinates_[4], uvCoordinates_[6]);
		
		_flippedHorizontal = x; 
	}

	void TexturedQuad::setFlippedVertical(bool x)
	{
		if (_flippedVertical == x) return;
		
		// swap vMin and vMax
		swap(uvCoordinates_[1], uvCoordinates_[5]);
		swap(uvCoordinates_[3], uvCoordinates_[7]);
		
		_flippedVertical = x;
	}

	Size TexturedQuad::getRealSize() const
	{
		return Size(realBound.right - realBound.left, realBound.top - realBound.bottom);
	}

	Point TexturedQuad::getRealBoundCenter() const
	{
		return Point((realBound.right + realBound.left) / 2, (realBound.top + realBound.bottom) / 2);
	}
}
