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

#include "voxel_world.h"

const String VoxelChunkDefault::BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE = "Normal,Process,Physics Process";

_FORCE_INLINE_ bool VoxelChunkDefault::get_is_build_threaded() const {
	return _is_build_threaded;
}
_FORCE_INLINE_ void VoxelChunkDefault::set_is_build_threaded(bool value) {
	_is_build_threaded = value;
}

_FORCE_INLINE_ VoxelChunkDefault::ActiveBuildPhaseType VoxelChunkDefault::get_active_build_phase_type() const {
	return _active_build_phase_type;
}
_FORCE_INLINE_ void VoxelChunkDefault::set_active_build_phase_type(const VoxelChunkDefault::ActiveBuildPhaseType value) {
	_active_build_phase_type = value;

	if (_active_build_phase_type == VoxelChunkDefault::BUILD_PHASE_TYPE_PHYSICS_PROCESS) {
		set_physics_process_internal(true);
	} else {
		set_physics_process_internal(false);
	}
}

bool VoxelChunkDefault::get_build_phase_done() const {
	_build_phase_done_mutex->lock();
	bool v = _build_phase_done;
	_build_phase_done_mutex->unlock();

	return v;
}
void VoxelChunkDefault::set_build_phase_done(bool value) {
	_build_phase_done_mutex->lock();
	_build_phase_done = value;
	_build_phase_done_mutex->unlock();
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

int VoxelChunkDefault::get_current_build_phase() {
	return _current_build_phase;
}
void VoxelChunkDefault::set_current_build_phase(int value) {
	_current_build_phase = value;
}

int VoxelChunkDefault::get_max_build_phase() {
	return _max_build_phases;
}
void VoxelChunkDefault::set_max_build_phase(int value) {
	_max_build_phases = value;
}
bool VoxelChunkDefault::get_create_collider() const {
	return _create_collider;
}
void VoxelChunkDefault::set_create_collider(bool value) {
	_create_collider = value;
}

bool VoxelChunkDefault::get_bake_lights() const {
	return _bake_lights;
}
void VoxelChunkDefault::set_bake_lights(bool value) {
	_bake_lights = value;
}

RID VoxelChunkDefault::get_mesh_rid() {
	return _mesh_rid;
}
RID VoxelChunkDefault::get_mesh_instance_rid() {
	return _mesh_instance_rid;
}
RID VoxelChunkDefault::get_shape_rid() {
	return _shape_rid;
}
RID VoxelChunkDefault::get_body_rid() {
	return _body_rid;
}

RID VoxelChunkDefault::get_prop_mesh_rid() {
	return _prop_mesh_rid;
}
RID VoxelChunkDefault::get_prop_mesh_instance_rid() {
	return _prop_mesh_instance_rid;
}
RID VoxelChunkDefault::get_prop_shape_rid() {
	return _prop_shape_rid;
}
RID VoxelChunkDefault::get_prop_body_rid() {
	return _prop_body_rid;
}

RID VoxelChunkDefault::get_liquid_mesh_rid() {
	return _liquid_mesh_rid;
}
RID VoxelChunkDefault::get_liquid_mesh_instance_rid() {
	return _liquid_mesh_instance_rid;
}

RID VoxelChunkDefault::get_clutter_mesh_rid() {
	return _clutter_mesh_rid;
}
RID VoxelChunkDefault::get_clutter_mesh_instance_rid() {
	return _clutter_mesh_instance_rid;
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

		set_process_internal(true);

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

		set_process_internal(true);

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

void VoxelChunkDefault::create_colliders() {
	ERR_FAIL_COND(_voxel_world == NULL);

	_shape_rid = PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_CONCAVE_POLYGON);
	_body_rid = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);

	PhysicsServer::get_singleton()->body_set_collision_layer(_body_rid, 1);
	PhysicsServer::get_singleton()->body_set_collision_mask(_body_rid, 1);

	PhysicsServer::get_singleton()->body_add_shape(_body_rid, _shape_rid);

	PhysicsServer::get_singleton()->body_set_state(_body_rid, PhysicsServer::BODY_STATE_TRANSFORM, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
	PhysicsServer::get_singleton()->body_set_space(_body_rid, get_voxel_world()->get_world()->get_space());
}

void VoxelChunkDefault::remove_colliders() {
	if (_body_rid != RID()) {
		PhysicsServer::get_singleton()->free(_body_rid);
		PhysicsServer::get_singleton()->free(_shape_rid);

		_body_rid = RID();
		_shape_rid = RID();
	}
}

void VoxelChunkDefault::allocate_main_mesh() {
	ERR_FAIL_COND(_voxel_world == NULL);

	ERR_FAIL_COND(!get_library().is_valid());

	_mesh_instance_rid = VS::get_singleton()->instance_create();

	if (get_voxel_world()->get_world().is_valid())
		VS::get_singleton()->instance_set_scenario(_mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

	_mesh_rid = VS::get_singleton()->mesh_create();

	VS::get_singleton()->instance_set_base(_mesh_instance_rid, _mesh_rid);

	VS::get_singleton()->instance_set_transform(_mesh_instance_rid, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
}

void VoxelChunkDefault::free_main_mesh() {
	if (_mesh_instance_rid != RID()) {
		VS::get_singleton()->free(_mesh_instance_rid);
		VS::get_singleton()->free(_mesh_rid);

		_mesh_instance_rid = RID();
		_mesh_rid = RID();
	}
}

void VoxelChunkDefault::allocate_prop_mesh() {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(!get_library().is_valid());

	_prop_mesh_instance_rid = VS::get_singleton()->instance_create();

	if (get_voxel_world()->get_world().is_valid())
		VS::get_singleton()->instance_set_scenario(_prop_mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

	_prop_mesh_rid = VS::get_singleton()->mesh_create();

	VS::get_singleton()->instance_set_base(_prop_mesh_instance_rid, _prop_mesh_rid);

	VS::get_singleton()->instance_set_transform(_prop_mesh_instance_rid, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
}

void VoxelChunkDefault::free_prop_mesh() {
	if (_prop_mesh_instance_rid != RID()) {
		VS::get_singleton()->free(_prop_mesh_instance_rid);
		VS::get_singleton()->free(_prop_mesh_rid);

		_prop_mesh_instance_rid = RID();
		_prop_mesh_rid = RID();
	}
}

void VoxelChunkDefault::allocate_prop_colliders() {
	ERR_FAIL_COND(_voxel_world == NULL);

	_prop_shape_rid = PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_CONCAVE_POLYGON);
	_prop_body_rid = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);

	PhysicsServer::get_singleton()->body_set_collision_layer(_prop_body_rid, 1);
	PhysicsServer::get_singleton()->body_set_collision_mask(_prop_body_rid, 1);

	PhysicsServer::get_singleton()->body_add_shape(_prop_body_rid, _prop_shape_rid);

	PhysicsServer::get_singleton()->body_set_state(_prop_body_rid, PhysicsServer::BODY_STATE_TRANSFORM, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
	PhysicsServer::get_singleton()->body_set_space(_prop_body_rid, get_voxel_world()->get_world()->get_space());
}
void VoxelChunkDefault::free_prop_colliders() {
	if (_prop_body_rid != RID()) {
		PhysicsServer::get_singleton()->free(_prop_body_rid);
		PhysicsServer::get_singleton()->free(_prop_shape_rid);

		_prop_body_rid = RID();
		_prop_shape_rid = RID();
	}
}

//Liquid mesh
void VoxelChunkDefault::allocate_liquid_mesh() {
	ERR_FAIL_COND(_voxel_world == NULL);

	ERR_FAIL_COND(!get_library().is_valid());

	_liquid_mesh_instance_rid = VS::get_singleton()->instance_create();

	if (get_voxel_world()->get_world().is_valid())
		VS::get_singleton()->instance_set_scenario(_liquid_mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

	_liquid_mesh_rid = VS::get_singleton()->mesh_create();

	VS::get_singleton()->instance_set_base(_liquid_mesh_instance_rid, _liquid_mesh_rid);

	VS::get_singleton()->instance_set_transform(_liquid_mesh_instance_rid, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
}

void VoxelChunkDefault::free_liquid_mesh() {
	if (_liquid_mesh_instance_rid != RID()) {
		VS::get_singleton()->free(_liquid_mesh_instance_rid);
		VS::get_singleton()->free(_liquid_mesh_rid);

		_liquid_mesh_instance_rid = RID();
		_liquid_mesh_rid = RID();
	}
}

//Clutter mesh
void VoxelChunkDefault::allocate_clutter_mesh() {
	ERR_FAIL_COND(_voxel_world == NULL);

	ERR_FAIL_COND(!get_library().is_valid());

	_clutter_mesh_instance_rid = VS::get_singleton()->instance_create();

	if (get_voxel_world()->get_world().is_valid())
		VS::get_singleton()->instance_set_scenario(_clutter_mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

	_clutter_mesh_rid = VS::get_singleton()->mesh_create();

	VS::get_singleton()->instance_set_base(_clutter_mesh_instance_rid, _clutter_mesh_rid);

	VS::get_singleton()->instance_set_transform(_clutter_mesh_instance_rid, Transform(Basis(), Vector3(_position_x * _size_x * _voxel_scale, _position_y * _size_y * _voxel_scale, _position_z * _size_z * _voxel_scale)));
}

void VoxelChunkDefault::free_clutter_mesh() {
	if (_clutter_mesh_instance_rid != RID()) {
		VS::get_singleton()->free(_clutter_mesh_instance_rid);
		VS::get_singleton()->free(_clutter_mesh_rid);

		_clutter_mesh_instance_rid = RID();
		_clutter_mesh_rid = RID();
	}
}

void VoxelChunkDefault::update_transforms() {
	if (get_mesh_instance_rid() != RID())
		VS::get_singleton()->instance_set_transform(get_mesh_instance_rid(), get_transform());

	if (get_prop_mesh_instance_rid() != RID())
		VS::get_singleton()->instance_set_transform(get_prop_mesh_instance_rid(), get_transform());

	if (get_body_rid() != RID())
		PhysicsServer::get_singleton()->body_set_state(get_body_rid(), PhysicsServer::BODY_STATE_TRANSFORM, get_transform());

	if (get_prop_body_rid() != RID())
		PhysicsServer::get_singleton()->body_set_state(get_prop_body_rid(), PhysicsServer::BODY_STATE_TRANSFORM, get_transform());
}

void VoxelChunkDefault::create_debug_immediate_geometry() {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(_debug_drawer != NULL);

	_debug_drawer = memnew(ImmediateGeometry());

	add_child(_debug_drawer);

	if (Engine::get_singleton()->is_editor_hint())
		_debug_drawer->set_owner(get_tree()->get_edited_scene_root());

	//_debug_drawer->set_transform(Transform(Basis(), Vector3(_position.x * _size.x * _voxel_scale, _position.y * _size.y * _voxel_scale, _position.z * _size.z * _voxel_scale)));
	//_debug_drawer->set_transform(Transform(Basis(), Vector3(_position.x * _size.x * _voxel_scale, _position.y * _size.y * _voxel_scale, _position.z * _size.z * _voxel_scale)));
}

void VoxelChunkDefault::free_debug_immediate_geometry() {
	if (ObjectDB::instance_validate(_debug_drawer)) {
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

void VoxelChunkDefault::set_visibility(bool visible) {
	if (get_mesh_instance_rid() != RID())
		VS::get_singleton()->instance_set_visible(get_mesh_instance_rid(), is_visible_in_tree());

	if (get_prop_mesh_instance_rid() != RID())
		VS::get_singleton()->instance_set_visible(get_prop_mesh_instance_rid(), is_visible_in_tree());
}

void VoxelChunkDefault::free_chunk() {
	free_main_mesh();
	remove_colliders();
	free_prop_mesh();
	free_prop_colliders();
	free_spawn_props();
	free_liquid_mesh();
	free_clutter_mesh();
}

VoxelChunkDefault::VoxelChunkDefault() {
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
	_build_step_in_progress = false;

	_active_build_phase_type = BUILD_PHASE_TYPE_NORMAL;
}

VoxelChunkDefault::~VoxelChunkDefault() {
	if (_build_thread) {
		_abort_build = true;
		wait_and_finish_thread();
	}

	memdelete(_build_phase_done_mutex);
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
			if (!get_create_collider()) {
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

			if (_is_build_threaded) {
				set_active_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
				return;
			}

			if (_body_rid == RID()) {
				create_colliders();
			}

			PhysicsServer::get_singleton()->shape_set_data(_shape_rid, temp_arr_collider);

			//temp_arr_collider.resize(0);

			next_phase();

			return;
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

			if (_mesh_rid != RID())
				VS::get_singleton()->mesh_clear(_mesh_rid);

			Array temp_mesh_arr = mesher->build_mesh();

			if (_mesh_rid == RID()) {
				allocate_main_mesh();
			}

			VS::get_singleton()->mesh_add_surface_from_arrays(_mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

			if (_library->get_material(0).is_valid())
				VS::get_singleton()->mesh_surface_set_material(_mesh_rid, 0, _library->get_material(0)->get_rid());

			next_phase();

			return;
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
					mesher->set_material(get_library()->get_material(0));

					ERR_FAIL_COND(_prop_mesh_rid == RID());

					VS::get_singleton()->mesh_clear(_prop_mesh_rid);

					if (mesher->get_vertex_count() == 0) {
						next_phase();
						return;
					}

					Array arr = mesher->build_mesh();

					VS::get_singleton()->mesh_add_surface_from_arrays(_prop_mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, arr);

					if (_library->get_material(0).is_valid())
						VS::get_singleton()->mesh_surface_set_material(_prop_mesh_rid, 0, _library->get_material(0)->get_rid());
				}
			}

			next_phase();

			return;
		}
		case BUILD_PHASE_PROP_COLLIDER: {
			if (!get_create_collider()) {
				next_phase();
				return;
			}

			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				temp_arr_collider.append_array(mesher->build_collider());
			}

			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->reset();
			}

			if (temp_arr_collider.size() == 0) {
				next_phase();
				return;
			}

			if (_is_build_threaded) {
				set_active_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
				return;
			}

			if (_prop_body_rid == RID()) {
				allocate_prop_colliders();
			}

			PhysicsServer::get_singleton()->shape_set_data(_shape_rid, temp_arr_collider);

			//temp_arr_collider.resize(0);

			next_phase();

			return;
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

		if (_body_rid == RID()) {
			create_colliders();
		}

		PhysicsServer::get_singleton()->shape_set_data(_shape_rid, temp_arr_collider);
		//temp_arr_collider.resize(0);

		set_active_build_phase_type(BUILD_PHASE_TYPE_NORMAL);
		next_phase();

	} else if (phase == BUILD_PHASE_PROP_COLLIDER) {

		if (_prop_body_rid == RID()) {
			allocate_prop_colliders();
		}

		PhysicsServer::get_singleton()->shape_set_data(_prop_shape_rid, temp_arr_collider);
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
	add_mesher(Ref<VoxelMesher>(memnew(VoxelMesherCubic())));

	for (int i = 0; i < _meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_lod_size(get_lod_size());
		mesher->set_voxel_scale(get_voxel_scale());
	}
}

void VoxelChunkDefault::_build(bool immediate) {
	build_deferred();
}

void VoxelChunkDefault::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_VISIBILITY_CHANGED: {
			if (is_inside_tree()) {
				set_visibility(is_visible_in_tree());
			}
		}
		case NOTIFICATION_ENTER_TREE: {
			set_notify_transform(true);
		} break;
		case NOTIFICATION_EXIT_TREE: {
			if (_build_thread) {
				_abort_build = true;

				wait_and_finish_thread();
			}

			free_main_mesh();
			remove_colliders();
			free_prop_mesh();
			free_prop_colliders();
			free_liquid_mesh();
			free_clutter_mesh();

		} break;
		case NOTIFICATION_INTERNAL_PROCESS: {
			if (!get_is_generating()) {
				set_process_internal(false);
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
			break;
			case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
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
			} break;
			case NOTIFICATION_TRANSFORM_CHANGED: {
				update_transforms();
			} break;
		}
	}
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

	ADD_GROUP("Meshing", "meshing");
	ClassDB::bind_method(D_METHOD("meshing_get_create_collider"), &VoxelChunkDefault::get_create_collider);
	ClassDB::bind_method(D_METHOD("meshing_set_create_collider", "value"), &VoxelChunkDefault::set_create_collider);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "meshing_create_collider"), "meshing_set_create_collider", "meshing_get_create_collider");

	ClassDB::bind_method(D_METHOD("meshing_get_bake_lights"), &VoxelChunkDefault::get_bake_lights);
	ClassDB::bind_method(D_METHOD("meshing_set_bake_lights", "value"), &VoxelChunkDefault::set_bake_lights);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "meshing_bake_lights"), "meshing_set_bake_lights", "meshing_get_bake_lights");

	ClassDB::bind_method(D_METHOD("get_mesh_rid"), &VoxelChunkDefault::get_mesh_rid);
	ClassDB::bind_method(D_METHOD("get_mesh_instance_rid"), &VoxelChunkDefault::get_mesh_instance_rid);
	ClassDB::bind_method(D_METHOD("get_shape_rid"), &VoxelChunkDefault::get_shape_rid);
	ClassDB::bind_method(D_METHOD("get_body_rid"), &VoxelChunkDefault::get_body_rid);

	ClassDB::bind_method(D_METHOD("get_prop_mesh_rid"), &VoxelChunkDefault::get_prop_mesh_rid);
	ClassDB::bind_method(D_METHOD("get_prop_mesh_instance_rid"), &VoxelChunkDefault::get_prop_mesh_instance_rid);
	ClassDB::bind_method(D_METHOD("get_prop_shape_rid"), &VoxelChunkDefault::get_prop_shape_rid);
	ClassDB::bind_method(D_METHOD("get_prop_body_rid"), &VoxelChunkDefault::get_prop_body_rid);

	ClassDB::bind_method(D_METHOD("get_liquid_mesh_rid"), &VoxelChunkDefault::get_liquid_mesh_rid);
	ClassDB::bind_method(D_METHOD("get_liquid_mesh_instance_rid"), &VoxelChunkDefault::get_liquid_mesh_instance_rid);

	ClassDB::bind_method(D_METHOD("get_clutter_mesh_rid"), &VoxelChunkDefault::get_clutter_mesh_rid);
	ClassDB::bind_method(D_METHOD("get_clutter_mesh_instance_rid"), &VoxelChunkDefault::get_clutter_mesh_instance_rid);

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

	ClassDB::bind_method(D_METHOD("create_colliders"), &VoxelChunkDefault::create_colliders);
	ClassDB::bind_method(D_METHOD("remove_colliders"), &VoxelChunkDefault::remove_colliders);

	ClassDB::bind_method(D_METHOD("allocate_main_mesh"), &VoxelChunkDefault::allocate_main_mesh);
	ClassDB::bind_method(D_METHOD("free_main_mesh"), &VoxelChunkDefault::free_main_mesh);

	ClassDB::bind_method(D_METHOD("allocate_prop_mesh"), &VoxelChunkDefault::allocate_prop_mesh);
	ClassDB::bind_method(D_METHOD("free_prop_mesh"), &VoxelChunkDefault::free_prop_mesh);

	ClassDB::bind_method(D_METHOD("allocate_prop_colliders"), &VoxelChunkDefault::allocate_prop_colliders);
	ClassDB::bind_method(D_METHOD("free_prop_colliders"), &VoxelChunkDefault::free_prop_colliders);

	ClassDB::bind_method(D_METHOD("allocate_liquid_mesh"), &VoxelChunkDefault::allocate_liquid_mesh);
	ClassDB::bind_method(D_METHOD("free_liquid_mesh"), &VoxelChunkDefault::free_liquid_mesh);

	ClassDB::bind_method(D_METHOD("allocate_clutter_mesh"), &VoxelChunkDefault::allocate_clutter_mesh);
	ClassDB::bind_method(D_METHOD("free_clutter_mesh"), &VoxelChunkDefault::free_clutter_mesh);

	ClassDB::bind_method(D_METHOD("create_debug_immediate_geometry"), &VoxelChunkDefault::create_debug_immediate_geometry);
	ClassDB::bind_method(D_METHOD("free_debug_immediate_geometry"), &VoxelChunkDefault::free_debug_immediate_geometry);

	ClassDB::bind_method(D_METHOD("free_chunk"), &VoxelChunkDefault::free_chunk);

	ClassDB::bind_method(D_METHOD("emit_build_finished"), &VoxelChunkDefault::emit_build_finished);

	BIND_VMETHOD(MethodInfo("_draw_debug_voxel_lights", PropertyInfo(Variant::OBJECT, "debug_drawer", PROPERTY_HINT_RESOURCE_TYPE, "ImmediateGeometry")));

	ClassDB::bind_method(D_METHOD("draw_cross_voxels", "max"), &VoxelChunkDefault::draw_cross_voxels);
	ClassDB::bind_method(D_METHOD("draw_cross_voxels_fill", "max", "fill"), &VoxelChunkDefault::draw_cross_voxels_fill);
	ClassDB::bind_method(D_METHOD("draw_debug_voxels", "max", "color"), &VoxelChunkDefault::draw_debug_voxels, DEFVAL(Color(1, 1, 1)));

	ClassDB::bind_method(D_METHOD("draw_debug_voxel_lights"), &VoxelChunkDefault::draw_debug_voxel_lights);

	ClassDB::bind_method(D_METHOD("_setup_channels"), &VoxelChunkDefault::_setup_channels);
	ClassDB::bind_method(D_METHOD("_build_phase", "phase"), &VoxelChunkDefault::_build_phase);
	ClassDB::bind_method(D_METHOD("_build_phase_process", "phase"), &VoxelChunkDefault::_build_phase_process);
	ClassDB::bind_method(D_METHOD("_build_phase_physics_process", "phase"), &VoxelChunkDefault::_build_phase_physics_process);

	ClassDB::bind_method(D_METHOD("_add_light", "local_x", "local_y", "local_z", "size", "color"), &VoxelChunkDefault::_add_light);

	ClassDB::bind_method(D_METHOD("_clear_baked_lights"), &VoxelChunkDefault::_clear_baked_lights);
	ClassDB::bind_method(D_METHOD("_create_meshers"), &VoxelChunkDefault::_create_meshers);
	ClassDB::bind_method(D_METHOD("_build", "immediate"), &VoxelChunkDefault::_build);

	//ClassDB::bind_method(D_METHOD("set_visibility", "visible"), &VoxelChunkDefault::set_visibility);

	BIND_CONSTANT(BUILD_PHASE_DONE);
	BIND_CONSTANT(BUILD_PHASE_SETUP);
	BIND_CONSTANT(BUILD_PHASE_TERRARIN_MESH_SETUP);
	BIND_CONSTANT(BUILD_PHASE_TERRARIN_MESH_COLLIDER);
	BIND_CONSTANT(BUILD_PHASE_TERRARIN_MESH);
	BIND_CONSTANT(BUILD_PHASE_LIGHTS);
	BIND_CONSTANT(BUILD_PHASE_PROP_MESH);
	BIND_CONSTANT(BUILD_PHASE_PROP_COLLIDER);
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
}
