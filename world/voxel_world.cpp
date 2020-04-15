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

#include "voxel_world.h"

#include "voxel_chunk.h"
#include "voxel_chunk_prop_data.h"

#include "core/version.h"

#if VERSION_MAJOR >= 4
#define REAL FLOAT
#endif

bool VoxelWorld::get_editable() const {
	return _editable;
}
void VoxelWorld::set_editable(const bool value) {
	_editable = value;
}

int VoxelWorld::get_chunk_size_x() const {
	return _chunk_size_x;
}
void VoxelWorld::set_chunk_size_x(const int value) {
	_chunk_size_x = value;
}

int VoxelWorld::get_chunk_size_y() const {
	return _chunk_size_y;
}
void VoxelWorld::set_chunk_size_y(const int value) {
	_chunk_size_y = value;
}

int VoxelWorld::get_chunk_size_z() const {
	return _chunk_size_z;
}
void VoxelWorld::set_chunk_size_z(const int value) {
	_chunk_size_z = value;
}

int VoxelWorld::get_data_margin_start() const {
	return _data_margin_start;
}
void VoxelWorld::set_data_margin_start(const int value) {
	_data_margin_start = value;
}

int VoxelWorld::get_data_margin_end() const {
	return _data_margin_end;
}
void VoxelWorld::set_data_margin_end(const int value) {
	_data_margin_end = value;
}

int VoxelWorld::get_current_seed() const {
	return _current_seed;
}
void VoxelWorld::set_current_seed(const int value) {
	_current_seed = value;
}

bool VoxelWorld::get_use_threads() {
	return _use_threads;
}
void VoxelWorld::set_use_threads(bool value) {
	_use_threads = OS::get_singleton()->can_use_threads() ? value : false;
}

int VoxelWorld::get_max_concurrent_generations() const {
	return _max_concurrent_generations;
}
void VoxelWorld::set_max_concurrent_generations(const int value) {
	_max_concurrent_generations = OS::get_singleton()->can_use_threads() ? value : 1;
}

int VoxelWorld::get_max_frame_chunk_build_steps() const {
	return _max_frame_chunk_build_steps;
}
void VoxelWorld::set_max_frame_chunk_build_steps(const int value) {
	_max_frame_chunk_build_steps = value;
}

Ref<VoxelmanLibrary> VoxelWorld::get_library() {
	return _library;
}
void VoxelWorld::set_library(const Ref<VoxelmanLibrary> library) {
	_library = library;
}

Ref<VoxelmanLevelGenerator> VoxelWorld::get_level_generator() const {
	return _level_generator;
}
void VoxelWorld::set_level_generator(const Ref<VoxelmanLevelGenerator> level_generator) {
	_level_generator = level_generator;
}

float VoxelWorld::get_voxel_scale() const {
	return _voxel_scale;
}
void VoxelWorld::set_voxel_scale(const float value) {
	_voxel_scale = value;
}

int VoxelWorld::get_chunk_spawn_range() const {
	return _chunk_spawn_range;
}
void VoxelWorld::set_chunk_spawn_range(const int value) {
	_chunk_spawn_range = value;
}

NodePath VoxelWorld::get_player_path() {
	return _player_path;
}

void VoxelWorld::set_player_path(NodePath player_path) {
	_player_path = player_path;
}

Spatial *VoxelWorld::get_player() const {
	return _player;
}
void VoxelWorld::set_player(Spatial *player) {
	_player = player;
}
void VoxelWorld::set_player_bind(Node *player) {
	set_player(Object::cast_to<Spatial>(player));
}

Ref<WorldArea> VoxelWorld::get_world_area(const int index) const {
	ERR_FAIL_INDEX_V(index, _world_areas.size(), Ref<WorldArea>());

	return _world_areas.get(index);
}
void VoxelWorld::add_world_area(Ref<WorldArea> area) {
	_world_areas.push_back(area);
}
void VoxelWorld::remove_world_area(const int index) {
	ERR_FAIL_INDEX(index, _world_areas.size());

	_world_areas.remove(index);
}
void VoxelWorld::clear_world_areas() {
	_world_areas.clear();
}
int VoxelWorld::get_world_area_count() const {
	return _world_areas.size();
}

void VoxelWorld::add_chunk(Ref<VoxelChunk> chunk, const int x, const int y, const int z) {
	ERR_FAIL_COND(!chunk.is_valid());

	IntPos pos(x, y, z);

	ERR_FAIL_COND(_chunks.has(pos));

	chunk->set_voxel_world(this);
	chunk->set_position(x, y, z);
	chunk->world_transform_changed();

	_chunks.set(pos, chunk);
	_chunks_vector.push_back(chunk);

	chunk->enter_tree();

	if (has_method("_chunk_added"))
		call("_chunk_added", chunk);
}
bool VoxelWorld::has_chunk(const int x, const int y, const int z) const {
	return _chunks.has(IntPos(x, y, z));
}
Ref<VoxelChunk> VoxelWorld::get_chunk(const int x, const int y, const int z) {
	IntPos pos(x, y, z);

	if (_chunks.has(pos))
		return _chunks.get(pos);

	return NULL;
}
Ref<VoxelChunk> VoxelWorld::remove_chunk(const int x, const int y, const int z) {
	IntPos pos(x, y, z);

	if (!_chunks.has(pos))
		return NULL;

	Ref<VoxelChunk> chunk = _chunks.get(pos);

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		if (_chunks_vector.get(i) == chunk) {
			_chunks_vector.remove(i);
			break;
		}
	}

	chunk->exit_tree();

	ERR_FAIL_COND_V(!_chunks.erase(pos), NULL);

	//_chunks.erase(pos);

	return chunk;
}
Ref<VoxelChunk> VoxelWorld::remove_chunk_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	Ref<VoxelChunk> chunk = _chunks_vector.get(index);
	_chunks_vector.remove(index);
	_chunks.erase(IntPos(chunk->get_position_x(), chunk->get_position_y(), chunk->get_position_z()));
	chunk->exit_tree();

	return chunk;
}

Ref<VoxelChunk> VoxelWorld::get_chunk_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	return _chunks_vector.get(index);
}
int VoxelWorld::get_chunk_count() const {
	return _chunks_vector.size();
}

void VoxelWorld::add_to_generation_queue(Ref<VoxelChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	set_process_internal(true);

	_generation_queue.push_back(chunk);
}
Ref<VoxelChunk> VoxelWorld::get_generation_queue_index(int index) {
	ERR_FAIL_INDEX_V(index, _generation_queue.size(), NULL);

	return _generation_queue.get(index);
}
void VoxelWorld::remove_generation_queue_index(int index) {
	ERR_FAIL_INDEX(index, _generation_queue.size());

	_generation_queue.remove(index);
}
int VoxelWorld::get_generation_queue_size() {
	return _generation_queue.size();
}

void VoxelWorld::add_to_generation(Ref<VoxelChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	_generating.push_back(chunk);
}
Ref<VoxelChunk> VoxelWorld::get_generation_index(int index) {
	ERR_FAIL_INDEX_V(index, _generating.size(), NULL);

	return _generating.get(index);
}
void VoxelWorld::remove_generation_index(int index) {
	ERR_FAIL_INDEX(index, _generating.size());

	_generating.remove(index);
}
int VoxelWorld::get_generation_size() {
	return _generating.size();
}

void VoxelWorld::clear_chunks() {
	for (int i = 0; i < _chunks_vector.size(); ++i) {
		_chunks_vector.get(i)->exit_tree();
	}

	_chunks_vector.clear();

	_chunks.clear();

	_generation_queue.clear();
	_generating.clear();
}

Ref<VoxelChunk> VoxelWorld::get_or_create_chunk(int x, int y, int z) {
	Ref<VoxelChunk> chunk = get_chunk(x, y, z);

	if (!chunk.is_valid()) {
		chunk = create_chunk(x, y, z);
	}

	return chunk;
}

Ref<VoxelChunk> VoxelWorld::create_chunk(int x, int y, int z) {
	Ref<VoxelChunk> c = call("_create_chunk", x, y, z, Ref<VoxelChunk>());

	return c;
}
Ref<VoxelChunk> VoxelWorld::_create_chunk(int x, int y, int z, Ref<VoxelChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk.instance();
	}

	ERR_FAIL_COND_V(!chunk.is_valid(), NULL);

	chunk->set_name("Chunk[" + String::num(x) + "," + String::num(y) + "," + String::num(z) + "]");

	chunk->set_voxel_world(this);

	//TODO this will need to be changed
	if (chunk->has_method("set_is_build_threaded"))
		chunk->call("set_is_build_threaded", _use_threads);

	chunk->set_position(x, y, z);
	chunk->set_library(_library);
	chunk->set_voxel_scale(_voxel_scale);
	chunk->set_size(_chunk_size_x, _chunk_size_y, _chunk_size_z, _data_margin_start, _data_margin_end);
	//chunk->set_translation(Vector3(x * _chunk_size_x * _voxel_scale, y * _chunk_size_y * _voxel_scale, z * _chunk_size_z * _voxel_scale));

	add_chunk(chunk, x, y, z);

	add_to_generation_queue(chunk);

	return chunk;
}

void VoxelWorld::generate_chunk(Ref<VoxelChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	chunk->set_is_generating(true);

	if (has_method("_prepare_chunk_for_generation"))
		call("_prepare_chunk_for_generation", chunk);

	call("_generate_chunk", chunk);

	chunk->build();
}

bool VoxelWorld::can_chunk_do_build_step() {
	if (_max_frame_chunk_build_steps == 0) {
		return true;
	}

	return _num_frame_chunk_build_steps++ < _max_frame_chunk_build_steps;
}

bool VoxelWorld::is_position_walkable(const Vector3 &p_pos) {

	int x = static_cast<int>(Math::floor(p_pos.x / (_chunk_size_x * _voxel_scale)));
	int y = static_cast<int>(Math::floor(p_pos.y / (_chunk_size_y * _voxel_scale)));
	int z = static_cast<int>(Math::floor(p_pos.z / (_chunk_size_z * _voxel_scale)));

	Ref<VoxelChunk> c = get_chunk(x, y, z);

	if (!c.is_valid())
		return false;

	return !c->get_is_generating();
}

void VoxelWorld::on_chunk_mesh_generation_finished(Ref<VoxelChunk> p_chunk) {
	call_deferred("emit_signal", "chunk_mesh_generation_finished", p_chunk);
}

Vector<Variant> VoxelWorld::get_chunks() {
	Vector<Variant> r;
	for (int i = 0; i < _chunks_vector.size(); i++) {
#if VERSION_MAJOR < 4
		r.push_back(_chunks_vector[i].get_ref_ptr());
#else
		r.push_back(_chunks_vector[i]);
#endif
	}
	return r;
}

void VoxelWorld::set_chunks(const Vector<Variant> &chunks) {

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<VoxelChunk> chunk = Ref<VoxelChunk>(_chunks_vector[i]);

		if (chunks.find(chunk) == -1) {
			remove_chunk_index(i);
			_generation_queue.erase(chunk);
			_generating.erase(chunk);
			--i;
		}
	}

	for (int i = 0; i < chunks.size(); ++i) {
		Ref<VoxelChunk> chunk = Ref<VoxelChunk>(chunks[i]);

		if (!chunk.is_valid())
			continue;

		if (_chunks_vector.find(chunk) != -1) {
			continue;
		}

		add_chunk(chunk, chunk->get_position_x(), chunk->get_position_y(), chunk->get_position_z());
	}
}

//Props
void VoxelWorld::add_prop(Ref<VoxelChunkPropData> prop) {
	ERR_FAIL_COND(!has_method("_add_prop"));

	call("_add_prop", prop);
}

//Lights
void VoxelWorld::add_light(const Ref<VoxelLight> &light) {
	_lights.push_back(light);

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<VoxelChunk> chunk = _chunks_vector[i];

		if (chunk.is_valid()) {
			chunk->world_light_added(light);
		}
	}
}
Ref<VoxelLight> VoxelWorld::get_light(const int index) {
	ERR_FAIL_INDEX_V(index, _lights.size(), Ref<VoxelLight>());

	return _lights.get(index);
}
void VoxelWorld::remove_light(const int index) {
	ERR_FAIL_INDEX(index, _lights.size());

	Ref<VoxelLight> light = _lights[index];

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<VoxelChunk> chunk = _chunks_vector[i];

		if (chunk.is_valid()) {
			chunk->world_light_removed(light);
		}
	}
}
int VoxelWorld::get_light_count() const {
	return _lights.size();
}
void VoxelWorld::clear_lights() {
	for (int i = 0; i < _lights.size(); ++i) {

		Ref<VoxelLight> light = _lights[i];

		if (!light.is_valid())
			continue;

		for (int j = 0; j < _chunks_vector.size(); ++j) {
			Ref<VoxelChunk> chunk = _chunks_vector[j];

			if (chunk.is_valid()) {
				chunk->world_light_removed(light);
			}
		}
	}

	_lights.clear();
}

Vector<Variant> VoxelWorld::get_lights() {
	Vector<Variant> r;
	for (int i = 0; i < _lights.size(); i++) {
#if VERSION_MAJOR < 4
		r.push_back(_lights[i].get_ref_ptr());
#else
		r.push_back(_lights[i]);
#endif
	}
	return r;
}
void VoxelWorld::set_lights(const Vector<Variant> &chunks) {
	clear_lights();

	for (int i = 0; i < chunks.size(); ++i) {
		Ref<VoxelLight> light = Ref<VoxelLight>(chunks[i]);

		add_light(light);
	}
}

VoxelWorld::VoxelWorld() {
	_editable = false;

	_is_priority_generation = true;

	_chunk_size_x = 16;
	_chunk_size_y = 16;
	_chunk_size_z = 16;
	_current_seed = 0;
	_data_margin_start = 0;
	_data_margin_end = 0;

	set_use_threads(true);
	set_max_concurrent_generations(3);

	_voxel_scale = 1;
	_chunk_spawn_range = 4;

	_player = NULL;
	_max_frame_chunk_build_steps = 0;
	_num_frame_chunk_build_steps = 0;
}

VoxelWorld ::~VoxelWorld() {
	_chunks.clear();
	_chunks_vector.clear();
	_world_areas.clear();

	_library.unref();
	_level_generator.unref();

	_player = NULL;

	_generation_queue.clear();
	_generating.clear();

	_lights.clear();
}

void VoxelWorld::_generate_chunk(Ref<VoxelChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());
	ERR_FAIL_COND(!_level_generator.is_valid());

	_level_generator->generate_chunk(chunk);
}

void VoxelWorld::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			set_process_internal(true);
			set_physics_process_internal(true);
			set_notify_transform(true);

			if (_library.is_valid())
				_library->refresh_rects();

			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<VoxelChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					IntPos pos(chunk->get_position_x(), chunk->get_position_y(), chunk->get_position_z());

					chunk->set_voxel_world(this);
					chunk->world_transform_changed();

					_chunks.set(pos, chunk);

					chunk->enter_tree();

					chunk->build();
				}
			}
		} break;
		case NOTIFICATION_INTERNAL_PROCESS: {
			_num_frame_chunk_build_steps = 0;

			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<VoxelChunk> chunk = _chunks_vector[i];

				ERR_CONTINUE(!chunk.is_valid());

				if (chunk->get_process()) {
					chunk->process(get_physics_process_delta_time());
				}
			}

			if (_is_priority_generation && _generation_queue.empty() && _generating.empty()) {
				_is_priority_generation = false;

				call("_generation_finished");

				emit_signal("generation_finished");

				return;
			}

			for (int i = 0; i < _generating.size(); ++i) {
				Ref<VoxelChunk> chunk = _generating.get(i);

				if (!chunk.is_valid() || !chunk->get_is_generating()) {
					_generating.remove(i);
					--i;
					continue;
				}
			}

			if (_generating.size() >= _max_concurrent_generations)
				return;

			if (_generation_queue.size() == 0)
				return;

			while (_generating.size() < _max_concurrent_generations && _generation_queue.size() != 0) {
				Ref<VoxelChunk> chunk = _generation_queue.get(0);
				_generation_queue.remove(0);

				ERR_FAIL_COND(!chunk.is_valid());

				_generating.push_back(chunk);

				generate_chunk(chunk);
			}
		} break;
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<VoxelChunk> chunk = _chunks_vector[i];

				ERR_CONTINUE(!chunk.is_valid());

				if (chunk->get_process()) {
					chunk->physics_process(get_physics_process_delta_time());
				}
			}

		} break;
		case NOTIFICATION_EXIT_TREE: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<VoxelChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					chunk->exit_tree();
				}
			}

		} break;
		case NOTIFICATION_TRANSFORM_CHANGED: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<VoxelChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					chunk->world_transform_changed();
				}
			}

		} break;
	}
}

void VoxelWorld::_bind_methods() {
	ADD_SIGNAL(MethodInfo("chunk_mesh_generation_finished", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	ClassDB::bind_method(D_METHOD("get_editable"), &VoxelWorld::get_editable);
	ClassDB::bind_method(D_METHOD("set_editable", "value"), &VoxelWorld::set_editable);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "editable"), "set_editable", "get_editable");

	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &VoxelWorld::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &VoxelWorld::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_y"), &VoxelWorld::get_chunk_size_y);
	ClassDB::bind_method(D_METHOD("set_chunk_size_y", "value"), &VoxelWorld::set_chunk_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_y"), "set_chunk_size_y", "get_chunk_size_y");

	ClassDB::bind_method(D_METHOD("get_chunk_size_z"), &VoxelWorld::get_chunk_size_z);
	ClassDB::bind_method(D_METHOD("set_chunk_size_z", "value"), &VoxelWorld::set_chunk_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_z"), "set_chunk_size_z", "get_chunk_size_z");

	ClassDB::bind_method(D_METHOD("get_data_margin_start"), &VoxelWorld::get_data_margin_start);
	ClassDB::bind_method(D_METHOD("set_data_margin_start", "value"), &VoxelWorld::set_data_margin_start);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_margin_start"), "set_data_margin_start", "get_data_margin_start");

	ClassDB::bind_method(D_METHOD("get_data_margin_end"), &VoxelWorld::get_data_margin_end);
	ClassDB::bind_method(D_METHOD("set_data_margin_end", "value"), &VoxelWorld::set_data_margin_end);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_margin_end"), "set_data_margin_end", "get_data_margin_end");

	ClassDB::bind_method(D_METHOD("get_current_seed"), &VoxelWorld::get_current_seed);
	ClassDB::bind_method(D_METHOD("set_current_seed", "value"), &VoxelWorld::set_current_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_seed"), "set_current_seed", "get_current_seed");

	ClassDB::bind_method(D_METHOD("get_use_threads"), &VoxelWorld::get_use_threads);
	ClassDB::bind_method(D_METHOD("set_use_threads", "value"), &VoxelWorld::set_use_threads);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_threads"), "set_use_threads", "get_use_threads");

	ClassDB::bind_method(D_METHOD("get_max_concurrent_generations"), &VoxelWorld::get_max_concurrent_generations);
	ClassDB::bind_method(D_METHOD("set_max_concurrent_generations", "value"), &VoxelWorld::set_max_concurrent_generations);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_concurrent_generations"), "set_max_concurrent_generations", "get_max_concurrent_generations");

	ClassDB::bind_method(D_METHOD("get_max_frame_chunk_build_steps"), &VoxelWorld::get_max_frame_chunk_build_steps);
	ClassDB::bind_method(D_METHOD("set_max_frame_chunk_build_steps", "value"), &VoxelWorld::set_max_frame_chunk_build_steps);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_frame_chunk_build_steps"), "set_max_frame_chunk_build_steps", "get_max_frame_chunk_build_steps");

	ClassDB::bind_method(D_METHOD("get_library"), &VoxelWorld::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "library"), &VoxelWorld::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_level_generator"), &VoxelWorld::get_level_generator);
	ClassDB::bind_method(D_METHOD("set_level_generator", "level_generator"), &VoxelWorld::set_level_generator);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "level_generator", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanLevelGenerator"), "set_level_generator", "get_level_generator");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &VoxelWorld::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &VoxelWorld::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_chunk_spawn_range"), &VoxelWorld::get_chunk_spawn_range);
	ClassDB::bind_method(D_METHOD("set_chunk_spawn_range", "value"), &VoxelWorld::set_chunk_spawn_range);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_spawn_range"), "set_chunk_spawn_range", "get_chunk_spawn_range");

	ClassDB::bind_method(D_METHOD("get_player_path"), &VoxelWorld::get_player_path);
	ClassDB::bind_method(D_METHOD("set_player_path", "value"), &VoxelWorld::set_player_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "player_path"), "set_player_path", "get_player_path");

	ClassDB::bind_method(D_METHOD("get_player"), &VoxelWorld::get_player);
	ClassDB::bind_method(D_METHOD("set_player", "player"), &VoxelWorld::set_player_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "player", PROPERTY_HINT_RESOURCE_TYPE, "Spatial"), "set_player", "get_player");

	ClassDB::bind_method(D_METHOD("get_chunks"), &VoxelWorld::get_chunks);
	ClassDB::bind_method(D_METHOD("set_chunks"), &VoxelWorld::set_chunks);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "chunks", PROPERTY_HINT_NONE, "17/17:VoxelChunk", PROPERTY_USAGE_DEFAULT, "VoxelChunk"), "set_chunks", "get_chunks");

	ClassDB::bind_method(D_METHOD("get_world_area", "index"), &VoxelWorld::get_world_area);
	ClassDB::bind_method(D_METHOD("add_world_area", "area"), &VoxelWorld::add_world_area);
	ClassDB::bind_method(D_METHOD("remove_world_area", "index"), &VoxelWorld::remove_world_area);
	ClassDB::bind_method(D_METHOD("clear_world_areas"), &VoxelWorld::clear_world_areas);
	ClassDB::bind_method(D_METHOD("get_world_area_count"), &VoxelWorld::get_world_area_count);

	BIND_VMETHOD(MethodInfo("_chunk_added", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	ClassDB::bind_method(D_METHOD("add_chunk", "chunk", "x", "y", "z"), &VoxelWorld::add_chunk);
	ClassDB::bind_method(D_METHOD("has_chunk", "x", "y", "z"), &VoxelWorld::has_chunk);
	ClassDB::bind_method(D_METHOD("get_chunk", "x", "y", "z"), &VoxelWorld::get_chunk);
	ClassDB::bind_method(D_METHOD("remove_chunk", "x", "y", "z"), &VoxelWorld::remove_chunk);
	ClassDB::bind_method(D_METHOD("remove_chunk_index", "index"), &VoxelWorld::remove_chunk_index);

	ClassDB::bind_method(D_METHOD("get_chunk_index", "index"), &VoxelWorld::get_chunk_index);
	ClassDB::bind_method(D_METHOD("get_chunk_count"), &VoxelWorld::get_chunk_count);

	ClassDB::bind_method(D_METHOD("add_to_generation_queue", "chunk"), &VoxelWorld::add_to_generation_queue);
	ClassDB::bind_method(D_METHOD("get_generation_queue_index", "index"), &VoxelWorld::get_generation_queue_index);
	ClassDB::bind_method(D_METHOD("remove_generation_queue_index", "index"), &VoxelWorld::remove_generation_queue_index);
	ClassDB::bind_method(D_METHOD("get_generation_queue_size"), &VoxelWorld::get_generation_queue_size);

	ClassDB::bind_method(D_METHOD("add_to_generation", "chunk"), &VoxelWorld::add_to_generation);
	ClassDB::bind_method(D_METHOD("get_generation_index", "index"), &VoxelWorld::get_generation_index);
	ClassDB::bind_method(D_METHOD("remove_generation_index", "index"), &VoxelWorld::remove_generation_index);
	ClassDB::bind_method(D_METHOD("get_generation_size"), &VoxelWorld::get_generation_size);

	ClassDB::bind_method(D_METHOD("clear_chunks"), &VoxelWorld::clear_chunks);

	ADD_SIGNAL(MethodInfo("generation_finished"));
	BIND_VMETHOD(MethodInfo("_generation_finished"));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"), "_create_chunk", PropertyInfo(Variant::INT, "x"), PropertyInfo(Variant::INT, "y"), PropertyInfo(Variant::INT, "z"), PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_prepare_chunk_for_generation", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	ClassDB::bind_method(D_METHOD("get_or_create_chunk", "x", "y", "z"), &VoxelWorld::get_or_create_chunk);
	ClassDB::bind_method(D_METHOD("create_chunk", "x", "y", "z"), &VoxelWorld::create_chunk);
	ClassDB::bind_method(D_METHOD("_create_chunk", "x", "y", "z", "chunk"), &VoxelWorld::_create_chunk);

	ClassDB::bind_method(D_METHOD("_generate_chunk", "chunk"), &VoxelWorld::_generate_chunk);

	ClassDB::bind_method(D_METHOD("can_chunk_do_build_step"), &VoxelWorld::can_chunk_do_build_step);
	ClassDB::bind_method(D_METHOD("is_position_walkable", "position"), &VoxelWorld::is_position_walkable);
	ClassDB::bind_method(D_METHOD("on_chunk_mesh_generation_finished", "chunk"), &VoxelWorld::on_chunk_mesh_generation_finished);

	//Props
	BIND_VMETHOD(MethodInfo("_add_prop", PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunkPropData")));

	ClassDB::bind_method(D_METHOD("add_prop", "prop"), &VoxelWorld::add_prop);

	//Lights
	ClassDB::bind_method(D_METHOD("add_light", "light"), &VoxelWorld::add_light);
	ClassDB::bind_method(D_METHOD("get_light", "index"), &VoxelWorld::get_light);
	ClassDB::bind_method(D_METHOD("remove_light", "index"), &VoxelWorld::remove_light);
	ClassDB::bind_method(D_METHOD("get_light_count"), &VoxelWorld::get_light_count);
	ClassDB::bind_method(D_METHOD("clear_lights"), &VoxelWorld::clear_lights);

	ClassDB::bind_method(D_METHOD("get_lights"), &VoxelWorld::get_lights);
	ClassDB::bind_method(D_METHOD("set_lights", "chunks"), &VoxelWorld::set_lights);
}
