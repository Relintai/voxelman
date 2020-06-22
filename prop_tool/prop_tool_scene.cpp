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

#include "prop_tool_scene.h"

#ifdef TOOLS_ENABLED

Ref<PropDataScene> PropToolScene::get_data() {
	if (!is_visible() || !_scene_data.is_valid())
		return Ref<PropDataScene>();

	if (!_prop_scene.is_valid())
		_prop_scene.instance();

	_prop_scene->set_scene(_scene_data);
	_prop_scene->set_snap_to_mesh(_snap_to_mesh);
	_prop_scene->set_snap_axis(_snap_axis);

	return _prop_scene;
}
void PropToolScene::set_data(const Ref<PropDataScene> &data) {
	_prop_scene = data;

	if (!_prop_scene.is_valid())
		return;

	_scene_data = _prop_scene->get_scene();
	_snap_to_mesh = _prop_scene->get_snap_to_mesh();
	_snap_axis = _prop_scene->get_snap_axis();
}

Ref<PackedScene> PropToolScene::get_scene_data() {
	return _scene_data;
}
void PropToolScene::set_scene_data(const Ref<PackedScene> &data) {
	_scene_data = data;
}

bool PropToolScene::get_snap_to_mesh() const {
	return _snap_to_mesh;
}
void PropToolScene::set_snap_to_mesh(const bool value) {
	_snap_to_mesh = value;
}

Vector3 PropToolScene::get_snap_axis() const {
	return _snap_axis;
}
void PropToolScene::set_snap_axis(const Vector3 &value) {
	_snap_axis = value;
}

PropToolScene::PropToolScene() {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, -1, 0);
}
PropToolScene::~PropToolScene() {
	_scene_data.unref();
	_prop_scene.unref();
}

void PropToolScene::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_data"), &PropToolScene::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "value"), &PropToolScene::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "PropDataScene"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropToolScene::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropToolScene::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropToolScene::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropToolScene::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

	ClassDB::bind_method(D_METHOD("get_scene_data"), &PropToolScene::get_scene_data);
	ClassDB::bind_method(D_METHOD("set_scene_data", "value"), &PropToolScene::set_scene_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene_data", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene_data", "get_scene_data");
}

#endif