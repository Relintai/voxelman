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

#include "voxel_chunk_default.h"

#include "../../defines.h"

#include visual_server_h
#include physics_server_h

#include "../../../opensimplex/open_simplex_noise.h"
#include "../../meshers/default/voxel_mesher_default.h"
#include "../voxel_world.h"

#include "../jobs/voxel_job.h"

#include "voxel_world_default.h"

#include "../jobs/voxel_light_job.h"
#include "../jobs/voxel_prop_job.h"
#include "../jobs/voxel_terrain_job.h"

const String VoxelChunkDefault::BINDING_STRING_BUILD_FLAGS = "Use Isolevel,Use Lighting,Use AO,Use RAO,Generate AO,Generate RAO,Bake Lights,Create Collider,Create Lods";

_FORCE_INLINE_ int VoxelChunkDefault::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void VoxelChunkDefault::set_build_flags(const int flags) {
	_build_flags = flags;
}

bool VoxelChunkDefault::get_lights_dirty() const {
	return _lights_dirty;
}
void VoxelChunkDefault::set_lights_dirty(const bool value) {
	_lights_dirty = value;
}

int VoxelChunkDefault::get_lod_num() const {
	return _lod_num;
}
void VoxelChunkDefault::set_lod_num(const int value) {
	_lod_num = value;
}

int VoxelChunkDefault::get_current_lod_level() const {
	return _current_lod_level;
}
void VoxelChunkDefault::set_current_lod_level(const int value) {
	_current_lod_level = value;

	if ((_build_flags & BUILD_FLAG_CREATE_LODS) == 0)
		return;

	if (_current_lod_level < 0)
		_current_lod_level = 0;

	if (_current_lod_level > _lod_num)
		_current_lod_level = _lod_num;

	for (int i = 0; i < _lod_num + 1; ++i) {
		bool vis = false;

		if (i == _current_lod_level)
			vis = true;

		RID rid = mesh_rid_get_index(MESH_INDEX_TERRAIN, MESH_TYPE_INDEX_MESH_INSTANCE, i);

		if (rid != RID())
			VisualServer::get_singleton()->instance_set_visible(rid, vis);

		rid = mesh_rid_get_index(MESH_INDEX_PROP, MESH_TYPE_INDEX_MESH_INSTANCE, i);

		if (rid != RID())
			VisualServer::get_singleton()->instance_set_visible(rid, vis);
	}
}

void VoxelChunkDefault::emit_build_finished() {
	emit_signal("mesh_generation_finished", this);

	if (_voxel_world != NULL) {
		_voxel_world->on_chunk_mesh_generation_finished(this);
	}
}

//Meshes
Dictionary VoxelChunkDefault::mesh_rids_get() {
	return _rids;
}
void VoxelChunkDefault::mesh_rids_set(const Dictionary &rids) {
	_rids = rids;
}

RID VoxelChunkDefault::mesh_rid_get(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return RID();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return RID();

	Variant v = m[mesh_type_index];

#if VERSION_MAJOR > 3
	if (v.get_type() != Variant::RID)
		return RID();
#else
	if (v.get_type() != Variant::_RID)
		return RID();
#endif

	return v;
}
void VoxelChunkDefault::mesh_rid_set(const int mesh_index, const int mesh_type_index, RID value) {
	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index)) {
		m[mesh_type_index] = value;
		_rids[mesh_index] = m;
		return;
	}

	Variant v = m[mesh_type_index];

#if VERSION_MAJOR > 3
	ERR_FAIL_COND(v.get_type() != Variant::RID);
#else
	ERR_FAIL_COND(v.get_type() != Variant::_RID);
#endif

	m[mesh_type_index] = value;
	_rids[mesh_index] = m;
}
RID VoxelChunkDefault::mesh_rid_get_index(const int mesh_index, const int mesh_type_index, const int index) {
	if (!_rids.has(mesh_index))
		return RID();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return RID();

	Variant v = m[mesh_type_index];

	if (v.get_type() != Variant::ARRAY)
		return RID();

	Array arr = v;

	ERR_FAIL_INDEX_V(index, arr.size(), RID());

	return arr[index];
}
void VoxelChunkDefault::mesh_rid_set_index(const int mesh_index, const int mesh_type_index, const int index, RID value) {
	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index)) {
		Array arr;
		arr.resize(index + 1);
		arr[index] = value;

		m[mesh_type_index] = arr;
		_rids[mesh_index] = m;
		return;
	}

	Variant v = m[mesh_type_index];

	ERR_FAIL_COND(v.get_type() != Variant::ARRAY);

	Array arr = m[mesh_type_index];

	if (arr.size() <= index)
		arr.resize(index + 1);

	arr[index] = value;

	m[mesh_type_index] = arr;
	_rids[mesh_index] = m;
}
int VoxelChunkDefault::mesh_rid_get_count(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return 0;

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return 0;

	Variant v = m[mesh_type_index];

	if (v.get_type() != Variant::ARRAY)
		return 0;

	Array arr = v;

	return arr.size();
}
void VoxelChunkDefault::mesh_rids_clear(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return;

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return;

	m.erase(mesh_type_index);
}
Array VoxelChunkDefault::meshes_get(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return Array();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return Array();

	Variant v = m[mesh_type_index];

	if (v.get_type() != Variant::ARRAY)
		return Array();

	return v;
}
void VoxelChunkDefault::meshes_set(const int mesh_index, const int mesh_type_index, const Array &meshes) {
	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	m[mesh_type_index] = meshes;
	_rids[mesh_index] = m;
}
bool VoxelChunkDefault::meshes_has(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return false;

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return false;

	return true;
}

void VoxelChunkDefault::rids_clear() {
	_rids.clear();
}

void VoxelChunkDefault::rids_free() {
	List<Variant> keys;

	_rids.get_key_list(&keys);

	for (List<Variant>::Element *E = keys.front(); E; E = E->next()) {
		Variant v = E->get();

		if (v.get_type() != Variant::INT)
			continue;

		free_index(v);
	}
}

void VoxelChunkDefault::meshes_create(const int mesh_index, const int mesh_count) {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(!get_library().is_valid());

	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_MESH));
	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_MESH_INSTANCE));

	Array am;
	Array ami;

	for (int i = 0; i < mesh_count; ++i) {
		RID mesh_instance_rid = VS::get_singleton()->instance_create();

		if (get_voxel_world()->GET_WORLD().is_valid())
			VS::get_singleton()->instance_set_scenario(mesh_instance_rid, get_voxel_world()->GET_WORLD()->get_scenario());

		RID mesh_rid = VS::get_singleton()->mesh_create();

		VS::get_singleton()->instance_set_base(mesh_instance_rid, mesh_rid);

		VS::get_singleton()->instance_set_transform(mesh_instance_rid, get_transform());

		if (i != 0)
			VS::get_singleton()->instance_set_visible(mesh_instance_rid, false);

		am.push_back(mesh_rid);
		ami.push_back(mesh_instance_rid);
	}

	m[MESH_TYPE_INDEX_MESH] = am;
	m[MESH_TYPE_INDEX_MESH_INSTANCE] = ami;

	_rids[mesh_index] = m;
}
void VoxelChunkDefault::meshes_free(const int mesh_index) {
	if (!_rids.has(mesh_index))
		return;

	Dictionary m = _rids[mesh_index];
	RID rid;

	if (m.has(MESH_TYPE_INDEX_MESH)) {
		Array a = m[MESH_TYPE_INDEX_MESH];

		for (int i = 0; i < a.size(); ++i) {
			RID r = a[i];

			if (r != rid) {
				VS::get_singleton()->free(r);
			}
		}
	}

	if (m.has(MESH_TYPE_INDEX_MESH_INSTANCE)) {
		Array a = m[MESH_TYPE_INDEX_MESH_INSTANCE];

		for (int i = 0; i < a.size(); ++i) {
			RID r = a[i];

			if (r != rid) {
				VS::get_singleton()->free(r);
			}
		}
	}

	m.erase(MESH_TYPE_INDEX_MESH);
	m.erase(MESH_TYPE_INDEX_MESH_INSTANCE);
}

void VoxelChunkDefault::colliders_create(const int mesh_index, const int layer_mask) {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(PhysicsServer::get_singleton()->is_flushing_queries());
	//ERR_FAIL_COND(!get_voxel_world()->is_inside_tree());

	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_BODY));
	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_SHAPE));

	RID shape_rid = PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_CONCAVE_POLYGON);
#if VERSION_MAJOR < 4
	RID body_rid = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);
#else
	RID body_rid = PhysicsServer::get_singleton()->body_create();
	PhysicsServer::get_singleton()->body_set_mode(body_rid, PhysicsServer::BODY_MODE_STATIC);
#endif

	PhysicsServer::get_singleton()->body_set_collision_layer(body_rid, layer_mask);
	PhysicsServer::get_singleton()->body_set_collision_mask(body_rid, layer_mask);

	PhysicsServer::get_singleton()->body_add_shape(body_rid, shape_rid);

	PhysicsServer::get_singleton()->body_set_state(body_rid, PhysicsServer::BODY_STATE_TRANSFORM, get_transform());

	if (get_voxel_world()->is_inside_tree() && get_voxel_world()->is_inside_world()) {
		Ref<World> world = get_voxel_world()->GET_WORLD();

		if (world.is_valid() && world->get_space() != RID())
			PhysicsServer::get_singleton()->body_set_space(body_rid, world->get_space());
	}

	m[MESH_TYPE_INDEX_BODY] = body_rid;
	m[MESH_TYPE_INDEX_SHAPE] = shape_rid;

	_rids[mesh_index] = m;
}
void VoxelChunkDefault::colliders_create_area(const int mesh_index, const int layer_mask) {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(PhysicsServer::get_singleton()->is_flushing_queries());

	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_AREA));
	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_SHAPE));

	RID shape_rid = PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_CONCAVE_POLYGON);
	RID area_rid = PhysicsServer::get_singleton()->area_create();

	PhysicsServer::get_singleton()->area_attach_object_instance_id(area_rid, _voxel_world->get_instance_id());
	PhysicsServer::get_singleton()->area_set_param(area_rid, PhysicsServer::AREA_PARAM_GRAVITY, 9.8);
	PhysicsServer::get_singleton()->area_set_param(area_rid, PhysicsServer::AREA_PARAM_GRAVITY_VECTOR, Vector3(0, -1, 0));

	//PhysicsServer::get_singleton()->area_set_monitor_callback(area_rid, this, "_body_area_inout");
	//PhysicsServer::get_singleton()->area_set_area_monitor_callback(area_rid, this, "_body_area_area_inout");
	//PhysicsServer::get_singleton()->area_set_monitorable(area_rid, true);

	PhysicsServer::get_singleton()->area_set_collision_layer(area_rid, layer_mask);
	PhysicsServer::get_singleton()->area_set_collision_mask(area_rid, layer_mask);

	if (get_voxel_world()->is_inside_tree() && get_voxel_world()->is_inside_world()) {
		Ref<World> world = get_voxel_world()->GET_WORLD();

		if (world.is_valid() && world->get_space() != RID())
			PhysicsServer::get_singleton()->area_set_space(area_rid, world->get_space());
	}

	PhysicsServer::get_singleton()->area_add_shape(area_rid, shape_rid, get_transform());

	m[MESH_TYPE_INDEX_AREA] = area_rid;
	m[MESH_TYPE_INDEX_SHAPE] = shape_rid;

	_rids[mesh_index] = m;
}

void VoxelChunkDefault::colliders_free(const int mesh_index) {
	if (!_rids.has(mesh_index))
		return;

	Dictionary m = _rids[mesh_index];
	RID rid;

	if (m.has(MESH_TYPE_INDEX_SHAPE)) {
		RID r = m[MESH_TYPE_INDEX_SHAPE];

		PhysicsServer::get_singleton()->free(r);
	}

	if (m.has(MESH_TYPE_INDEX_BODY)) {
		RID r = m[MESH_TYPE_INDEX_BODY];

		PhysicsServer::get_singleton()->free(r);
	}

	m.erase(MESH_TYPE_INDEX_SHAPE);
	m.erase(MESH_TYPE_INDEX_BODY);

	_rids[mesh_index] = m;
}

void VoxelChunkDefault::free_index(const int mesh_index) {
	meshes_free(mesh_index);
	colliders_free(mesh_index);
}

void VoxelChunkDefault::update_transforms() {
	RID empty_rid;
	Transform t = get_transform();

	List<Variant> keys;

	_rids.get_key_list(&keys);

	for (List<Variant>::Element *E = keys.front(); E; E = E->next()) {
		Variant v = E->get();

		if (v.get_type() != Variant::INT)
			continue;

		Dictionary d = _rids[v];

		if (d.has(MESH_TYPE_INDEX_MESH_INSTANCE)) {
			Array arr = d[MESH_TYPE_INDEX_MESH_INSTANCE];

			for (int i = 0; i < arr.size(); ++i) {
				RID rid = arr[i];

				if (rid != empty_rid)
					VS::get_singleton()->instance_set_transform(rid, get_transform());
			}
		}

		if (d.has(MESH_TYPE_INDEX_BODY)) {
			RID rid = d[MESH_TYPE_INDEX_BODY];

			if (rid != empty_rid)
				PhysicsServer::get_singleton()->body_set_state(rid, PhysicsServer::BODY_STATE_TRANSFORM, t);
		}

		if (d.has(MESH_TYPE_INDEX_AREA)) {
			RID rid = d[MESH_TYPE_INDEX_AREA];

			if (rid != empty_rid)
				PhysicsServer::get_singleton()->area_set_shape_transform(rid, 0, t);
		}
	}

	for (int i = 0; i < collider_get_count(); ++i) {
		PhysicsServer::get_singleton()->body_set_state(collider_get_body(i), PhysicsServer::BODY_STATE_TRANSFORM, get_transform() * collider_get_transform(i));
	}

	if (_debug_mesh_instance != RID()) {
		VS::get_singleton()->instance_set_transform(_debug_mesh_instance, get_transform());
	}
}

//Lights
Ref<VoxelLight> VoxelChunkDefault::get_light(const int index) {
	ERR_FAIL_INDEX_V(index, _lights.size(), Ref<VoxelLight>());

	return _lights.get(index);
}
int VoxelChunkDefault::get_light_count() const {
	return _lights.size();
}

void VoxelChunkDefault::debug_mesh_allocate() {
	if (_debug_mesh_rid == RID()) {
		_debug_mesh_rid = VisualServer::get_singleton()->mesh_create();
	}

	if (_debug_mesh_instance == RID()) {
		_debug_mesh_instance = VisualServer::get_singleton()->instance_create();

		if (get_voxel_world()->GET_WORLD().is_valid())
			VS::get_singleton()->instance_set_scenario(_debug_mesh_instance, get_voxel_world()->GET_WORLD()->get_scenario());

		VS::get_singleton()->instance_set_base(_debug_mesh_instance, _debug_mesh_rid);
		VS::get_singleton()->instance_set_transform(_debug_mesh_instance, get_transform());
		VS::get_singleton()->instance_set_visible(_debug_mesh_instance, true);
	}
}
void VoxelChunkDefault::debug_mesh_free() {
	if (_debug_mesh_instance != RID()) {
		VisualServer::get_singleton()->free(_debug_mesh_instance);
	}

	if (_debug_mesh_rid != RID()) {
		VisualServer::get_singleton()->free(_debug_mesh_rid);
	}
}
bool VoxelChunkDefault::debug_mesh_has() {
	return _debug_mesh_rid != RID();
}
void VoxelChunkDefault::debug_mesh_clear() {
	if (_debug_mesh_rid != RID()) {
		VisualServer::get_singleton()->mesh_clear(_debug_mesh_rid);
	}
}
void VoxelChunkDefault::debug_mesh_array_clear() {
	_debug_mesh_array.resize(0);
}
void VoxelChunkDefault::debug_mesh_add_vertices_to(const PoolVector3Array &arr) {
	_debug_mesh_array.append_array(arr);

	if (_debug_mesh_array.size() % 2 == 1) {
		_debug_mesh_array.append(_debug_mesh_array[_debug_mesh_array.size() - 1]);
	}
}
void VoxelChunkDefault::debug_mesh_send() {
	debug_mesh_allocate();
	debug_mesh_clear();

	if (_debug_mesh_array.size() == 0)
		return;

	SceneTree *st = SceneTree::get_singleton();

	Array arr;
	arr.resize(VisualServer::ARRAY_MAX);
	arr[VisualServer::ARRAY_VERTEX] = _debug_mesh_array;

	VisualServer::get_singleton()->mesh_add_surface_from_arrays(_debug_mesh_rid, VisualServer::PRIMITIVE_LINES, arr);

	if (st) {
		VisualServer::get_singleton()->mesh_surface_set_material(_debug_mesh_rid, 0, SceneTree::get_singleton()->get_debug_collision_material()->get_rid());
	}

	debug_mesh_array_clear();
}

void VoxelChunkDefault::draw_cross_voxels(Vector3 pos) {
	pos *= _voxel_scale;

	int size = _debug_mesh_array.size();
	_debug_mesh_array.resize(_debug_mesh_array.size() + 6);

	_debug_mesh_array.set(size, pos + Vector3(0, 0, -0.2));
	_debug_mesh_array.set(size + 1, pos + Vector3(0, 0, 0.2));

	_debug_mesh_array.set(size + 2, pos + Vector3(0, -0.2, 0));
	_debug_mesh_array.set(size + 3, pos + Vector3(0, 0.2, 0));

	_debug_mesh_array.set(size + 4, pos + Vector3(-0.2, 0, 0));
	_debug_mesh_array.set(size + 5, pos + Vector3(0.2, 0, 0));
}

void VoxelChunkDefault::draw_cross_voxels_fill(Vector3 pos, float fill) {
	pos *= _voxel_scale;

	int size = _debug_mesh_array.size();
	_debug_mesh_array.resize(_debug_mesh_array.size() + 6);

	_debug_mesh_array.set(size, pos + Vector3(0, 0, -0.2 * fill));
	_debug_mesh_array.set(size + 1, pos + Vector3(0, 0, 0.2 * fill));

	_debug_mesh_array.set(size + 2, pos + Vector3(0, -0.2 * fill, 0));
	_debug_mesh_array.set(size + 3, pos + Vector3(0, 0.2 * fill, 0));

	_debug_mesh_array.set(size + 4, pos + Vector3(-0.2 * fill, 0, 0));
	_debug_mesh_array.set(size + 5, pos + Vector3(0.2 * fill, 0, 0));
}

void VoxelChunkDefault::draw_debug_voxels(int max, Color color) {
	if (!debug_mesh_has()) {
		debug_mesh_allocate();
	}

	//debug_mesh_array_clear();

	//_debug_drawer->begin(Mesh::PRIMITIVE_LINES);

	int a = 0;

	int64_t sx = static_cast<int64_t>(_size_x);
	int64_t sy = static_cast<int64_t>(_size_y);
	int64_t sz = static_cast<int64_t>(_size_y);

	for (int y = 0; y < sy; ++y) {
		for (int z = 0; z < sz; ++z) {
			for (int x = 0; x < sx; ++x) {
				int type = get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE);

				if (type == 0) {
					continue;
				}

				draw_cross_voxels_fill(Vector3(x, y, z), get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL) / 255.0 * get_voxel_scale() * 2.0);

				++a;

				if (a > max) {
					break;
				}
			}
		}
	}

	debug_mesh_send();
}

void VoxelChunkDefault::draw_debug_voxel_lights() {
	if (!debug_mesh_has()) {
		debug_mesh_allocate();
	}

	//debug_mesh_array_clear();

	//_debug_drawer->begin(Mesh::PrimitiveType::PRIMITIVE_LINES);

	for (int i = 0; i < _lights.size(); ++i) {
		Ref<VoxelLight> v = _lights[i];

		int pos_x = v->get_world_position_x() - (_size_x * _position_x);
		int pos_y = v->get_world_position_y() - (_size_y * _position_y);
		int pos_z = v->get_world_position_z() - (_size_z * _position_z);

		draw_cross_voxels_fill(Vector3(pos_x, pos_y, pos_z), 1.0);
	}

	debug_mesh_send();
}

void VoxelChunkDefault::draw_debug_mdr_colliders() {
	if (!debug_mesh_has()) {
		debug_mesh_allocate();
	}

	for (int i = 0; i < collider_get_count(); ++i) {
		Ref<Shape> shape = collider_get_shape(i);

		if (!shape.is_valid())
			continue;

		Transform t = collider_get_transform(i);

		shape->add_vertices_to_array(_debug_mesh_array, t);
	}
}

void VoxelChunkDefault::_visibility_changed(bool visible) {
	if (visible) {
		set_current_lod_level(_current_lod_level);
		return;
	}

	for (int i = 0; i < _lod_num + 1; ++i) {
		RID rid = mesh_rid_get_index(MESH_INDEX_TERRAIN, MESH_TYPE_INDEX_MESH_INSTANCE, i);

		if (rid != RID())
			VisualServer::get_singleton()->instance_set_visible(rid, false);

		rid = mesh_rid_get_index(MESH_INDEX_LIQUID, MESH_TYPE_INDEX_MESH_INSTANCE, i);

		if (rid != RID())
			VisualServer::get_singleton()->instance_set_visible(rid, false);

		rid = mesh_rid_get_index(MESH_INDEX_PROP, MESH_TYPE_INDEX_MESH_INSTANCE, i);

		if (rid != RID())
			VisualServer::get_singleton()->instance_set_visible(rid, false);
	}
}

void VoxelChunkDefault::_exit_tree() {
	VoxelChunk::_exit_tree();

	rids_free();
}

void VoxelChunkDefault::_world_transform_changed() {
	VoxelChunk::_world_transform_changed();

	update_transforms();
}

//Lights
void VoxelChunkDefault::_bake_lights() {
	clear_baked_lights();

	for (int i = 0; i < _lights.size(); ++i) {
		bake_light(_lights.get(i));
	}
}
void VoxelChunkDefault::_bake_light(Ref<VoxelLight> light) {
	ERR_FAIL_COND(!light.is_valid());

	Color color = light->get_color();
	int size = light->get_size();

	int local_x = light->get_world_position_x() - (_position_x * _size_x);
	int local_y = light->get_world_position_y() - (_position_y * _size_y);
	int local_z = light->get_world_position_z() - (_position_z * _size_z);

	ERR_FAIL_COND(size < 0);

	int64_t dsx = static_cast<int64_t>(_data_size_x);
	int64_t dsy = static_cast<int64_t>(_data_size_y);
	int64_t dsz = static_cast<int64_t>(_data_size_z);

	uint8_t *channel_color_r = channel_get(VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
	uint8_t *channel_color_g = channel_get(VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
	uint8_t *channel_color_b = channel_get(VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

	ERR_FAIL_COND(channel_color_r == NULL || channel_color_g == NULL || channel_color_b == NULL);

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

				int index = get_data_index(x, y, z);

				int r = color.r * str * 255.0;
				int g = color.g * str * 255.0;
				int b = color.b * str * 255.0;

				r += channel_color_r[index];
				g += channel_color_g[index];
				b += channel_color_b[index];

				if (r > 255)
					r = 255;

				if (g > 255)
					g = 255;

				if (b > 255)
					b = 255;

				channel_color_r[index] = r;
				channel_color_g[index] = g;
				channel_color_b[index] = b;
			}
		}
	}
}
void VoxelChunkDefault::_clear_baked_lights() {
	channel_fill(0, DEFAULT_CHANNEL_LIGHT_COLOR_R);
	channel_fill(0, DEFAULT_CHANNEL_LIGHT_COLOR_G);
	channel_fill(0, DEFAULT_CHANNEL_LIGHT_COLOR_B);
}
void VoxelChunkDefault::_world_light_added(const Ref<VoxelLight> &light) {
	_lights.push_back(light);

	set_lights_dirty(true);
}
void VoxelChunkDefault::_world_light_removed(const Ref<VoxelLight> &light) {
	int index = _lights.find(light);

	if (index != -1) {
#if VERSION_MAJOR < 4
		_lights.remove(index);
#else
		_lights.remove_at(index);
#endif

		set_lights_dirty(true);
	}
}

void VoxelChunkDefault::free_chunk() {
	rids_free();
}

void VoxelChunkDefault::_finalize_build() {
	ERR_FAIL_COND(!_library.is_valid());

#if TOOLS_ENABLED
	if (_debug_mesh_array.size() > 0) {
		debug_mesh_send();
	}
#endif

	set_current_lod_level(get_current_lod_level());

	call_deferred("update_transforms");
}

VoxelChunkDefault::VoxelChunkDefault() {
	_abort_build = false;

	_enabled = true;

	_lod_num = 3;
	_current_lod_level = 0;

	_build_flags = BUILD_FLAG_CREATE_COLLIDER | BUILD_FLAG_CREATE_LODS;
}

VoxelChunkDefault::~VoxelChunkDefault() {
	_abort_build = true;

	_lights.clear();

	debug_mesh_free();
}

void VoxelChunkDefault::_channel_setup() {
	channel_set_count(MAX_DEFAULT_CHANNELS);
}

void VoxelChunkDefault::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_build_flags"), &VoxelChunkDefault::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &VoxelChunkDefault::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, BINDING_STRING_BUILD_FLAGS, 0), "set_build_flags", "get_build_flags");

	ClassDB::bind_method(D_METHOD("get_lights_dirty"), &VoxelChunkDefault::get_lights_dirty);
	ClassDB::bind_method(D_METHOD("set_lights_dirty", "value"), &VoxelChunkDefault::set_lights_dirty);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "lights_dirty", PROPERTY_HINT_NONE, "", 0), "set_lights_dirty", "get_lights_dirty");

	ClassDB::bind_method(D_METHOD("get_lod_num"), &VoxelChunkDefault::get_lod_num);
	ClassDB::bind_method(D_METHOD("set_lod_num"), &VoxelChunkDefault::set_lod_num);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_num", PROPERTY_HINT_NONE, "", 0), "set_lod_num", "get_lod_num");

	ClassDB::bind_method(D_METHOD("get_current_lod_level"), &VoxelChunkDefault::get_current_lod_level);
	ClassDB::bind_method(D_METHOD("set_current_lod_level"), &VoxelChunkDefault::set_current_lod_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_lod_level"), "set_current_lod_level", "get_current_lod_level");

	//Meshes
	ClassDB::bind_method(D_METHOD("get_mesh_rids"), &VoxelChunkDefault::mesh_rids_get);
	ClassDB::bind_method(D_METHOD("set_mesh_rids", "rids"), &VoxelChunkDefault::mesh_rids_set);
	ClassDB::bind_method(D_METHOD("clear_rids"), &VoxelChunkDefault::rids_clear);

	ClassDB::bind_method(D_METHOD("mesh_rid_get", "mesh_index", "mesh_type_index"), &VoxelChunkDefault::mesh_rid_get);
	ClassDB::bind_method(D_METHOD("mesh_rid_set", "mesh_index", "mesh_type_index", "value"), &VoxelChunkDefault::mesh_rid_set);
	ClassDB::bind_method(D_METHOD("mesh_rid_get_index", "mesh_index", "mesh_type_index", "index"), &VoxelChunkDefault::mesh_rid_get_index);
	ClassDB::bind_method(D_METHOD("mesh_rid_set_index", "mesh_index", "mesh_type_index", "index", "value"), &VoxelChunkDefault::mesh_rid_set_index);
	ClassDB::bind_method(D_METHOD("mesh_rid_get_count", "mesh_index", "mesh_type_index"), &VoxelChunkDefault::mesh_rid_get_count);
	ClassDB::bind_method(D_METHOD("mesh_rids_clear", "mesh_index", "mesh_type_index"), &VoxelChunkDefault::mesh_rids_clear);
	ClassDB::bind_method(D_METHOD("meshes_get", "mesh_index", "mesh_type_index"), &VoxelChunkDefault::meshes_get);
	ClassDB::bind_method(D_METHOD("meshes_set", "mesh_index", "mesh_type_index", "meshes"), &VoxelChunkDefault::meshes_set);
	ClassDB::bind_method(D_METHOD("meshes_has", "mesh_index", "mesh_type_index"), &VoxelChunkDefault::meshes_has);

	ClassDB::bind_method(D_METHOD("rids_free"), &VoxelChunkDefault::rids_free);
	ClassDB::bind_method(D_METHOD("free_index", "mesh_index"), &VoxelChunkDefault::free_index);

	ClassDB::bind_method(D_METHOD("meshes_create", "mesh_index", "mesh_count"), &VoxelChunkDefault::meshes_create);
	ClassDB::bind_method(D_METHOD("meshes_free", "mesh_index"), &VoxelChunkDefault::meshes_free);

	ClassDB::bind_method(D_METHOD("create_colliders", "mesh_index", "layer_mask"), &VoxelChunkDefault::colliders_create, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("free_colliders", "mesh_index"), &VoxelChunkDefault::colliders_free);

	//Lights
	ClassDB::bind_method(D_METHOD("get_light", "index"), &VoxelChunkDefault::get_light);
	ClassDB::bind_method(D_METHOD("get_light_count"), &VoxelChunkDefault::get_light_count);

	//Debug
	ClassDB::bind_method(D_METHOD("debug_mesh_allocate"), &VoxelChunkDefault::debug_mesh_allocate);
	ClassDB::bind_method(D_METHOD("debug_mesh_free"), &VoxelChunkDefault::debug_mesh_free);

	ClassDB::bind_method(D_METHOD("debug_mesh_has"), &VoxelChunkDefault::debug_mesh_has);
	ClassDB::bind_method(D_METHOD("debug_mesh_clear"), &VoxelChunkDefault::debug_mesh_clear);
	ClassDB::bind_method(D_METHOD("debug_mesh_array_clear"), &VoxelChunkDefault::debug_mesh_array_clear);
	ClassDB::bind_method(D_METHOD("debug_mesh_add_vertices_to", "arr"), &VoxelChunkDefault::debug_mesh_add_vertices_to);
	ClassDB::bind_method(D_METHOD("debug_mesh_send"), &VoxelChunkDefault::debug_mesh_send);

	ClassDB::bind_method(D_METHOD("draw_cross_voxels", "max"), &VoxelChunkDefault::draw_cross_voxels);
	ClassDB::bind_method(D_METHOD("draw_cross_voxels_fill", "max", "fill"), &VoxelChunkDefault::draw_cross_voxels_fill);
	ClassDB::bind_method(D_METHOD("draw_debug_voxels", "max", "color"), &VoxelChunkDefault::draw_debug_voxels, DEFVAL(Color(1, 1, 1)));

	ClassDB::bind_method(D_METHOD("draw_debug_voxel_lights"), &VoxelChunkDefault::draw_debug_voxel_lights);
	ClassDB::bind_method(D_METHOD("draw_debug_mdr_colliders"), &VoxelChunkDefault::draw_debug_mdr_colliders);

	//Free
	ClassDB::bind_method(D_METHOD("free_chunk"), &VoxelChunkDefault::free_chunk);

	//etc
	ClassDB::bind_method(D_METHOD("emit_build_finished"), &VoxelChunkDefault::emit_build_finished);

	//virtuals
	ClassDB::bind_method(D_METHOD("_channel_setup"), &VoxelChunkDefault::_channel_setup);

	ClassDB::bind_method(D_METHOD("_visibility_changed", "visible"), &VoxelChunkDefault::_visibility_changed);

	//lights
	ClassDB::bind_method(D_METHOD("_bake_lights"), &VoxelChunkDefault::_bake_lights);
	ClassDB::bind_method(D_METHOD("_bake_light", "light"), &VoxelChunkDefault::_bake_light);
	ClassDB::bind_method(D_METHOD("_clear_baked_lights"), &VoxelChunkDefault::_clear_baked_lights);

	ClassDB::bind_method(D_METHOD("_world_light_added", "light"), &VoxelChunkDefault::_world_light_added);
	ClassDB::bind_method(D_METHOD("_world_light_removed", "light"), &VoxelChunkDefault::_world_light_removed);

	ClassDB::bind_method(D_METHOD("_finalize_build"), &VoxelChunkDefault::_finalize_build);

	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_TYPE);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_ISOLEVEL);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_ALT_TYPE);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_ALT_ISOLEVEL);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIGHT_COLOR_R);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIGHT_COLOR_G);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIGHT_COLOR_B);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_AO);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_RANDOM_AO);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIQUID_FLOW);
	BIND_ENUM_CONSTANT(MAX_DEFAULT_CHANNELS);

	BIND_CONSTANT(MESH_INDEX_TERRAIN);
	BIND_CONSTANT(MESH_INDEX_PROP);
	BIND_CONSTANT(MESH_INDEX_LIQUID);
	BIND_CONSTANT(MESH_INDEX_CLUTTER);

	BIND_CONSTANT(MESH_TYPE_INDEX_MESH);
	BIND_CONSTANT(MESH_TYPE_INDEX_MESH_INSTANCE);
	BIND_CONSTANT(MESH_TYPE_INDEX_SHAPE);
	BIND_CONSTANT(MESH_TYPE_INDEX_BODY);

	BIND_ENUM_CONSTANT(BUILD_FLAG_USE_ISOLEVEL);
	BIND_ENUM_CONSTANT(BUILD_FLAG_USE_LIGHTING);
	BIND_ENUM_CONSTANT(BUILD_FLAG_USE_AO);
	BIND_ENUM_CONSTANT(BUILD_FLAG_USE_RAO);
	BIND_ENUM_CONSTANT(BUILD_FLAG_GENERATE_AO);
	BIND_ENUM_CONSTANT(BUILD_FLAG_AUTO_GENERATE_RAO);
	BIND_ENUM_CONSTANT(BUILD_FLAG_BAKE_LIGHTS);
	BIND_ENUM_CONSTANT(BUILD_FLAG_CREATE_COLLIDER);
	BIND_ENUM_CONSTANT(BUILD_FLAG_CREATE_LODS);
}
