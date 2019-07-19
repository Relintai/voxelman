#include "voxelman_prop_data.h"

#include "voxelman_prop.h"

Ref<MeshDataResource> VoxelmanPropData::get_mesh() const {
	return _mesh;
}
void VoxelmanPropData::set_mesh(const Ref<MeshDataResource> mesh) {
	_mesh = mesh;
}

int VoxelmanPropData::get_has_light() const {
	return _has_light;
}
void VoxelmanPropData::set_has_light(const int value){
	_has_light = value;
}

Color VoxelmanPropData::get_light_color() const {
	return _light_color;
}
void VoxelmanPropData::set_light_color(const Color value) {
	_light_color = value;
}

int VoxelmanPropData::get_light_size() const {
	return _light_size;
}
void VoxelmanPropData::set_light_size(const int value) {
	_has_light = value;
}

Ref<PackedScene> VoxelmanPropData::get_scene() const {
	return _scene;
}
void VoxelmanPropData::set_scene(const Ref<PackedScene> value) {
	_scene = value;
}

Vector3 VoxelmanPropData::get_position() const {
	return _position;
}
void VoxelmanPropData::set_position(const Vector3 value) {
	_position = value;
}

Vector3 VoxelmanPropData::get_rotation() const {
	return _rotation;
}
void VoxelmanPropData::set_rotation(const Vector3 value) {
	_rotation = value;
}

Vector3 VoxelmanPropData::get_scale() const {
	return _scale;
}
void VoxelmanPropData::set_scale(const Vector3 value) {
	_scale = value;
}

Ref<VoxelmanProp> VoxelmanPropData::get_prop() const {
	return _prop;
}
void VoxelmanPropData::set_prop(const Ref<VoxelmanProp> prop) {
	_prop = prop;
}

VoxelmanPropData::VoxelmanPropData() {
	_has_light = false;
	_light_size = 5;

	_scale = Vector3(1.0, 1.0, 1.0);
}
VoxelmanPropData::~VoxelmanPropData() {
	if (_mesh.is_valid())
		_mesh.unref();

	if (_scene.is_valid())
		_scene.unref();
}

void VoxelmanPropData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh"), &VoxelmanPropData::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "value"), &VoxelmanPropData::set_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh", "get_mesh");

	ClassDB::bind_method(D_METHOD("get_has_light"), &VoxelmanPropData::get_has_light);
	ClassDB::bind_method(D_METHOD("set_has_light", "value"), &VoxelmanPropData::set_has_light);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_light"), "set_has_light", "get_has_light");

	ClassDB::bind_method(D_METHOD("get_light_color"), &VoxelmanPropData::get_light_color);
	ClassDB::bind_method(D_METHOD("set_light_color", "value"), &VoxelmanPropData::set_light_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");

	ClassDB::bind_method(D_METHOD("get_light_size"), &VoxelmanPropData::get_light_size);
	ClassDB::bind_method(D_METHOD("set_light_size", "value"), &VoxelmanPropData::set_light_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_size"), "set_light_size", "get_light_size");

	ClassDB::bind_method(D_METHOD("get_scene"), &VoxelmanPropData::get_scene);
	ClassDB::bind_method(D_METHOD("set_scene", "value"), &VoxelmanPropData::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");

	ClassDB::bind_method(D_METHOD("get_prop"), &VoxelmanPropData::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "value"), &VoxelmanPropData::set_prop);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanProp"), "set_prop", "get_prop");

	ClassDB::bind_method(D_METHOD("get_position"), &VoxelmanPropData::get_position);
	ClassDB::bind_method(D_METHOD("set_position", "value"), &VoxelmanPropData::set_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "position"), "set_position", "get_position");

	ClassDB::bind_method(D_METHOD("get_rotation"), &VoxelmanPropData::get_rotation);
	ClassDB::bind_method(D_METHOD("set_rotation", "value"), &VoxelmanPropData::set_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "rotation"), "set_rotation", "get_rotation");

	ClassDB::bind_method(D_METHOD("get_scale"), &VoxelmanPropData::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &VoxelmanPropData::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "scale"), "set_scale", "get_scale");
}
