#include "voxel_world.h"

#include "voxel_chunk.h"

int VoxelWorld::get_chunk_size_x() const {
	return _chunk_size.x;
}
void VoxelWorld::set_chunk_size_x(const int value) {
	_chunk_size.x = value;
}

int VoxelWorld::get_chunk_size_y() const {
	return _chunk_size.y;
}
void VoxelWorld::set_chunk_size_y(const int value) {
	_chunk_size.y = value;
}

int VoxelWorld::get_chunk_size_z() const {
	return _chunk_size.z;
}
void VoxelWorld::set_chunk_size_z(const int value) {
	_chunk_size.z = value;
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
	chunk->set_chunk_position(x, y, z);

	_chunks.set(Vector3i(x, y, z), chunk);
	_chunks_vector.push_back(chunk);
}
void VoxelWorld::add_chunk_bind(Node *chunk, const int x, const int y, const int z) {
	VoxelChunk *v = Object::cast_to<VoxelChunk>(chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(v));

	add_chunk(v, x, y, z);
}
VoxelChunk *VoxelWorld::get_chunk(const int x, const int y, const int z) const {
	if (_chunks.has(Vector3i(x, y, z)))
		return _chunks.get(Vector3i(x, y, z));

	return NULL;
}
VoxelChunk *VoxelWorld::remove_chunk(const int x, const int y, const int z) {
	ERR_FAIL_COND_V(!_chunks.has(Vector3i(x, y, z)), NULL);

	VoxelChunk *chunk = _chunks.get(Vector3i(x, y, z));

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

VoxelWorld::VoxelWorld() {
	_chunk_size = Vector3i(16, 16, 16);

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
}

void VoxelWorld::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &VoxelWorld::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &VoxelWorld::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_y"), &VoxelWorld::get_chunk_size_y);
	ClassDB::bind_method(D_METHOD("set_chunk_size_y", "value"), &VoxelWorld::set_chunk_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_y"), "set_chunk_size_y", "get_chunk_size_y");

	ClassDB::bind_method(D_METHOD("get_chunk_size_z"), &VoxelWorld::get_chunk_size_z);
	ClassDB::bind_method(D_METHOD("set_chunk_size_z", "value"), &VoxelWorld::set_chunk_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_z"), "set_chunk_size_z", "get_chunk_size_z");

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
}
