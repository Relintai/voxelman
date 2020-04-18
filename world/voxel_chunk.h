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

#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H

#include "core/resource.h"

#include "core/version.h"

#if VERSION_MAJOR < 4
#include "core/pool_vector.h"
#else
#include "core/vector.h"
#endif

#include "core/engine.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "core/os/thread_safe.h"
#include "core/ustring.h"

#include "core/array.h"

#include "scene/resources/packed_scene.h"

#include "voxel_world.h"

#include "../data/voxel_light.h"

#include "../meshers/cubic_mesher/voxel_mesher_cubic.h"
#include "../meshers/voxel_mesher.h"

#include "../library/voxel_surface.h"
#include "../library/voxelman_library.h"

#include "voxel_chunk_prop_data.h"

#include "core/version.h"

#if VERSION_MAJOR >= 4
#define Texture Texture2D
#endif

class VoxelWorld;

class VoxelChunk : public Resource {
	GDCLASS(VoxelChunk, Resource);

public:
	enum {
		VOXEL_CHUNK_STATE_OK = 0,
	};

public:
	bool get_is_build_threaded() const;
	void set_is_build_threaded(const bool value);

	bool get_process() const;
	void set_process(const bool value);

	bool get_physics_process() const;
	void set_physics_process(const bool value);

	bool get_visible() const;
	void set_visible(const bool value);

	bool get_is_generating() const;
	void set_is_generating(const bool value);

	bool get_dirty() const;
	void set_dirty(const bool value);

	int get_state() const;
	void set_state(int value);

	int get_position_x() const;
	void set_position_x(const int value);
	int get_position_y() const;
	void set_position_y(const int value);
	int get_position_z() const;
	void set_position_z(const int value);

	int get_size_x() const;
	int get_size_y() const;
	int get_size_z() const;
	void set_size_x(const int value);
	void set_size_y(const int value);
	void set_size_z(const int value);

	int get_data_size_x() const;
	int get_data_size_y() const;
	int get_data_size_z() const;
	void set_data_size_x(const int value);
	void set_data_size_y(const int value);
	void set_data_size_z(const int value);

	Vector3 get_position() const;
	Vector3 get_size() const;
	void set_position(const int x, const int y, const int z);

	int get_margin_start() const;
	int get_margin_end() const;
	void set_margin_start(const int value);
	void set_margin_end(const int value);

	Ref<VoxelmanLibrary> get_library();
	void set_library(const Ref<VoxelmanLibrary> &value);

	float get_voxel_scale() const;
	void set_voxel_scale(const float value);

	VoxelWorld *get_voxel_world() const;
	void set_voxel_world(VoxelWorld *world);
	void set_voxel_world_bind(Node *world);

	//Meshers
	Ref<VoxelMesher> get_mesher(const int index) const;
	void set_mesher(const int index, const Ref<VoxelMesher> &mesher);
	void remove_mesher(const int index);
	void add_mesher(const Ref<VoxelMesher> &mesher);
	int get_mesher_count() const;

	//Channels
	void setup_channels();

	void set_size(const int size_x, const int size_y, const int size_z, const int margin_start = 0, const int margin_end = 0);

	bool validate_channel_data_position(const int x, const int y, const int z) const;

	uint8_t get_voxel(const int p_x, const int p_y, const int p_z, const int p_channel_index) const;
	void set_voxel(const uint8_t p_value, const int p_x, const int p_y, const int p_z, const int p_channel_index);

	int get_channel_count() const;
	void set_channel_count(const int count);

	void allocate_channel(const int channel_index, const uint8_t default_value = 0);
	void fill_channel(const uint8_t value, const int channel_index);
	void dealloc_channel(const int channel_index);

	uint8_t *get_channel(const int channel_index);
	uint8_t *get_valid_channel(const int channel_index, const uint8_t default_value = 0);

	PoolByteArray get_channel_array(const int channel_index) const;
	void set_channel_array(const int channel_index, const PoolByteArray &array);

	PoolByteArray get_channel_compressed(const int channel_index) const;
	void set_channel_compressed(const int channel_index, const PoolByteArray &data);

	int get_index(const int x, const int y, const int z) const;
	int get_data_index(const int x, const int y, const int z) const;
	int get_data_size() const;

	//Meshing
	void create_meshers();
	void build(const bool immediate = false);
	void clear();

	Array merge_mesh_array(Array arr) const;
	Array bake_mesh_array_uv(Array arr, Ref<Texture> tex, float mul_color = 0.7) const;

	//light Baking
	void bake_lights();
	void bake_light(Ref<VoxelLight> light);
	void clear_baked_lights();

	//props
	void add_prop(Ref<VoxelChunkPropData> prop);
	Ref<VoxelChunkPropData> get_prop(const int index);
	int get_prop_count() const;
	void remove_prop(const int index);
	void clear_props();

	//handlers
	void enter_tree();
	void exit_tree();
	void process(const float delta);
	void physics_process(const float delta);
	void world_transform_changed();
	void visibility_changed(const bool visible);
	void world_light_added(const Ref<VoxelLight> &light);
	void world_light_removed(const Ref<VoxelLight> &light);

	Transform get_transform() const;
	void set_transform(const Transform &transform);

	VoxelChunk();
	~VoxelChunk();

protected:
	virtual void _world_transform_changed();

	/*
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	*/
	static void _bind_methods();

	bool _is_build_threaded;

	bool _is_processing;
	bool _is_phisics_processing;

	bool _is_visible;

	bool _is_generating;
	bool _dirty;
	int _state;

	VoxelWorld *_voxel_world;

	int _position_x;
	int _position_y;
	int _position_z;

	int _size_x;
	int _size_y;
	int _size_z;

	int _data_size_x;
	int _data_size_y;
	int _data_size_z;

	int _margin_start;
	int _margin_end;

	Vector<uint8_t *> _channels;

	float _voxel_scale;

	Ref<VoxelmanLibrary> _library;
	Vector<Ref<VoxelMesher> > _meshers;

	//mergeable props
	Vector<Ref<VoxelChunkPropData> > _props;

	Transform _transform;
};

#endif
