#include "voxelman_prop_light.h"


Color VoxelmanPropLight::get_light_color() const {
	return _light_color;
}
void VoxelmanPropLight::set_light_color(const Color value) {
	_light_color = value;
}

int VoxelmanPropLight::get_light_size() const {
	return _light_size;
}
void VoxelmanPropLight::set_light_size(const int value) {
	_light_size = value;
}

VoxelmanPropLight::VoxelmanPropLight() {
	_light_size = 5;
}
VoxelmanPropLight::~VoxelmanPropLight() {
}

void VoxelmanPropLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_light_color"), &VoxelmanPropLight::get_light_color);
	ClassDB::bind_method(D_METHOD("set_light_color", "value"), &VoxelmanPropLight::set_light_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");

	ClassDB::bind_method(D_METHOD("get_light_size"), &VoxelmanPropLight::get_light_size);
	ClassDB::bind_method(D_METHOD("set_light_size", "value"), &VoxelmanPropLight::set_light_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_size"), "set_light_size", "get_light_size");
}
