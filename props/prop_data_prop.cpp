#include "prop_data_prop.h"

Ref<PropData> PropDataProp::get_prop() const {
	return _prop;
}
void PropDataProp::set_prop(const Ref<PropData> value) {
	_prop = value;
}

bool PropDataProp::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void PropDataProp::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 PropDataProp::get_snap_axis() {
	return _snap_axis;
}
void PropDataProp::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

PropDataProp::PropDataProp() {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, 1, 0);
}
PropDataProp::~PropDataProp() {
	if (_prop.is_valid())
		_prop.unref();
}

void PropDataProp::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop"), &PropDataProp::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "value"), &PropDataProp::set_prop);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "PropData"), "set_prop", "get_prop");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropDataProp::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropDataProp::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropDataProp::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropDataProp::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");
}
