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

#include "core/hash_map.h"
#include "voxel_chunk.h"

class VoxelStructure : public Reference {
	GDCLASS(VoxelStructure, Reference);

public:
	int get_chunk_size_x() const;
	void set_chunk_size_x(const int value);

	int get_chunk_size_y() const;
	void set_chunk_size_y(const int value);

	int get_chunk_size_z() const;
	void set_chunk_size_z(const int value);

	int get_world_position_x() const;
	void set_world_position_x(const int value);

	int get_world_position_y() const;
	void set_world_position_y(const int value);

	int get_world_position_z() const;
	void set_world_position_z(const int value);

	VoxelChunk *get_chunk_voxel_pos(int x, int y, int z);

	int get_voxel(int x, int y, int z, unsigned int channel_index = 0);
	void set_voxel(int value, int x, int y, int z, unsigned int channel_index = 0);
	void set_voxel_v(int value, Vector3 pos, unsigned int channel_index = 0);

	void add_chunk_bind(Node *chunk, const int x, const int y, const int z);
	void add_chunk(VoxelChunk *chunk, const int x, const int y, const int z);
	VoxelChunk *get_chunk(const int x, const int y, const int z);
	VoxelChunk *remove_chunk(const int x, const int y, const int z);

	VoxelChunk *get_chunk_index(const int index);
	int get_chunk_count() const;

	void clear_chunks();

	VoxelStructure();
	~VoxelStructure();

protected:
	static void _bind_methods();

	struct IntPos {
		int x;
		int y;
		int z;
	};

	struct IntPosHasher {
		static _FORCE_INLINE_ uint32_t hash(const IntPos &v) {
			uint32_t hash = hash_djb2_one_32(v.x);
			hash = hash_djb2_one_32(v.y, hash);
			return hash_djb2_one_32(v.z, hash);
		}
	};

private:
	int _chunk_size_x;
	int _chunk_size_y;
	int _chunk_size_z;

	int _world_position_x;
	int _world_position_y;
	int _world_position_z;

	HashMap<IntPos, VoxelChunk *, IntPosHasher> _chunks;
	Vector<VoxelChunk *> _chunks_vector;
};

#endif
