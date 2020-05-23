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

#ifndef VOXEL_WORLD_H
#define VOXEL_WORLD_H

#include "../defines.h"

#include navigation_h

#include "core/engine.h"
#include "core/hash_map.h"

#include "../areas/world_area.h"
#include "../level_generator/voxelman_level_generator.h"
#include "../library/voxelman_library.h"

#include "core/os/os.h"

class VoxelStructure;
class VoxelChunk;
class VoxelChunkPropData;

class VoxelWorld : public Navigation {
	GDCLASS(VoxelWorld, Navigation);

public:
	enum ChannelTypeInfo {
		CHANNEL_TYPE_INFO_TYPE = 0,
		CHANNEL_TYPE_INFO_ISOLEVEL,
		CHANNEL_TYPE_INFO_LIQUID_FLOW,
	};

	static const String BINDING_STRING_CHANNEL_TYPE_INFO;

public:
	bool get_editable() const;
	void set_editable(const bool value);

	int get_chunk_size_x() const;
	void set_chunk_size_x(const int value);

	int get_chunk_size_y() const;
	void set_chunk_size_y(const int value);

	int get_chunk_size_z() const;
	void set_chunk_size_z(const int value);

	int get_data_margin_start() const;
	void set_data_margin_start(const int value);

	int get_data_margin_end() const;
	void set_data_margin_end(const int value);

	int get_current_seed() const;
	void set_current_seed(const int value);

	bool get_use_threads() const;
	void set_use_threads(const bool value);

	int get_max_concurrent_generations() const;
	void set_max_concurrent_generations(const int value);

	int get_max_frame_chunk_build_steps() const;
	void set_max_frame_chunk_build_steps(const int value);

	Ref<VoxelmanLibrary> get_library();
	void set_library(const Ref<VoxelmanLibrary> &library);

	Ref<VoxelmanLevelGenerator> get_level_generator() const;
	void set_level_generator(const Ref<VoxelmanLevelGenerator> &level_generator);

	float get_voxel_scale() const;
	void set_voxel_scale(const float value);

	int get_chunk_spawn_range() const;
	void set_chunk_spawn_range(const int value);

	NodePath get_player_path() const;
	void set_player_path(const NodePath &player_path);

	Spatial *get_player() const;
	void set_player(Spatial *player);
	void set_player_bind(Node *player);

	//World Areas
	Ref<WorldArea> get_world_area(const int index) const;
	void add_world_area(const Ref<WorldArea> &area);
	void remove_world_area(const int index);
	void clear_world_areas();
	int get_world_area_count() const;

	//Voxel Structures
	Ref<VoxelStructure> get_voxel_structure(const int index) const;
	void add_voxel_structure(const Ref<VoxelStructure> &structure);
	void remove_voxel_structure(const Ref<VoxelStructure> &structure);
	void remove_voxel_structure_index(const int index);
	void clear_voxel_structures();
	int get_voxel_structure_count() const;
	void add_voxel_structure_at_position(Ref<VoxelStructure> structure, const Vector3 &world_position);

	Vector<Variant> get_voxel_structures();
	void set_voxel_structures(const Vector<Variant> &structures);

	//Chunks
	void add_chunk(Ref<VoxelChunk> chunk, const int x, const int y, const int z);
	bool has_chunk(const int x, const int y, const int z) const;
	Ref<VoxelChunk> get_chunk(const int x, const int y, const int z);
	Ref<VoxelChunk> remove_chunk(const int x, const int y, const int z);
	Ref<VoxelChunk> remove_chunk_index(const int index);
	Ref<VoxelChunk> get_chunk_index(const int index);

	int get_chunk_count() const;

	void add_to_generation_queue(const Ref<VoxelChunk> &chunk);
	Ref<VoxelChunk> get_generation_queue_index(const int index);
	void remove_generation_queue_index(const int index);
	int get_generation_queue_size() const;

	void add_to_generation(const Ref<VoxelChunk> &chunk);
	Ref<VoxelChunk> get_generation_index(const int index);
	void remove_generation_index(const int index);
	int get_generation_size() const;

	void clear_chunks();

	Ref<VoxelChunk> get_or_create_chunk(const int x, const int y, const int z);
	Ref<VoxelChunk> create_chunk(const int x, const int y, const int z);

	void generate_chunk(Ref<VoxelChunk> chunk);

	bool can_chunk_do_build_step();
	bool is_position_walkable(const Vector3 &p_pos);

	void on_chunk_mesh_generation_finished(Ref<VoxelChunk> p_chunk);

	Vector<Variant> get_chunks();
	void set_chunks(const Vector<Variant> &chunks);

	//Props
	void add_prop(Ref<VoxelChunkPropData> prop);

	//Lights
	void add_light(const Ref<VoxelLight> &light);
	Ref<VoxelLight> get_light(const int index);
	void remove_light(const int index);
	int get_light_count() const;
	void clear_lights();

	Vector<Variant> get_lights();
	void set_lights(const Vector<Variant> &chunks);

	//Helpers
	uint8_t get_voxel_at_world_position(const Vector3 &world_position, const int channel_index);
	void set_voxel_at_world_position(const Vector3 &world_position, const uint8_t data, const int channel_index);
	Ref<VoxelChunk> get_chunk_at_world_position(const Vector3 &world_position);
	Ref<VoxelChunk> get_or_create_chunk_at_world_position(const Vector3 &world_position);

	int get_channel_index_info(const ChannelTypeInfo channel_type);

	VoxelWorld();
	~VoxelWorld();

protected:
	virtual void _generate_chunk(Ref<VoxelChunk> chunk);
	virtual Ref<VoxelChunk> _create_chunk(int x, int y, int z, Ref<VoxelChunk> p_chunk);
	virtual int _get_channel_index_info(const ChannelTypeInfo channel_type);

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

		IntPos(const Vector3 &p) {
			x = p.x;
			y = p.y;
			z = p.z;
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
	bool _editable;

	bool _is_priority_generation;

	int _chunk_size_x;
	int _chunk_size_y;
	int _chunk_size_z;
	int _current_seed;
	int _data_margin_start;
	int _data_margin_end;

	Ref<VoxelmanLibrary> _library;
	Ref<VoxelmanLevelGenerator> _level_generator;
	float _voxel_scale;
	int _chunk_spawn_range;

	HashMap<IntPos, Ref<VoxelChunk>, IntPosHasher> _chunks;
	Vector<Ref<VoxelChunk>> _chunks_vector;

	Vector<Ref<WorldArea>> _world_areas;

	Vector<Ref<VoxelStructure>> _voxel_structures;

	NodePath _player_path;
	Spatial *_player;

	bool _use_threads;
	int _max_concurrent_generations;
	Vector<Ref<VoxelChunk>> _generation_queue;
	Vector<Ref<VoxelChunk>> _generating;
	int _max_frame_chunk_build_steps;
	int _num_frame_chunk_build_steps;

	Vector<Ref<VoxelLight>> _lights;
};

_FORCE_INLINE_ bool operator==(const VoxelWorld::IntPos &a, const VoxelWorld::IntPos &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

VARIANT_ENUM_CAST(VoxelWorld::ChannelTypeInfo);

#endif
