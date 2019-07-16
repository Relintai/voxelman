#include "voxel_light.h"

int VoxelLight::get_chunk_position_x() {
    return _chunk_position.x;
}
int VoxelLight::get_chunk_position_y() {
    return _chunk_position.y;
}
int VoxelLight::get_chunk_position_z() {
    return _chunk_position.z;
}
Vector3i VoxelLight::get_chunk_position() { 
    return _chunk_position; 
}
void VoxelLight::set_chunk_position(int x, int y, int z) { 
    _chunk_position.x = x; 
    _chunk_position.y = y; 
    _chunk_position.z = z; 
}

int VoxelLight::get_world_position_x() {
    return _world_position.x;
}
int VoxelLight::get_world_position_y() {
    return _world_position.y;
}
int VoxelLight::get_world_position_z() {
    return _world_position.z;
}
Vector3i VoxelLight::get_world_position() { 
    return _world_position; 
    
}
void VoxelLight::set_world_position(int x, int y, int z) {
    _world_position.x = x; 
    _world_position.y = y; 
    _world_position.z = z; 
}

Color VoxelLight::get_color() { 
    return _color; 
    
}
void VoxelLight::set_color(Color color) { 
    _color = color; 
    
}

float VoxelLight::get_size() { 
    return _size; 
    
}
void VoxelLight::set_size(float size) { 
    _size = size; 
}

VoxelLight::VoxelLight() {
    _size = 0;
}

VoxelLight::~VoxelLight() {
}

void VoxelLight::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_chunk_position_x"), &VoxelLight::get_chunk_position_x);
    ClassDB::bind_method(D_METHOD("get_chunk_position_y"), &VoxelLight::get_chunk_position_y);
    ClassDB::bind_method(D_METHOD("get_chunk_position_z"), &VoxelLight::get_chunk_position_z);
    ClassDB::bind_method(D_METHOD("set_chunk_position", "x", "y", "z"), &VoxelLight::set_chunk_position);
    
    ClassDB::bind_method(D_METHOD("get_world_position_x"), &VoxelLight::get_world_position_x);
    ClassDB::bind_method(D_METHOD("get_world_position_y"), &VoxelLight::get_world_position_y);
    ClassDB::bind_method(D_METHOD("get_world_position_z"), &VoxelLight::get_world_position_z);
    ClassDB::bind_method(D_METHOD("set_world_position", "x", "y", "z"), &VoxelLight::set_world_position);
    
    ClassDB::bind_method(D_METHOD("get_color"), &VoxelLight::get_color);
    ClassDB::bind_method(D_METHOD("set_color"), &VoxelLight::set_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
    
    ClassDB::bind_method(D_METHOD("get_size"), &VoxelLight::get_size);
    ClassDB::bind_method(D_METHOD("set_size"), &VoxelLight::set_size);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");
}
