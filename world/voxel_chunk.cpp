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

#include "../defines.h"

#include "jobs/voxel_job.h"
#include "voxel_structure.h"

#if THREAD_POOL_PRESENT
#include "../../thread_pool/thread_pool.h"
#endif

_FORCE_INLINE_ bool VoxelChunk::get_is_build_threaded() const {
	return _is_build_threaded;
}
_FORCE_INLINE_ void VoxelChunk::set_is_build_threaded(const bool value) {
	_is_build_threaded = value;
}

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
_FORCE_INLINE_ void VoxelChunk::set_is_generating(const bool value) {
	_is_generating = value;
}

bool VoxelChunk::is_in_tree() const {
	return _is_in_tree;
}

_FORCE_INLINE_ bool VoxelChunk::get_dirty() const {
	return _dirty;
}
_FORCE_INLINE_ void VoxelChunk::set_dirty(const bool value) {
	_dirty = value;
}

_FORCE_INLINE_ int VoxelChunk::get_state() const {
	return _state;
}
_FORCE_INLINE_ void VoxelChunk::set_state(const int value) {
	_state = value;
}

_FORCE_INLINE_ int VoxelChunk::get_position_x() const {
	return _position_x;
}
void VoxelChunk::set_position_x(const int value) {
	_position_x = value;
}
_FORCE_INLINE_ int VoxelChunk::get_position_y() const {
	return _position_y;
}
void VoxelChunk::set_position_y(const int value) {
	_position_y = value;
}
_FORCE_INLINE_ int VoxelChunk::get_position_z() const {
	return _position_z;
}
void VoxelChunk::set_position_z(const int value) {
	_position_z = value;
}

_FORCE_INLINE_ Vector3 VoxelChunk::get_position() const {
	return Vector3(_position_x, _position_y, _position_z);
}
_FORCE_INLINE_ Vector3 VoxelChunk::get_world_position() const {
	return Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale);
}

_FORCE_INLINE_ Vector3 VoxelChunk::get_world_size() const {
	return Vector3(_size_x * _voxel_scale, _size_y * _voxel_scale, _size_z * _voxel_scale);
}

_FORCE_INLINE_ AABB VoxelChunk::get_world_aabb() const {
	return AABB(get_world_position(), get_world_size());
}

_FORCE_INLINE_ int VoxelChunk::get_size_x() const {
	return _size_x;
}
_FORCE_INLINE_ int VoxelChunk::get_size_y() const {
	return _size_y;
}
_FORCE_INLINE_ int VoxelChunk::get_size_z() const {
	return _size_z;
}

_FORCE_INLINE_ void VoxelChunk::set_size_x(const int value) {
	_size_x = value;
}
_FORCE_INLINE_ void VoxelChunk::set_size_y(const int value) {
	_size_y = value;
}
_FORCE_INLINE_ void VoxelChunk::set_size_z(const int value) {
	_size_z = value;
}

_FORCE_INLINE_ Vector3 VoxelChunk::get_size() const {
	return Vector3(_size_x, _size_y, _size_z);
}

_FORCE_INLINE_ int VoxelChunk::get_data_size_x() const {
	return _data_size_x;
}
_FORCE_INLINE_ int VoxelChunk::get_data_size_y() const {
	return _data_size_y;
}
_FORCE_INLINE_ int VoxelChunk::get_data_size_z() const {
	return _data_size_z;
}

_FORCE_INLINE_ void VoxelChunk::set_data_size_x(const int value) {
	_data_size_x = value;
}
_FORCE_INLINE_ void VoxelChunk::set_data_size_y(const int value) {
	_data_size_y = value;
}
_FORCE_INLINE_ void VoxelChunk::set_data_size_z(const int value) {
	_data_size_z = value;
}

void VoxelChunk::set_position(const int x, const int y, const int z) {
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

_FORCE_INLINE_ void VoxelChunk::set_margin_start(const int value) {
	_margin_start = value;
}
_FORCE_INLINE_ void VoxelChunk::set_margin_end(const int value) {
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
void VoxelChunk::set_voxel_scale(const float value) {
	_voxel_scale = value;
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

Ref<VoxelJob> VoxelChunk::get_job(int index) const {
	ERR_FAIL_INDEX_V(index, _jobs.size(), Ref<VoxelJob>());

	return _jobs.get(index);
}
void VoxelChunk::set_job(int index, const Ref<VoxelJob> &job) {
	ERR_FAIL_INDEX(index, _jobs.size());

	_jobs.set(index, job);
}
void VoxelChunk::remove_job(const int index) {
	ERR_FAIL_INDEX(index, _jobs.size());

	_jobs.remove(index);
}
void VoxelChunk::add_job(const Ref<VoxelJob> &job) {
	_jobs.push_back(job);
}
int VoxelChunk::get_job_count() const {
	return _jobs.size();
}

int VoxelChunk::get_current_job_index() {
	return _current_job;
}
void VoxelChunk::next_job() {
	_THREAD_SAFE_METHOD_

	++_current_job;

	if (_current_job >= _jobs.size()) {
		_current_job = -1;
		set_is_generating(false);
		finalize_build();
		return;
	}

	Ref<VoxelJob> j = _jobs[_current_job];

	if (!j.is_valid()) {
		//skip if invalid
		next_job();
	}

	j->reset();
	j->set_complete(false);

	if (j->get_build_phase_type() == VoxelJob::BUILD_PHASE_TYPE_NORMAL) {
#if THREAD_POOL_PRESENT
		ThreadPool::get_singleton()->add_job(j);
#else
		j->execute();
#endif
	}
}
Ref<VoxelJob> VoxelChunk::get_current_job() {
	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size()) {
		return Ref<VoxelJob>();
	}

	return _jobs[_current_job];
}

//Voxel Data
void VoxelChunk::setup_channels() {
	ERR_FAIL_COND_MSG(!has_method("_setup_channels"), "VoxelChunk: _setup_channels() is missing! Please implement it!");

	call("_setup_channels");
}

void VoxelChunk::set_size(const int size_x, const int size_y, const int size_z, const int margin_start, const int margin_end) {
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

bool VoxelChunk::validate_channel_data_position(const int x, const int y, const int z) const {
	return x < _data_size_x && y < _data_size_y && z < _data_size_z;
}

uint8_t VoxelChunk::get_voxel(const int p_x, const int p_y, const int p_z, const int p_channel_index) const {
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
void VoxelChunk::set_voxel(const uint8_t p_value, const int p_x, const int p_y, const int p_z, const int p_channel_index) {
	int x = p_x + _margin_start;
	int y = p_y + _margin_start;
	int z = p_z + _margin_start;

	ERR_FAIL_INDEX(p_channel_index, _channels.size());
	ERR_FAIL_COND_MSG(!validate_channel_data_position(x, y, z), "Error, index out of range! " + String::num(x) + " " + String::num(y) + " " + String::num(z));

	uint8_t *ch = get_valid_channel(p_channel_index);

	ch[get_data_index(x, y, z)] = p_value;
}

int VoxelChunk::get_channel_count() const {
	return _channels.size();
}

void VoxelChunk::set_channel_count(const int count) {
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
bool VoxelChunk::is_channel_allocated(const int channel_index) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), false);

	return _channels[channel_index] != NULL;
}
void VoxelChunk::ensure_channel_allocated(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	if (_channels[channel_index] == NULL)
		allocate_channel(channel_index, default_value);
}
void VoxelChunk::allocate_channel(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	if (_channels[channel_index] != NULL)
		return;

	uint32_t size = _data_size_x * _data_size_y * _data_size_z;

	uint8_t *ch = memnew_arr(uint8_t, size);
	memset(ch, default_value, size);

	_channels.set(channel_index, ch);
}
void VoxelChunk::fill_channel(const uint8_t value, const int channel_index) {
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
void VoxelChunk::dealloc_channel(const int channel_index) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	uint8_t *ch = _channels.get(channel_index);

	if (ch != NULL) {
		memdelete_arr(ch);

		_channels.set(channel_index, NULL);
	}
}

uint8_t *VoxelChunk::get_channel(const int channel_index) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), NULL);

	return _channels.get(channel_index);
}
uint8_t *VoxelChunk::get_valid_channel(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), 0);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		allocate_channel(channel_index, default_value);

		return _channels.get(channel_index);
	}

	return ch;
}

PoolByteArray VoxelChunk::get_channel_array(const int channel_index) const {
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
void VoxelChunk::set_channel_array(const int channel_index, const PoolByteArray &array) {
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

PoolByteArray VoxelChunk::get_channel_compressed(const int channel_index) const {
	PoolByteArray arr;

	int size = _data_size_x * _data_size_y * _data_size_z;

	if (channel_index >= _channels.size())
		return arr;

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL)
		return arr;

	int bound = LZ4_compressBound(size);
	arr.resize(bound);

#if !GODOT4
	PoolByteArray::Write w = arr.write();

	int ns = LZ4_compress_default(reinterpret_cast<char *>(ch), reinterpret_cast<char *>(w.ptr()), size, bound);

	w.release();
#else
	int ns = LZ4_compress_default(reinterpret_cast<char *>(ch), reinterpret_cast<char *>(arr.ptrw()), size, bound);
#endif
	arr.resize(ns);

	return arr;
}
void VoxelChunk::set_channel_compressed(const int channel_index, const PoolByteArray &data) {
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

#if !GODOT4
	PoolByteArray::Read r = data.read();

	//We are not going to write to it
	uint8_t *data_arr = const_cast<uint8_t *>(r.ptr());

	LZ4_decompress_safe(reinterpret_cast<char *>(data_arr), reinterpret_cast<char *>(ch), ds, size);
#else
	//We are not going to write to it
	uint8_t *data_arr = const_cast<uint8_t *>(data.ptr());

	LZ4_decompress_safe(reinterpret_cast<char *>(data_arr), reinterpret_cast<char *>(ch), ds, size);
#endif
}

_FORCE_INLINE_ int VoxelChunk::get_index(const int x, const int y, const int z) const {
	return (y + _margin_start) + _data_size_y * ((x + _margin_start) + _data_size_x * (z + _margin_start));
}

_FORCE_INLINE_ int VoxelChunk::get_data_index(const int x, const int y, const int z) const {
	return y + _data_size_y * (x + _data_size_x * z);
}

_FORCE_INLINE_ int VoxelChunk::get_data_size() const {
	return _data_size_x * _data_size_y * _data_size_z;
}

//Voxel Structures

Ref<VoxelStructure> VoxelChunk::voxel_structure_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_structures.size(), Ref<VoxelStructure>());

	return _voxel_structures.get(index);
}
void VoxelChunk::voxel_structure_add(const Ref<VoxelStructure> &structure) {
	_voxel_structures.push_back(structure);
}
void VoxelChunk::voxel_structure_remove(const Ref<VoxelStructure> &structure) {
	if (!structure.is_valid())
		return;

	int index = _voxel_structures.find(structure);

	if (index != -1)
		_voxel_structures.remove(index);
}
void VoxelChunk::voxel_structure_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _voxel_structures.size());

	_voxel_structures.remove(index);
}
void VoxelChunk::voxel_structure_clear() {
	_voxel_structures.clear();
}
int VoxelChunk::voxel_structure_get_count() const {
	return _voxel_structures.size();
}
void VoxelChunk::voxel_structure_add_at_position(Ref<VoxelStructure> structure, const Vector3 &world_position) {
	ERR_FAIL_COND(!structure.is_valid());

	structure->set_position_x(static_cast<int>(world_position.x / _voxel_scale));
	structure->set_position_y(static_cast<int>(world_position.y / _voxel_scale));
	structure->set_position_z(static_cast<int>(world_position.z / _voxel_scale));

	voxel_structure_add(structure);
}

Vector<Variant> VoxelChunk::voxel_structures_get() {
	VARIANT_ARRAY_GET(_voxel_structures);
}
void VoxelChunk::voxel_structures_set(const Vector<Variant> &structures) {
	voxel_structure_clear();

	for (int i = 0; i < structures.size(); ++i) {
		Ref<VoxelLight> structure = Ref<VoxelLight>(structures[i]);

		voxel_structure_add(structure);
	}
}

void VoxelChunk::build() {
	ERR_FAIL_COND(!INSTANCE_VALIDATE(get_voxel_world()));
	ERR_FAIL_COND(!get_voxel_world()->is_inside_tree());
	ERR_FAIL_COND(!is_in_tree());

	call("_build");
}

void VoxelChunk::_build() {
	if (get_is_generating()) {
		_queued_generation = true;
		return;
	}

	_is_generating = true;

	next_job();
}

void VoxelChunk::clear() {
	ERR_FAIL_COND_MSG(!has_method("_clear"), "VoxelChunk: _clear() is missing! Please implement it!");

	call("_clear");
}

void VoxelChunk::finalize_build() {
	if (has_method("_finalize_build")) {
		call("_finalize_build");
	}
}

void VoxelChunk::bake_lights() {
	if (has_method("_bake_lights"))
		call("_bake_lights");
}
void VoxelChunk::bake_light(Ref<VoxelLight> light) {
	if (!light.is_valid())
		return;

	if (has_method("_bake_lights"))
		call("_bake_light", light);
}
void VoxelChunk::clear_baked_lights() {
	if (has_method("_clear_baked_lights"))
		call("_clear_baked_lights");
}

#if PROPS_PRESENT
void VoxelChunk::add_prop(const Transform &tarnsform, const Ref<PropData> &prop) {
	ERR_FAIL_COND(!prop.is_valid());

	PropDataStore s;
	s.transform = tarnsform;
	s.prop = prop;

	_props.push_back(s);
}
Ref<PropData> VoxelChunk::get_prop(int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<PropData>());

	return _props.get(index).prop;
}
Transform VoxelChunk::get_prop_tarnsform(const int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Transform());

	return _props.get(index).transform;
}
int VoxelChunk::get_prop_count() const {
	return _props.size();
}
void VoxelChunk::remove_prop(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}
void VoxelChunk::clear_props() {
	_props.clear();
}
#endif

#if MESH_DATA_RESOURCE_PRESENT
int VoxelChunk::add_mesh_data_resourcev(const Vector3 &local_data_pos, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture, const Color &color, const bool apply_voxel_scale) {
	ERR_FAIL_COND_V(!mesh.is_valid(), 0);

	int index = _mesh_data_resources.size();

	MeshDataResourceEntry e;

	if (apply_voxel_scale) {
		e.transform = Transform(Basis().scaled(Vector3(_voxel_scale, _voxel_scale, _voxel_scale)));
		e.transform.origin = local_data_pos * _voxel_scale;
	} else {
		e.transform.origin = local_data_pos;
	}

	e.mesh = mesh;
	e.texture = texture;
	e.color = color;

	AABB aabb = AABB(Vector3(), get_world_size());
	AABB mesh_aabb = e.transform.xform(mesh->get_aabb());
	e.is_inside = aabb.encloses(mesh_aabb);

	if (get_library().is_valid() && texture.is_valid()) {
		e.uv_rect = get_library()->get_prop_uv_rect(texture);
	} else {
		e.uv_rect = Rect2(0, 0, 1, 1);
	}

	_mesh_data_resources.push_back(e);

	if (has_method("_mesh_data_resource_added"))
		call("_mesh_data_resource_added", index);

	return index;
}

int VoxelChunk::add_mesh_data_resource(const Transform &local_transform, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture, const Color &color, const bool apply_voxel_scale) {
	ERR_FAIL_COND_V(!mesh.is_valid(), 0);

	int index = _mesh_data_resources.size();

	MeshDataResourceEntry e;

	e.transform = local_transform;

	if (apply_voxel_scale) {
		e.transform.basis = e.transform.basis.scaled(Vector3(_voxel_scale, _voxel_scale, _voxel_scale));
		e.transform.origin = e.transform.origin * _voxel_scale;
	}

	e.mesh = mesh;
	e.texture = texture;
	e.color = color;

	AABB aabb = AABB(Vector3(), get_world_size());
	AABB mesh_aabb = e.transform.xform(mesh->get_aabb());
	e.is_inside = aabb.encloses(mesh_aabb);

	if (get_library().is_valid() && texture.is_valid())
		e.uv_rect = get_library()->get_prop_uv_rect(texture);
	else
		e.uv_rect = Rect2(0, 0, 1, 1);

	_mesh_data_resources.push_back(e);

	if (has_method("_mesh_data_resource_added"))
		call("_mesh_data_resource_added", index);

	return index;
}

Ref<MeshDataResource> VoxelChunk::get_mesh_data_resource(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Ref<MeshDataResource>());

	return _mesh_data_resources[index].mesh;
}

void VoxelChunk::set_mesh_data_resource(const int index, const Ref<MeshDataResource> &mesh) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());
}

Ref<Texture> VoxelChunk::get_mesh_data_resource_texture(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Ref<Texture>());

	return _mesh_data_resources[index].texture;
}
void VoxelChunk::set_mesh_data_resource_texture(const int index, const Ref<Texture> &texture) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].texture = texture;
}

Color VoxelChunk::get_mesh_data_resource_color(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Color());

	return _mesh_data_resources[index].color;
}
void VoxelChunk::set_mesh_data_resource_color(const int index, const Color &color) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].color = color;
}

Rect2 VoxelChunk::get_mesh_data_resource_uv_rect(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Rect2());

	return _mesh_data_resources[index].uv_rect;
}
void VoxelChunk::set_mesh_data_resource_uv_rect(const int index, const Rect2 &uv_rect) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].uv_rect = uv_rect;
}

Transform VoxelChunk::get_mesh_data_resource_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Transform());

	return _mesh_data_resources.write[index].transform;
}
void VoxelChunk::set_mesh_data_resource_transform(const int index, const Transform &transform) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].transform = transform;
}

bool VoxelChunk::get_mesh_data_resource_is_inside(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), true);

	return _mesh_data_resources[index].is_inside;
}
void VoxelChunk::set_mesh_data_resource_is_inside(const int index, const bool &inside) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].is_inside = inside;
}

int VoxelChunk::get_mesh_data_resource_count() const {
	return _mesh_data_resources.size();
}
void VoxelChunk::remove_mesh_data_resource(const int index) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.remove(index);
}
void VoxelChunk::clear_mesh_data_resources() {
	_mesh_data_resources.clear();
}

#endif

int VoxelChunk::add_collider(const Transform &local_transform, const Ref<Shape> &shape, const RID &shape_rid, const RID &body) {
	ERR_FAIL_COND_V(!shape.is_valid() && shape_rid == RID(), 0);

	int index = _colliders.size();

	ColliderBody e;
	e.transform = local_transform;
	e.body = body;
	e.shape = shape;
	e.shape_rid = shape_rid;

	_colliders.push_back(e);

	return index;
}

Transform VoxelChunk::get_collider_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Transform());

	return _colliders[index].transform;
}
void VoxelChunk::set_collider_transform(const int index, const Transform &transform) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].transform = transform;
}

Ref<Shape> VoxelChunk::get_collider_shape(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Ref<Shape>());

	return _colliders[index].shape;
}

void VoxelChunk::set_collider_shape(const int index, const Ref<Shape> &shape) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].shape = shape;
}

RID VoxelChunk::get_collider_shape_rid(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].shape_rid;
}
void VoxelChunk::set_collider_shape_rid(const int index, const RID &rid) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].shape_rid = rid;
}

RID VoxelChunk::get_collider_body(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].body;
}
void VoxelChunk::set_collider_body(const int index, const RID &rid) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].body = rid;
}

int VoxelChunk::get_collider_count() const {
	return _colliders.size();
}
void VoxelChunk::remove_collider(const int index) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.remove(index);
}
void VoxelChunk::clear_colliders() {
	_colliders.clear();
}

void VoxelChunk::enter_tree() {
	_is_in_tree = true;

	if (has_method("_enter_tree"))
		call("_enter_tree");
}
void VoxelChunk::exit_tree() {
	_is_in_tree = false;

	if (has_method("_exit_tree"))
		call("_exit_tree");
}
void VoxelChunk::process(const float delta) {
	if (has_method("_process"))
		call("_process", delta);
}
void VoxelChunk::physics_process(const float delta) {
	if (has_method("_physics_process"))
		call("_physics_process", delta);
}
void VoxelChunk::world_transform_changed() {
	call("_world_transform_changed");
}
void VoxelChunk::visibility_changed(const bool visible) {
	if (has_method("_visibility_changed"))
		call("_visibility_changed", _is_visible);
}
void VoxelChunk::world_light_added(const Ref<VoxelLight> &light) {
	if (has_method("_world_light_added"))
		call("_world_light_added", light);
}
void VoxelChunk::world_light_removed(const Ref<VoxelLight> &light) {
	if (has_method("_world_light_removed"))
		call("_world_light_removed", light);
}
void VoxelChunk::generation_process(const float delta) {
	call("_generation_process", delta);
}
void VoxelChunk::generation_physics_process(const float delta) {
	call("_generation_physics_process", delta);
}

Transform VoxelChunk::get_transform() const {
	return _transform;
}
void VoxelChunk::set_transform(const Transform &transform) {
	_transform = transform;
}

Transform VoxelChunk::get_global_transform() const {

	ERR_FAIL_COND_V(!get_voxel_world(), Transform());

	return get_voxel_world()->get_global_transform() * _transform;
}

Vector3 VoxelChunk::to_local(Vector3 p_global) const {

	return get_global_transform().affine_inverse().xform(p_global);
}

Vector3 VoxelChunk::to_global(Vector3 p_local) const {

	return get_global_transform().xform(p_local);
}

VoxelChunk::VoxelChunk() {
	_is_build_threaded = false;
	_is_processing = false;
	_is_phisics_processing = false;
	_is_in_tree = false;

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

	_current_job = -1;

	_queued_generation = false;
}

VoxelChunk::~VoxelChunk() {
	if (_library.is_valid()) {
		_library.unref();
	}

#if PROPS_PRESENT
	clear_props();
#endif

#if MESH_DATA_RESOURCE_PRESENT
	clear_mesh_data_resources();
#endif

	for (int i = 0; i < _channels.size(); ++i) {
		uint8_t *ch = _channels[i];

		if (ch != NULL) {
			memdelete_arr(ch);
		}
	}

	for (int i = 0; i < _colliders.size(); ++i) {
		PhysicsServer::get_singleton()->free(_colliders[i].body);
	}

	_colliders.clear();

	_jobs.clear();
}

void VoxelChunk::_enter_tree() {
	for (int i = 0; i < _jobs.size(); ++i) {
		Ref<VoxelJob> j = _jobs[i];

		if (j.is_valid()) {
			j->set_chunk(Ref<VoxelChunk>(this));
		}
	}
}

void VoxelChunk::_exit_tree() {
	_abort_build = true;

	for (int i = 0; i < _jobs.size(); ++i) {
		Ref<VoxelJob> j = _jobs[i];

		if (j.is_valid()) {
			j->chunk_exit_tree();
		}
	}
}

void VoxelChunk::_generation_process(const float delta) {
	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size())
		return;

	Ref<VoxelJob> job = _jobs[_current_job];

	ERR_FAIL_COND(!job.is_valid());

	if (job->get_build_phase_type() == VoxelJob::BUILD_PHASE_TYPE_PROCESS) {
		if (!_voxel_world->can_chunk_do_build_step())
			return;

		job->process(delta);

		if (job->get_build_phase_type() == VoxelJob::BUILD_PHASE_TYPE_NORMAL) {
#if THREAD_POOL_PRESENT
			ThreadPool::get_singleton()->add_job(job);
#else
			job->execute();
#endif
		}
	}
}
void VoxelChunk::_generation_physics_process(const float delta) {
	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size())
		return;

	Ref<VoxelJob> job = _jobs[_current_job];

	ERR_FAIL_COND(!job.is_valid());

	if (job->get_build_phase_type() == VoxelJob::BUILD_PHASE_TYPE_PHYSICS_PROCESS) {
		if (!_voxel_world->can_chunk_do_build_step())
			return;

		job->physics_process(delta);

		if (job->get_build_phase_type() == VoxelJob::BUILD_PHASE_TYPE_NORMAL) {
#if THREAD_POOL_PRESENT
			ThreadPool::get_singleton()->add_job(job);
#else
			job->execute();
#endif
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

	BIND_VMETHOD(MethodInfo("_mesh_data_resource_added", PropertyInfo(Variant::INT, "index")));

	BIND_VMETHOD(MethodInfo("_setup_channels"));

	BIND_VMETHOD(MethodInfo("_bake_lights"));
	BIND_VMETHOD(MethodInfo("_bake_light", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "VoxelLight")));
	BIND_VMETHOD(MethodInfo("_clear_baked_lights"));

	ClassDB::bind_method(D_METHOD("bake_lights"), &VoxelChunk::bake_lights);
	ClassDB::bind_method(D_METHOD("bake_light", "light"), &VoxelChunk::bake_light);
	ClassDB::bind_method(D_METHOD("clear_baked_lights"), &VoxelChunk::clear_baked_lights);

	BIND_VMETHOD(MethodInfo("_enter_tree"));
	BIND_VMETHOD(MethodInfo("_exit_tree"));
	BIND_VMETHOD(MethodInfo("_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_physics_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_world_transform_changed"));
	BIND_VMETHOD(MethodInfo("_visibility_changed", PropertyInfo(Variant::BOOL, "visible")));
	BIND_VMETHOD(MethodInfo("_world_light_added", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "VoxelLight")));
	BIND_VMETHOD(MethodInfo("_world_light_removed", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "VoxelLight")));

	BIND_VMETHOD(MethodInfo("_generation_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_generation_physics_process", PropertyInfo(Variant::REAL, "delta")));

	BIND_VMETHOD(MethodInfo("_finalize_build"));

	ClassDB::bind_method(D_METHOD("enter_tree"), &VoxelChunk::enter_tree);
	ClassDB::bind_method(D_METHOD("exit_tree"), &VoxelChunk::exit_tree);
	ClassDB::bind_method(D_METHOD("process", "delta"), &VoxelChunk::process);
	ClassDB::bind_method(D_METHOD("physics_process", "delta"), &VoxelChunk::physics_process);
	ClassDB::bind_method(D_METHOD("world_transform_changed"), &VoxelChunk::world_transform_changed);
	ClassDB::bind_method(D_METHOD("visibility_changed", "visible"), &VoxelChunk::visibility_changed);
	ClassDB::bind_method(D_METHOD("world_light_added", "light"), &VoxelChunk::world_light_added);
	ClassDB::bind_method(D_METHOD("world_light_removed", "light"), &VoxelChunk::world_light_removed);

	ClassDB::bind_method(D_METHOD("generation_process", "delta"), &VoxelChunk::generation_process);
	ClassDB::bind_method(D_METHOD("generation_physics_process", "delta"), &VoxelChunk::generation_physics_process);

	ClassDB::bind_method(D_METHOD("finalize_build"), &VoxelChunk::finalize_build);

	ClassDB::bind_method(D_METHOD("get_process"), &VoxelChunk::get_process);
	ClassDB::bind_method(D_METHOD("set_process", "value"), &VoxelChunk::set_process);

	ClassDB::bind_method(D_METHOD("get_physics_process"), &VoxelChunk::get_physics_process);
	ClassDB::bind_method(D_METHOD("set_physics_process", "value"), &VoxelChunk::set_physics_process);

	ClassDB::bind_method(D_METHOD("is_in_tree"), &VoxelChunk::is_in_tree);

	ClassDB::bind_method(D_METHOD("get_is_build_threaded"), &VoxelChunk::get_is_build_threaded);
	ClassDB::bind_method(D_METHOD("set_is_build_threaded", "value"), &VoxelChunk::set_is_build_threaded);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_build_threaded", PROPERTY_HINT_NONE, "", 0), "set_is_build_threaded", "get_is_build_threaded");

	ClassDB::bind_method(D_METHOD("get_transform"), &VoxelChunk::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &VoxelChunk::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");

	ClassDB::bind_method(D_METHOD("get_visible"), &VoxelChunk::get_visible);
	ClassDB::bind_method(D_METHOD("set_visible", "value"), &VoxelChunk::set_visible);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "visible"), "set_visible", "get_visible");

	ClassDB::bind_method(D_METHOD("get_is_generating"), &VoxelChunk::get_is_generating);
	ClassDB::bind_method(D_METHOD("set_is_generating", "value"), &VoxelChunk::set_is_generating);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_generating", PROPERTY_HINT_NONE, "", 0), "set_is_generating", "get_is_generating");

	ClassDB::bind_method(D_METHOD("get_dirty"), &VoxelChunk::get_dirty);
	ClassDB::bind_method(D_METHOD("set_dirty", "value"), &VoxelChunk::set_dirty);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dirty", PROPERTY_HINT_NONE, "", 0), "set_dirty", "get_dirty");

	ClassDB::bind_method(D_METHOD("get_state"), &VoxelChunk::get_state);
	ClassDB::bind_method(D_METHOD("set_state", "value"), &VoxelChunk::set_state);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "state", PROPERTY_HINT_NONE, "", 0), "set_state", "get_state");

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

	ClassDB::bind_method(D_METHOD("get_world_position"), &VoxelChunk::get_world_position);
	ClassDB::bind_method(D_METHOD("get_world_size"), &VoxelChunk::get_world_size);
	ClassDB::bind_method(D_METHOD("get_world_aabb"), &VoxelChunk::get_world_aabb);

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

	ClassDB::bind_method(D_METHOD("get_job", "index"), &VoxelChunk::get_job);
	ClassDB::bind_method(D_METHOD("set_job", "index", "job"), &VoxelChunk::set_job);
	ClassDB::bind_method(D_METHOD("remove_job", "index"), &VoxelChunk::remove_job);
	ClassDB::bind_method(D_METHOD("add_job", "job"), &VoxelChunk::add_job);
	ClassDB::bind_method(D_METHOD("get_job_count"), &VoxelChunk::get_job_count);

	ClassDB::bind_method(D_METHOD("get_current_job_index"), &VoxelChunk::get_current_job_index);
	ClassDB::bind_method(D_METHOD("next_job"), &VoxelChunk::next_job);
	ClassDB::bind_method(D_METHOD("get_current_job"), &VoxelChunk::get_current_job);

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

	ClassDB::bind_method(D_METHOD("is_channel_allocated", "channel_index"), &VoxelChunk::is_channel_allocated);
	ClassDB::bind_method(D_METHOD("ensure_channel_allocated", "channel_index", "default_value"), &VoxelChunk::ensure_channel_allocated);
	ClassDB::bind_method(D_METHOD("allocate_channel", "channel_index", "default_value"), &VoxelChunk::allocate_channel);
	ClassDB::bind_method(D_METHOD("fill_channel", "value", "channel_index"), &VoxelChunk::fill_channel);
	ClassDB::bind_method(D_METHOD("dealloc_channel", "channel_index"), &VoxelChunk::dealloc_channel);

	ClassDB::bind_method(D_METHOD("get_channel_array", "channel_index"), &VoxelChunk::get_channel_array);
	ClassDB::bind_method(D_METHOD("set_channel_array", "channel_index", "array"), &VoxelChunk::set_channel_array);

	ClassDB::bind_method(D_METHOD("get_channel_compressed", "channel_index"), &VoxelChunk::get_channel_compressed);
	ClassDB::bind_method(D_METHOD("set_channel_compressed", "channel_index", "array"), &VoxelChunk::set_channel_compressed);

	ClassDB::bind_method(D_METHOD("get_index", "x", "y", "z"), &VoxelChunk::get_index);
	ClassDB::bind_method(D_METHOD("get_data_index", "x", "y", "z"), &VoxelChunk::get_data_index);
	ClassDB::bind_method(D_METHOD("get_data_size"), &VoxelChunk::get_data_size);

	ClassDB::bind_method(D_METHOD("voxel_structure_get", "index"), &VoxelChunk::voxel_structure_get);
	ClassDB::bind_method(D_METHOD("voxel_structure_add", "structure"), &VoxelChunk::voxel_structure_add);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove", "structure"), &VoxelChunk::voxel_structure_remove);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove_index", "index"), &VoxelChunk::voxel_structure_remove_index);
	ClassDB::bind_method(D_METHOD("voxel_structure_clear"), &VoxelChunk::voxel_structure_clear);
	ClassDB::bind_method(D_METHOD("voxel_structure_get_count"), &VoxelChunk::voxel_structure_get_count);
	ClassDB::bind_method(D_METHOD("voxel_structure_add_at_position", "structure", "world_position"), &VoxelChunk::voxel_structure_add_at_position);

	ClassDB::bind_method(D_METHOD("voxel_structures_get"), &VoxelChunk::voxel_structures_get);
	ClassDB::bind_method(D_METHOD("voxel_structures_set"), &VoxelChunk::voxel_structures_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_structures", PROPERTY_HINT_NONE, "17/17:VoxelStructure", PROPERTY_USAGE_DEFAULT, "VoxelStructure"), "voxel_structures_set", "voxel_structures_get");

	//Meshes

#if PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("add_prop", "prop"), &VoxelChunk::add_prop);
	ClassDB::bind_method(D_METHOD("get_prop", "index"), &VoxelChunk::get_prop);
	ClassDB::bind_method(D_METHOD("get_prop_count"), &VoxelChunk::get_prop_count);
	ClassDB::bind_method(D_METHOD("remove_prop", "index"), &VoxelChunk::remove_prop);
	ClassDB::bind_method(D_METHOD("clear_props"), &VoxelChunk::clear_props);
#endif

#if MESH_DATA_RESOURCE_PRESENT
	ClassDB::bind_method(D_METHOD("add_mesh_data_resourcev", "local_data_pos", "mesh", "texture", "color", "apply_voxel_scale"), &VoxelChunk::add_mesh_data_resourcev, DEFVAL(Ref<Texture>()), DEFVAL(Color(1, 1, 1, 1)), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource", "local_transform", "mesh", "texture", "color", "apply_voxel_scale"), &VoxelChunk::add_mesh_data_resource, DEFVAL(Ref<Texture>()), DEFVAL(Color(1, 1, 1, 1)), DEFVAL(true));

	ClassDB::bind_method(D_METHOD("get_mesh_data_resource", "index"), &VoxelChunk::get_mesh_data_resource);
	ClassDB::bind_method(D_METHOD("set_mesh_data_resource", "index", "mesh"), &VoxelChunk::set_mesh_data_resource);

	ClassDB::bind_method(D_METHOD("get_mesh_data_resource_texture", "index"), &VoxelChunk::get_mesh_data_resource_texture);
	ClassDB::bind_method(D_METHOD("set_mesh_data_resource_texture", "index", "texture"), &VoxelChunk::set_mesh_data_resource_texture);

	ClassDB::bind_method(D_METHOD("get_mesh_data_resource_color", "index"), &VoxelChunk::get_mesh_data_resource_color);
	ClassDB::bind_method(D_METHOD("set_mesh_data_resource_color", "index", "color"), &VoxelChunk::set_mesh_data_resource_color);

	ClassDB::bind_method(D_METHOD("get_mesh_data_resource_uv_rect", "index"), &VoxelChunk::get_mesh_data_resource_uv_rect);
	ClassDB::bind_method(D_METHOD("set_mesh_data_resource_uv_rect", "index", "uv_rect"), &VoxelChunk::set_mesh_data_resource_uv_rect);

	ClassDB::bind_method(D_METHOD("get_mesh_data_resource_transform", "index"), &VoxelChunk::get_mesh_data_resource_transform);
	ClassDB::bind_method(D_METHOD("set_mesh_data_resource_transform", "index", "transform"), &VoxelChunk::set_mesh_data_resource_transform);

	ClassDB::bind_method(D_METHOD("get_mesh_data_resource_is_inside", "index"), &VoxelChunk::get_mesh_data_resource_is_inside);
	ClassDB::bind_method(D_METHOD("set_mesh_data_resource_is_inside", "index", "inside"), &VoxelChunk::set_mesh_data_resource_is_inside);

	ClassDB::bind_method(D_METHOD("get_mesh_data_resource_count"), &VoxelChunk::get_mesh_data_resource_count);
	ClassDB::bind_method(D_METHOD("remove_mesh_data_resource", "index"), &VoxelChunk::remove_mesh_data_resource);
	ClassDB::bind_method(D_METHOD("clear_mesh_data_resources"), &VoxelChunk::clear_mesh_data_resources);
#endif

	ClassDB::bind_method(D_METHOD("add_collider", "local_transform", "shape", "shape_rid", "body"), &VoxelChunk::add_collider, DEFVAL(RID()), DEFVAL(RID()));

	ClassDB::bind_method(D_METHOD("get_collider_transform", "index"), &VoxelChunk::get_collider_transform);
	ClassDB::bind_method(D_METHOD("set_collider_transform", "index", "transform"), &VoxelChunk::set_collider_transform);

	ClassDB::bind_method(D_METHOD("get_collider_shape", "index"), &VoxelChunk::get_collider_shape);
	ClassDB::bind_method(D_METHOD("set_collider_shape", "index", "shape"), &VoxelChunk::set_collider_shape);

	ClassDB::bind_method(D_METHOD("get_collider_shape_rid", "index"), &VoxelChunk::get_collider_shape_rid);
	ClassDB::bind_method(D_METHOD("set_collider_shape_rid", "index", "rid"), &VoxelChunk::set_collider_shape_rid);

	ClassDB::bind_method(D_METHOD("get_collider_body", "index"), &VoxelChunk::get_collider_body);
	ClassDB::bind_method(D_METHOD("set_collider_body", "index", "rid"), &VoxelChunk::set_collider_body);

	ClassDB::bind_method(D_METHOD("get_collider_count"), &VoxelChunk::get_collider_count);
	ClassDB::bind_method(D_METHOD("remove_collider", "index"), &VoxelChunk::remove_collider);
	ClassDB::bind_method(D_METHOD("clear_colliders"), &VoxelChunk::clear_colliders);

	BIND_VMETHOD(MethodInfo("_build"));
	ClassDB::bind_method(D_METHOD("build"), &VoxelChunk::build);
	ClassDB::bind_method(D_METHOD("_build"), &VoxelChunk::_build);

	ClassDB::bind_method(D_METHOD("get_global_transform"), &VoxelChunk::get_global_transform);
	ClassDB::bind_method(D_METHOD("to_local", "global"), &VoxelChunk::to_local);
	ClassDB::bind_method(D_METHOD("to_global", "local"), &VoxelChunk::to_global);

	ClassDB::bind_method(D_METHOD("_world_transform_changed"), &VoxelChunk::_world_transform_changed);
	ClassDB::bind_method(D_METHOD("_enter_tree"), &VoxelChunk::_enter_tree);
	ClassDB::bind_method(D_METHOD("_exit_tree"), &VoxelChunk::_exit_tree);

	ClassDB::bind_method(D_METHOD("_generation_process"), &VoxelChunk::_generation_process);
	ClassDB::bind_method(D_METHOD("_generation_physics_process"), &VoxelChunk::_generation_physics_process);
}
