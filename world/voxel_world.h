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

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/templates/hash_map.h"
#include "core/config/engine.h"
#else
#include "core/hash_map.h"
#include "core/engine.h"
#endif

#include "../defines.h"

#include navigation_h

#include "../areas/world_area.h"
#include "../level_generator/voxel_level_generator.h"
#include "../library/voxel_library.h"

#include "core/os/os.h"

#if PROPS_PRESENT
#include "../../props/props/prop_data.h"
#endif

class VoxelStructure;
class VoxelChunk;
class PropData;

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

	Ref<VoxelLibrary> get_library();
	void set_library(const Ref<VoxelLibrary> &library);

	Ref<VoxelLevelGenerator> get_level_generator() const;
	void set_level_generator(const Ref<VoxelLevelGenerator> &level_generator);

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
	Ref<WorldArea> world_area_get(const int index) const;
	void world_area_add(const Ref<WorldArea> &area);
	void world_area_remove(const int index);
	void world_areas_clear();
	int world_area_get_count() const;

	//Voxel Structures
	Ref<VoxelStructure> voxel_structure_get(const int index) const;
	void voxel_structure_add(const Ref<VoxelStructure> &structure);
	void voxel_structure_remove(const Ref<VoxelStructure> &structure);
	void voxel_structure_remove_index(const int index);
	void voxel_structures_clear();
	int voxel_structure_get_count() const;
	void voxel_structure_add_at_position(Ref<VoxelStructure> structure, const Vector3 &world_position);

	Vector<Variant> voxel_structures_get();
	void voxel_structures_set(const Vector<Variant> &structures);

	//Chunks
	void chunk_add(Ref<VoxelChunk> chunk, const int x, const int y, const int z);
	bool chunk_has(const int x, const int y, const int z) const;
	Ref<VoxelChunk> chunk_get(const int x, const int y, const int z);
	Ref<VoxelChunk> chunk_remove(const int x, const int y, const int z);
	Ref<VoxelChunk> chunk_remove_index(const int index);
	Ref<VoxelChunk> chunk_get_index(const int index);

	int chunk_get_count() const;

	void chunks_clear();

	Ref<VoxelChunk> chunk_get_or_create(const int x, const int y, const int z);
	Ref<VoxelChunk> chunk_create(const int x, const int y, const int z);
	void chunk_setup(Ref<VoxelChunk> chunk);

	void chunk_generate(Ref<VoxelChunk> chunk);

	Vector<Variant> chunks_get();
	void chunks_set(const Vector<Variant> &chunks);

	bool can_chunk_do_build_step();
	bool is_position_walkable(const Vector3 &p_pos);

	void on_chunk_mesh_generation_finished(Ref<VoxelChunk> p_chunk);

	void generation_queue_add_to(const Ref<VoxelChunk> &chunk);
	Ref<VoxelChunk> generation_queue_get_index(const int index);
	void generation_queue_remove_index(const int index);
	int generation_queue_get_size() const;

	void generation_add_to(const Ref<VoxelChunk> &chunk);
	Ref<VoxelChunk> generation_get_index(const int index);
	void generation_remove_index(const int index);
	int generation_get_size() const;

#if PROPS_PRESENT
	void prop_add(Transform tarnsform, const Ref<PropData> &prop, const bool apply_voxel_scael = true);
#endif

	//Lights
	void light_add(const Ref<VoxelLight> &light);
	Ref<VoxelLight> light_get(const int index);
	void light_remove(const int index);
	int light_get_count() const;
	void lights_clear();

	Vector<Variant> lights_get();
	void lights_set(const Vector<Variant> &chunks);

	//Helpers
	uint8_t get_voxel_at_world_position(const Vector3 &world_position, const int channel_index);
	void set_voxel_at_world_position(const Vector3 &world_position, const uint8_t data, const int channel_index, const bool rebuild = true);
	Ref<VoxelChunk> get_chunk_at_world_position(const Vector3 &world_position);
	Ref<VoxelChunk> get_or_create_chunk_at_world_position(const Vector3 &world_position);
	void set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel);

	int get_channel_index_info(const ChannelTypeInfo channel_type);

#if VERSION_MAJOR >= 4
	GDVIRTUAL1(_chunk_added, Ref<VoxelChunk>);

	GDVIRTUAL0(_generation_finished);

	GDVIRTUAL4R(Ref<VoxelChunk>, _create_chunk, int, int, int, Ref<VoxelChunk>);
	GDVIRTUAL1(_prepare_chunk_for_generation, Ref<VoxelChunk>);
	GDVIRTUAL1(_generate_chunk, Ref<VoxelChunk>);

	GDVIRTUAL1R(int, _get_channel_index_info, int);

	GDVIRTUAL5(_set_voxel_with_tool, bool, Vector3, Vector3, int, int);
#endif

	VoxelWorld();
	~VoxelWorld();

protected:
	virtual void _generate_chunk(Ref<VoxelChunk> chunk);
	virtual Ref<VoxelChunk> _create_chunk(int x, int y, int z, Ref<VoxelChunk> p_chunk);
	virtual int _get_channel_index_info(const ChannelTypeInfo channel_type);
	virtual void _set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel);

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

	Ref<VoxelLibrary> _library;
	Ref<VoxelLevelGenerator> _level_generator;
	float _voxel_scale;
	int _chunk_spawn_range;

	HashMap<IntPos, Ref<VoxelChunk>, IntPosHasher> _chunks;
	Vector<Ref<VoxelChunk> > _chunks_vector;

	Vector<Ref<WorldArea> > _world_areas;

	Vector<Ref<VoxelStructure> > _voxel_structures;

	NodePath _player_path;
	Spatial *_player;

	bool _use_threads;
	int _max_concurrent_generations;
	Vector<Ref<VoxelChunk> > _generation_queue;
	Vector<Ref<VoxelChunk> > _generating;
	int _max_frame_chunk_build_steps;
	int _num_frame_chunk_build_steps;

	Vector<Ref<VoxelLight> > _lights;
};

_FORCE_INLINE_ bool operator==(const VoxelWorld::IntPos &a, const VoxelWorld::IntPos &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

VARIANT_ENUM_CAST(VoxelWorld::ChannelTypeInfo);

#endif
