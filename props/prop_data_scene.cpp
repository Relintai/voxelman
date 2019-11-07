#include "prop_data_scene.h"

Ref<PackedScene> PropDataScene::get_scene() const {
	return _scene;
}
void PropDataScene::set_scene(const Ref<PackedScene> value) {
	_scene = value;
}

bool PropDataScene::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void PropDataScene::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 PropDataScene::get_snap_axis() {
	return _snap_axis;
}
void PropDataScene::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

PropDataScene::PropDataScene() {
	_snap_to_mesh = true;
	_snap_axis = Vector3(0, 1, 0);
}
PropDataScene::~PropDataScene() {
	if (_scene.is_valid())
		_scene.unref();
}

void PropDataScene::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scene"), &PropDataScene::get_scene);
	ClassDB::bind_method(D_METHOD("set_scene", "value"), &PropDataScene::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropDataScene::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropDataScene::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropDataScene::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropDataScene::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");
}
