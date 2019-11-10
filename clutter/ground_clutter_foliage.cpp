#include "ground_clutter_foliage.h"

int GroundClutterFoliage::get_texture_count() const {
	return _textures.size();
}
Ref<Texture> GroundClutterFoliage::get_texture(const int index) {
	ERR_FAIL_COND_V(index, _textures.size(), Ref<Texture>());

	return _textures.get(index);
}
void GroundClutterFoliage::remove_texture(const int index) {
	ERR_FAIL_COND(index, _textures.size());

	_textures.remove(index);
}
void GroundClutterFoliage::add_texture(Ref<Texture> texture) {
	_textures.push_back(texture);
}

GroundClutterFoliage::GroundClutterFoliage() {
}

GroundClutterFoliage::~GroundClutterFoliage() {
	_textures.clear();
}

void GroundClutterFoliage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_count"), &VoxelMesher::get_texture_count);
	ClassDB::bind_method(D_METHOD("get_texture", "index"), &VoxelMesher::get_texture);
	ClassDB::bind_method(D_METHOD("remove_texture", "index"), &VoxelMesher::remove_texture);
	ClassDB::bind_method(D_METHOD("add_texture", "texture"), &VoxelMesher::add_texture);
}
