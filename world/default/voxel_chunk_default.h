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

#ifndef VOXEL_CHUNK_DEFAULT_H
#define VOXEL_CHUNK_DEFAULT_H

#include "../voxel_chunk.h"

#include "../../defines.h"

#include immediate_geometry_h

#include "core/engine.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "core/os/thread_safe.h"
#include "core/ustring.h"

#include "core/array.h"

#include "scene/resources/packed_scene.h"

#include "../voxel_world.h"

#include "../../data/voxel_light.h"

#include "../../meshers/voxel_mesher.h"

#include "../../library/voxel_surface.h"
#include "../../library/voxelman_library.h"

#include "../voxel_chunk_prop_data.h"

class VoxelWorld;

class VoxelChunkDefault : public VoxelChunk {
	GDCLASS(VoxelChunkDefault, VoxelChunk);

	_THREAD_SAFE_CLASS_

public:
	static const String BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE;
	static const String BINDING_STRING_BUILD_FLAGS;

	enum {
		VOXEL_CHUNK_STATE_GENERATION_QUEUED = 1,
		VOXEL_CHUNK_STATE_GENERATION,
		VOXEL_CHUNK_STATE_MESH_GENERATION_QUEUED,
		VOXEL_CHUNK_STATE_MESH_GENERATION,
		VOXEL_CHUNK_STATE_MAX,
	};

	enum {
		BUILD_PHASE_DONE = 0,
		BUILD_PHASE_SETUP,
		BUILD_PHASE_TERRARIN_MESH_SETUP,
		BUILD_PHASE_COLLIDER,
		BUILD_PHASE_LIGHTS,
		BUILD_PHASE_TERRARIN_MESH,
		BUILD_PHASE_PROP_MESH,
		BUILD_PHASE_FINALIZE,
		BUILD_PHASE_MAX
	};

	enum DefaultChannels {
		DEFAULT_CHANNEL_TYPE = 0,
		DEFAULT_CHANNEL_ISOLEVEL,
		DEFAULT_CHANNEL_LIGHT_COLOR_R,
		DEFAULT_CHANNEL_LIGHT_COLOR_G,
		DEFAULT_CHANNEL_LIGHT_COLOR_B,
		DEFAULT_CHANNEL_AO,
		DEFAULT_CHANNEL_RANDOM_AO,
		DEFAULT_CHANNEL_LIQUID_FLOW,
		MAX_DEFAULT_CHANNELS
	};

	enum ActiveBuildPhaseType {
		BUILD_PHASE_TYPE_NORMAL = 0,
		BUILD_PHASE_TYPE_PROCESS,
		BUILD_PHASE_TYPE_PHYSICS_PROCESS,
	};

	enum {
		MESH_INDEX_TERRARIN = 0,
		MESH_INDEX_PROP,
		MESH_INDEX_LIQUID,
		MESH_INDEX_CLUTTER,
	};

	enum {
		MESH_TYPE_INDEX_MESH = 0,
		MESH_TYPE_INDEX_MESH_INSTANCE,
		MESH_TYPE_INDEX_SHAPE,
		MESH_TYPE_INDEX_BODY,
		MESH_TYPE_INDEX_AREA,
	};

	enum BuildFlags {
		BUILD_FLAG_USE_ISOLEVEL = 1 << 0,
		BUILD_FLAG_USE_LIGHTING = 1 << 1,
		BUILD_FLAG_USE_AO = 1 << 2,
		BUILD_FLAG_USE_RAO = 1 << 3,
		BUILD_FLAG_GENERATE_AO = 1 << 4,
		BUILD_FLAG_AUTO_GENERATE_RAO = 1 << 5,
		BUILD_FLAG_BAKE_LIGHTS = 1 << 6,
		BUILD_FLAG_CREATE_COLLIDER = 1 << 7,
		BUILD_FLAG_CREATE_LODS = 1 << 8,
	};

public:
	int get_build_flags() const;
	void set_build_flags(const int flags);

	ActiveBuildPhaseType get_active_build_phase_type() const;
	void set_active_build_phase_type(const ActiveBuildPhaseType value);

	bool get_build_phase_done() const;
	void set_build_phase_done(const bool value);

	int get_lod_size() const;
	void set_lod_size(const int lod_size);

	int get_current_build_phase() const;
	void set_current_build_phase(const int value);

	int get_max_build_phase() const;
	void set_max_build_phase(const int value);

	bool get_lights_dirty() const;
	void set_lights_dirty(const bool value);

	//Lod
	int get_lod_num() const;
	void set_lod_num(const int value);

	int get_current_lod_level() const;
	void set_current_lod_level(const int value);

	//Data Management functions
	void generate_ao();

	//Meshing
	static void _build_step_threaded(void *_userdata);

	void build_step();

	void build_phase();
	void build_phase_process();
	void build_phase_physics_process();

	bool has_next_phase();
	void next_phase();

	//Meshes
	Dictionary get_mesh_rids();
	void set_mesh_rids(const Dictionary &rids);
	void clear_rids();

	RID get_mesh_rid(const int mesh_index, const int mesh_type_index);
	void set_mesh_rid(const int mesh_index, const int mesh_type_index, RID value);
	RID get_mesh_rid_index(const int mesh_index, const int mesh_type_index, const int index);
	void set_mesh_rid_index(const int mesh_index, const int mesh_type_index, const int index, RID value);
	int get_mesh_rid_count(const int mesh_index, const int mesh_type_index);
	void clear_mesh_rids(const int mesh_index, const int mesh_type_index);
	Array get_meshes(const int mesh_index, const int mesh_type_index);
	void set_meshes(const int mesh_index, const int mesh_type_index, const Array &meshes);
	bool has_meshes(const int mesh_index, const int mesh_type_index);

	void free_rids();
	void free_index(const int mesh_index);

	void create_meshes(const int mesh_index, const int mesh_count);
	void free_meshes(const int mesh_index);

	void create_colliders(const int mesh_index, const int layer_mask = 1);
	void create_colliders_area(const int mesh_index, const int layer_mask = 1);
	void free_colliders(const int mesh_index);

	//Transform
	void update_transforms();

	//Lights
	Ref<VoxelLight> get_light(const int index);
	int get_light_count() const;

	//Debug
	void create_debug_immediate_geometry();
	void free_debug_immediate_geometry();

	void draw_cross_voxels(Vector3 pos);
	void draw_cross_voxels_fill(Vector3 pos, float fill);
	void draw_debug_voxels(int max, Color color = Color(1, 1, 1));
	void draw_debug_voxel_lights();

	//Visibility
	void visibility_changed(bool visible);

	//free
	void free_chunk();

	//etc
	void emit_build_finished();

	void generate_random_ao(const int seed, const int octaves = 4, const int period = 30, const float persistence = 0.3, const float scale_factor = 0.6);

	VoxelChunkDefault();
	~VoxelChunkDefault();

protected:
	virtual void _setup_channels();
	virtual void _build_phase(int phase);
	virtual void _build_phase_process(int phase);
	virtual void _build_phase_physics_process(int phase);

	virtual void _create_meshers();
	virtual void _build(bool immediate);
	virtual void _visibility_changed(bool visible);

	virtual void _exit_tree();
	virtual void _process(float delta);
	virtual void _physics_process(float delta);
	virtual void _world_transform_changed();

	//Props
	virtual void _add_prop(Ref<VoxelChunkPropData> prop);

	//lights
	virtual void _bake_lights();
	virtual void _bake_light(Ref<VoxelLight> light);
	virtual void _clear_baked_lights();
	virtual void _world_light_added(const Ref<VoxelLight> &light);
	virtual void _world_light_removed(const Ref<VoxelLight> &light);

	void wait_and_finish_thread();

	static void _bind_methods();

	int _build_flags;

	bool _abort_build;
	bool _queued_generation;

	int _current_build_phase;
	int _max_build_phases;
	bool _enabled;

	bool _lights_dirty;

	int _lod_size;

	//lod
	int _lod_num;
	int _current_lod_level;

	//Meshes
	Dictionary _rids;

	//debug
	ImmediateGeometry *_debug_drawer;

	bool _build_prioritized;
	bool _build_phase_done;
	Thread *_build_thread;
	bool _build_step_in_progress;

	Array temp_array;
	PoolVector<Vector3> temp_arr_collider;
	PoolVector<Vector3> temp_arr_collider_liquid;
	PoolVector<Vector3> temp_arr_collider_prop;

	ActiveBuildPhaseType _active_build_phase_type;

	Vector<Ref<VoxelLight> > _lights;
};

VARIANT_ENUM_CAST(VoxelChunkDefault::DefaultChannels);
VARIANT_ENUM_CAST(VoxelChunkDefault::ActiveBuildPhaseType);
VARIANT_ENUM_CAST(VoxelChunkDefault::BuildFlags);

#endif
