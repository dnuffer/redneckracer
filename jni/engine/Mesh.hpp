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

#ifndef engine_Mesh_hpp_INCLUDED_
#define engine_Mesh_hpp_INCLUDED_

#include "EngineConfig.hpp"
#include "GL.hpp"
#include "graphlib/GenericPoint3.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "boost/range/algorithm_ext/push_back.hpp"
#include <vector>

namespace engine
{

class Mesh : public virtual IntrusiveCountableBase
{
public:
	template <typename VertsRangeT, typename ColorsRangeT>
	Mesh(const VertsRangeT& verts, int coordsPerVertex, GLenum renderStyle, const ColorsRangeT& colors, int componentsPerColor)
		: coordsPerVertex_(coordsPerVertex)
		, componentsPerColor_(componentsPerColor)
		, renderStyle_(renderStyle)
		, radius_(0)
	{
		boost::range::push_back(vertexes_, verts);
		boost::range::push_back(colors_, colors);
		calculateCentroid();
		calculateRadius();
	}

	virtual void draw();

protected:
	// must be 2,3,or 4
	int coordsPerVertex_;
	vector<GLfloat> vertexes_;
	int componentsPerColor_;
	vector<GLfloat> colors_;

	GLenum renderStyle_;

	GenericPoint3<float> centroid_;
	float radius_;

	void calculateCentroid();
	void calculateRadius();

	GLsizei vertexCount() const { return vertexes_.size() / coordsPerVertex_; }
};

}

#endif
