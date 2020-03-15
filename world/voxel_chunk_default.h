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

#include "voxel_chunk.h"

#include "scene/3d/spatial.h"

#include "core/engine.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "core/os/thread_safe.h"
#include "core/ustring.h"

#include "core/array.h"
#include "core/pool_vector.h"
#include "scene/3d/collision_shape.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/physics_body.h"
#include "scene/3d/spatial.h"
#include "scene/resources/concave_polygon_shape.h"
#include "scene/resources/packed_scene.h"

#include "voxel_world.h"

#include "../data/voxel_light.h"

#include "../meshers/cubic_mesher/voxel_mesher_cubic.h"
#include "../meshers/voxel_mesher.h"

#include "../library/voxel_surface.h"
#include "../library/voxelman_library.h"

#include "../../mesh_data_resource/mesh_data_resource.h"
#include "../props/prop_data.h"
#include "../props/prop_data_entry.h"
#include "../props/prop_data_light.h"
#include "../props/prop_data_mesh.h"
#include "../props/prop_data_scene.h"
#include "voxel_chunk_prop_data.h"

class VoxelWorld;

class VoxelChunkDefault : public VoxelChunk {
	GDCLASS(VoxelChunkDefault, VoxelChunk);

	_THREAD_SAFE_CLASS_

public:
	static const String BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE;

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
		BUILD_PHASE_TERRARIN_MESH_COLLIDER,
		BUILD_PHASE_LIGHTS,
		BUILD_PHASE_TERRARIN_MESH,
		BUILD_PHASE_PROP_MESH,
		BUILD_PHASE_PROP_COLLIDER,
		//BUILD_PHASE_LIQUID,
		//BUILD_PHASE_CLUTTER,
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
		DEFAULT_CHANNEL_LIQUID_TYPES,
		DEFAULT_CHANNEL_LIQUID_FILL,
		DEFAULT_CHANNEL_LIQUID_FLOW,
		MAX_DEFAULT_CHANNELS
	};

	enum ActiveBuildPhaseType {
		BUILD_PHASE_TYPE_NORMAL = 0,
		BUILD_PHASE_TYPE_PROCESS,
		BUILD_PHASE_TYPE_PHYSICS_PROCESS,
	};

public:
	bool get_is_build_threaded() const;
	void set_is_build_threaded(bool value);

	ActiveBuildPhaseType get_active_build_phase_type() const;
	void set_active_build_phase_type(const ActiveBuildPhaseType value);

	bool get_build_phase_done() const;
	void set_build_phase_done(bool value);

	int get_lod_size() const;
	void set_lod_size(int lod_size);

	int get_current_build_phase();
	void set_current_build_phase(int value);

	int get_max_build_phase();
	void set_max_build_phase(int value);

	bool get_create_collider() const;
	void set_create_collider(bool value);

	bool get_bake_lights() const;
	void set_bake_lights(bool value);

	RID get_mesh_rid();
	RID get_mesh_instance_rid();
	RID get_shape_rid();
	RID get_body_rid();

	RID get_prop_mesh_rid();
	RID get_prop_mesh_instance_rid();
	RID get_prop_shape_rid();
	RID get_prop_body_rid();

	RID get_liquid_mesh_rid();
	RID get_liquid_mesh_instance_rid();

	RID get_clutter_mesh_rid();
	RID get_clutter_mesh_instance_rid();

	//Data Management functions
	void generate_ao();

	//Meshing
	void build_deferred();
	void build_prioritized();
	static void _build_step_threaded(void *_userdata);

	void build_step();

	void build_phase();
	void build_phase_process();
	void build_phase_physics_process();

	bool has_next_phase();
	void next_phase();

	void clear();

	//Colliders
	void create_colliders();
	void remove_colliders();

	//Meshes
	void allocate_main_mesh();
	void free_main_mesh();

	void allocate_prop_mesh();
	void free_prop_mesh();

	void allocate_prop_colliders();
	void free_prop_colliders();

	void allocate_liquid_mesh();
	void free_liquid_mesh();

	void allocate_clutter_mesh();
	void free_clutter_mesh();

	void update_transforms();

	//Debug
	void create_debug_immediate_geometry();
	void free_debug_immediate_geometry();

	void draw_cross_voxels(Vector3 pos);
	void draw_cross_voxels_fill(Vector3 pos, float fill);
	void draw_debug_voxels(int max, Color color = Color(1, 1, 1));
	void draw_debug_voxel_lights();

	void set_visibility(bool visible);

	//free
	void free_chunk();

	VoxelChunkDefault();
	~VoxelChunkDefault();

protected:
	virtual void _setup_channels();
	virtual void _build_phase(int phase);
	virtual void _build_phase_process(int phase);
	virtual void _build_phase_physics_process(int phase);

	virtual void _add_light(int local_x, int local_y, int local_z, int size, Color color);
	virtual void _clear_baked_lights();
	virtual void _create_meshers();
	virtual void _build(bool immediate);

	void wait_and_finish_thread();

	void _notification(int p_what);

	static void _bind_methods();

	bool _is_build_threaded;
	bool _abort_build;

	int _current_build_phase;
	int _max_build_phases;
	bool _enabled;

	int _lod_size;

	//voxel mesh
	RID _mesh_rid;
	RID _mesh_instance_rid;

	RID _shape_rid;
	RID _body_rid;

	RID _prop_mesh_rid;
	RID _prop_mesh_instance_rid;

	RID _prop_shape_rid;
	RID _prop_body_rid;

	//liquids
	RID _liquid_mesh_rid;
	RID _liquid_mesh_instance_rid;

	//clutter
	RID _clutter_mesh_rid;
	RID _clutter_mesh_instance_rid;

	//debug
	ImmediateGeometry *_debug_drawer;

	bool _build_mesh;
	bool _create_collider;

	bool _bake_lights;

	bool _build_prioritized;
	Mutex *_build_phase_done_mutex;
	bool _build_phase_done;
	Thread *_build_thread;
	bool _build_step_in_progress;

	Array temp_array;
	PoolVector<Vector3> temp_arr_collider;

	ActiveBuildPhaseType _active_build_phase_type;
};

VARIANT_ENUM_CAST(VoxelChunkDefault::DefaultChannels);
VARIANT_ENUM_CAST(VoxelChunkDefault::ActiveBuildPhaseType);

#endif
