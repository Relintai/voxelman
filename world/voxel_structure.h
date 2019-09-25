#ifndef VOXEL_STRUCTURE_H
#define VOXEL_STRUCTURE_H

#include "core/reference.h"

#include "core/hash_map.h"
#include "../math/vector3i.h"
#include "voxel_buffer.h"

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

	Ref<VoxelBuffer> get_chunk_voxel_pos(int x, int y, int z);

	int get_voxel(int x, int y, int z, unsigned int channel_index = 0);
	void set_voxel(int value, int x, int y, int z, unsigned int channel_index = 0);
	void set_voxel_v(int value, Vector3 pos, unsigned int channel_index = 0);

	void add_chunk(Ref<VoxelBuffer> chunk, const int x, const int y, const int z);
	Ref<VoxelBuffer> get_chunk(const int x, const int y, const int z);
	Ref<VoxelBuffer> remove_chunk(const int x, const int y, const int z);

	Ref<VoxelBuffer> get_chunk_index(const int index);
	int get_chunk_count() const;

	void clear_chunks();

	VoxelStructure();
	~VoxelStructure();
    
protected:
	static void _bind_methods();

	struct VoxelStructureChunk {
		Vector3i position;
		Ref<VoxelBuffer> buffer;
	};

private:
	Vector3i _chunk_size;
	Vector3i _world_position;

	HashMap<Vector3i, Ref<VoxelBuffer>, Vector3iHasher> _chunks;
	Vector<VoxelStructureChunk> _chunks_vector;
};

#endif
