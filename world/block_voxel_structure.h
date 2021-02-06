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

#ifndef BLOCK_VOXEL_STRUCTURE_H
#define BLOCK_VOXEL_STRUCTURE_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/templates/vector.h"
#else
#include "core/vector.h"
#endif

#include "voxel_structure.h"

#include "../defines.h"

#include pool_vector_h
include_pool_vector

#include "voxel_chunk.h"
		;

class BlockVoxelStructure : public VoxelStructure {
	GDCLASS(BlockVoxelStructure, VoxelStructure);

public:
	int get_channel_type() const;
	void set_channel_type(const int value);

	int get_channel_isolevel() const;
	void set_channel_isolevel(const int value);

	int get_voxel_type(int p_x, int p_y, int p_z) const;
	int get_voxel_isolevel(int p_x, int p_y, int p_z) const;

	void set_voxel(int p_x, int p_y, int p_z, int p_type, int p_isolevel);

	void _write_to_chunk(Ref<VoxelChunk> chunk);

	void clear();

	BlockVoxelStructure();
	~BlockVoxelStructure();

protected:
	static void _bind_methods();

public:
	struct DataEntry {
		int x;
		int y;
		int z;
		int data_type;
		int data_isolevel;
	};

private:
	int _channel_type;
	int _channel_isolevel;

	Vector<DataEntry> _data;
};

#endif
