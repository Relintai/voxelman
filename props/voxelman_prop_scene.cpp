#include "voxelman_prop_scene.h"

Ref<PackedScene> VoxelmanPropScene::get_scene() const {
	return _scene;
}
void VoxelmanPropScene::set_scene(const Ref<PackedScene> value) {
	_scene = value;
}

VoxelmanPropScene::VoxelmanPropScene() {
}
VoxelmanPropScene::~VoxelmanPropScene() {
	if (_scene.is_valid())
		_scene.unref();
}

void VoxelmanPropScene::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scene"), &VoxelmanPropScene::get_scene);
	ClassDB::bind_method(D_METHOD("set_scene", "value"), &VoxelmanPropScene::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");
}
