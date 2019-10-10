#include "voxel_chunk_prop_data.h"

int VoxelChunkPropData::get_x() {
	return _x;
}
void VoxelChunkPropData::set_x(int value) {
	_x = value;
}

int VoxelChunkPropData::get_y() {
	return _y;
}
void VoxelChunkPropData::set_y(int value) {
	_y = value;
}

int VoxelChunkPropData::get_z() {
	return _z;
}
void VoxelChunkPropData::set_z(int value) {
	_z = value;
}

Vector3 VoxelChunkPropData::get_rotation() {
	return _rotation;
}
void VoxelChunkPropData::set_rotation(Vector3 value) {
	_rotation = value;
}

Vector3 VoxelChunkPropData::get_scale() {
	return _scale;
}
void VoxelChunkPropData::set_scale(Vector3 value) {
	_scale = value;
}

bool VoxelChunkPropData::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void VoxelChunkPropData::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 VoxelChunkPropData::get_snap_axis() {
	return _snap_axis;
}
void VoxelChunkPropData::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

Ref<MeshDataResource> VoxelChunkPropData::get_mesh() const {
	return _mesh;
}
void VoxelChunkPropData::set_mesh(const Ref<MeshDataResource> value) {
	_mesh = value;
}

Ref<VoxelmanPropLight> VoxelChunkPropData::get_light() const {
	return _light;
}
void VoxelChunkPropData::set_light(const Ref<VoxelmanPropLight> value) {
	_light = value;
}

Ref<VoxelmanProp> VoxelChunkPropData::get_prop() const {
	return _prop;
}
void VoxelChunkPropData::set_prop(const Ref<VoxelmanProp> value) {
	_prop = value;
}

Ref<PackedScene> VoxelChunkPropData::get_scene() const {
	return _scene;
}
void VoxelChunkPropData::set_scene(const Ref<PackedScene> value) {
	_scene = value;
}

VoxelChunkPropData::VoxelChunkPropData() {
	_x = 0;
	_y = 0;
	_z = 0;
	_scale = Vector3(1, 1, 1);
	_snap_to_mesh = false;
}
VoxelChunkPropData::~VoxelChunkPropData() {
	_mesh.unref();
	_light.unref();
	_prop.unref();
	_scene.unref();
}

void VoxelChunkPropData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_x"), &VoxelChunkPropData::get_x);
	ClassDB::bind_method(D_METHOD("set_x", "value"), &VoxelChunkPropData::set_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "x"), "set_x", "get_x");

	ClassDB::bind_method(D_METHOD("get_y"), &VoxelChunkPropData::get_y);
	ClassDB::bind_method(D_METHOD("set_y", "value"), &VoxelChunkPropData::set_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "y"), "set_y", "get_y");

	ClassDB::bind_method(D_METHOD("get_z"), &VoxelChunkPropData::get_z);
	ClassDB::bind_method(D_METHOD("set_z", "value"), &VoxelChunkPropData::set_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "z"), "set_z", "get_z");

	ClassDB::bind_method(D_METHOD("get_rotation"), &VoxelChunkPropData::get_rotation);
	ClassDB::bind_method(D_METHOD("set_rotation", "value"), &VoxelChunkPropData::set_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "rotation"), "set_rotation", "get_rotation");

	ClassDB::bind_method(D_METHOD("get_scale"), &VoxelChunkPropData::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &VoxelChunkPropData::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "scale"), "set_scale", "get_scale");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &VoxelChunkPropData::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &VoxelChunkPropData::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &VoxelChunkPropData::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &VoxelChunkPropData::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

	ClassDB::bind_method(D_METHOD("get_mesh"), &VoxelChunkPropData::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "value"), &VoxelChunkPropData::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh", "get_mesh");

	ClassDB::bind_method(D_METHOD("get_light"), &VoxelChunkPropData::get_light);
	ClassDB::bind_method(D_METHOD("set_light", "value"), &VoxelChunkPropData::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanPropLight"), "set_light", "get_light");

	ClassDB::bind_method(D_METHOD("get_prop"), &VoxelChunkPropData::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "value"), &VoxelChunkPropData::set_prop);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanProp"), "set_prop", "get_prop");

	ClassDB::bind_method(D_METHOD("get_scene"), &VoxelChunkPropData::get_scene);
	ClassDB::bind_method(D_METHOD("set_scene", "value"), &VoxelChunkPropData::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");
}
