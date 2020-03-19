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

#include "voxel_structure.h"

#include "core/hash_map.h"
#include "core/pool_vector.h"
#include "voxel_chunk.h"

class BlockVoxelStructure : public VoxelStructure {
	GDCLASS(BlockVoxelStructure, VoxelStructure);

public:
	int get_channel_count() const;
	void set_channel_count(const int value);

	uint8_t get_voxel(int p_x, int p_y, int p_z, int p_channel_index) const;
	void set_voxel(uint8_t p_value, int p_x, int p_y, int p_z, int p_channel_index);

	PoolByteArray get_voxel_data(int p_x, int p_y, int p_z) const;
	void set_voxel_data(PoolByteArray p_arr, int p_x, int p_y, int p_z);

	//void _write_to_chunk(Node *chunk);

	void clear();

	BlockVoxelStructure();
	~BlockVoxelStructure();

protected:
	static void _bind_methods();

public:
	struct VSIntPos {
		int x;
		int y;
		int z;
	};

	struct VSIntPosHasher {
		static _FORCE_INLINE_ uint32_t hash(const VSIntPos &v) {
			uint32_t hash = hash_djb2_one_32(v.x);
			hash = hash_djb2_one_32(v.y, hash);
			return hash_djb2_one_32(v.z, hash);
		}
	};

private:
	int _channel_count;

	HashMap<VSIntPos, PoolByteArray, VSIntPosHasher> _data;
};

_FORCE_INLINE_ bool operator==(const BlockVoxelStructure::VSIntPos &a, const BlockVoxelStructure::VSIntPos &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

#endif
