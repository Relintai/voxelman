#include "voxelman_library.h"

VoxelmanLibrary::VoxelmanLibrary() {
}

VoxelmanLibrary::~VoxelmanLibrary() {
	_material.unref();
	_prop_material.unref();
}

//Surfaces
Ref<VoxelSurface> VoxelmanLibrary::get_voxel_surface(int index) const {
	return Ref<VoxelSurface>();
}
void VoxelmanLibrary::set_voxel_surface(int index, Ref<VoxelSurface> value) {
}
void VoxelmanLibrary::remove_surface(int index) {
}
int VoxelmanLibrary::get_num_surfaces() {
	return 0;
}
void VoxelmanLibrary::clear_surfaces() {
}

//Liquids
Ref<VoxelSurface> VoxelmanLibrary::get_liquid_voxel_surface(int index) const {
	return Ref<VoxelSurface>();
}
void VoxelmanLibrary::set_liquid_voxel_surface(int index, Ref<VoxelSurface> value) {
}
void VoxelmanLibrary::remove_liquid_surface(int index) {
}
int VoxelmanLibrary::get_liquid_num_surfaces() {
	return 0;
}
void VoxelmanLibrary::clear_liquid_surfaces() {
}

//Rects
void VoxelmanLibrary::refresh_rects() {
}

void VoxelmanLibrary::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_material"), &VoxelmanLibrary::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &VoxelmanLibrary::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_prop_material"), &VoxelmanLibrary::get_prop_material);
	ClassDB::bind_method(D_METHOD("set_prop_material", "value"), &VoxelmanLibrary::set_prop_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_prop_material", "get_prop_material");

	ClassDB::bind_method(D_METHOD("get_voxel_surface", "index"), &VoxelmanLibrary::get_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_voxel_surface", "index", "surface"), &VoxelmanLibrary::set_voxel_surface);
	ClassDB::bind_method(D_METHOD("remove_surface", "index"), &VoxelmanLibrary::remove_surface);
	ClassDB::bind_method(D_METHOD("get_num_surfaces"), &VoxelmanLibrary::get_num_surfaces);
	ClassDB::bind_method(D_METHOD("clear_surfaces"), &VoxelmanLibrary::clear_surfaces);

	ClassDB::bind_method(D_METHOD("get_liquid_voxel_surface", "index"), &VoxelmanLibrary::get_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_liquid_voxel_surface", "index", "surface"), &VoxelmanLibrary::set_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("remove_liquid_surface", "index"), &VoxelmanLibrary::remove_liquid_surface);
	ClassDB::bind_method(D_METHOD("get_liquid_num_surfaces"), &VoxelmanLibrary::get_liquid_num_surfaces);
	ClassDB::bind_method(D_METHOD("clear_liquid_surfaces"), &VoxelmanLibrary::clear_liquid_surfaces);

	ClassDB::bind_method(D_METHOD("refresh_rects"), &VoxelmanLibrary::refresh_rects);
}
