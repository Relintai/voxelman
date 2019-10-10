#include "voxelman_prop_scene.h"

Ref<PackedScene> VoxelmanPropScene::get_scene() const {
	return _scene;
}
void VoxelmanPropScene::set_scene(const Ref<PackedScene> value) {
	_scene = value;
}

bool VoxelmanPropScene::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void VoxelmanPropScene::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 VoxelmanPropScene::get_snap_axis() {
	return _snap_axis;
}
void VoxelmanPropScene::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

VoxelmanPropScene::VoxelmanPropScene() {
	_snap_to_mesh = true;
	_snap_axis = Vector3(0, 1, 0);
}
VoxelmanPropScene::~VoxelmanPropScene() {
	if (_scene.is_valid())
		_scene.unref();
}

void VoxelmanPropScene::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scene"), &VoxelmanPropScene::get_scene);
	ClassDB::bind_method(D_METHOD("set_scene", "value"), &VoxelmanPropScene::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &VoxelmanPropScene::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &VoxelmanPropScene::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &VoxelmanPropScene::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &VoxelmanPropScene::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");
}
