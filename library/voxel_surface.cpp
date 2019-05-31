#include "voxel_surface.h"

int VoxelSurface::get_id() const {
    return _id;
}
void VoxelSurface::set_id(int value) {
    _id = value;
}

Color VoxelSurface::get_voxel_color() const {
    return _voxel_color;
}
void VoxelSurface::set_voxel_color(Color value) {
    _voxel_color = value;
}

int VoxelSurface::get_atlas_texture_id() const {
    return _atlas_texture_id;
}
void VoxelSurface::set_atlas_texture_id(int value) {
    _atlas_texture_id = value;
}

bool VoxelSurface::has_face_color() const {
    return false;
}

int VoxelSurface::get_mesh_id() const {
    return _mesh_id;
}
void VoxelSurface::set_mesh_id(int value) {
    _mesh_id = value;
}

Vector3 VoxelSurface::get_mesh_offset() const {
    return _mesh_offset;
}
void VoxelSurface::set_mesh_offset(Vector3 value) {
    _mesh_offset = value;
}

int VoxelSurface::get_prefab_id() const {
    return _prefab_id;
}
void VoxelSurface::set_prefab_id(int value) {
    _prefab_id = value;
}

Vector3 VoxelSurface::get_prefab_offset() const {
    return _prefab_offset;
}
void VoxelSurface::set_prefab_offset(Vector3 value) {
    _prefab_offset = value;
}

Vector3 VoxelSurface::get_prefab_rotation() const {
    return _prefab_rotation;
}
void VoxelSurface::set_prefab_rotation(Vector3 value) {
    _prefab_rotation = value;
}

bool VoxelSurface::get_light() const {
    return _light;
}
void VoxelSurface::set_light(bool value) {
    _light = value;
}

float VoxelSurface::get_light_strength() const {
    return _light_strength;
}
void VoxelSurface::set_light_strength(float value) {
    _light_strength = value;
}

Color VoxelSurface::get_light_color() const {
    return _light_color;
}
void VoxelSurface::set_light_color(Color value) {
    _light_color = value;
}

Vector3 VoxelSurface::get_light_offset() const {
    return _light_offset;
}
void VoxelSurface::set_light_offset(Vector3 value) {
    _light_offset = value;
}

bool VoxelSurface::is_transparent() const {
    return _is_transparent;
}
void VoxelSurface::set_transparent(bool transparent) {
    _is_transparent = transparent;
}

String VoxelSurface::get_voxel_name() const {
    return _name;
}
void VoxelSurface::set_voxel_name(String name) {
    _name = name;
}

Ref<VoxelmanLibrary> VoxelSurface::get_library() const {
    return Ref<VoxelmanLibrary>(_library);
}

void VoxelSurface::set_library(Ref<VoxelmanLibrary> library) {
    _library = library;
}

VoxelSurface::VoxelSurface() {
    _id = -1;
    _atlas_texture_id = 0;
    _mesh_id = 0;
    _prefab_id = 0;
    _light = false;
    _light_strength = 0;
}

VoxelSurface::VoxelSurface(int id) {
    _id = id;
    _atlas_texture_id = 0;

    _mesh_id = 0;
    _prefab_id = 0;
    _light = false;
    _light_strength = 0;
}

VoxelSurface::~VoxelSurface() {
}

void VoxelSurface::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_id"), &VoxelSurface::get_id);
    ClassDB::bind_method(D_METHOD("set_id", "value"), &VoxelSurface::set_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

    ClassDB::bind_method(D_METHOD("get_voxel_color"), &VoxelSurface::get_voxel_color);
    ClassDB::bind_method(D_METHOD("set_voxel_color", "value"), &VoxelSurface::set_voxel_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_voxel_color", "get_voxel_color");

    ClassDB::bind_method(D_METHOD("get_atlas_texture_id"), &VoxelSurface::get_atlas_texture_id);
    ClassDB::bind_method(D_METHOD("set_atlas_texture_id", "value"), &VoxelSurface::set_atlas_texture_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_texture_id"), "set_atlas_texture_id", "get_atlas_texture_id");

    ClassDB::bind_method(D_METHOD("get_mesh_id"), &VoxelSurface::get_mesh_id);
    ClassDB::bind_method(D_METHOD("set_mesh_id", "value"), &VoxelSurface::set_mesh_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "mesh_id"), "set_mesh_id", "get_mesh_id");

    ClassDB::bind_method(D_METHOD("get_mesh_offset"), &VoxelSurface::get_mesh_offset);
    ClassDB::bind_method(D_METHOD("set_mesh_offset", "value"), &VoxelSurface::set_mesh_offset);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "mesh_offset"), "set_mesh_offset", "get_mesh_offset");

    ClassDB::bind_method(D_METHOD("get_prefab_id"), &VoxelSurface::get_prefab_id);
    ClassDB::bind_method(D_METHOD("set_prefab_id", "value"), &VoxelSurface::set_prefab_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "prefab_id"), "set_prefab_id", "get_prefab_id");

    ClassDB::bind_method(D_METHOD("get_prefab_offset"), &VoxelSurface::get_prefab_offset);
    ClassDB::bind_method(D_METHOD("set_prefab_offset", "value"), &VoxelSurface::set_prefab_offset);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "prefab_offset"), "set_prefab_offset", "get_prefab_offset");

    ClassDB::bind_method(D_METHOD("get_prefab_rotation"), &VoxelSurface::get_prefab_rotation);
    ClassDB::bind_method(D_METHOD("set_prefab_rotation", "value"), &VoxelSurface::set_prefab_rotation);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "prefab_rotation"), "set_prefab_rotation", "get_prefab_rotation");

    ClassDB::bind_method(D_METHOD("get_light"), &VoxelSurface::get_light);
    ClassDB::bind_method(D_METHOD("set_light", "value"), &VoxelSurface::set_light);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_light"), "set_prefab_id", "get_light");

    ClassDB::bind_method(D_METHOD("get_light_strength"), &VoxelSurface::get_light_strength);
    ClassDB::bind_method(D_METHOD("set_light_strength", "value"), &VoxelSurface::set_light_strength);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "light_strength"), "set_light_strength", "get_light_strength");

    ClassDB::bind_method(D_METHOD("get_light_color"), &VoxelSurface::get_light_color);
    ClassDB::bind_method(D_METHOD("set_light_color", "value"), &VoxelSurface::set_light_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");

    ClassDB::bind_method(D_METHOD("get_light_offset"), &VoxelSurface::get_light_offset);
    ClassDB::bind_method(D_METHOD("set_light_offset", "value"), &VoxelSurface::set_light_offset);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "light_offset"), "set_light_offset", "get_light_offset");

    ClassDB::bind_method(D_METHOD("set_transparent", "transparent"), &VoxelSurface::set_transparent, DEFVAL(true));
    ClassDB::bind_method(D_METHOD("is_transparent"), &VoxelSurface::is_transparent);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "transparent"), "set_transparent", "is_transparent");

    ClassDB::bind_method(D_METHOD("set_voxel_name", "name"), &VoxelSurface::set_voxel_name);
    ClassDB::bind_method(D_METHOD("get_voxel_name"), &VoxelSurface::get_voxel_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "voxel_name"), "set_name", "get_name");
}
