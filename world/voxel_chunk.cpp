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

#include "../thirdparty/lz4/lz4.h"

_FORCE_INLINE_ bool VoxelChunk::get_process() const {
	return _is_processing;
}
_FORCE_INLINE_ void VoxelChunk::set_process(const bool value) {
	_is_processing = value;
}

_FORCE_INLINE_ bool VoxelChunk::get_physics_process() const {
	return _is_phisics_processing;
}
_FORCE_INLINE_ void VoxelChunk::set_physics_process(const bool value) {
	_is_phisics_processing = value;
}

bool VoxelChunk::get_visible() const {
	return _is_visible;
}
void VoxelChunk::set_visible(const bool value) {
	_is_visible = value;

	visibility_changed(value);
}

_FORCE_INLINE_ bool VoxelChunk::get_is_generating() const {
	return _is_generating;
}
_FORCE_INLINE_ void VoxelChunk::set_is_generating(bool value) {
	_is_generating = value;
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
void VoxelChunk::set_library(const Ref<VoxelmanLibrary> &value) {
	_library = value;
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

//Voxel Data
void VoxelChunk::setup_channels() {
	ERR_FAIL_COND_MSG(!has_method("_setup_channels"), "VoxelChunk: _setup_channels() is missing! Please implement it!");

	call("_setup_channels");
}

void VoxelChunk::set_size(int size_x, int size_y, int size_z, int margin_start, int margin_end) {
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

bool VoxelChunk::validate_channel_data_position(int x, int y, int z) const {
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

PoolByteArray VoxelChunk::get_channel_compressed(int channel_index) const {
	PoolByteArray arr;

	int size = _data_size_x * _data_size_y * _data_size_z;

	if (channel_index >= _channels.size())
		return arr;

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL)
		return arr;

	int bound = LZ4_compressBound(size);
	arr.resize(bound);

	PoolByteArray::Write w = arr.write();

	int ns = LZ4_compress_default(reinterpret_cast<char *>(ch), reinterpret_cast<char *>(w.ptr()), size, bound);

	w.release();
	arr.resize(ns);

	return arr;
}
void VoxelChunk::set_channel_compressed(int channel_index, const PoolByteArray &data) {
	if (data.size() == 0)
		return;

	int size = _data_size_x * _data_size_y * _data_size_z;

	if (_channels.size() <= channel_index)
		set_channel_count(channel_index + 1);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		if (_channels[channel_index] != NULL)
			return;

		ch = memnew_arr(uint8_t, size);
		_channels.set(channel_index, ch);
	}

	int ds = data.size();

	PoolByteArray::Read r = data.read();

	//We are not going to write to it
	uint8_t *data_arr = const_cast<uint8_t *>(r.ptr());

	LZ4_decompress_safe(reinterpret_cast<char *>(data_arr), reinterpret_cast<char *>(ch), ds, size);
}

_FORCE_INLINE_ int VoxelChunk::get_data_index(const int x, const int y, const int z) const {
	return y + _data_size_y * (x + _data_size_x * z);
}

_FORCE_INLINE_ int VoxelChunk::get_data_size() const {
	return _data_size_x * _data_size_y * _data_size_z;
}

void VoxelChunk::create_meshers() {
	ERR_FAIL_COND_MSG(!has_method("_create_meshers"), "VoxelChunk: _create_meshers() is missing! Please implement it!");

	call("_create_meshers");
}

void VoxelChunk::build(const bool immediate) {
	ERR_FAIL_COND_MSG(!has_method("_build"), "VoxelChunk: _build(immediate : bool) is missing! Please implement it!");

	call("_build", immediate);
}

void VoxelChunk::clear() {
	ERR_FAIL_COND_MSG(!has_method("_clear"), "VoxelChunk: _clear() is missing! Please implement it!");

	call("_clear");
}

Array VoxelChunk::merge_mesh_array(Array arr) const {
	ERR_FAIL_COND_V(arr.size() != VisualServer::ARRAY_MAX, arr);

	PoolVector3Array verts = arr[VisualServer::ARRAY_VERTEX];
	PoolVector3Array normals = arr[VisualServer::ARRAY_NORMAL];
	PoolVector2Array uvs = arr[VisualServer::ARRAY_TEX_UV];
	PoolColorArray colors = arr[VisualServer::ARRAY_COLOR];
	PoolIntArray indices = arr[VisualServer::ARRAY_INDEX];

	int i = 0;
	while (i < verts.size()) {
		Vector3 v = verts[i];

		Array equals;
		for (int j = i + 1; j < verts.size(); ++j) {
			Vector3 vc = verts[j];

			if (Math::is_equal_approx(v.x, vc.x) && Math::is_equal_approx(v.y, vc.y) && Math::is_equal_approx(v.z, vc.z))
				equals.push_back(j);
		}

		for (int k = 0; k < equals.size(); ++k) {
			int rem = equals[k];
			int remk = rem - k;

			verts.remove(remk);
			normals.remove(remk);
			uvs.remove(remk);
			colors.remove(remk);

			for (int j = 0; j < indices.size(); ++j) {
				int indx = indices[j];

				if (indx == remk)
					indices.set(j, i);
				else if (indx > remk)
					indices.set(j, indx - 1);
			}
		}

		++i;
	}

	arr[VisualServer::ARRAY_VERTEX] = verts;
	arr[VisualServer::ARRAY_NORMAL] = normals;
	arr[VisualServer::ARRAY_TEX_UV] = uvs;
	arr[VisualServer::ARRAY_COLOR] = colors;
	arr[VisualServer::ARRAY_INDEX] = indices;

	return arr;
}
Array VoxelChunk::bake_mesh_array_uv(Array arr, Ref<Texture> tex, float mul_color) const {
	ERR_FAIL_COND_V(arr.size() != VisualServer::ARRAY_MAX, arr);
	ERR_FAIL_COND_V(!tex.is_valid(), arr);

	Ref<Image> img = tex->get_data();

	ERR_FAIL_COND_V(!img.is_valid(), arr);

	Vector2 imgsize = img->get_size();

	PoolVector2Array uvs = arr[VisualServer::ARRAY_TEX_UV];
	PoolColorArray colors = arr[VisualServer::ARRAY_COLOR];

	img->lock();

	for (int i = 0; i < uvs.size(); ++i) {
		Vector2 uv = uvs[i];
		uv *= imgsize;

		Color c = img->get_pixelv(uv);

		colors.set(i, colors[i] * c * mul_color);
	}

	img->unlock();

	arr[VisualServer::ARRAY_COLOR] = colors;

	return arr;
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

	add_light(wpx, wpy, wpz, light->get_size(), light->get_color());
}

void VoxelChunk::clear_baked_lights() {
	ERR_FAIL_COND_MSG(!has_method("_clear_baked_lights"), "VoxelChunk: _clear_baked_lights() is missing! Please implement it!");

	call("_clear_baked_lights");
}

void VoxelChunk::add_light(int local_x, int local_y, int local_z, int size, Color color) {
	ERR_FAIL_COND_MSG(!has_method("_add_light"), "VoxelChunk: _add_light() is missing! Please implement it!");

	call("_add_light", local_x, local_y, local_z, size, color);
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

void VoxelChunk::free_spawn_props() {
	for (int i = 0; i < _spawned_props.size(); ++i) {
		_spawned_props[i]->queue_delete();
	}

	_spawned_props.clear();
}

void VoxelChunk::enter_tree() {
	if (has_method("_enter_tree"))
		call("_enter_tree");
}
void VoxelChunk::exit_tree() {
	if (has_method("_exit_tree"))
		call("_exit_tree");
}
void VoxelChunk::process(float delta) {
	if (has_method("_process"))
		call("_process", delta);
}
void VoxelChunk::physics_process(float delta) {
	if (has_method("_physics_process"))
		call("_physics_process", delta);
}
void VoxelChunk::world_transform_changed() {
	call("_world_transform_changed");
}
void VoxelChunk::visibility_changed(bool visible) {
	if (has_method("_visibility_changed"))
		call("_visibility_changed", _is_visible);
}

Transform VoxelChunk::get_transform() const {
	return _transform;
}
void VoxelChunk::set_transform(const Transform &transform) {
	_transform = transform;
}

VoxelChunk::VoxelChunk() {
	_is_processing = false;
	_is_phisics_processing = false;

	_is_visible = true;

	_is_generating = false;
	_dirty = false;
	_state = VOXEL_CHUNK_STATE_OK;

	_voxel_scale = 1;

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
}

VoxelChunk::~VoxelChunk() {
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
}

void VoxelChunk::_world_transform_changed() {
	Transform wt;

	if (_voxel_world != NULL) {
		wt = _voxel_world->get_transform();
	}

	set_transform(wt * Transform(Basis(), Vector3(_position_x * static_cast<int>(_size_x) * _voxel_scale, _position_y * static_cast<int>(_size_y) * _voxel_scale, _position_z * static_cast<int>(_size_z) * _voxel_scale)));
}

/*
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
*/

void VoxelChunk::_bind_methods() {
	ADD_SIGNAL(MethodInfo("mesh_generation_finished", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	BIND_VMETHOD(MethodInfo("_prop_added", PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunkPropData")));
	BIND_VMETHOD(MethodInfo("_create_meshers"));
	BIND_VMETHOD(MethodInfo("_setup_channels"));
	BIND_VMETHOD(MethodInfo("_add_light", PropertyInfo(Variant::INT, "local_x"), PropertyInfo(Variant::INT, "local_y"), PropertyInfo(Variant::INT, "local_z"), PropertyInfo(Variant::INT, "size"), PropertyInfo(Variant::COLOR, "color")));

	BIND_VMETHOD(MethodInfo("_enter_tree"));
	BIND_VMETHOD(MethodInfo("_exit_tree"));
	BIND_VMETHOD(MethodInfo("_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_physics_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_world_transform_changed"));
	BIND_VMETHOD(MethodInfo("_visibility_changed", PropertyInfo(Variant::BOOL, "visible")));

	ClassDB::bind_method(D_METHOD("visibility_changed", "visible"), &VoxelChunk::visibility_changed);

	ClassDB::bind_method(D_METHOD("get_process"), &VoxelChunk::get_process);
	ClassDB::bind_method(D_METHOD("set_process", "value"), &VoxelChunk::set_process);

	ClassDB::bind_method(D_METHOD("get_physics_process"), &VoxelChunk::get_physics_process);
	ClassDB::bind_method(D_METHOD("set_physics_process", "value"), &VoxelChunk::set_physics_process);

	ClassDB::bind_method(D_METHOD("get_transform"), &VoxelChunk::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &VoxelChunk::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");

	ClassDB::bind_method(D_METHOD("get_visible"), &VoxelChunk::get_visible);
	ClassDB::bind_method(D_METHOD("set_visible", "value"), &VoxelChunk::set_visible);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "visible"), "set_visible", "get_visible");

	ClassDB::bind_method(D_METHOD("get_is_generating"), &VoxelChunk::get_is_generating);
	ClassDB::bind_method(D_METHOD("set_is_generating", "value"), &VoxelChunk::set_is_generating);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_generating"), "set_is_generating", "get_is_generating");

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

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &VoxelChunk::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &VoxelChunk::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_mesher", "index"), &VoxelChunk::get_mesher);
	ClassDB::bind_method(D_METHOD("set_mesher", "index", "mesher"), &VoxelChunk::set_mesher);
	ClassDB::bind_method(D_METHOD("remove_mesher", "index"), &VoxelChunk::remove_mesher);
	ClassDB::bind_method(D_METHOD("add_mesher", "mesher"), &VoxelChunk::add_mesher);
	ClassDB::bind_method(D_METHOD("get_mesher_count"), &VoxelChunk::get_mesher_count);

	ClassDB::bind_method(D_METHOD("get_voxel_world"), &VoxelChunk::get_voxel_world);
	ClassDB::bind_method(D_METHOD("set_voxel_world", "world"), &VoxelChunk::set_voxel_world_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "voxel_world", PROPERTY_HINT_RESOURCE_TYPE, "VoxelWorld", 0), "set_voxel_world", "get_voxel_world");

	//Voxel Data
	ClassDB::bind_method(D_METHOD("setup_channels"), &VoxelChunk::setup_channels);

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

	ClassDB::bind_method(D_METHOD("get_channel_compressed", "channel_index"), &VoxelChunk::get_channel_compressed);
	ClassDB::bind_method(D_METHOD("set_channel_compressed", "channel_index", "array"), &VoxelChunk::set_channel_compressed);

	ClassDB::bind_method(D_METHOD("get_data_index", "x", "y", "z"), &VoxelChunk::get_data_index);
	ClassDB::bind_method(D_METHOD("get_data_size"), &VoxelChunk::get_data_size);

	ClassDB::bind_method(D_METHOD("merge_mesh_array", "arr"), &VoxelChunk::merge_mesh_array);
	ClassDB::bind_method(D_METHOD("bake_mesh_array_uv", "arr", "tex", "mul_color"), &VoxelChunk::bake_mesh_array_uv, DEFVAL(0.7));

	//Meshes
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

	ClassDB::bind_method(D_METHOD("free_spawn_props"), &VoxelChunk::free_spawn_props);

	ClassDB::bind_method(D_METHOD("create_meshers"), &VoxelChunk::create_meshers);

	ClassDB::bind_method(D_METHOD("_world_transform_changed"), &VoxelChunk::_world_transform_changed);
}
