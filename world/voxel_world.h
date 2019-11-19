#ifndef VOXEL_WORLD_H
#define VOXEL_WORLD_H

#include "scene/3d/navigation.h"
#include "core/hash_map.h"
#include "core/engine.h"

#include "../library/voxelman_library.h"
#include "../level_generator/voxelman_level_generator.h"
#include "../areas/world_area.h"

#include "core/os/os.h"

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

    int get_current_seed() const;
    void set_current_seed(const int value);

	bool get_use_threads();
	void set_use_threads(bool value);

	uint32_t get_max_concurrent_generations();
	void set_max_concurrent_generations(uint32_t value);
    
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

	Ref<WorldArea> get_world_area(const int index) const;
	void add_world_area(Ref<WorldArea> area);
	void remove_world_area(const int index);
	void clear_world_areas();
	int get_world_area_count() const;

	void add_chunk(VoxelChunk *chunk, const int x, const int y, const int z);
	void add_chunk_bind(Node *chunk, const int x, const int y, const int z);
	VoxelChunk *get_chunk(const int x, const int y, const int z) const;
	VoxelChunk *remove_chunk(const int x, const int y, const int z);

	VoxelChunk *get_chunk_index(const int index);
	int get_chunk_count() const;

	void add_to_generation_queue_bind(Node *chunk);
	void add_to_generation_queue(VoxelChunk *chunk);
	VoxelChunk *get_generation_queue_index(int index);
	void remove_generation_queue_index(int index);
	int get_generation_queue_size();

	void add_to_generation_bind(Node *chunk);
	void add_to_generation(VoxelChunk *chunk);
	VoxelChunk *get_generation_index(int index);
	void remove_generation_index(int index);
	int get_generation_size();

	void clear();

	VoxelChunk *create_chunk(int x, int y, int z);
	VoxelChunk *_create_chunk(int x, int y, int z, Node *p_chunk);

	void generate_chunk_bind(Node *p_chunk);
	void generate_chunk(VoxelChunk *p_chunk);
	void _generate_chunk(Node *p_chunk);

	VoxelWorld();
	~VoxelWorld();

protected:
	virtual void _notification(int p_what);
	static void _bind_methods();

public:
	struct IntPos {
		int x;
		int y;
		int z;

		IntPos() {
			x = 0;
			y = 0;
			z = 0;
		}

		IntPos(int p_x, int p_y, int p_z) {
			x = p_x;
			y = p_y;
			z = p_z;
		}
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
	int _current_seed;
	
    Ref<VoxelmanLibrary> _library;
    Ref<VoxelmanLevelGenerator> _level_generator;
	float _voxel_scale;
	int _chunk_spawn_range;
    
	HashMap<IntPos, VoxelChunk *, IntPosHasher> _chunks; 
    Vector<VoxelChunk *> _chunks_vector;

	Vector<Ref<WorldArea> > _world_areas;

	NodePath _player_path;
	Spatial *_player;

	bool _use_threads;
	uint32_t _max_concurrent_generations;
	Vector<VoxelChunk *> _generation_queue;
	Vector<VoxelChunk *> _generating;
};

_FORCE_INLINE_ bool operator==(const VoxelWorld::IntPos &a, const VoxelWorld::IntPos &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

#endif
