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

#include "voxel_chunk.h"

#include "voxel_world.h"

_FORCE_INLINE_ bool VoxelChunk::get_is_generating() const {
	return _is_generating;
}
_FORCE_INLINE_ void VoxelChunk::set_is_generating(bool value) {
	_is_generating = value;
}

_FORCE_INLINE_ bool VoxelChunk::get_is_build_threaded() const {
	return _is_build_threaded;
}
_FORCE_INLINE_ void VoxelChunk::set_is_build_threaded(bool value) {
	_is_build_threaded = value;
}

bool VoxelChunk::get_build_phase_done() const {
	_build_phase_done_mutex->lock();
	bool v = _build_phase_done_mutex;
	_build_phase_done_mutex->unlock();

	return v;
}
void VoxelChunk::set_build_phase_done(bool value) {
	_build_phase_done_mutex->lock();
	_build_phase_done = value;
	_build_phase_done_mutex->unlock();
}

_FORCE_INLINE_ bool VoxelChunk::get_dirty() const {
	return _dirty;
}
_FORCE_INLINE_ void VoxelChunk::set_dirty(bool value) {
	_dirty = value;
}

_FORCE_INLINE_ int VoxelChunk::get_state() const {
	return _state;
}
_FORCE_INLINE_ void VoxelChunk::set_state(int value) {
	_state = value;
}

_FORCE_INLINE_ int VoxelChunk::get_position_x() {
	return _position_x;
}
void VoxelChunk::set_position_x(int value) {
	_position_x = value;
}
_FORCE_INLINE_ int VoxelChunk::get_position_y() {
	return _position_y;
}
void VoxelChunk::set_position_y(int value) {
	_position_y = value;
}
_FORCE_INLINE_ int VoxelChunk::get_position_z() {
	return _position_z;
}
void VoxelChunk::set_position_z(int value) {
	_position_z = value;
}

_FORCE_INLINE_ Vector3 VoxelChunk::get_position() const {
	return Vector3(_position_x, _position_y, _position_z);
}

_FORCE_INLINE_ int VoxelChunk::get_size_x() {
	return _size_x;
}
_FORCE_INLINE_ int VoxelChunk::get_size_y() {
	return _size_y;
}
_FORCE_INLINE_ int VoxelChunk::get_size_z() {
	return _size_z;
}

_FORCE_INLINE_ void VoxelChunk::set_size_x(int value) {
	_size_x = value;
}
_FORCE_INLINE_ void VoxelChunk::set_size_y(int value) {
	_size_y = value;
}
_FORCE_INLINE_ void VoxelChunk::set_size_z(int value) {
	_size_z = value;
}

_FORCE_INLINE_ Vector3 VoxelChunk::get_size() const {
	return Vector3(_size_x, _size_y, _size_z);
}

_FORCE_INLINE_ int VoxelChunk::get_data_size_x() {
	return _data_size_x;
}
_FORCE_INLINE_ int VoxelChunk::get_data_size_y() {
	return _data_size_y;
}
_FORCE_INLINE_ int VoxelChunk::get_data_size_z() {
	return _data_size_z;
}

_FORCE_INLINE_ void VoxelChunk::set_data_size_x(int value) {
	_data_size_x = value;
}
_FORCE_INLINE_ void VoxelChunk::set_data_size_y(int value) {
	_data_size_y = value;
}
_FORCE_INLINE_ void VoxelChunk::set_data_size_z(int value) {
	_data_size_z = value;
}

void VoxelChunk::set_position(int x, int y, int z) {
	_position_x = x;
	_position_y = y;
	_position_z = z;
}

_FORCE_INLINE_ int VoxelChunk::get_margin_start() const {
	return _margin_start;
}
_FORCE_INLINE_ int VoxelChunk::get_margin_end() const {
	return _margin_end;
}

_FORCE_INLINE_ void VoxelChunk::set_margin_start(int value) {
	_margin_start = value;
}
_FORCE_INLINE_ void VoxelChunk::set_margin_end(int value) {
	_margin_end = value;
}

Ref<VoxelmanLibrary> VoxelChunk::get_library() {
	return _library;
}
void VoxelChunk::set_library(Ref<VoxelmanLibrary> value) {
	_library = value;
}

int VoxelChunk::get_lod_size() const {
	return _lod_size;
}
void VoxelChunk::set_lod_size(const int lod_size) {
	_lod_size = lod_size;

	for (int i = 0; i < _meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_lod_size(_lod_size);
	}
}

float VoxelChunk::get_voxel_scale() const {
	return _voxel_scale;
}
void VoxelChunk::set_voxel_scale(float value) {
	_voxel_scale = value;

	for (int i = 0; i < _meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_voxel_scale(_voxel_scale);
	}
}

int VoxelChunk::get_current_build_phase() {
	return _current_build_phase;
}
void VoxelChunk::set_current_build_phase(int value) {
	_current_build_phase = value;
}

int VoxelChunk::get_max_build_phase() {
	return _max_build_phases;
}
void VoxelChunk::set_max_build_phase(int value) {
	_max_build_phases = value;
}

Ref<VoxelMesher> VoxelChunk::get_mesher(int index) const {
	ERR_FAIL_INDEX_V(index, _meshers.size(), Ref<VoxelMesher>());

	return _meshers.get(index);
}
void VoxelChunk::set_mesher(int index, Ref<VoxelMesher> mesher) {
	ERR_FAIL_INDEX(index, _meshers.size());

	_meshers.set(index, mesher);
}
void VoxelChunk::remove_mesher(int index) {
	ERR_FAIL_INDEX(index, _meshers.size());

	_meshers.remove(index);
}
void VoxelChunk::add_mesher(Ref<VoxelMesher> mesher) {
	_meshers.push_back(mesher);
}
int VoxelChunk::get_mesher_count() {
	return _meshers.size();
}

VoxelWorld *VoxelChunk::get_voxel_world() const {
	return _voxel_world;
}
void VoxelChunk::set_voxel_world(VoxelWorld *world) {
	_voxel_world = world;
}
void VoxelChunk::set_voxel_world_bind(Node *world) {
	if (world == NULL) {
		_voxel_world = NULL;
		return;
	}

	_voxel_world = Object::cast_to<VoxelWorld>(world);
}

bool VoxelChunk::get_create_collider() const {
	return _create_collider;
}
void VoxelChunk::set_create_collider(bool value) {
	_create_collider = value;
}

bool VoxelChunk::get_bake_lights() const {
	return _bake_lights;
}
void VoxelChunk::set_bake_lights(bool value) {
	_bake_lights = value;
}

RID VoxelChunk::get_mesh_rid() {
	return _mesh_rid;
}
RID VoxelChunk::get_mesh_instance_rid() {
	return _mesh_instance_rid;
}
RID VoxelChunk::get_shape_rid() {
	return _shape_rid;
}
RID VoxelChunk::get_body_rid() {
	return _body_rid;
}

RID VoxelChunk::get_prop_mesh_rid() {
	return _prop_mesh_rid;
}
RID VoxelChunk::get_prop_mesh_instance_rid() {
	return _prop_mesh_instance_rid;
}
RID VoxelChunk::get_prop_shape_rid() {
	return _prop_shape_rid;
}
RID VoxelChunk::get_prop_body_rid() {
	return _prop_body_rid;
}

RID VoxelChunk::get_liquid_mesh_rid() {
	return _liquid_mesh_rid;
}
RID VoxelChunk::get_liquid_mesh_instance_rid() {
	return _liquid_mesh_instance_rid;
}

RID VoxelChunk::get_clutter_mesh_rid() {
	return _clutter_mesh_rid;
}
RID VoxelChunk::get_clutter_mesh_instance_rid() {
	return _clutter_mesh_instance_rid;
}

//Voxel Data
void VoxelChunk::setup_channels() {
	call("_setup_channels");
}
void VoxelChunk::_setup_channels() {
	set_channel_count(MAX_DEFAULT_CHANNELS);
}

void VoxelChunk::set_size(uint32_t size_x, uint32_t size_y, uint32_t size_z, uint32_t margin_start, uint32_t margin_end) {
	if (_size_x == size_x && _size_y == size_y && _size_z == size_z && _margin_start == margin_start && _margin_end == margin_end) {
		return;
	}

	for (int i = 0; i < _channels.size(); ++i) {
		uint8_t *ch = _channels[i];

		if (ch != NULL) {
			memdelete_arr(ch);
		}
	}

	setup_channels();

	_size_x = size_x;
	_size_y = size_y;
	_size_z = size_z;

	_data_size_x = size_x + margin_start + margin_end;
	_data_size_y = size_y + margin_start + margin_end;
	_data_size_z = size_z + margin_start + margin_end;

	_margin_start = margin_start;
	_margin_end = margin_end;
}

bool VoxelChunk::validate_channel_data_position(uint32_t x, uint32_t y, uint32_t z) const {
	return x < _data_size_x && y < _data_size_y && z < _data_size_z;
}

uint8_t VoxelChunk::get_voxel(int p_x, int p_y, int p_z, int p_channel_index) const {
	int x = p_x + _margin_start;
	int y = p_y + _margin_start;
	int z = p_z + _margin_start;

	ERR_FAIL_INDEX_V(p_channel_index, _channels.size(), 0);
	ERR_FAIL_COND_V_MSG(!validate_channel_data_position(x, y, z), 0, "Error, index out of range! " + String::num(x) + " " + String::num(y) + " " + String::num(z));

	uint8_t *ch = _channels.get(p_channel_index);

	if (!ch)
		return 0;

	return ch[get_data_index(x, y, z)];
}
void VoxelChunk::set_voxel(uint8_t p_value, int p_x, int p_y, int p_z, int p_channel_index) {
	int x = p_x + _margin_start;
	int y = p_y + _margin_start;
	int z = p_z + _margin_start;

	ERR_FAIL_INDEX(p_channel_index, _channels.size());
	ERR_FAIL_COND_MSG(!validate_channel_data_position(x, y, z), "Error, index out of range! " + String::num(x) + " " + String::num(y) + " " + String::num(z));

	uint8_t *ch = get_valid_channel(p_channel_index);

	ch[get_data_index(x, y, z)] = p_value;
}

int VoxelChunk::get_channel_count() {
	return _channels.size();
}

void VoxelChunk::set_channel_count(int count) {
	if (count == _channels.size())
		return;

	if (_channels.size() >= count) {
		for (int i = count; i < _channels.size(); ++i) {
			uint8_t *ch = _channels[i];

			if (ch != NULL) {
				memdelete_arr(ch);
			}
		}

		_channels.resize(count);
		return;
	}

	int s = _channels.size();
	_channels.resize(count);

	for (int i = s; i < count; ++i) {
		_channels.set(i, NULL);
	}
}
void VoxelChunk::allocate_channel(int channel_index, uint8_t default_value) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	if (_channels[channel_index] != NULL)
		return;

	uint32_t size = _data_size_x * _data_size_y * _data_size_z;

	uint8_t *ch = memnew_arr(uint8_t, size);
	memset(ch, default_value, size);

	_channels.set(channel_index, ch);
}
void VoxelChunk::fill_channel(uint8_t value, int channel_index) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		allocate_channel(channel_index, value);
		return;
	}

	uint32_t size = get_data_size();

	for (uint32_t i = 0; i < size; ++i) {
		ch[i] = value;
	}
}
void VoxelChunk::dealloc_channel(int channel_index) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	uint8_t *ch = _channels.get(channel_index);

	if (ch != NULL) {
		memdelete_arr(ch);

		_channels.set(channel_index, NULL);
	}
}

uint8_t *VoxelChunk::get_channel(int channel_index) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), NULL);

	return _channels.get(channel_index);
}
uint8_t *VoxelChunk::get_valid_channel(int channel_index, uint8_t default_value) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), 0);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		allocate_channel(channel_index, default_value);

		return _channels.get(channel_index);
	}

	return ch;
}

PoolByteArray VoxelChunk::get_channel_array(int channel_index) const {
	PoolByteArray arr;

	uint32_t size = _data_size_x * _data_size_y * _data_size_z;

	if (channel_index >= _channels.size())
		return arr;

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL)
		return arr;

	arr.resize(size);

	for (uint32_t i = 0; i < size; ++i) {
		arr.set(i, ch[i]);
	}

	return arr;
}
void VoxelChunk::set_channel_array(int channel_index, const PoolByteArray &array) {
	if (array.size() == 0)
		return;

	if (_channels.size() <= channel_index)
		set_channel_count(channel_index + 1);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		if (_channels[channel_index] != NULL)
			return;

		ch = memnew_arr(uint8_t, array.size());
		_channels.set(channel_index, ch);
	}

	for (int i = 0; i < array.size(); ++i) {
		ch[i] = array[i];
	}
}

_FORCE_INLINE_ uint32_t VoxelChunk::get_data_index(uint32_t x, uint32_t y, uint32_t z) const {
	return y + _data_size_y * (x + _data_size_x * z);
}

_FORCE_INLINE_ uint32_t VoxelChunk::get_data_size() const {
	return _data_size_x * _data_size_y * _data_size_z;
}

//Data Management functions
void VoxelChunk::generate_ao() {
	ERR_FAIL_COND(_data_size_x == 0 || _data_size_y == 0 || _data_size_z == 0);

	int size_x = get_size_x() + get_margin_end();
	int size_y = get_size_y() + get_margin_end();
	int size_z = get_size_z() + get_margin_end();

	for (int y = get_margin_start() - 1; y < size_y - 1; ++y) {
		for (int z = get_margin_start() - 1; z < size_z - 1; ++z) {
			for (int x = get_margin_start() - 1; x < size_x - 1; ++x) {
				int current = get_voxel(x, y, z, DEFAULT_CHANNEL_ISOLEVEL);

				int sum = get_voxel(x + 1, y, z, DEFAULT_CHANNEL_ISOLEVEL);
				sum += get_voxel(x - 1, y, z, DEFAULT_CHANNEL_ISOLEVEL);
				sum += get_voxel(x, y + 1, z, DEFAULT_CHANNEL_ISOLEVEL);
				sum += get_voxel(x, y - 1, z, DEFAULT_CHANNEL_ISOLEVEL);
				sum += get_voxel(x, y, z + 1, DEFAULT_CHANNEL_ISOLEVEL);
				sum += get_voxel(x, y, z - 1, DEFAULT_CHANNEL_ISOLEVEL);

				sum /= 6;

				sum -= current;

				if (sum < 0)
					sum = 0;

				set_voxel(sum, x, y, z, DEFAULT_CHANNEL_AO);
			}
		}
	}
}

void VoxelChunk::add_light(int local_x, int local_y, int local_z, int size, Color color) {
	ERR_FAIL_COND(size < 0);

	//float sizef = static_cast<float>(size);
	//float rf = (color.r / sizef);
	//float gf = (color.g / sizef);
	//float bf = (color.b / sizef);

	int64_t dsx = static_cast<int64_t>(_data_size_x);
	int64_t dsy = static_cast<int64_t>(_data_size_y);
	int64_t dsz = static_cast<int64_t>(_data_size_z);

	for (int y = local_y - size; y <= local_y + size; ++y) {
		if (y < 0 || y >= dsy)
			continue;

		for (int z = local_z - size; z <= local_z + size; ++z) {
			if (z < 0 || z >= dsz)
				continue;

			for (int x = local_x - size; x <= local_x + size; ++x) {
				if (x < 0 || x >= dsx)
					continue;

				int lx = x - local_x;
				int ly = y - local_y;
				int lz = z - local_z;

				float str = size - (((float)lx * lx + ly * ly + lz * lz));
				str /= size;

				if (str < 0)
					continue;

				int r = color.r * str * 255.0;
				int g = color.g * str * 255.0;
				int b = color.b * str * 255.0;

				r += get_voxel(x, y, z, DEFAULT_CHANNEL_LIGHT_COLOR_R);
				g += get_voxel(x, y, z, DEFAULT_CHANNEL_LIGHT_COLOR_G);
				b += get_voxel(x, y, z, DEFAULT_CHANNEL_LIGHT_COLOR_B);

				if (r > 255)
					r = 255;

				if (g > 255)
					g = 255;

				if (b > 255)
					b = 255;

				set_voxel(r, x, y, z, DEFAULT_CHANNEL_LIGHT_COLOR_R);
				set_voxel(g, x, y, z, DEFAULT_CHANNEL_LIGHT_COLOR_G);
				set_voxel(b, x, y, z, DEFAULT_CHANNEL_LIGHT_COLOR_B);
			}
		}
	}
}
void VoxelChunk::clear_baked_lights() {
	fill_channel(0, DEFAULT_CHANNEL_LIGHT_COLOR_R);
	fill_channel(0, DEFAULT_CHANNEL_LIGHT_COLOR_G);
	fill_channel(0, DEFAULT_CHANNEL_LIGHT_COLOR_B);
}

void VoxelChunk::create_meshers() {
	call("_create_meshers");

	for (int i = 0; i < _meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_lod_size(get_lod_size());
		mesher->set_voxel_scale(get_voxel_scale());
	}
}

void VoxelChunk::_create_meshers() {
	add_mesher(Ref<VoxelMesher>(memnew(VoxelMesherCubic())));
}

void VoxelChunk::build_deferred() {
	if (_current_build_phase == BUILD_PHASE_DONE) {
		_build_prioritized = false;

		set_process_internal(true);

		wait_and_finish_thread();

		_is_generating = true;

		next_phase();
	}
}

void VoxelChunk::build_prioritized() {
	if (_current_build_phase == BUILD_PHASE_DONE) {
		_build_prioritized = true;

		set_process_internal(true);

		wait_and_finish_thread();

		_is_generating = true;

		next_phase();
		_build_step();
	}
}

void VoxelChunk::_build_step() {
	ERR_FAIL_COND(!has_next_phase());

	while (has_next_phase() && build_phase())
		;

	//call the next non-threaded phase aswell
	if (has_next_phase())
		build_phase();
}

void VoxelChunk::_build_threaded(void *_userdata) {
	VoxelChunk *vc = (VoxelChunk *)_userdata;

	while (vc->has_next_phase() && vc->build_phase())
		;
}

bool VoxelChunk::build_phase() {

	_THREAD_SAFE_METHOD_

	set_build_phase_done(false);

	return call("_build_phase", _current_build_phase);
}

bool VoxelChunk::_build_phase(int phase) {
	ERR_FAIL_COND_V(!_library.is_valid(), true);

	switch (phase) {
		case BUILD_PHASE_DONE:
			return true;
		case BUILD_PHASE_SETUP: {
			if (_meshers.size() == 0) {
				create_meshers();
			}

			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->set_library(_library);
				mesher->reset();
			}

			next_phase();

			return true;
		}
		case BUILD_PHASE_TERRARIN_MESH_SETUP: {
			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->add_chunk(this);
			}

			next_phase();

			return true;
		}
		case BUILD_PHASE_TERRARIN_MESH_COLLIDER: {
			if (get_create_collider()) {
				if (_body_rid == RID()) {
					create_colliders();
				}

				for (int i = 0; i < _meshers.size(); ++i) {
					Ref<VoxelMesher> mesher = _meshers.get(i);

					ERR_CONTINUE(!mesher.is_valid());

					mesher->build_collider(_shape_rid);
				}
			}

			next_phase();

			return true;
		}
		case BUILD_PHASE_TERRARIN_MESH: {
			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->bake_colors(this);
			}

			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->set_library(_library);
			}

			if (_mesh_rid == RID()) {
				allocate_main_mesh();
			}

			Ref<VoxelMesher> mesher;
			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> m = _meshers.get(i);

				ERR_CONTINUE(!m.is_valid());

				if (!mesher.is_valid()) {
					mesher = m;
					mesher->set_material(get_library()->get_material());
					continue;
				}

				mesher->set_material(get_library()->get_material());
				mesher->add_mesher(m);
			}

			ERR_FAIL_COND_V(!mesher.is_valid(), false);
			ERR_FAIL_COND_V(_mesh_rid == RID(), false);

			VS::get_singleton()->mesh_clear(_mesh_rid);

			if (mesher->get_vertex_count() == 0) {
				next_phase();
				return true;
			}

			Array arr = mesher->build_mesh();

			VS::get_singleton()->mesh_add_surface_from_arrays(_mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, arr);

			if (_library->get_material().is_valid())
				VS::get_singleton()->mesh_surface_set_material(_mesh_rid, 0, _library->get_material()->get_rid());

			next_phase();

			return true;
		}
		case BUILD_PHASE_PROP_MESH: {
			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->reset();
			}

			if (_props.size() > 0) {
				if (_prop_mesh_rid == RID()) {
					allocate_prop_mesh();
				}

				for (int i = 0; i < _meshers.size(); ++i) {
					Ref<VoxelMesher> mesher = _meshers.get(i);

					ERR_CONTINUE(!mesher.is_valid());

					mesher->bake_colors(this);
					mesher->set_material(get_library()->get_material());

					ERR_FAIL_COND_V(_prop_mesh_rid == RID(), false);

					VS::get_singleton()->mesh_clear(_prop_mesh_rid);

					if (mesher->get_vertex_count() == 0) {
						next_phase();
						return true;
					}

					Array arr = mesher->build_mesh();

					VS::get_singleton()->mesh_add_surface_from_arrays(_prop_mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, arr);

					if (_library->get_material().is_valid())
						VS::get_singleton()->mesh_surface_set_material(_prop_mesh_rid, 0, _library->get_material()->get_rid());
				}
			}

			next_phase();

			return false;
		}
		case BUILD_PHASE_PROP_COLLIDER: {

			if (_props.size() > 0) {
				if (get_create_collider()) {
					build_prop_collider();
				}
			}

			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->reset();
			}

			next_phase();

			return true;
		}
		/*
		case BUILD_PHASE_LIQUID: {
			next_phase();
			return;
		}
		case BUILD_PHASE_CLUTTER: {
			next_phase();
			return;
		}
		*/
		case BUILD_PHASE_FINALIZE: {
			if (_mesh_instance_rid != RID())
				VS::get_singleton()->instance_set_visible(_mesh_instance_rid, is_visible());

			if (_prop_mesh_instance_rid != RID())
				VS::get_singleton()->instance_set_visible(_prop_mesh_instance_rid, is_visible());

			if (_liquid_mesh_instance_rid != RID())
				VS::get_singleton()->instance_set_visible(_liquid_mesh_instance_rid, is_visible());

			if (_clutter_mesh_instance_rid != RID())
				VS::get_singleton()->instance_set_visible(_clutter_mesh_instance_rid, is_visible());

			next_phase();

			return true;
		}
	}

	return true;
	//next_phase();
}

bool VoxelChunk::has_next_phase() {
	if (_current_build_phase == BUILD_PHASE_DONE)
		return false;

	return true;
}

void VoxelChunk::next_phase() {
	set_build_phase_done(true);

	if (_abort_build) {
		_current_build_phase = BUILD_PHASE_DONE;
		_is_generating = false;
		set_process_internal(false);

		return;
	}

	++_current_build_phase;

	if (_current_build_phase >= _max_build_phases) {
		_current_build_phase = BUILD_PHASE_DONE;
		_is_generating = false;
		set_process_internal(false);

		emit_signal("mesh_generation_finished", this);
	}
}

void VoxelChunk::clear() {
	_voxel_lights.clear();
}

void VoxelChunk::create_colliders() {
	ERR_FAIL_COND(_voxel_world == NULL);

	_shape_rid = PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_CONCAVE_POLYGON);
	_body_rid = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);

	PhysicsServer::get_singleton()->body_set_collision_layer(_body_rid, 1);
	PhysicsServer::get_singleton()->body_set_collision_mask(_body_rid, 1);

	PhysicsServer::get_singleton()->body_add_shape(_body_rid, _shape_rid);

	PhysicsServer::get_singleton()->body_set_state(_body_rid, PhysicsServer::BODY_STATE_TRANSFORM, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
	PhysicsServer::get_singleton()->body_set_space(_body_rid, get_voxel_world()->get_world()->get_space());
}

void VoxelChunk::remove_colliders() {
	if (_body_rid != RID()) {
		PhysicsServer::get_singleton()->free(_body_rid);
		PhysicsServer::get_singleton()->free(_shape_rid);

		_body_rid = RID();
		_shape_rid = RID();
	}
}

void VoxelChunk::add_lights(Array lights) {
	for (int i = 0; i < lights.size(); ++i) {
		Ref<VoxelLight> light = Ref<VoxelLight>(lights.get(i));

		if (light.is_valid()) {
			add_voxel_light(light);
		}
	}
}
void VoxelChunk::add_voxel_light(Ref<VoxelLight> light) {
	_voxel_lights.push_back(light);
}

void VoxelChunk::create_voxel_light(const Color color, const int size, const int x, const int y, const int z) {
	Ref<VoxelLight> light;
	light.instance();

	light->set_world_position(_position_x * _size_x + x, _position_y * _size_y + y, _position_z * _size_z + z);
	light->set_color(color);
	light->set_size(size);

	add_voxel_light(light);
}

void VoxelChunk::remove_voxel_light(Ref<VoxelLight> light) {
	for (int i = 0; i < _voxel_lights.size(); ++i) {
		if (_voxel_lights[i] == light) {
			_voxel_lights.remove(i);
			return;
		}
	}
}
void VoxelChunk::clear_voxel_lights() {
	_voxel_lights.clear();
}

void VoxelChunk::add_lights_into(Array target) {
	for (int i = 0; i < _voxel_lights.size(); ++i) {
		target.append(_voxel_lights[i]);
	}
}

void VoxelChunk::add_unique_lights_into(Array target) {
	for (int i = 0; i < _voxel_lights.size(); ++i) {
		Ref<VoxelLight> l = _voxel_lights.get(i);

		bool append = true;
		for (int j = 0; j < target.size(); ++j) {
			Ref<VoxelLight> l2 = target.get(j);

			if (!l2.is_valid())
				continue;

			if (l2->get_world_position() == l->get_world_position() && l2->get_size() == l->get_size()) {
				append = false;
				break;
			}
		}

		if (append)
			target.append(l);
	}
}

Array VoxelChunk::get_lights() {
	Array target;

	for (int i = 0; i < _voxel_lights.size(); ++i) {
		target.append(_voxel_lights[i]);
	}

	return target;
}

void VoxelChunk::bake_lights() {
	clear_baked_lights();

	for (int i = 0; i < _voxel_lights.size(); ++i) {
		bake_light(_voxel_lights[i]);
	}
}
void VoxelChunk::bake_light(Ref<VoxelLight> light) {
	ERR_FAIL_COND(!light.is_valid());

	int wpx = light->get_world_position_x() - (_position_x * _size_x);
	int wpy = light->get_world_position_y() - (_position_y * _size_y);
	int wpz = light->get_world_position_z() - (_position_z * _size_z);

	//int wpx = (int)(wp.x / _size.x) - _position.x;
	//int wpy = (int)(wp.y / _size.y) - _position.y;
	//int wpz = (int)(wp.z / _size.z) - _position.z;

	add_light(wpx, wpy, wpz, light->get_size(), light->get_color());
}

void VoxelChunk::add_prop_light(Ref<VoxelLight> light) {
	bake_light(light);
}

void VoxelChunk::add_prop(Ref<VoxelChunkPropData> prop) {
	_props.push_back(prop);

	if (has_method("_prop_added"))
		call("_prop_added", prop);
}
Ref<VoxelChunkPropData> VoxelChunk::get_prop(int index) {
	return _props.get(index);
}
int VoxelChunk::get_prop_count() {
	return _props.size();
}
void VoxelChunk::remove_prop(int index) {
	return _props.remove(index);
}
void VoxelChunk::clear_props() {
	_props.clear();
}

void VoxelChunk::allocate_main_mesh() {
	ERR_FAIL_COND(_voxel_world == NULL);

	ERR_FAIL_COND(!get_library().is_valid());

	_mesh_instance_rid = VS::get_singleton()->instance_create();

	if (get_voxel_world()->get_world().is_valid())
		VS::get_singleton()->instance_set_scenario(_mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

	_mesh_rid = VS::get_singleton()->mesh_create();

	VS::get_singleton()->instance_set_base(_mesh_instance_rid, _mesh_rid);

	VS::get_singleton()->instance_set_transform(_mesh_instance_rid, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
}

void VoxelChunk::free_main_mesh() {
	if (_mesh_instance_rid != RID()) {
		VS::get_singleton()->free(_mesh_instance_rid);
		VS::get_singleton()->free(_mesh_rid);

		_mesh_instance_rid = RID();
		_mesh_rid = RID();
	}
}

void VoxelChunk::allocate_prop_mesh() {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(!get_library().is_valid());

	_prop_mesh_instance_rid = VS::get_singleton()->instance_create();

	if (get_voxel_world()->get_world().is_valid())
		VS::get_singleton()->instance_set_scenario(_prop_mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

	_prop_mesh_rid = VS::get_singleton()->mesh_create();

	VS::get_singleton()->instance_set_base(_prop_mesh_instance_rid, _prop_mesh_rid);

	VS::get_singleton()->instance_set_transform(_prop_mesh_instance_rid, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
}

void VoxelChunk::free_prop_mesh() {
	if (_prop_mesh_instance_rid != RID()) {
		VS::get_singleton()->free(_prop_mesh_instance_rid);
		VS::get_singleton()->free(_prop_mesh_rid);

		_prop_mesh_instance_rid = RID();
		_prop_mesh_rid = RID();
	}
}

void VoxelChunk::allocate_prop_colliders() {
	ERR_FAIL_COND(_voxel_world == NULL);

	_prop_shape_rid = PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_CONCAVE_POLYGON);
	_prop_body_rid = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);

	PhysicsServer::get_singleton()->body_set_collision_layer(_prop_body_rid, 1);
	PhysicsServer::get_singleton()->body_set_collision_mask(_prop_body_rid, 1);

	PhysicsServer::get_singleton()->body_add_shape(_prop_body_rid, _prop_shape_rid);

	PhysicsServer::get_singleton()->body_set_state(_prop_body_rid, PhysicsServer::BODY_STATE_TRANSFORM, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
	PhysicsServer::get_singleton()->body_set_space(_prop_body_rid, get_voxel_world()->get_world()->get_space());
}
void VoxelChunk::build_prop_collider() {
	if (_prop_shape_rid == RID()) {
		allocate_prop_colliders();
	}

	for (int i = 0; i < _meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->build_collider(_prop_shape_rid);
	}
}
void VoxelChunk::free_prop_colliders() {
	if (_prop_body_rid != RID()) {
		PhysicsServer::get_singleton()->free(_prop_body_rid);
		PhysicsServer::get_singleton()->free(_prop_shape_rid);

		_prop_body_rid = RID();
		_prop_shape_rid = RID();
	}
}

//Liquid mesh
void VoxelChunk::allocate_liquid_mesh() {
	ERR_FAIL_COND(_voxel_world == NULL);

	ERR_FAIL_COND(!get_library().is_valid());

	_liquid_mesh_instance_rid = VS::get_singleton()->instance_create();

	if (get_voxel_world()->get_world().is_valid())
		VS::get_singleton()->instance_set_scenario(_liquid_mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

	_liquid_mesh_rid = VS::get_singleton()->mesh_create();

	VS::get_singleton()->instance_set_base(_liquid_mesh_instance_rid, _liquid_mesh_rid);

	VS::get_singleton()->instance_set_transform(_liquid_mesh_instance_rid, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
}

void VoxelChunk::free_liquid_mesh() {
	if (_liquid_mesh_instance_rid != RID()) {
		VS::get_singleton()->free(_liquid_mesh_instance_rid);
		VS::get_singleton()->free(_liquid_mesh_rid);

		_liquid_mesh_instance_rid = RID();
		_liquid_mesh_rid = RID();
	}
}

//Clutter mesh
void VoxelChunk::allocate_clutter_mesh() {
	ERR_FAIL_COND(_voxel_world == NULL);

	ERR_FAIL_COND(!get_library().is_valid());

	_clutter_mesh_instance_rid = VS::get_singleton()->instance_create();

	if (get_voxel_world()->get_world().is_valid())
		VS::get_singleton()->instance_set_scenario(_clutter_mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

	_clutter_mesh_rid = VS::get_singleton()->mesh_create();

	VS::get_singleton()->instance_set_base(_clutter_mesh_instance_rid, _clutter_mesh_rid);

	VS::get_singleton()->instance_set_transform(_clutter_mesh_instance_rid, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
}

void VoxelChunk::free_clutter_mesh() {
	if (_clutter_mesh_instance_rid != RID()) {
		VS::get_singleton()->free(_clutter_mesh_instance_rid);
		VS::get_singleton()->free(_clutter_mesh_rid);

		_clutter_mesh_instance_rid = RID();
		_clutter_mesh_rid = RID();
	}
}

void VoxelChunk::free_spawn_props() {
	for (int i = 0; i < _spawned_props.size(); ++i) {
		_spawned_props[i]->queue_delete();
	}

	_spawned_props.clear();
}

void VoxelChunk::create_debug_immediate_geometry() {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(_debug_drawer != NULL);

	_debug_drawer = memnew(ImmediateGeometry());

	add_child(_debug_drawer);

	if (Engine::get_singleton()->is_editor_hint())
		_debug_drawer->set_owner(get_tree()->get_edited_scene_root());

	//_debug_drawer->set_transform(Transform(Basis(), Vector3(_position.x * _size.x * _voxel_scale, _position.y * _size.y * _voxel_scale, _position.z * _size.z * _voxel_scale)));
	//_debug_drawer->set_transform(Transform(Basis(), Vector3(_position.x * _size.x * _voxel_scale, _position.y * _size.y * _voxel_scale, _position.z * _size.z * _voxel_scale)));
}

void VoxelChunk::free_debug_immediate_geometry() {
	if (ObjectDB::instance_validate(_debug_drawer)) {
		_debug_drawer->queue_delete();

		_debug_drawer = NULL;
	}
}

void VoxelChunk::draw_cross_voxels(Vector3 pos) {
	pos *= _voxel_scale;

	_debug_drawer->add_vertex(pos + Vector3(0, 0, -0.2));
	_debug_drawer->add_vertex(pos + Vector3(0, 0, 0.2));

	_debug_drawer->add_vertex(pos + Vector3(0, -0.2, 0));
	_debug_drawer->add_vertex(pos + Vector3(0, 0.2, 0));

	_debug_drawer->add_vertex(pos + Vector3(-0.2, 0, 0));
	_debug_drawer->add_vertex(pos + Vector3(0.2, 0, 0));
}

void VoxelChunk::draw_cross_voxels_fill(Vector3 pos, float fill) {
	pos *= _voxel_scale;

	_debug_drawer->add_vertex(pos + Vector3(0, 0, -0.5 * fill));
	_debug_drawer->add_vertex(pos + Vector3(0, 0, 0.5 * fill));

	_debug_drawer->add_vertex(pos + Vector3(0, -0.5 * fill, 0));
	_debug_drawer->add_vertex(pos + Vector3(0, 0.5 * fill, 0));

	_debug_drawer->add_vertex(pos + Vector3(-0.5 * fill, 0, 0));
	_debug_drawer->add_vertex(pos + Vector3(0.5 * fill, 0, 0));
}

void VoxelChunk::draw_debug_voxels(int max, Color color) {
	if (_debug_drawer == NULL) {
		create_debug_immediate_geometry();
	}

	ERR_FAIL_COND(_debug_drawer == NULL);

	_debug_drawer->clear();
	_debug_drawer->begin(Mesh::PRIMITIVE_LINES);
	_debug_drawer->set_color(color);

	int a = 0;

	int64_t sx = static_cast<int64_t>(_size_x);
	int64_t sy = static_cast<int64_t>(_size_y);
	int64_t sz = static_cast<int64_t>(_size_y);

	for (int y = 0; y < sy; ++y) {
		for (int z = 0; z < sz; ++z) {
			for (int x = 0; x < sx; ++x) {

				int type = get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_TYPE);

				if (type == 0) {
					continue;
				}

				draw_cross_voxels_fill(Vector3(x, y, z), get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_ISOLEVEL) / 255.0 * get_voxel_scale() * 2.0);

				++a;

				if (a > max) {
					break;
				}
			}
		}
	}

	_debug_drawer->end();
}

void VoxelChunk::draw_debug_voxel_lights() {
	if (_debug_drawer == NULL) {
		create_debug_immediate_geometry();
	}

	ERR_FAIL_COND(_debug_drawer == NULL);

	_debug_drawer->clear();
	_debug_drawer->begin(Mesh::PrimitiveType::PRIMITIVE_LINES);
	_debug_drawer->set_color(Color(1, 1, 1));

	for (int i = 0; i < _voxel_lights.size(); ++i) {
		Ref<VoxelLight> v = _voxel_lights[i];

		int pos_x = v->get_world_position_x() - (_size_x * _position_x);
		int pos_y = v->get_world_position_y() - (_size_y * _position_y);
		int pos_z = v->get_world_position_z() - (_size_z * _position_z);

		draw_cross_voxels_fill(Vector3(pos_x, pos_y, pos_z), 1.0);
	}

	if (has_method("_draw_debug_voxel_lights"))
		call("_draw_debug_voxel_lights", _debug_drawer);

	_debug_drawer->end();
}

void VoxelChunk::free_chunk() {
	free_main_mesh();
	remove_colliders();
	free_prop_mesh();
	free_prop_colliders();
	free_spawn_props();
	free_liquid_mesh();
	free_clutter_mesh();
}

VoxelChunk::VoxelChunk() {
	_is_generating = false;
	_is_build_threaded = false;
	_abort_build = false;
	_dirty = false;
	_state = VOXEL_CHUNK_STATE_OK;

	_enabled = true;
	_build_mesh = true;
	_create_collider = true;
	_bake_lights = true;
	_current_build_phase = BUILD_PHASE_DONE;
	_max_build_phases = BUILD_PHASE_MAX;

	_voxel_scale = 1;
	_lod_size = 1;

	_debug_drawer = NULL;
	_voxel_world = NULL;

	_position_x = 0;
	_position_y = 0;
	_position_z = 0;

	_size_x = 0;
	_size_y = 0;
	_size_z = 0;

	_data_size_x = 0;
	_data_size_y = 0;
	_data_size_z = 0;

	_margin_start = 0;
	_margin_end = 0;

	_build_prioritized = false;
	_build_phase_done_mutex = Mutex::create();
	_build_phase_done = false;
	_build_thread = NULL;
}

VoxelChunk::~VoxelChunk() {
	free_main_mesh();
	remove_colliders();
	free_prop_mesh();
	free_prop_colliders();
	free_liquid_mesh();
	free_clutter_mesh();
	//do not call free here, the app will crash on exit, if you try to free nodes too.

	_voxel_lights.clear();

	_meshers.clear();

	if (_library.is_valid()) {
		_library.unref();
	}

	_props.clear();

	for (int i = 0; i < _channels.size(); ++i) {
		uint8_t *ch = _channels[i];

		if (ch != NULL) {
			memdelete_arr(ch);
		}
	}

	memdelete(_build_phase_done_mutex);
}

void VoxelChunk::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_EXIT_TREE: {
			if (_build_thread) {
				_abort_build = true;

				wait_and_finish_thread();
			}
		}
		case NOTIFICATION_INTERNAL_PROCESS: {
			if (has_next_phase() && get_build_phase_done()) {
				wait_and_finish_thread();

				_build_step();
			}
		}
	}
}

void VoxelChunk::wait_and_finish_thread() {
	if (_build_thread) {
		Thread::wait_to_finish(_build_thread);
		memdelete(_build_thread);
		_build_thread = NULL;
	}
}

bool VoxelChunk::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name.begins_with("channels/")) {

		int index = name.get_slicec('/', 1).to_int();

		if (_channels.size() <= index) {
			set_channel_count(index);
		}

		PoolByteArray arr = p_value;

		if (arr.size() == 0)
			return true;

		set_channel_array(index, arr);
	}

	return true;
}

bool VoxelChunk::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("channels/")) {

		int index = name.get_slicec('/', 1).to_int();

		r_ret = get_channel_array(index);

		return true;
	}

	return false;
}

void VoxelChunk::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < _channels.size(); ++i) {
		p_list->push_back(PropertyInfo(Variant::POOL_BYTE_ARRAY, "channels/" + String::num(i), PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL));
	}
}

void VoxelChunk::_bind_methods() {
	ADD_SIGNAL(MethodInfo("mesh_generation_finished", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	BIND_VMETHOD(MethodInfo("_prop_added", PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunkPropData")));

	BIND_VMETHOD(MethodInfo("_create_meshers"));
	ClassDB::bind_method(D_METHOD("_create_meshers"), &VoxelChunk::_create_meshers);

	ClassDB::bind_method(D_METHOD("get_is_generating"), &VoxelChunk::get_is_generating);
	ClassDB::bind_method(D_METHOD("set_is_generating", "value"), &VoxelChunk::set_is_generating);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_generating"), "set_is_generating", "get_is_generating");

	ClassDB::bind_method(D_METHOD("get_is_build_threaded"), &VoxelChunk::get_is_build_threaded);
	ClassDB::bind_method(D_METHOD("set_is_build_threaded", "value"), &VoxelChunk::set_is_build_threaded);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_build_threaded"), "set_is_build_threaded", "get_is_build_threaded");

	ClassDB::bind_method(D_METHOD("get_dirty"), &VoxelChunk::get_dirty);
	ClassDB::bind_method(D_METHOD("set_dirty", "value"), &VoxelChunk::set_dirty);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dirty"), "set_dirty", "get_dirty");

	ClassDB::bind_method(D_METHOD("get_state"), &VoxelChunk::get_state);
	ClassDB::bind_method(D_METHOD("set_state", "value"), &VoxelChunk::set_state);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "state"), "set_state", "get_state");

	ClassDB::bind_method(D_METHOD("get_position_x"), &VoxelChunk::get_position_x);
	ClassDB::bind_method(D_METHOD("set_position_x", "value"), &VoxelChunk::set_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_x"), "set_position_x", "get_position_x");

	ClassDB::bind_method(D_METHOD("get_position_y"), &VoxelChunk::get_position_y);
	ClassDB::bind_method(D_METHOD("set_position_y", "value"), &VoxelChunk::set_position_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_y"), "set_position_y", "get_position_y");

	ClassDB::bind_method(D_METHOD("get_position_z"), &VoxelChunk::get_position_z);
	ClassDB::bind_method(D_METHOD("set_position_z", "value"), &VoxelChunk::set_position_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_z"), "set_position_z", "get_position_z");

	ClassDB::bind_method(D_METHOD("get_size_x"), &VoxelChunk::get_size_x);
	ClassDB::bind_method(D_METHOD("set_size_x"), &VoxelChunk::set_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size_x"), "set_size_x", "get_size_x");

	ClassDB::bind_method(D_METHOD("get_size_y"), &VoxelChunk::get_size_y);
	ClassDB::bind_method(D_METHOD("set_size_y"), &VoxelChunk::set_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size_y"), "set_size_y", "get_size_y");

	ClassDB::bind_method(D_METHOD("get_size_z"), &VoxelChunk::get_size_z);
	ClassDB::bind_method(D_METHOD("set_size_z"), &VoxelChunk::set_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size_z"), "set_size_z", "get_size_z");

	ClassDB::bind_method(D_METHOD("get_data_size_x"), &VoxelChunk::get_data_size_x);
	ClassDB::bind_method(D_METHOD("set_data_size_x"), &VoxelChunk::set_data_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_size_x"), "set_data_size_x", "get_data_size_x");

	ClassDB::bind_method(D_METHOD("get_data_size_y"), &VoxelChunk::get_data_size_y);
	ClassDB::bind_method(D_METHOD("set_data_size_y"), &VoxelChunk::set_data_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_size_y"), "set_data_size_y", "get_data_size_y");

	ClassDB::bind_method(D_METHOD("get_data_size_z"), &VoxelChunk::get_data_size_z);
	ClassDB::bind_method(D_METHOD("set_data_size_z"), &VoxelChunk::set_data_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_size_z"), "set_data_size_z", "get_data_size_z");

	ClassDB::bind_method(D_METHOD("get_position"), &VoxelChunk::get_position);
	ClassDB::bind_method(D_METHOD("set_position", "x", "y", "z"), &VoxelChunk::set_position);

	ClassDB::bind_method(D_METHOD("get_margin_start"), &VoxelChunk::get_margin_start);
	ClassDB::bind_method(D_METHOD("set_margin_start"), &VoxelChunk::set_margin_start);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin_start"), "set_margin_start", "get_margin_start");

	ClassDB::bind_method(D_METHOD("get_margin_end"), &VoxelChunk::get_margin_end);
	ClassDB::bind_method(D_METHOD("set_margin_end"), &VoxelChunk::set_margin_end);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin_end"), "set_margin_end", "get_margin_end");

	ClassDB::bind_method(D_METHOD("get_library"), &VoxelChunk::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "value"), &VoxelChunk::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_lod_size"), &VoxelChunk::get_lod_size);
	ClassDB::bind_method(D_METHOD("set_lod_size", "value"), &VoxelChunk::set_lod_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_size"), "set_lod_size", "get_lod_size");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &VoxelChunk::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &VoxelChunk::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_current_build_phase"), &VoxelChunk::get_current_build_phase);
	ClassDB::bind_method(D_METHOD("set_current_build_phase", "value"), &VoxelChunk::set_current_build_phase);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_build_phase"), "set_current_build_phase", "get_current_build_phase");

	ClassDB::bind_method(D_METHOD("get_max_build_phase"), &VoxelChunk::get_max_build_phase);
	ClassDB::bind_method(D_METHOD("set_max_build_phase", "value"), &VoxelChunk::set_max_build_phase);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_build_phase"), "set_max_build_phase", "get_max_build_phase");

	ADD_GROUP("Meshing", "meshing");

	ClassDB::bind_method(D_METHOD("meshing_get_create_collider"), &VoxelChunk::get_create_collider);
	ClassDB::bind_method(D_METHOD("meshing_set_create_collider", "value"), &VoxelChunk::set_create_collider);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "meshing_create_collider"), "meshing_set_create_collider", "meshing_get_create_collider");

	ClassDB::bind_method(D_METHOD("meshing_get_bake_lights"), &VoxelChunk::get_bake_lights);
	ClassDB::bind_method(D_METHOD("meshing_set_bake_lights", "value"), &VoxelChunk::set_bake_lights);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "meshing_bake_lights"), "meshing_set_bake_lights", "meshing_get_bake_lights");

	ADD_GROUP("Settings", "setting");

	ClassDB::bind_method(D_METHOD("get_mesher", "index"), &VoxelChunk::get_mesher);
	ClassDB::bind_method(D_METHOD("set_mesher", "index", "mesher"), &VoxelChunk::set_mesher);
	ClassDB::bind_method(D_METHOD("remove_mesher", "index"), &VoxelChunk::remove_mesher);
	ClassDB::bind_method(D_METHOD("add_mesher", "mesher"), &VoxelChunk::add_mesher);
	ClassDB::bind_method(D_METHOD("get_mesher_count"), &VoxelChunk::get_mesher_count);

	ClassDB::bind_method(D_METHOD("get_voxel_world"), &VoxelChunk::get_voxel_world);
	ClassDB::bind_method(D_METHOD("set_voxel_world", "world"), &VoxelChunk::set_voxel_world_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "voxel_world", PROPERTY_HINT_RESOURCE_TYPE, "VoxelWorld"), "set_voxel_world", "get_voxel_world");

	ClassDB::bind_method(D_METHOD("get_mesh_rid"), &VoxelChunk::get_mesh_rid);
	ClassDB::bind_method(D_METHOD("get_mesh_instance_rid"), &VoxelChunk::get_mesh_instance_rid);
	ClassDB::bind_method(D_METHOD("get_shape_rid"), &VoxelChunk::get_shape_rid);
	ClassDB::bind_method(D_METHOD("get_body_rid"), &VoxelChunk::get_body_rid);

	ClassDB::bind_method(D_METHOD("get_prop_mesh_rid"), &VoxelChunk::get_prop_mesh_rid);
	ClassDB::bind_method(D_METHOD("get_prop_mesh_instance_rid"), &VoxelChunk::get_prop_mesh_instance_rid);
	ClassDB::bind_method(D_METHOD("get_prop_shape_rid"), &VoxelChunk::get_prop_shape_rid);
	ClassDB::bind_method(D_METHOD("get_prop_body_rid"), &VoxelChunk::get_prop_body_rid);

	ClassDB::bind_method(D_METHOD("get_liquid_mesh_rid"), &VoxelChunk::get_liquid_mesh_rid);
	ClassDB::bind_method(D_METHOD("get_liquid_mesh_instance_rid"), &VoxelChunk::get_liquid_mesh_instance_rid);

	ClassDB::bind_method(D_METHOD("get_clutter_mesh_rid"), &VoxelChunk::get_clutter_mesh_rid);
	ClassDB::bind_method(D_METHOD("get_clutter_mesh_instance_rid"), &VoxelChunk::get_clutter_mesh_instance_rid);

	//Voxel Data
	BIND_VMETHOD(MethodInfo("_setup_channels"));

	ClassDB::bind_method(D_METHOD("setup_channels"), &VoxelChunk::setup_channels);
	ClassDB::bind_method(D_METHOD("_setup_channels"), &VoxelChunk::_setup_channels);

	ClassDB::bind_method(D_METHOD("set_size", "size_x", "size_y", "size_z", "margin_start", "margin_end"), &VoxelChunk::set_size, DEFVAL(0), DEFVAL(0));

	ClassDB::bind_method(D_METHOD("validate_channel_data_position", "x", "y", "z"), &VoxelChunk::validate_channel_data_position);

	ClassDB::bind_method(D_METHOD("get_voxel", "x", "y", "z", "channel_index"), &VoxelChunk::get_voxel);
	ClassDB::bind_method(D_METHOD("set_voxel", "value", "x", "y", "z", "channel_index"), &VoxelChunk::set_voxel);

	ClassDB::bind_method(D_METHOD("get_channel_count"), &VoxelChunk::get_channel_count);
	ClassDB::bind_method(D_METHOD("set_channel_count", "count"), &VoxelChunk::set_channel_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_count"), "set_channel_count", "get_channel_count");

	ClassDB::bind_method(D_METHOD("allocate_channel", "channel_index", "default_value"), &VoxelChunk::allocate_channel);
	ClassDB::bind_method(D_METHOD("fill_channel", "value", "channel_index"), &VoxelChunk::fill_channel);
	ClassDB::bind_method(D_METHOD("dealloc_channel", "channel_index"), &VoxelChunk::dealloc_channel);

	ClassDB::bind_method(D_METHOD("get_channel_array", "channel_index"), &VoxelChunk::get_channel_array);
	ClassDB::bind_method(D_METHOD("set_channel_array", "channel_index", "array"), &VoxelChunk::set_channel_array);

	ClassDB::bind_method(D_METHOD("get_data_index", "x", "y", "z"), &VoxelChunk::get_data_index);
	ClassDB::bind_method(D_METHOD("get_data_size"), &VoxelChunk::get_data_size);

	//Data Management functions
	ClassDB::bind_method(D_METHOD("generate_ao"), &VoxelChunk::generate_ao);
	ClassDB::bind_method(D_METHOD("add_light", "local_x", "local_y", "local_z", "size", "color"), &VoxelChunk::add_light);
	ClassDB::bind_method(D_METHOD("clear_baked_lights"), &VoxelChunk::clear_baked_lights);

	//Meshes
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "is_done"), "_build_phase", PropertyInfo(Variant::INT, "phase")));

	ClassDB::bind_method(D_METHOD("build_deferred"), &VoxelChunk::build_deferred);
	ClassDB::bind_method(D_METHOD("build_prioritized"), &VoxelChunk::build_prioritized);
	ClassDB::bind_method(D_METHOD("build_phase"), &VoxelChunk::build_phase);
	ClassDB::bind_method(D_METHOD("_build_phase", "phase"), &VoxelChunk::_build_phase);
	ClassDB::bind_method(D_METHOD("next_phase"), &VoxelChunk::next_phase);
	ClassDB::bind_method(D_METHOD("has_next_phase"), &VoxelChunk::has_next_phase);
	ClassDB::bind_method(D_METHOD("clear"), &VoxelChunk::clear);

	ClassDB::bind_method(D_METHOD("create_colliders"), &VoxelChunk::create_colliders);
	ClassDB::bind_method(D_METHOD("remove_colliders"), &VoxelChunk::remove_colliders);

	ClassDB::bind_method(D_METHOD("add_lights", "lights"), &VoxelChunk::add_lights);
	ClassDB::bind_method(D_METHOD("add_voxel_light", "light"), &VoxelChunk::add_voxel_light);
	ClassDB::bind_method(D_METHOD("create_voxel_light", "color", "size", "x", "y", "z"), &VoxelChunk::create_voxel_light);

	ClassDB::bind_method(D_METHOD("remove_voxel_light", "light"), &VoxelChunk::remove_voxel_light);
	ClassDB::bind_method(D_METHOD("clear_voxel_lights"), &VoxelChunk::clear_voxel_lights);
	ClassDB::bind_method(D_METHOD("add_lights_into", "lights"), &VoxelChunk::add_lights_into);
	ClassDB::bind_method(D_METHOD("add_unique_lights_into", "lights"), &VoxelChunk::add_unique_lights_into);
	ClassDB::bind_method(D_METHOD("get_lights"), &VoxelChunk::get_lights);

	ClassDB::bind_method(D_METHOD("bake_lights"), &VoxelChunk::bake_lights);
	ClassDB::bind_method(D_METHOD("bake_light", "light"), &VoxelChunk::bake_light);

	ClassDB::bind_method(D_METHOD("add_prop_light", "light"), &VoxelChunk::add_prop_light);

	ClassDB::bind_method(D_METHOD("add_prop", "prop"), &VoxelChunk::add_prop);
	ClassDB::bind_method(D_METHOD("get_prop", "index"), &VoxelChunk::get_prop);
	ClassDB::bind_method(D_METHOD("get_prop_count"), &VoxelChunk::get_prop_count);
	ClassDB::bind_method(D_METHOD("remove_prop", "index"), &VoxelChunk::remove_prop);
	ClassDB::bind_method(D_METHOD("clear_props"), &VoxelChunk::clear_props);

	ClassDB::bind_method(D_METHOD("build_prop_collider"), &VoxelChunk::build_prop_collider);
	ClassDB::bind_method(D_METHOD("free_spawn_props"), &VoxelChunk::free_spawn_props);

	ClassDB::bind_method(D_METHOD("allocate_main_mesh"), &VoxelChunk::allocate_main_mesh);
	ClassDB::bind_method(D_METHOD("free_main_mesh"), &VoxelChunk::free_main_mesh);

	ClassDB::bind_method(D_METHOD("allocate_prop_mesh"), &VoxelChunk::allocate_prop_mesh);
	ClassDB::bind_method(D_METHOD("free_prop_mesh"), &VoxelChunk::free_prop_mesh);

	ClassDB::bind_method(D_METHOD("allocate_prop_colliders"), &VoxelChunk::allocate_prop_colliders);
	ClassDB::bind_method(D_METHOD("free_prop_colliders"), &VoxelChunk::free_prop_colliders);

	ClassDB::bind_method(D_METHOD("allocate_liquid_mesh"), &VoxelChunk::allocate_liquid_mesh);
	ClassDB::bind_method(D_METHOD("free_liquid_mesh"), &VoxelChunk::free_liquid_mesh);

	ClassDB::bind_method(D_METHOD("allocate_clutter_mesh"), &VoxelChunk::allocate_clutter_mesh);
	ClassDB::bind_method(D_METHOD("free_clutter_mesh"), &VoxelChunk::free_clutter_mesh);

	ClassDB::bind_method(D_METHOD("create_meshers"), &VoxelChunk::create_meshers);

	ClassDB::bind_method(D_METHOD("create_debug_immediate_geometry"), &VoxelChunk::create_debug_immediate_geometry);
	ClassDB::bind_method(D_METHOD("free_debug_immediate_geometry"), &VoxelChunk::free_debug_immediate_geometry);

	ClassDB::bind_method(D_METHOD("free_chunk"), &VoxelChunk::free_chunk);

	BIND_VMETHOD(MethodInfo("_draw_debug_voxel_lights", PropertyInfo(Variant::OBJECT, "debug_drawer", PROPERTY_HINT_RESOURCE_TYPE, "ImmediateGeometry")));

	ClassDB::bind_method(D_METHOD("draw_cross_voxels", "max"), &VoxelChunk::draw_cross_voxels);
	ClassDB::bind_method(D_METHOD("draw_cross_voxels_fill", "max", "fill"), &VoxelChunk::draw_cross_voxels_fill);
	ClassDB::bind_method(D_METHOD("draw_debug_voxels", "max", "color"), &VoxelChunk::draw_debug_voxels, DEFVAL(Color(1, 1, 1)));

	ClassDB::bind_method(D_METHOD("draw_debug_voxel_lights"), &VoxelChunk::draw_debug_voxel_lights);

	BIND_CONSTANT(BUILD_PHASE_DONE);
	BIND_CONSTANT(BUILD_PHASE_SETUP);
	BIND_CONSTANT(BUILD_PHASE_TERRARIN_MESH_SETUP);
	BIND_CONSTANT(BUILD_PHASE_TERRARIN_MESH_COLLIDER);
	BIND_CONSTANT(BUILD_PHASE_LIGHTS);
	BIND_CONSTANT(BUILD_PHASE_TERRARIN_MESH);
	BIND_CONSTANT(BUILD_PHASE_PROP_MESH);
	BIND_CONSTANT(BUILD_PHASE_PROP_COLLIDER);
	BIND_CONSTANT(BUILD_PHASE_FINALIZE);
	BIND_CONSTANT(BUILD_PHASE_MAX);

	BIND_CONSTANT(VOXEL_CHUNK_STATE_OK);
	BIND_CONSTANT(VOXEL_CHUNK_STATE_GENERATION_QUEUED);
	BIND_CONSTANT(VOXEL_CHUNK_STATE_GENERATION);
	BIND_CONSTANT(VOXEL_CHUNK_STATE_MESH_GENERATION_QUEUED);
	BIND_CONSTANT(VOXEL_CHUNK_STATE_MESH_GENERATION);
	BIND_CONSTANT(VOXEL_CHUNK_STATE_MAX);

	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_TYPE);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_ISOLEVEL);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIGHT_COLOR_R);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIGHT_COLOR_G);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIGHT_COLOR_B);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_AO);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_RANDOM_AO);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIQUID_TYPES);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIQUID_FILL);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIQUID_FLOW);
	BIND_ENUM_CONSTANT(MAX_DEFAULT_CHANNELS);
}
