/*
Copyright (c) 2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef VOXEL_LIGHT_NODE_H
#define VOXEL_LIGHT_NODE_H

#include "core/version.h"

#if VERSION_MAJOR < 4
#include "scene/3d/spatial.h"
#else
#include "scene/3d/node_3d.h"

#define Spatial Node3D
#endif

#include "core/math/vector3.h"

class VoxelLightNode : public Spatial {
	GDCLASS(VoxelLightNode, Spatial);

public:
	//make it turn into a normal light if voxelman isn't present?

	VoxelLightNode();
	~VoxelLightNode();

protected:
	static void _bind_methods();

private:
};

#endif
