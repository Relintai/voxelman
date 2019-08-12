#include "voxel_world.h"

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
void VoxelWorld::set_library(const ref<VoxelmanLibrary> library) {
    _library = library;
}

NodePath VoxelWorld::get_player_path() {
	return _player_path;
}

void VoxelWorld::set_player_path(NodePath player_path) {
	_player_path = player_path;
}

VoxelWorld::VoxelWorld() {
    _chunk_size = Vector3i(16, 16, 16);
}

VoxelWorld ::~VoxelWorld() {
	_chunks.clear();
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
	ClassDB::bind_method(D_METHOD("set_library", "value"), &VoxelWorld::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE), "set_library", "get_library");
    
	ClassDB::bind_method(D_METHOD("get_player_path"), &VoxelWorld::get_player_path);
	ClassDB::bind_method(D_METHOD("set_player_path", "value"), &VoxelWorld::set_player_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "player_path"), "set_player_path", "get_player_path");
}
