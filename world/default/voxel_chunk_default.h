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

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/string/ustring.h"
#include "core/config/engine.h"
#include "core/variant/array.h"
#else
#include "core/ustring.h"
#include "core/engine.h"
#include "core/array.h"
#endif


#include "../voxel_chunk.h"

#include "../../defines.h"

#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "core/os/thread_safe.h"

#include "scene/resources/packed_scene.h"
#include "../voxel_world.h"
#include "../../data/voxel_light.h"
#include "../../meshers/voxel_mesher.h"
#include "../../library/voxel_surface.h"
#include "../../library/voxel_library.h"

class VoxelWorld;
class VoxelJob;

class VoxelChunkDefault : public VoxelChunk {
	GDCLASS(VoxelChunkDefault, VoxelChunk);

	_THREAD_SAFE_CLASS_

public:
	static const String BINDING_STRING_BUILD_FLAGS;

	enum DefaultChannels {
		DEFAULT_CHANNEL_TYPE = 0,
		DEFAULT_CHANNEL_ISOLEVEL,
		DEFAULT_CHANNEL_ALT_TYPE,
		DEFAULT_CHANNEL_ALT_ISOLEVEL,
		DEFAULT_CHANNEL_LIGHT_COLOR_R,
		DEFAULT_CHANNEL_LIGHT_COLOR_G,
		DEFAULT_CHANNEL_LIGHT_COLOR_B,
		DEFAULT_CHANNEL_AO,
		DEFAULT_CHANNEL_RANDOM_AO,
		DEFAULT_CHANNEL_LIQUID_FLOW,
		MAX_DEFAULT_CHANNELS
	};

	enum {
		MESH_INDEX_TERRAIN = 0,
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

	//add dirty flags
	bool get_lights_dirty() const;
	void set_lights_dirty(const bool value);

	//Lod
	int get_lod_num() const;
	void set_lod_num(const int value);

	int get_current_lod_level() const;
	void set_current_lod_level(const int value);

	//Meshes
	Dictionary mesh_rids_get();
	void mesh_rids_set(const Dictionary &rids);

	RID mesh_rid_get(const int mesh_index, const int mesh_type_index);
	void mesh_rid_set(const int mesh_index, const int mesh_type_index, RID value);
	RID mesh_rid_get_index(const int mesh_index, const int mesh_type_index, const int index);
	void mesh_rid_set_index(const int mesh_index, const int mesh_type_index, const int index, RID value);
	int mesh_rid_get_count(const int mesh_index, const int mesh_type_index);
	void mesh_rids_clear(const int mesh_index, const int mesh_type_index);

	Array meshes_get(const int mesh_index, const int mesh_type_index);
	void meshes_set(const int mesh_index, const int mesh_type_index, const Array &meshes);
	bool meshes_has(const int mesh_index, const int mesh_type_index);

	void rids_clear();

	void rids_free();

	void meshes_create(const int mesh_index, const int mesh_count);
	void meshes_free(const int mesh_index);

	void colliders_create(const int mesh_index, const int layer_mask = 1);
	void colliders_create_area(const int mesh_index, const int layer_mask = 1);
	void colliders_free(const int mesh_index);

	void free_index(const int mesh_index);

	//Transform
	void update_transforms();

	//Lights
	Ref<VoxelLight> get_light(const int index);
	int get_light_count() const;

	//Debug
	void debug_mesh_allocate();
	void debug_mesh_free();
	bool debug_mesh_has();
	void debug_mesh_clear();
	void debug_mesh_array_clear();
	void debug_mesh_add_vertices_to(const PoolVector3Array &arr);
	void debug_mesh_send();

	void draw_cross_voxels(Vector3 pos);
	void draw_cross_voxels_fill(Vector3 pos, float fill);
	void draw_debug_voxels(int max, Color color = Color(1, 1, 1));
	void draw_debug_voxel_lights();
	void draw_debug_mdr_colliders();

	//Visibility
	void visibility_changed(bool visible);

	//free
	void free_chunk();

	//etc
	void emit_build_finished();

	void generate_random_ao(const int seed, const int octaves = 4, const int period = 30, const float persistence = 0.3, const float scale_factor = 0.6);

	bool get_build_step_in_progress() const;
	void set_build_step_in_progress(const bool value);

	void _finalize_build();

	VoxelChunkDefault();
	~VoxelChunkDefault();

protected:
	virtual void _channel_setup();

	virtual void _visibility_changed(bool visible);

	virtual void _exit_tree();
	virtual void _world_transform_changed();

	//lights
	virtual void _bake_lights();
	virtual void _bake_light(Ref<VoxelLight> light);
	virtual void _clear_baked_lights();
	virtual void _world_light_added(const Ref<VoxelLight> &light);
	virtual void _world_light_removed(const Ref<VoxelLight> &light);

	static void _bind_methods();

	int _build_flags;

	bool _enabled;

	bool _lights_dirty;

	//lod
	int _lod_num;
	int _current_lod_level;

	//Meshes
	Dictionary _rids;

	//debug
	RID _debug_mesh_rid;
	RID _debug_mesh_instance;
	PoolVector3Array _debug_mesh_array;

	Vector<Ref<VoxelLight> > _lights;
};

VARIANT_ENUM_CAST(VoxelChunkDefault::DefaultChannels);
VARIANT_ENUM_CAST(VoxelChunkDefault::BuildFlags);

#endif
