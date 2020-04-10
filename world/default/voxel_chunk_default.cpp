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

#include "core/version.h"

#if VERSION_MAJOR < 4
#include "servers/visual_server.h"
#else
#include "servers/rendering_server.h"

typedef class RenderingServer VisualServer;
typedef class RenderingServer VS;

#include "servers/physics_server_3d.h"

typedef class PhysicsServer3D PhysicsServer;

typedef class StandardMaterial3D SpatialMaterial;
#endif

#include "../../../opensimplex/open_simplex_noise.h"
#include "../../meshers/default/voxel_mesher_default.h"
#include "../voxel_world.h"

const String VoxelChunkDefault::BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE = "Normal,Process,Physics Process";
const String VoxelChunkDefault::BINDING_STRING_BUILD_FLAGS = "Use Isolevel,Use Lighting,Use AO,Use RAO,Generate AO,Generate RAO,Bake Lights,Create Collider,Create Lods";

_FORCE_INLINE_ bool VoxelChunkDefault::get_is_build_threaded() const {
	return _is_build_threaded;
}
_FORCE_INLINE_ void VoxelChunkDefault::set_is_build_threaded(const bool value) {
	_is_build_threaded = value;
}

_FORCE_INLINE_ int VoxelChunkDefault::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void VoxelChunkDefault::set_build_flags(const int flags) {
	_build_flags = flags;
}

_FORCE_INLINE_ VoxelChunkDefault::ActiveBuildPhaseType VoxelChunkDefault::get_active_build_phase_type() const {
	return _active_build_phase_type;
}
_FORCE_INLINE_ void VoxelChunkDefault::set_active_build_phase_type(const VoxelChunkDefault::ActiveBuildPhaseType value) {
	if (_active_build_phase_type == value)
		return;

	if (_active_build_phase_type == VoxelChunkDefault::BUILD_PHASE_TYPE_PHYSICS_PROCESS) {
		set_physics_process(false);
	} else if (_active_build_phase_type == VoxelChunkDefault::BUILD_PHASE_TYPE_PROCESS) {
		set_process(false);
	}

	_active_build_phase_type = value;

	if (_active_build_phase_type == VoxelChunkDefault::BUILD_PHASE_TYPE_PHYSICS_PROCESS) {
		set_physics_process(true);
	} else if (_active_build_phase_type == VoxelChunkDefault::BUILD_PHASE_TYPE_PROCESS) {
		set_process(true);
	}
}

bool VoxelChunkDefault::get_build_phase_done() const {
	return _build_phase_done;
}
void VoxelChunkDefault::set_build_phase_done(const bool value) {
	_build_phase_done = value;
}

int VoxelChunkDefault::get_lod_size() const {
	return _lod_size;
}
void VoxelChunkDefault::set_lod_size(const int lod_size) {
	_lod_size = lod_size;

	for (int i = 0; i < _meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_lod_size(_lod_size);
	}
}

int VoxelChunkDefault::get_current_build_phase() const {
	return _current_build_phase;
}
void VoxelChunkDefault::set_current_build_phase(const int value) {
	_current_build_phase = value;
}

int VoxelChunkDefault::get_max_build_phase() const {
	return _max_build_phases;
}
void VoxelChunkDefault::set_max_build_phase(const int value) {
	_max_build_phases = value;
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

		RID rid = get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH_INSTANCE, i);

		if (rid != RID())
			VisualServer::get_singleton()->instance_set_visible(rid, vis);
	}
}

//Data Management functions
void VoxelChunkDefault::generate_ao() {
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

void VoxelChunkDefault::build_deferred() {
	if (_current_build_phase == BUILD_PHASE_DONE) {
		_build_prioritized = true;

		wait_and_finish_thread();

		set_process(true);

		_is_generating = true;

		next_phase();

		if (!_voxel_world->can_chunk_do_build_step())
			return;

		build_step();
	}
}

void VoxelChunkDefault::build_prioritized() {
	if (_current_build_phase == BUILD_PHASE_DONE) {
		_build_prioritized = true;

		wait_and_finish_thread();

		set_process(true);

		_is_generating = true;

		next_phase();

		if (!_voxel_world->can_chunk_do_build_step())
			return;

		build_step();
	}
}

void VoxelChunkDefault::build_step() {
	ERR_FAIL_COND(!has_next_phase());
	ERR_FAIL_COND(_build_step_in_progress);

	_build_step_in_progress = true;

	if (get_is_build_threaded()) {
		if (_build_thread) {
			wait_and_finish_thread();
		}

		_build_thread = Thread::create(_build_step_threaded, this);
		return;
	}

	while (has_next_phase() && _active_build_phase_type == BUILD_PHASE_TYPE_NORMAL) {
		build_phase();

		if (!get_build_phase_done())
			break;
	}

	_build_step_in_progress = false;
}

void VoxelChunkDefault::_build_step_threaded(void *_userdata) {
	VoxelChunkDefault *vc = (VoxelChunkDefault *)_userdata;

	while (vc->has_next_phase() && vc->_active_build_phase_type == BUILD_PHASE_TYPE_NORMAL) {
		vc->build_phase();

		if (!vc->get_build_phase_done())
			break;
	}

	vc->_build_step_in_progress = false;
}

void VoxelChunkDefault::build_phase() {

	_THREAD_SAFE_METHOD_

	if (_abort_build)
		return;

	set_build_phase_done(false);

	call("_build_phase", _current_build_phase);
}

void VoxelChunkDefault::build_phase_process() {
	if (_abort_build)
		return;

	set_build_phase_done(false);

	call("_build_phase_process", _current_build_phase);
}

void VoxelChunkDefault::build_phase_physics_process() {
	if (_abort_build)
		return;

	set_build_phase_done(false);

	call("_build_phase_physics_process", _current_build_phase);
}

bool VoxelChunkDefault::has_next_phase() {
	if (_current_build_phase == BUILD_PHASE_DONE)
		return false;

	return true;
}

void VoxelChunkDefault::next_phase() {
	set_build_phase_done(true);

	if (_abort_build) {
		_current_build_phase = BUILD_PHASE_DONE;
		_is_generating = false;

		return;
	}

	++_current_build_phase;

	if (_current_build_phase >= _max_build_phases) {
		_current_build_phase = BUILD_PHASE_DONE;
		_is_generating = false;

		call_deferred("emit_build_finished");
	}
}

void VoxelChunkDefault::clear() {
	_voxel_lights.clear();
}

void VoxelChunkDefault::emit_build_finished() {
	emit_signal("mesh_generation_finished", this);

	if (_voxel_world != NULL) {
		_voxel_world->on_chunk_mesh_generation_finished(this);
	}
}

void VoxelChunkDefault::generate_random_ao(int seed, int octaves, int period, float persistence, float scale_factor) {

	Ref<OpenSimplexNoise> noise;
	noise.instance();

	noise->set_seed(seed);
	noise->set_octaves(octaves);
	noise->set_period(period);
	noise->set_persistence(persistence);

	for (int x = -get_margin_start(); x < _size_x + get_margin_end(); ++x) {
		for (int z = -get_margin_start(); z < _size_z + get_margin_end(); ++z) {
			for (int y = -get_margin_start(); y < _size_y + get_margin_end(); ++y) {
				float val = noise->get_noise_3d(x + (_position_x * _size_x), y + (_position_y * _size_y), z + (_position_z * _size_z));

				val *= scale_factor;

				if (val > 1)
					val = 1;

				if (val < 0)
					val = -val;

				set_voxel(int(val * 255.0), x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
			}
		}
	}
}

//Meshes
Dictionary VoxelChunkDefault::get_mesh_rids() {
	return _rids;
}
void VoxelChunkDefault::set_mesh_rids(const Dictionary &rids) {
	_rids = rids;
}
void VoxelChunkDefault::clear_rids() {
	_rids.clear();
}

RID VoxelChunkDefault::get_mesh_rid(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return RID();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return RID();

	Variant v = m[mesh_type_index];

	if (v.get_type() != Variant::_RID)
		return RID();

	return v;
}
void VoxelChunkDefault::set_mesh_rid(const int mesh_index, const int mesh_type_index, RID value) {
	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index)) {
		m[mesh_type_index] = value;
		_rids[mesh_index] = m;
		return;
	}

	Variant v = m[mesh_type_index];

	ERR_FAIL_COND(v.get_type() != Variant::_RID);

	m[mesh_type_index] = value;
	_rids[mesh_index] = m;
}
RID VoxelChunkDefault::get_mesh_rid_index(const int mesh_index, const int mesh_type_index, const int index) {
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
void VoxelChunkDefault::set_mesh_rid_index(const int mesh_index, const int mesh_type_index, const int index, RID value) {
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
int VoxelChunkDefault::get_mesh_rid_count(const int mesh_index, const int mesh_type_index) {
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
void VoxelChunkDefault::clear_mesh_rids(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return;

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return;

	m.erase(mesh_type_index);
}
Array VoxelChunkDefault::get_meshes(const int mesh_index, const int mesh_type_index) {
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
void VoxelChunkDefault::set_meshes(const int mesh_index, const int mesh_type_index, const Array &meshes) {
	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	m[mesh_type_index] = meshes;
	_rids[mesh_index] = m;
}
bool VoxelChunkDefault::has_meshes(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return false;

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return false;

	return true;
}

void VoxelChunkDefault::free_rids() {
	List<Variant> keys;

	_rids.get_key_list(&keys);

	for (List<Variant>::Element *E = keys.front(); E; E = E->next()) {
		Variant v = E->get();

		if (v.get_type() != Variant::INT)
			continue;

		free_index(v);
	}
}

void VoxelChunkDefault::free_index(const int mesh_index) {
	free_meshes(mesh_index);
	free_colliders(mesh_index);
}

void VoxelChunkDefault::create_meshes(const int mesh_index, const int mesh_count) {
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

		if (get_voxel_world()->get_world().is_valid())
			VS::get_singleton()->instance_set_scenario(mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

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
void VoxelChunkDefault::free_meshes(const int mesh_index) {
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

void VoxelChunkDefault::create_colliders(const int mesh_index, const int layer_mask) {
	ERR_FAIL_COND(_voxel_world == Variant());

	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_BODY));
	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_SHAPE));

	RID shape_rid = PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_CONCAVE_POLYGON);
	RID body_rid = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);

	PhysicsServer::get_singleton()->body_set_collision_layer(body_rid, layer_mask);
	PhysicsServer::get_singleton()->body_set_collision_mask(body_rid, layer_mask);

	PhysicsServer::get_singleton()->body_add_shape(body_rid, shape_rid);

	PhysicsServer::get_singleton()->body_set_state(body_rid, PhysicsServer::BODY_STATE_TRANSFORM, get_transform());
	PhysicsServer::get_singleton()->body_set_space(body_rid, get_voxel_world()->get_world()->get_space());

	m[MESH_TYPE_INDEX_BODY] = body_rid;
	m[MESH_TYPE_INDEX_SHAPE] = shape_rid;

	_rids[mesh_index] = m;
}
void VoxelChunkDefault::free_colliders(const int mesh_index) {
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
	}
}

void VoxelChunkDefault::create_debug_immediate_geometry() {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(_debug_drawer != NULL);

	_debug_drawer = memnew(ImmediateGeometry());

	_voxel_world->add_child(_debug_drawer);

	if (Engine::get_singleton()->is_editor_hint())
		_debug_drawer->set_owner(_voxel_world->get_tree()->get_edited_scene_root());

	//_debug_drawer->set_transform(Transform(Basis(), Vector3(_position.x * _size.x * _voxel_scale, _position.y * _size.y * _voxel_scale, _position.z * _size.z * _voxel_scale)));
	//_debug_drawer->set_transform(Transform(Basis(), Vector3(_position.x * _size.x * _voxel_scale, _position.y * _size.y * _voxel_scale, _position.z * _size.z * _voxel_scale)));
}

void VoxelChunkDefault::free_debug_immediate_geometry() {
	if (_debug_drawer != NULL) {
		_debug_drawer->queue_delete();

		_debug_drawer = NULL;
	}
}

void VoxelChunkDefault::draw_cross_voxels(Vector3 pos) {
	pos *= _voxel_scale;

	_debug_drawer->add_vertex(pos + Vector3(0, 0, -0.2));
	_debug_drawer->add_vertex(pos + Vector3(0, 0, 0.2));

	_debug_drawer->add_vertex(pos + Vector3(0, -0.2, 0));
	_debug_drawer->add_vertex(pos + Vector3(0, 0.2, 0));

	_debug_drawer->add_vertex(pos + Vector3(-0.2, 0, 0));
	_debug_drawer->add_vertex(pos + Vector3(0.2, 0, 0));
}

void VoxelChunkDefault::draw_cross_voxels_fill(Vector3 pos, float fill) {
	pos *= _voxel_scale;

	_debug_drawer->add_vertex(pos + Vector3(0, 0, -0.5 * fill));
	_debug_drawer->add_vertex(pos + Vector3(0, 0, 0.5 * fill));

	_debug_drawer->add_vertex(pos + Vector3(0, -0.5 * fill, 0));
	_debug_drawer->add_vertex(pos + Vector3(0, 0.5 * fill, 0));

	_debug_drawer->add_vertex(pos + Vector3(-0.5 * fill, 0, 0));
	_debug_drawer->add_vertex(pos + Vector3(0.5 * fill, 0, 0));
}

void VoxelChunkDefault::draw_debug_voxels(int max, Color color) {
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

	_debug_drawer->end();
}

void VoxelChunkDefault::draw_debug_voxel_lights() {
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

void VoxelChunkDefault::_visibility_changed(bool visible) {
	if (visible) {
		set_current_lod_level(_current_lod_level);
		return;
	}

	for (int i = 0; i < _lod_num + 1; ++i) {
		RID rid = get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH_INSTANCE, i);

		if (rid != RID())
			VisualServer::get_singleton()->instance_set_visible(rid, false);
	}
}

void VoxelChunkDefault::_exit_tree() {
	if (_build_thread) {
		_abort_build = true;

		wait_and_finish_thread();
	}

	free_rids();
}
void VoxelChunkDefault::_process(float delta) {
	if (!get_is_generating()) {
		set_process(false);
	}

	if (!get_is_generating() || !has_next_phase() || _build_step_in_progress) {
		return;
	}

	switch (_active_build_phase_type) {
		case BUILD_PHASE_TYPE_PROCESS: {
			if (!_voxel_world->can_chunk_do_build_step())
				return;

			_build_step_in_progress = true;

			while (has_next_phase() && _active_build_phase_type == BUILD_PHASE_TYPE_PROCESS) {
				build_phase_process();

				if (!get_build_phase_done())
					break;
			}

			_build_step_in_progress = false;
			return;
		}
		case BUILD_PHASE_TYPE_NORMAL: {
			//normal mode -> build step is not in progress -> need to restart building

			if (!_voxel_world->can_chunk_do_build_step())
				return;

			build_step();

			return;
		}
		case BUILD_PHASE_TYPE_PHYSICS_PROCESS:
			return;
	}
}
void VoxelChunkDefault::_physics_process(float delta) {
	if (!get_is_generating() || !has_next_phase() || _build_step_in_progress) {
		return;
	}

	if (_active_build_phase_type == BUILD_PHASE_TYPE_PHYSICS_PROCESS) {

		if (!_voxel_world->can_chunk_do_build_step())
			return;

		_build_step_in_progress = true;

		while (has_next_phase() && _active_build_phase_type == BUILD_PHASE_TYPE_PHYSICS_PROCESS) {
			build_phase_physics_process();

			if (!get_build_phase_done())
				break;
		}

		_build_step_in_progress = false;

		return;
	}
}
void VoxelChunkDefault::_world_transform_changed() {
	VoxelChunk::_world_transform_changed();

	update_transforms();
}

void VoxelChunkDefault::free_chunk() {
	free_rids();
}

VoxelChunkDefault::VoxelChunkDefault() {
	_is_generating = false;
	_is_build_threaded = false;
	_abort_build = false;
	_dirty = false;
	_state = VOXEL_CHUNK_STATE_OK;

	_enabled = true;
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
	_build_phase_done = false;
	_build_thread = NULL;
	_build_step_in_progress = false;

	_active_build_phase_type = BUILD_PHASE_TYPE_NORMAL;

	_lod_num = 3;
	_current_lod_level = 0;

	_build_flags = BUILD_FLAG_CREATE_COLLIDER | BUILD_FLAG_CREATE_LODS;
}

VoxelChunkDefault::~VoxelChunkDefault() {
	if (_build_thread) {
		_abort_build = true;
		wait_and_finish_thread();
	}
}

void VoxelChunkDefault::_setup_channels() {
	set_channel_count(MAX_DEFAULT_CHANNELS);
}

void VoxelChunkDefault::_build_phase(int phase) {
	ERR_FAIL_COND(!_library.is_valid());

	switch (phase) {
		case BUILD_PHASE_DONE:
			return;
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

			return;
		}
		case BUILD_PHASE_TERRARIN_MESH_SETUP: {
			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->add_chunk(this);
			}

			next_phase();

			return;
		}
		case BUILD_PHASE_TERRARIN_MESH_COLLIDER: {
			if ((_build_flags & BUILD_FLAG_CREATE_COLLIDER) == 0) {
				next_phase();
				return;
			}

			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				temp_arr_collider.append_array(mesher->build_collider());
			}

			if (temp_arr_collider.size() == 0) {
				next_phase();
				return;
			}

			//if (_is_build_threaded) {
			//	set_active_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
			//	return;
			//}

			if (!has_meshes(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_BODY)) {
				create_colliders(MESH_INDEX_TERRARIN);
			}

			PhysicsServer::get_singleton()->shape_set_data(get_mesh_rid(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_SHAPE), temp_arr_collider);

			//temp_arr_collider.resize(0);

			next_phase();

			return;
		}
		case BUILD_PHASE_TERRARIN_MESH: {
			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->set_library(_library);
			}

			if ((_build_flags & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
				for (int i = 0; i < _meshers.size(); ++i) {
					Ref<VoxelMesher> mesher = _meshers.get(i);

					ERR_CONTINUE(!mesher.is_valid());

					mesher->bake_colors(this);
				}
			}

			Ref<VoxelMesher> mesher;
			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> m = _meshers.get(i);

				ERR_CONTINUE(!m.is_valid());

				if (!mesher.is_valid()) {
					mesher = m;
					mesher->set_material(get_library()->get_material(0));
					continue;
				}

				mesher->set_material(get_library()->get_material(0));
				mesher->add_mesher(m);
			}

			ERR_FAIL_COND(!mesher.is_valid());

			if (mesher->get_vertex_count() == 0) {
				next_phase();
				return;
			}

			RID mesh_rid = get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 0);

			if (mesh_rid != RID())
				VS::get_singleton()->mesh_clear(mesh_rid);

			Array temp_mesh_arr = mesher->build_mesh();

			if (mesh_rid == RID()) {
				if ((_build_flags & BUILD_FLAG_CREATE_LODS) != 0)
					create_meshes(MESH_INDEX_TERRARIN, _lod_num + 1);
				else
					create_meshes(MESH_INDEX_TERRARIN, 1);

				mesh_rid = get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 0);
			}

			VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

			if (_library->get_material(0).is_valid())
				VS::get_singleton()->mesh_surface_set_material(mesh_rid, 0, _library->get_material(0)->get_rid());

			if ((_build_flags & BUILD_FLAG_CREATE_LODS) != 0) {
				if (_lod_num >= 1) {
					//for lod 1 just remove uv2
					temp_mesh_arr[VisualServer::ARRAY_TEX_UV2] = Variant();

					VisualServer::get_singleton()->mesh_add_surface_from_arrays(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 1), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

					if (get_library()->get_material(1).is_valid())
						VisualServer::get_singleton()->mesh_surface_set_material(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 1), 0, get_library()->get_material(1)->get_rid());
				}

				if (_lod_num >= 2) {
					Array temp_mesh_arr2 = merge_mesh_array(temp_mesh_arr);

					VisualServer::get_singleton()->mesh_add_surface_from_arrays(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 2), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr2);

					if (get_library()->get_material(2).is_valid())
						VisualServer::get_singleton()->mesh_surface_set_material(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 2), 0, get_library()->get_material(2)->get_rid());
				}

				if (_lod_num >= 3) {
					Ref<ShaderMaterial> mat = get_library()->get_material(0);
					Ref<SpatialMaterial> spmat = get_library()->get_material(0);
					Ref<Texture> tex;

					if (mat.is_valid()) {
						tex = mat->get_shader_param("texture_albedo");
					} else if (spmat.is_valid()) {
						tex = spmat->get_texture(SpatialMaterial::TEXTURE_ALBEDO);
					}

					if (tex.is_valid()) {
						temp_mesh_arr = bake_mesh_array_uv(temp_mesh_arr, tex);
						temp_mesh_arr[VisualServer::ARRAY_TEX_UV] = Variant();

						VisualServer::get_singleton()->mesh_add_surface_from_arrays(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 3), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

						if (get_library()->get_material(3).is_valid())
							VisualServer::get_singleton()->mesh_surface_set_material(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 3), 0, get_library()->get_material(3)->get_rid());
					}
				}

				/*
				if (_lod_num > 4) {
					Ref<FastQuadraticMeshSimplifier> fqms;
					fqms.instance();
					fqms->initialize(temp_mesh_arr);

					Array arr_merged_simplified;

					for (int i = 4; i < _lod_num; ++i) {
						fqms->simplify_mesh(arr_merged_simplified[0].size() * 0.8, 7);
						arr_merged_simplified = fqms->get_arrays();

						if (arr_merged_simplified[0].size() == 0)
							break;

						VisualServer::get_singleton()->mesh_add_surface_from_arrays(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, i), VisualServer::PRIMITIVE_TRIANGLES, arr_merged_simplified);

						if (get_library()->get_material(i).is_valid())
							VisualServer::get_singleton()->mesh_surface_set_material(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, i), 0, get_library()->get_material(i)->get_rid());
					}
				}
				*/
			}

			next_phase();

			return;
		}
		case BUILD_PHASE_LIGHTS: {
			bool gr = (_build_flags & BUILD_FLAG_AUTO_GENERATE_RAO) != 0;

			if (!gr && (_build_flags & BUILD_FLAG_USE_LIGHTING) == 0) {
				next_phase();
				return;
			}

			bool bl = (_build_flags & BUILD_FLAG_BAKE_LIGHTS) != 0;

			if (bl)
				clear_baked_lights();

			if (gr)
				generate_random_ao(_voxel_world->get_current_seed());

			if (bl)
				bake_lights();

			next_phase();

			return;
		}
		/*
		case BUILD_PHASE_LIQUID: {
			next_phase();
			return;
		}
		*/
		case BUILD_PHASE_FINALIZE: {
			update_transforms();

			next_phase();

			return;
		}
	}
}

void VoxelChunkDefault::_build_phase_process(int phase) {
}

void VoxelChunkDefault::_build_phase_physics_process(int phase) {
	if (phase == BUILD_PHASE_TERRARIN_MESH_COLLIDER) {

		if (!has_meshes(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_SHAPE)) {
			create_colliders(MESH_INDEX_TERRARIN);
		}

		PhysicsServer::get_singleton()->shape_set_data(get_mesh_rid(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_SHAPE), temp_arr_collider);
		//temp_arr_collider.resize(0);

		set_active_build_phase_type(BUILD_PHASE_TYPE_NORMAL);
		next_phase();
	}
}

void VoxelChunkDefault::_add_light(int local_x, int local_y, int local_z, int size, Color color) {
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
void VoxelChunkDefault::_clear_baked_lights() {
	fill_channel(0, DEFAULT_CHANNEL_LIGHT_COLOR_R);
	fill_channel(0, DEFAULT_CHANNEL_LIGHT_COLOR_G);
	fill_channel(0, DEFAULT_CHANNEL_LIGHT_COLOR_B);
}

void VoxelChunkDefault::_create_meshers() {
	for (int i = 0; i < _meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_lod_size(get_lod_size());
		mesher->set_voxel_scale(get_voxel_scale());

		Ref<VoxelMesherDefault> md = mesher;

		if (md.is_valid()) {
			md->set_build_flags(get_build_flags());
		}
	}
}

void VoxelChunkDefault::_build(bool immediate) {
	build_deferred();
}

void VoxelChunkDefault::wait_and_finish_thread() {
	if (_build_thread) {
		Thread::wait_to_finish(_build_thread);
		memdelete(_build_thread);
		_build_thread = NULL;
	}
}

void VoxelChunkDefault::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_build_threaded"), &VoxelChunkDefault::get_is_build_threaded);
	ClassDB::bind_method(D_METHOD("set_is_build_threaded", "value"), &VoxelChunkDefault::set_is_build_threaded);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_build_threaded"), "set_is_build_threaded", "get_is_build_threaded");

	ClassDB::bind_method(D_METHOD("get_build_flags"), &VoxelChunkDefault::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &VoxelChunkDefault::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");

	ClassDB::bind_method(D_METHOD("get_active_build_phase_type"), &VoxelChunkDefault::get_active_build_phase_type);
	ClassDB::bind_method(D_METHOD("set_active_build_phase_type", "value"), &VoxelChunkDefault::set_active_build_phase_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "active_build_phase_type", PROPERTY_HINT_ENUM, BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE), "set_active_build_phase_type", "get_active_build_phase_type");

	ClassDB::bind_method(D_METHOD("get_lod_size"), &VoxelChunkDefault::get_lod_size);
	ClassDB::bind_method(D_METHOD("set_lod_size", "value"), &VoxelChunkDefault::set_lod_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_size"), "set_lod_size", "get_lod_size");

	ClassDB::bind_method(D_METHOD("get_current_build_phase"), &VoxelChunkDefault::get_current_build_phase);
	ClassDB::bind_method(D_METHOD("set_current_build_phase", "value"), &VoxelChunkDefault::set_current_build_phase);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_build_phase"), "set_current_build_phase", "get_current_build_phase");

	ClassDB::bind_method(D_METHOD("get_max_build_phase"), &VoxelChunkDefault::get_max_build_phase);
	ClassDB::bind_method(D_METHOD("set_max_build_phase", "value"), &VoxelChunkDefault::set_max_build_phase);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_build_phase"), "set_max_build_phase", "get_max_build_phase");

	ClassDB::bind_method(D_METHOD("get_lod_num"), &VoxelChunkDefault::get_lod_num);
	ClassDB::bind_method(D_METHOD("set_lod_num"), &VoxelChunkDefault::set_lod_num);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_num"), "set_lod_num", "get_lod_num");

	ClassDB::bind_method(D_METHOD("get_current_lod_level"), &VoxelChunkDefault::get_current_lod_level);
	ClassDB::bind_method(D_METHOD("set_current_lod_level"), &VoxelChunkDefault::set_current_lod_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_lod_level"), "set_current_lod_level", "get_current_lod_level");

	//Voxel Data

	//Data Management functions
	ClassDB::bind_method(D_METHOD("generate_ao"), &VoxelChunkDefault::generate_ao);
	ClassDB::bind_method(D_METHOD("add_light", "local_x", "local_y", "local_z", "size", "color"), &VoxelChunkDefault::add_light);
	ClassDB::bind_method(D_METHOD("clear_baked_lights"), &VoxelChunkDefault::clear_baked_lights);

	//Meshes
	BIND_VMETHOD(MethodInfo("_build_phase", PropertyInfo(Variant::INT, "phase")));
	BIND_VMETHOD(MethodInfo("_build_phase_process", PropertyInfo(Variant::INT, "phase")));
	BIND_VMETHOD(MethodInfo("_build_phase_physics_process", PropertyInfo(Variant::INT, "phase")));

	ClassDB::bind_method(D_METHOD("build_deferred"), &VoxelChunkDefault::build_deferred);
	ClassDB::bind_method(D_METHOD("build_prioritized"), &VoxelChunkDefault::build_prioritized);

	ClassDB::bind_method(D_METHOD("build_phase"), &VoxelChunkDefault::build_phase);
	ClassDB::bind_method(D_METHOD("build_phase_process"), &VoxelChunkDefault::build_phase_process);
	ClassDB::bind_method(D_METHOD("build_phase_physics_process"), &VoxelChunkDefault::build_phase_physics_process);

	ClassDB::bind_method(D_METHOD("next_phase"), &VoxelChunkDefault::next_phase);
	ClassDB::bind_method(D_METHOD("has_next_phase"), &VoxelChunkDefault::has_next_phase);

	//Meshes
	ClassDB::bind_method(D_METHOD("get_mesh_rids"), &VoxelChunkDefault::get_mesh_rids);
	ClassDB::bind_method(D_METHOD("set_mesh_rids", "rids"), &VoxelChunkDefault::set_mesh_rids);
	ClassDB::bind_method(D_METHOD("clear_rids"), &VoxelChunkDefault::clear_rids);

	ClassDB::bind_method(D_METHOD("get_mesh_rid", "mesh_index", "mesh_type_index"), &VoxelChunkDefault::get_mesh_rid);
	ClassDB::bind_method(D_METHOD("set_mesh_rid", "mesh_index", "mesh_type_index", "value"), &VoxelChunkDefault::set_mesh_rid);
	ClassDB::bind_method(D_METHOD("get_mesh_rid_index", "mesh_index", "mesh_type_index", "index"), &VoxelChunkDefault::get_mesh_rid_index);
	ClassDB::bind_method(D_METHOD("set_mesh_rid_index", "mesh_index", "mesh_type_index", "index", "value"), &VoxelChunkDefault::set_mesh_rid_index);
	ClassDB::bind_method(D_METHOD("get_mesh_rid_count", "mesh_index", "mesh_type_index"), &VoxelChunkDefault::get_mesh_rid_count);
	ClassDB::bind_method(D_METHOD("clear_mesh_rids", "mesh_index", "mesh_type_index"), &VoxelChunkDefault::clear_mesh_rids);
	ClassDB::bind_method(D_METHOD("get_meshes", "mesh_index", "mesh_type_index"), &VoxelChunkDefault::get_meshes);
	ClassDB::bind_method(D_METHOD("set_meshes", "mesh_index", "mesh_type_index", "meshes"), &VoxelChunkDefault::set_meshes);
	ClassDB::bind_method(D_METHOD("has_meshes", "mesh_index", "mesh_type_index"), &VoxelChunkDefault::has_meshes);

	ClassDB::bind_method(D_METHOD("free_rids"), &VoxelChunkDefault::free_rids);
	ClassDB::bind_method(D_METHOD("free_index", "mesh_index"), &VoxelChunkDefault::free_index);

	ClassDB::bind_method(D_METHOD("create_meshes", "mesh_index", "mesh_count"), &VoxelChunkDefault::create_meshes);
	ClassDB::bind_method(D_METHOD("free_meshes", "mesh_index"), &VoxelChunkDefault::free_meshes);

	ClassDB::bind_method(D_METHOD("create_colliders", "mesh_index", "layer_mask"), &VoxelChunkDefault::create_colliders, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("free_colliders", "mesh_index"), &VoxelChunkDefault::free_colliders);

	ClassDB::bind_method(D_METHOD("create_debug_immediate_geometry"), &VoxelChunkDefault::create_debug_immediate_geometry);
	ClassDB::bind_method(D_METHOD("free_debug_immediate_geometry"), &VoxelChunkDefault::free_debug_immediate_geometry);

	ClassDB::bind_method(D_METHOD("free_chunk"), &VoxelChunkDefault::free_chunk);

	ClassDB::bind_method(D_METHOD("emit_build_finished"), &VoxelChunkDefault::emit_build_finished);

	BIND_VMETHOD(MethodInfo("_draw_debug_voxel_lights", PropertyInfo(Variant::OBJECT, "debug_drawer", PROPERTY_HINT_RESOURCE_TYPE, "ImmediateGeometry")));

	ClassDB::bind_method(D_METHOD("draw_cross_voxels", "max"), &VoxelChunkDefault::draw_cross_voxels);
	ClassDB::bind_method(D_METHOD("draw_cross_voxels_fill", "max", "fill"), &VoxelChunkDefault::draw_cross_voxels_fill);
	ClassDB::bind_method(D_METHOD("draw_debug_voxels", "max", "color"), &VoxelChunkDefault::draw_debug_voxels, DEFVAL(Color(1, 1, 1)));

	ClassDB::bind_method(D_METHOD("draw_debug_voxel_lights"), &VoxelChunkDefault::draw_debug_voxel_lights);

	ClassDB::bind_method(D_METHOD("generate_random_ao", "seed", "octaves", "period", "persistence", "scale_factor"), &VoxelChunkDefault::generate_random_ao, DEFVAL(4), DEFVAL(30), DEFVAL(0.3), DEFVAL(0.6));

	ClassDB::bind_method(D_METHOD("_setup_channels"), &VoxelChunkDefault::_setup_channels);
	ClassDB::bind_method(D_METHOD("_build_phase", "phase"), &VoxelChunkDefault::_build_phase);
	ClassDB::bind_method(D_METHOD("_build_phase_process", "phase"), &VoxelChunkDefault::_build_phase_process);
	ClassDB::bind_method(D_METHOD("_build_phase_physics_process", "phase"), &VoxelChunkDefault::_build_phase_physics_process);

	ClassDB::bind_method(D_METHOD("_add_light", "local_x", "local_y", "local_z", "size", "color"), &VoxelChunkDefault::_add_light);

	ClassDB::bind_method(D_METHOD("_clear_baked_lights"), &VoxelChunkDefault::_clear_baked_lights);
	ClassDB::bind_method(D_METHOD("_create_meshers"), &VoxelChunkDefault::_create_meshers);
	ClassDB::bind_method(D_METHOD("_build", "immediate"), &VoxelChunkDefault::_build);

	ClassDB::bind_method(D_METHOD("_exit_tree"), &VoxelChunkDefault::_exit_tree);
	ClassDB::bind_method(D_METHOD("_process", "delta"), &VoxelChunkDefault::_process);
	ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &VoxelChunkDefault::_physics_process);
	ClassDB::bind_method(D_METHOD("_visibility_changed", "visible"), &VoxelChunkDefault::_visibility_changed);

	BIND_CONSTANT(BUILD_PHASE_DONE);
	BIND_CONSTANT(BUILD_PHASE_SETUP);
	BIND_CONSTANT(BUILD_PHASE_TERRARIN_MESH_SETUP);
	BIND_CONSTANT(BUILD_PHASE_TERRARIN_MESH_COLLIDER);
	BIND_CONSTANT(BUILD_PHASE_TERRARIN_MESH);
	BIND_CONSTANT(BUILD_PHASE_LIGHTS);
	BIND_CONSTANT(BUILD_PHASE_FINALIZE);
	BIND_CONSTANT(BUILD_PHASE_MAX);

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

	BIND_ENUM_CONSTANT(BUILD_PHASE_TYPE_NORMAL);
	BIND_ENUM_CONSTANT(BUILD_PHASE_TYPE_PROCESS);
	BIND_ENUM_CONSTANT(BUILD_PHASE_TYPE_PHYSICS_PROCESS);

	BIND_CONSTANT(MESH_INDEX_TERRARIN);
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
