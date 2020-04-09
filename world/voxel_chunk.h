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

	_THREAD_SAFE_CLASS_

public:
	enum {
		VOXEL_CHUNK_STATE_OK = 0,
	};

public:
	bool get_process() const;
	void set_process(const bool value);

	bool get_physics_process() const;
	void set_physics_process(const bool value);

	bool get_visible() const;
	void set_visible(const bool value);

	bool get_is_generating() const;
	void set_is_generating(bool value);

	bool get_dirty() const;
	void set_dirty(bool value);

	int get_state() const;
	void set_state(int value);

	int get_position_x();
	void set_position_x(int value);
	int get_position_y();
	void set_position_y(int value);
	int get_position_z();
	void set_position_z(int value);

	int get_size_x();
	int get_size_y();
	int get_size_z();
	void set_size_x(int value);
	void set_size_y(int value);
	void set_size_z(int value);

	int get_data_size_x();
	int get_data_size_y();
	int get_data_size_z();
	void set_data_size_x(int value);
	void set_data_size_y(int value);
	void set_data_size_z(int value);

	Vector3 get_position() const;
	Vector3 get_size() const;
	void set_position(int x, int y, int z);

	int get_margin_start() const;
	int get_margin_end() const;
	void set_margin_start(int value);
	void set_margin_end(int value);

	Ref<VoxelmanLibrary> get_library();
	void set_library(const Ref<VoxelmanLibrary> &value);

	float get_voxel_scale() const;
	void set_voxel_scale(float value);

	VoxelWorld *get_voxel_world() const;
	void set_voxel_world(VoxelWorld *world);
	void set_voxel_world_bind(Node *world);

	//Meshers
	Ref<VoxelMesher> get_mesher(int index) const;
	void set_mesher(int index, Ref<VoxelMesher> mesher);
	void remove_mesher(int index);
	void add_mesher(Ref<VoxelMesher> mesher);
	int get_mesher_count();

	//Channels
	void setup_channels();

	void set_size(int size_x, int size_y, int size_z, int margin_start = 0, int margin_end = 0);

	bool validate_channel_data_position(int x, int y, int z) const;

	uint8_t get_voxel(int p_x, int p_y, int p_z, int p_channel_index) const;
	void set_voxel(uint8_t p_value, int p_x, int p_y, int p_z, int p_channel_index);

	int get_channel_count();
	void set_channel_count(int count);

	void allocate_channel(int channel_index, uint8_t default_value = 0);
	void fill_channel(uint8_t value, int channel_index);
	void dealloc_channel(int channel_index);

	uint8_t *get_channel(int channel_index);
	uint8_t *get_valid_channel(int channel_index, uint8_t default_value = 0);

	PoolByteArray get_channel_array(int channel_index) const;
	void set_channel_array(int channel_index, const PoolByteArray &array);

	PoolByteArray get_channel_compressed(int channel_index) const;
	void set_channel_compressed(int channel_index, const PoolByteArray &data);

	int get_index(int x, int y, int z) const;
	int get_data_index(int x, int y, int z) const;
	int get_data_size() const;

	//Meshing
	void create_meshers();
	void build(bool immediate = false);
	void clear();

	Array merge_mesh_array(Array arr) const;
	Array bake_mesh_array_uv(Array arr, Ref<Texture> tex, float mul_color = 0.7) const;

	//lights
	void add_lights(Array lights);
	void add_voxel_light(Ref<VoxelLight> light);
	void create_voxel_light(const Color color, const int size, const int x, const int y, const int z);
	void remove_voxel_light(Ref<VoxelLight> light);
	void clear_voxel_lights();

	void add_lights_into(Array target);
	void add_unique_lights_into(Array target);
	Array get_lights();

	void bake_lights();
	void bake_light(Ref<VoxelLight> light);
	void clear_baked_lights();

	void add_light(int local_x, int local_y, int local_z, int size, Color color);

	void add_prop_light(Ref<VoxelLight> light);

	//props
	void add_prop(Ref<VoxelChunkPropData> prop);
	Ref<VoxelChunkPropData> get_prop(int index);
	int get_prop_count();
	void remove_prop(int index);
	void clear_props();

	void free_spawn_props();

	void enter_tree();
	void exit_tree();
	void process(float delta);
	void physics_process(float delta);
	void world_transform_changed();
	void visibility_changed(bool visible);

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

	Vector<Ref<VoxelLight> > _voxel_lights;

	float _voxel_scale;

	Ref<VoxelmanLibrary> _library;
	Vector<Ref<VoxelMesher> > _meshers;

	//mergeable props
	Vector<Ref<VoxelChunkPropData> > _props;

	//spawned props
	Vector<Node *> _spawned_props;

	Transform _transform;
};

#endif
