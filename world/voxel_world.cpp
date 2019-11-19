#include "voxel_world.h"

#include "voxel_chunk.h"

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

uint32_t VoxelWorld::get_max_concurrent_generations() {
	return _max_concurrent_generations;
}
void VoxelWorld::set_max_concurrent_generations(uint32_t value) {
	_max_concurrent_generations = OS::get_singleton()->can_use_threads() ? value : 1;
}

Ref<VoxelmanLibrary> VoxelWorld::get_library() const {
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

void VoxelWorld::add_chunk(VoxelChunk *chunk, const int x, const int y, const int z) {
	chunk->set_position(x, y, z);

	_chunks.set(IntPos(x, y, z), chunk);
	_chunks_vector.push_back(chunk);
}
void VoxelWorld::add_chunk_bind(Node *chunk, const int x, const int y, const int z) {
	VoxelChunk *v = Object::cast_to<VoxelChunk>(chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(v));

	add_chunk(v, x, y, z);
}
VoxelChunk *VoxelWorld::get_chunk(const int x, const int y, const int z) const {
	if (_chunks.has(IntPos(x, y, z)))
		return _chunks.get(IntPos(x, y, z));

	return NULL;
}
VoxelChunk *VoxelWorld::remove_chunk(const int x, const int y, const int z) {
	ERR_FAIL_COND_V(!_chunks.has(IntPos(x, y, z)), NULL);

	VoxelChunk *chunk = _chunks.get(IntPos(x, y, z));

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		if (_chunks_vector.get(i) == chunk) {
			_chunks_vector.remove(i);
			break;
		}
	}

	return chunk;
}

VoxelChunk *VoxelWorld::get_chunk_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	return _chunks_vector.get(index);
}
int VoxelWorld::get_chunk_count() const {
	return _chunks_vector.size();
}

void VoxelWorld::clear_chunks() {
	for (int i = 0; i < _chunks_vector.size(); ++i) {
		_chunks_vector.get(i)->queue_delete();
	}

	_chunks_vector.clear();

	_chunks.clear();
}

void VoxelWorld::create_chunk(int x, int y, int z) {
	call("_create_chunk");
}

void VoxelWorld::_create_chunk(int x, int y, int z) {

}

void VoxelWorld::generate_chunk_bind(Node *p_chunk) {
	generate_chunk(Object::cast_to<VoxelChunk>(p_chunk));
}
void VoxelWorld::generate_chunk(VoxelChunk *p_chunk) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(p_chunk));

	p_chunk->set_is_generating(true);

	if (has_method("_prepare_chunk_for_generation"))
		call("_prepare_chunk_for_generation", p_chunk);


	call("_generate_chunk", p_chunk);
}

void VoxelWorld::_generate_chunk(Node *p_chunk) {
	VoxelChunk *chunk = Object::cast_to<VoxelChunk>(p_chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));
	ERR_FAIL_COND(!_level_generator.is_valid());

	_level_generator->generate_chunk(chunk);
}

VoxelWorld::VoxelWorld() {
	_chunk_size_x = 16;
	_chunk_size_y = 16;
	_chunk_size_z = 16;
	_current_seed = 0;

	set_use_threads(true);
	set_max_concurrent_generations(3);

	_voxel_scale = 1;
	_chunk_spawn_range = 4;

	_player_path;
	_player = NULL;
}

VoxelWorld ::~VoxelWorld() {
	_chunks.clear();
	_chunks_vector.clear();
	_world_areas.clear();

	_library.unref();
	_level_generator.unref();

	_player = NULL;

	_generation_queue.clear();
	_generating.clear();;
}

void VoxelWorld::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			set_process_internal(false);

			if (!Engine::get_singleton()->is_editor_hint() && _library.is_valid())
				_library->refresh_rects();
		}
		case NOTIFICATION_INTERNAL_PROCESS: {
			if (_generation_queue.empty() && _generating.empty()) {
				call("_generation_finished");

				emit_signal("generation_finished");

				set_process_internal(false);
				return;
			}

			for (int i = 0; i < _generating.size(); ++i) {
				VoxelChunk *chunk = _generating.get(i);

				if (!ObjectDB::instance_validate(chunk) || chunk->get_is_generating()) {
					_generating.remove(i);
					--i;
					continue;
				}
			}

			if (_generating.size() >= _max_concurrent_generations)
				return;

			if (_generation_queue.size == 0)
				return;

			VoxelChunk *chunk = _generation_queue.get(0);
			_generation_queue.remove(0);

			ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

			_generating.push_back(chunk);

			generate_chunk(chunk);
		}
	}
}

void VoxelWorld::_bind_methods() {
	ADD_SIGNAL(MethodInfo("generation_finished"));
	BIND_VMETHOD(MethodInfo("_generation_finished"));

	BIND_VMETHOD(MethodInfo("_prepare_chunk_for_generation", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &VoxelWorld::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &VoxelWorld::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_y"), &VoxelWorld::get_chunk_size_y);
	ClassDB::bind_method(D_METHOD("set_chunk_size_y", "value"), &VoxelWorld::set_chunk_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_y"), "set_chunk_size_y", "get_chunk_size_y");

	ClassDB::bind_method(D_METHOD("get_chunk_size_z"), &VoxelWorld::get_chunk_size_z);
	ClassDB::bind_method(D_METHOD("set_chunk_size_z", "value"), &VoxelWorld::set_chunk_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_z"), "set_chunk_size_z", "get_chunk_size_z");

	ClassDB::bind_method(D_METHOD("get_current_seed"), &VoxelWorld::get_current_seed);
	ClassDB::bind_method(D_METHOD("set_current_seed", "value"), &VoxelWorld::set_current_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_seed"), "set_current_seed", "get_current_seed");

	ClassDB::bind_method(D_METHOD("get_use_threads"), &VoxelWorld::get_use_threads);
	ClassDB::bind_method(D_METHOD("set_use_threads", "value"), &VoxelWorld::set_use_threads);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_threads"), "set_use_threads", "get_use_threads");

	ClassDB::bind_method(D_METHOD("get_max_concurrent_generations"), &VoxelWorld::get_max_concurrent_generations);
	ClassDB::bind_method(D_METHOD("set_max_concurrent_generations", "value"), &VoxelWorld::set_max_concurrent_generations);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_concurrent_generations"), "set_max_concurrent_generations", "get_max_concurrent_generations");

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

	ClassDB::bind_method(D_METHOD("get_world_area", "index"), &VoxelWorld::get_world_area);
	ClassDB::bind_method(D_METHOD("add_world_area", "area"), &VoxelWorld::add_world_area);
	ClassDB::bind_method(D_METHOD("remove_world_area", "index"), &VoxelWorld::remove_world_area);
	ClassDB::bind_method(D_METHOD("clear_world_areas"), &VoxelWorld::clear_world_areas);
	ClassDB::bind_method(D_METHOD("get_world_area_count"), &VoxelWorld::get_world_area_count);

	ClassDB::bind_method(D_METHOD("add_chunk", "chunk", "x", "y", "z"), &VoxelWorld::add_chunk_bind);
	ClassDB::bind_method(D_METHOD("get_chunk", "x", "y", "z"), &VoxelWorld::get_chunk);
	ClassDB::bind_method(D_METHOD("remove_chunk", "x", "y", "z"), &VoxelWorld::remove_chunk);

	ClassDB::bind_method(D_METHOD("get_chunk_index", "index"), &VoxelWorld::get_chunk_index);
	ClassDB::bind_method(D_METHOD("get_chunk_count"), &VoxelWorld::get_chunk_count);

	ClassDB::bind_method(D_METHOD("clear_chunks"), &VoxelWorld::clear_chunks);

	ClassDB::bind_method(D_METHOD("generate_chunk", "chunk"), &VoxelWorld::generate_chunk_bind);
}
