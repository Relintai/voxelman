#include "voxel.h"

Ref<VoxelSurface> Voxel::get_surface() {
	return _surface;
}

Vector3i Voxel::get_local_position() const {
	return _local_position;
}
void Voxel::set_local_position(Vector3i value) {
	_local_position = value;
}

Vector3 Voxel::get_local_position_bind() const {
	return _local_position.to_vec3();
}
void Voxel::set_local_position_bind(Vector3 value) {
	_local_position = value;
}

char Voxel::get_fill() {
	return _fill;
}
void Voxel::set_fill(char fill) {
	_fill = fill;
}

int Voxel::get_fill_bind() {
	return (unsigned int) _fill;
}
void Voxel::set_fill_bind(int fill) {
	_fill = (char) fill;
}

float Voxel::get_light() const {
	return _light;
}
void Voxel::set_light(float value) {
	_light = value;
}

Voxel::Voxel() {
	_light = (float)1;
}

Voxel::Voxel(Vector3i position, char fill, Ref<VoxelSurface> surface) {
	_light = (float)1;
	_fill = fill;

	set_surface(surface);

	_local_position = position;
}

Voxel::~Voxel() {
}

void Voxel::set_surface(Ref<VoxelSurface> surface) {
	_surface = Ref<VoxelSurface>(surface);
}

void Voxel::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_surface"), &Voxel::get_surface);
	ClassDB::bind_method(D_METHOD("set_surface", "Mesher"), &Voxel::set_surface);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "surface", PROPERTY_HINT_RESOURCE_TYPE, "VoxelSurface"), "set_surface", "get_surface");

	ClassDB::bind_method(D_METHOD("get_fill"), &Voxel::get_fill_bind);
	ClassDB::bind_method(D_METHOD("set_fill", "value"), &Voxel::set_fill_bind);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "fill"), "set_fill", "get_fill");

	ClassDB::bind_method(D_METHOD("get_local_position"), &Voxel::get_local_position_bind);
	ClassDB::bind_method(D_METHOD("set_local_position", "value"), &Voxel::set_local_position_bind);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "local_position"), "set_fill", "get_local_position");

	ClassDB::bind_method(D_METHOD("get_light"), &Voxel::get_light);
	ClassDB::bind_method(D_METHOD("set_light", "value"), &Voxel::set_light);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "light"), "set_light", "get_light");
}
