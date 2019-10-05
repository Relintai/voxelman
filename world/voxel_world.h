#ifndef VOXEL_WORLD_H
#define VOXEL_WORLD_H

#include "scene/3d/navigation.h"
#include "core/hash_map.h"

#include "../math/vector3i.h"
#include "../library/voxelman_library.h"
#include "../level_generator/voxelman_level_generator.h"
#include "voxel_buffer.h"

class VoxelChunk;

class VoxelWorld : public Navigation {
	GDCLASS(VoxelWorld, Navigation);

public:
    int get_chunk_size_x() const;
    void set_chunk_size_x(const int value);
    
    int get_chunk_size_y() const;
    void set_chunk_size_y(const int value);
    
    int get_chunk_size_z() const;
    void set_chunk_size_z(const int value);
    
    Ref<VoxelmanLibrary> get_library() const;
    void set_library(const Ref<VoxelmanLibrary> library);
    
    Ref<VoxelmanLevelGenerator> get_level_generator() const;
    void set_level_generator(const Ref<VoxelmanLevelGenerator> level_generator);

	float get_voxel_scale() const;
	void set_voxel_scale(const float value);

	int get_chunk_spawn_range() const;
	void set_chunk_spawn_range(const int value);

	NodePath get_player_path();
	void set_player_path(NodePath player_path);
    
    Spatial *get_player() const;
	void set_player(Spatial *player);
	void set_player_bind(Node *player);

	void add_chunk(Ref<VoxelChunk> chunk, const int x, const int y, const int z);
	Ref<VoxelChunk> get_chunk(const int x, const int y, const int z) const;
	Ref<VoxelChunk> remove_chunk(const int x, const int y, const int z);

	Ref<VoxelChunk> get_chunk_index(const int index);
	int get_chunk_count() const;

	void clear_chunks();

	VoxelWorld();
	~VoxelWorld();

protected:
	static void _bind_methods();

private:
    Vector3i _chunk_size;
    Ref<VoxelmanLibrary> _library;
    Ref<VoxelmanLevelGenerator> _level_generator;
	float _voxel_scale;
	int _chunk_spawn_range;
    
	HashMap<Vector3i, Ref<VoxelChunk>, Vector3iHasher> _chunks; 
    Vector<Ref<VoxelChunk> > _chunks_vector;

	NodePath _player_path;
	Spatial *_player;
};

#endif
