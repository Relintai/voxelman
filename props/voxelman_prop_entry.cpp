#include "voxelman_prop_entry.h"

#include "voxelman_prop.h"

Ref<MeshDataResource> VoxelmanPropEntry::get_mesh() const {
	return _mesh;
}
void VoxelmanPropEntry::set_mesh(const Ref<MeshDataResource> mesh) {
	_mesh = mesh;
}

int VoxelmanPropEntry::get_has_light() const {
	return _has_light;
}
void VoxelmanPropEntry::set_has_light(const int value){
	_has_light = value;
}

Color VoxelmanPropEntry::get_light_color() const {
	return _light_color;
}
void VoxelmanPropEntry::set_light_color(const Color value) {
	_light_color = value;
}

int VoxelmanPropEntry::get_light_size() const {
	return _light_size;
}
void VoxelmanPropEntry::set_light_size(const int value) {
	_has_light = value;
}

Ref<PackedScene> VoxelmanPropEntry::get_scene() const {
	return _scene;
}
void VoxelmanPropEntry::set_scene(const Ref<PackedScene> value) {
	_scene = value;
}

Vector3 VoxelmanPropEntry::get_position() const {
	return _position;
}
void VoxelmanPropEntry::set_position(const Vector3 value) {
	_position = value;
}

Vector3 VoxelmanPropEntry::get_rotation() const {
	return _rotation;
}
void VoxelmanPropEntry::set_rotation(const Vector3 value) {
	_rotation = value;
}

Vector3 VoxelmanPropEntry::get_scale() const {
	return _scale;
}
void VoxelmanPropEntry::set_scale(const Vector3 value) {
	_scale = value;
}

Ref<VoxelmanProp> VoxelmanPropEntry::get_prop() const {
	return _prop;
}
void VoxelmanPropEntry::set_prop(const Ref<VoxelmanProp> prop) {
	_prop = prop;
}

VoxelmanPropEntry::VoxelmanPropEntry() {
	_has_light = false;
	_light_size = 5;

	_scale = Vector3(1.0, 1.0, 1.0);
}
VoxelmanPropEntry::~VoxelmanPropEntry() {
	if (_mesh.is_valid())
		_mesh.unref();

	if (_scene.is_valid())
		_scene.unref();
}

void VoxelmanPropEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh"), &VoxelmanPropEntry::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "value"), &VoxelmanPropEntry::set_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh", "get_mesh");

	ClassDB::bind_method(D_METHOD("get_has_light"), &VoxelmanPropEntry::get_has_light);
	ClassDB::bind_method(D_METHOD("set_has_light", "value"), &VoxelmanPropEntry::set_has_light);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_light"), "set_has_light", "get_has_light");

	ClassDB::bind_method(D_METHOD("get_light_color"), &VoxelmanPropEntry::get_light_color);
	ClassDB::bind_method(D_METHOD("set_light_color", "value"), &VoxelmanPropEntry::set_light_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");

	ClassDB::bind_method(D_METHOD("get_light_size"), &VoxelmanPropEntry::get_light_size);
	ClassDB::bind_method(D_METHOD("set_light_size", "value"), &VoxelmanPropEntry::set_light_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_size"), "set_light_size", "get_light_size");

	ClassDB::bind_method(D_METHOD("get_scene"), &VoxelmanPropEntry::get_scene);
	ClassDB::bind_method(D_METHOD("set_scene", "value"), &VoxelmanPropEntry::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");

	ClassDB::bind_method(D_METHOD("get_prop"), &VoxelmanPropEntry::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "value"), &VoxelmanPropEntry::set_prop);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanProp"), "set_prop", "get_prop");

	ClassDB::bind_method(D_METHOD("get_position"), &VoxelmanPropEntry::get_position);
	ClassDB::bind_method(D_METHOD("set_position", "value"), &VoxelmanPropEntry::set_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "position"), "set_position", "get_position");

	ClassDB::bind_method(D_METHOD("get_rotation"), &VoxelmanPropEntry::get_rotation);
	ClassDB::bind_method(D_METHOD("set_rotation", "value"), &VoxelmanPropEntry::set_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "rotation"), "set_rotation", "get_rotation");

	ClassDB::bind_method(D_METHOD("get_scale"), &VoxelmanPropEntry::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &VoxelmanPropEntry::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "scale"), "set_scale", "get_scale");
}
