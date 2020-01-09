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
