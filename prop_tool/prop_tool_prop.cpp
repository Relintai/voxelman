/*
Copyright (c) 2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "prop_tool_prop.h"

#ifdef TOOLS_ENABLED

Ref<PropDataProp> PropToolProp::get_data() {
	if (!is_visible() || !_prop_data.is_valid())
		return Ref<PropDataProp>();

	if (!_prop_data.is_valid())
		_prop_data.instance();

	_prop_prop->set_prop(_prop_data);
	_prop_prop->set_snap_to_mesh(_snap_to_mesh);
	_prop_prop->set_snap_axis(_snap_axis);

	return _prop_prop;
}
void PropToolProp::set_data(const Ref<PropDataProp> &data) {
	_prop_prop = data;

	if (!_prop_prop.is_valid())
		return;

	_prop_data = _prop_prop->get_prop();
	_snap_to_mesh = _prop_prop->get_snap_to_mesh();
	_snap_axis = _prop_prop->get_snap_axis();
}

Ref<PropData> PropToolProp::get_prop_data() {
	return _prop_data;
}
void PropToolProp::set_prop_data(const Ref<PropData> &data) {
	_prop_data = data;
}

bool PropToolProp::get_snap_to_mesh() const {
	return _snap_to_mesh;
}
void PropToolProp::set_snap_to_mesh(const bool value) {
	_snap_to_mesh = value;
}

Vector3 PropToolProp::get_snap_axis() const {
	return _snap_axis;
}
void PropToolProp::set_snap_axis(const Vector3 &value) {
	_snap_axis = value;
}

PropToolProp::PropToolProp() {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, -1, 0);
}
PropToolProp::~PropToolProp() {
	_prop_data.unref();
	_prop_prop.unref();
}

void PropToolProp::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_data"), &PropToolProp::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "value"), &PropToolProp::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "PropDataProp"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropToolProp::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropToolProp::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropToolProp::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropToolProp::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

	ClassDB::bind_method(D_METHOD("get_prop_data"), &PropToolProp::get_prop_data);
	ClassDB::bind_method(D_METHOD("set_prop_data", "value"), &PropToolProp::set_prop_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_data", PROPERTY_HINT_RESOURCE_TYPE, "PropData"), "set_prop_data", "get_prop_data");
}

#endif