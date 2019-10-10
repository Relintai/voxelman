#include "voxelman_prop.h"

bool VoxelmanProp::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void VoxelmanProp::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 VoxelmanProp::get_snap_axis() {
	return _snap_axis;
}
void VoxelmanProp::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

Ref<VoxelmanPropEntry> VoxelmanProp::get_prop(const int index) const {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<VoxelmanPropEntry>());

	return _props.get(index);
}
void VoxelmanProp::set_prop(const int index, const Ref<VoxelmanPropEntry> prop) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.set(index, prop);
}
void VoxelmanProp::add_prop(const Ref<VoxelmanPropEntry> prop) {
	_props.push_back(prop);
}
void VoxelmanProp::remove_prop(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}

int VoxelmanProp::get_prop_count() const {
	return _props.size();
}

Vector<Variant> VoxelmanProp::get_props() {
	Vector<Variant> r;
	for (int i = 0; i < _props.size(); i++) {
		r.push_back(_props[i].get_ref_ptr());
	}
	return r;
}
void VoxelmanProp::set_props(const Vector<Variant> &props) {
	_props.clear();
	for (int i = 0; i < props.size(); i++) {
		Ref<VoxelmanPropEntry> prop = Ref<VoxelmanPropEntry>(props[i]);

		_props.push_back(prop);
	}
}

VoxelmanProp::VoxelmanProp() {
	_snap_to_mesh = true;
	_snap_axis = Vector3(0, 1, 0);
}
VoxelmanProp::~VoxelmanProp() {
	_props.clear();
}

void VoxelmanProp::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &VoxelmanProp::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &VoxelmanProp::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &VoxelmanProp::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &VoxelmanProp::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

	ClassDB::bind_method(D_METHOD("get_prop", "index"), &VoxelmanProp::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "index", "spell"), &VoxelmanProp::set_prop);
	ClassDB::bind_method(D_METHOD("add_prop", "prop"), &VoxelmanProp::add_prop);
	ClassDB::bind_method(D_METHOD("remove_prop", "index"), &VoxelmanProp::remove_prop);

	ClassDB::bind_method(D_METHOD("get_prop_count"), &VoxelmanProp::get_prop_count);

	ClassDB::bind_method(D_METHOD("get_props"), &VoxelmanProp::get_props);
	ClassDB::bind_method(D_METHOD("set_props", "props"), &VoxelmanProp::set_props);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "props", PROPERTY_HINT_NONE, "17/17:VoxelmanPropEntry", PROPERTY_USAGE_DEFAULT, "VoxelmanPropEntry"), "set_props", "get_props");
}
