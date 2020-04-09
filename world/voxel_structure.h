/*
Copyright (c) 2019-2020 Péter Magyar

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

#ifndef VOXEL_STRUCTURE_H
#define VOXEL_STRUCTURE_H

#include "core/reference.h"

#include "core/version.h"

#if VERSION_MAJOR < 4
#include "core/pool_vector.h"
#else
#include "core/vector.h"

template <class N>
using PoolVector = Vector<N>;
#endif

#include "core/hash_map.h"
#include "core/math/aabb.h"
#include "voxel_chunk.h"

class VoxelStructure : public Reference {
	GDCLASS(VoxelStructure, Reference);

public:
	bool get_use_aabb() const;
	void set_use_aabb(const bool value);

	AABB get_aabb() const;
	void set_aabb(const AABB &value);

	int get_world_position_x() const;
	void set_world_position_x(const int value);

	int get_world_position_y() const;
	void set_world_position_y(const int value);

	int get_world_position_z() const;
	void set_world_position_z(const int value);

	void write_to_chunk(Node *chunk);

	VoxelStructure();
	~VoxelStructure();

protected:
	static void _bind_methods();

private:
	bool _use_aabb;
	AABB _aabb;

	int _world_position_x;
	int _world_position_y;
	int _world_position_z;
};

#endif
