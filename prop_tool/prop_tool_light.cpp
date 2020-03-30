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

#include "prop_tool_light.h"

#ifdef TOOLS_ENABLED

Ref<PropDataLight> PropToolLight::get_data() {

	if (!is_visible())
		return Ref<PropDataLight>();

	if (!_prop_light.is_valid())
		_prop_light.instance();

	_prop_light->set_light_color(get_color());
	_prop_light->set_light_size(get_param(Light::PARAM_RANGE));

	return _prop_light;
}
void PropToolLight::set_data(const Ref<PropDataLight> &data) {
	_prop_light = data;

	if (!_prop_light.is_valid())
		return;

	set_color(_prop_light->get_light_color());
	set_param(Light::PARAM_RANGE, _prop_light->get_light_size());
	set_param(Light::PARAM_ENERGY, _prop_light->get_light_size());
}

bool PropToolLight::get_snap_to_mesh() const {
	return _snap_to_mesh;
}
void PropToolLight::set_snap_to_mesh(const bool value) {
	_snap_to_mesh = value;
}

PropToolLight::PropToolLight() {
}
PropToolLight::~PropToolLight() {
	_prop_light.unref();
}

void PropToolLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_data"), &PropToolLight::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "value"), &PropToolLight::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "PropDataLight"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropToolLight::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropToolLight::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");
}

#endif