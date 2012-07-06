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
#include "Mesh.hpp"

namespace engine
{

void Mesh::calculateCentroid()
{
	float xTotal = 0;
	float yTotal = 0;
	float zTotal = 0;
	int vertexCount = this->vertexCount();
	// step through each vertex and add them all up
	for (int index = 0; index < vertexCount; index++)
	{
		int position = index * coordsPerVertex_;
		xTotal += vertexes_[position];
		yTotal += vertexes_[position + 1];
		if (coordsPerVertex_ > 2) zTotal += vertexes_[position + 2];
	}
	// now average each total over the number of vertexes
	centroid_ = GenericPoint3<float>(xTotal/(float)vertexCount, yTotal/(float)vertexCount, zTotal/(float)vertexCount);

}

void Mesh::calculateRadius()
{
	radius_ = 0.0;
	for (int index = 0; index < vertexCount(); index++)
	{
		int position = index * coordsPerVertex_;
		GenericPoint3<float> vert;

		if (coordsPerVertex_ > 2)
		{
			vert = GenericPoint3<float>(vertexes_[position], vertexes_[position + 1], vertexes_[position + 2]);
		}
		else
		{
			vert = GenericPoint3<float>(vertexes_[position], vertexes_[position + 1], 0.0);
		}

		float thisRadius = length(centroid_ - vert);

		if (radius_ < thisRadius)
			radius_ = thisRadius;
	}
}

void Mesh::draw()
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	// load arrays into the engine
	glVertexPointer(coordsPerVertex_, GL_FLOAT, 0, &vertexes_[0]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(componentsPerColor_, GL_FLOAT, 0, &colors_[0]);
	glEnableClientState(GL_COLOR_ARRAY);

	//render
	glDrawArrays(renderStyle_, 0, vertexCount());
}

}
