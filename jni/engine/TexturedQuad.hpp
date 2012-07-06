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

#ifndef engine_TexturedQuad_hpp_INCLUDED_
#define engine_TexturedQuad_hpp_INCLUDED_

#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Size.hpp"
#include "Point.hpp"

namespace engine
{

class TexturedQuad : public Mesh
{
public:
	TexturedQuad(GLfloat uMin, GLfloat vMin, GLfloat uMax, GLfloat vMax, const TexturePtr& texture,
			UInt16 width, UInt16 height)
		: Mesh(s_texturedQuadVertexes, 2, GL_TRIANGLE_STRIP, s_texturedQuadColorValues, 4)
		, texture_(texture)
		, width(width)
		, height(height)
		, _flippedHorizontal(false)
		, _flippedVertical(false)
		, _scaleX(1.0)
		, _scaleY(1.0)
		, realBound(0, width, height, 0)

		
	{
		uvCoordinates_[0] = uMin;
		uvCoordinates_[1] = vMax;

		uvCoordinates_[2] = uMax;
		uvCoordinates_[3] = vMax;

		uvCoordinates_[4] = uMin;
		uvCoordinates_[5] = vMin;

		uvCoordinates_[6] = uMax;
		uvCoordinates_[7] = vMin;
	}

	TexturedQuad(GLfloat uMin, GLfloat vMin, GLfloat uMax, GLfloat vMax, const TexturePtr& texture,
			UInt16 width, UInt16 height, const Rectangle& realBoundingRect)
		: Mesh(s_texturedQuadVertexes, 2, GL_TRIANGLE_STRIP, s_texturedQuadColorValues, 4)
		, texture_(texture)
		, width(width)
		, height(height)
		, _flippedHorizontal(false)
		, _flippedVertical(false)
		, _scaleX(1.0)
		, _scaleY(1.0)
		, realBound(realBoundingRect)

	{
		uvCoordinates_[0] = uMin;
		uvCoordinates_[1] = vMax;
		uvCoordinates_[2] = uMax;
		uvCoordinates_[3] = vMax;

		uvCoordinates_[4] = uMin;
		uvCoordinates_[5] = vMin;

		uvCoordinates_[6] = uMax;
		uvCoordinates_[7] = vMin;
	}

	virtual void draw(const Rectangle& screen);

	// Return size of full image.
	Size size() const { return Size(width*_scaleX, height*_scaleY); }

	// Return size of real bounding rectangle.
	Size getRealSize() const;

	// Center relatively quad texture image.
	Point getRealBoundCenter() const;

	bool flippedHorizontal() { return _flippedHorizontal; }
	void setFlippedHorizontal(bool x);
	bool flippedVertical() { return _flippedVertical; }
	void setFlippedVertical(bool x);
	
	void scale(float scale) { scaleX(scale); scaleY(scale); }
	void scaleX(float scale) { _scaleX = scale; }
	void scaleY(float scale) { _scaleY = scale; }
	float getScaleX(){ return _scaleX; }
	float getScaleY(){ return _scaleY; }

private:
	TexturePtr texture_;
	GLfloat uvCoordinates_[8];
	UInt16 width;
	UInt16 height;
	bool _flippedHorizontal;
	bool _flippedVertical;
	float _scaleX;
	float _scaleY;
	static const GLfloat s_texturedQuadVertexes[8];
	static const GLfloat s_texturedQuadColorValues[16];
	Rectangle realBound;
};

}

#endif
