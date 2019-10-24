#include "voxelman_prop_prop.h"

Ref<VoxelmanProp> VoxelmanPropProp::get_prop() const {
	return _prop;
}
void VoxelmanPropProp::set_prop(const Ref<VoxelmanProp> value) {
	_prop = value;
}

bool VoxelmanPropProp::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void VoxelmanPropProp::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 VoxelmanPropProp::get_snap_axis() {
	return _snap_axis;
}
void VoxelmanPropProp::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

VoxelmanPropProp::VoxelmanPropProp() {
	_snap_to_mesh = true;
	_snap_axis = Vector3(0, 1, 0);
}
VoxelmanPropProp::~VoxelmanPropProp() {
	if (_prop.is_valid())
		_prop.unref();
}

void VoxelmanPropProp::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop"), &VoxelmanPropProp::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "value"), &VoxelmanPropProp::set_prop);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanProp"), "set_prop", "get_prop");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &VoxelmanPropProp::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &VoxelmanPropProp::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &VoxelmanPropProp::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &VoxelmanPropProp::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");
}
