#include "voxelman_library.h"

VoxelmanLibrary::VoxelmanLibrary() {

	_atlas_rows = 8;
	_atlas_columns = 8;

	_voxel_types_count = 0;
	_voxel_types_page = 0;

	_is_textured = true;

	for (int i = 0; i < MAX_VOXEL_TYPES; ++i) {
		if (_voxel_types[i] != NULL) {
			print_error("set_this");
			_voxel_types[i]->set_library(Ref<VoxelmanLibrary>(this));
		}
	}

}

VoxelmanLibrary::~VoxelmanLibrary() {
}

Ref<VoxelSurface> VoxelmanLibrary::get_voxel_surface(int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_types_count, Ref<VoxelSurface>(NULL));

	return _voxel_types[index];
}

void VoxelmanLibrary::set_voxel_surface(int index, Ref<VoxelSurface> value) {
	ERR_FAIL_COND(index < 0 || index > _voxel_types_count);
	ERR_FAIL_COND(value == NULL);

	if (value != NULL) {
	
		value->set_library(Ref<VoxelmanLibrary>(this));

		_voxel_types[index] = Ref<VoxelSurface>(value);
	}
}

int VoxelmanLibrary::get_voxel_types_count() {
	return _voxel_types_count;
}

void VoxelmanLibrary::set_voxel_types_count(int value) {
	_voxel_types_count = value;
}

int VoxelmanLibrary::get_voxel_types_page() {
	return _voxel_types_page;
}

void VoxelmanLibrary::set_voxel_types_page(int value) {
	if (value < 0 || value > (int)(_voxel_types_count / ITEMS_PER_PAGE)) {
		return;
	}

	_voxel_types_page = value;
}

int VoxelmanLibrary::get_voxel_count() const {
	int count = 0;
	for (int i = 0; i < MAX_VOXEL_TYPES; ++i) {
		if (_voxel_types[i].is_valid())
			++count;
	}
	return count;
}

void VoxelmanLibrary::load_default() {
}

void VoxelmanLibrary::set_atlas_columns(int s) {
	ERR_FAIL_COND(s <= 0);
	_atlas_columns = s;
}

void VoxelmanLibrary::set_atlas_rows(int s) {
	ERR_FAIL_COND(s <= 0);
	_atlas_rows = s;
}

bool VoxelmanLibrary::get_is_textured() const {
	return _is_textured;
}

void VoxelmanLibrary::set_is_textured(bool value) {
	_is_textured = value;
}

Ref<VoxelSurface> VoxelmanLibrary::create_voxel(int id, String name) {
	ERR_FAIL_COND_V(id < 0 || id >= MAX_VOXEL_TYPES, Ref<VoxelSurface>());
	Ref<VoxelSurface> voxel(memnew(VoxelSurface));
	voxel->set_library(Ref<VoxelmanLibrary>(this));
	voxel->set_id(id);
	voxel->set_voxel_name(name);
	_voxel_types[id] = voxel;
	return voxel;
}

Ref<VoxelSurface> VoxelmanLibrary::_get_voxel_bind(int id) {
	ERR_FAIL_COND_V(id < 0 || id >= MAX_VOXEL_TYPES, Ref<VoxelSurface>());
	return _voxel_types[id];
}

void VoxelmanLibrary::_validate_property(PropertyInfo &property) const {

	String prop = property.name;
	if (prop.begins_with("Voxel_")) {
		int frame = prop.get_slicec('/', 0).get_slicec('_', 1).to_int();
		if (frame >= _voxel_types_count || frame < ITEMS_PER_PAGE * _voxel_types_page || frame > ITEMS_PER_PAGE * (_voxel_types_page + 1)) {
			property.usage = 0;
		}
	}
}

void VoxelmanLibrary::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create_voxel", "id", "name"), &VoxelmanLibrary::create_voxel);
	ClassDB::bind_method(D_METHOD("get_voxel", "id"), &VoxelmanLibrary::_get_voxel_bind);

	ClassDB::bind_method(D_METHOD("get_atlas_columns"), &VoxelmanLibrary::get_atlas_columns);
	ClassDB::bind_method(D_METHOD("set_atlas_columns", "value"), &VoxelmanLibrary::set_atlas_columns);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_columns"), "set_atlas_columns", "get_atlas_columns");
	
	ClassDB::bind_method(D_METHOD("get_atlas_rows"), &VoxelmanLibrary::get_atlas_rows);
	ClassDB::bind_method(D_METHOD("set_atlas_rows", "value"), &VoxelmanLibrary::set_atlas_rows);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_rows"), "set_atlas_rows", "get_atlas_rows");

	ClassDB::bind_method(D_METHOD("get_is_textured"), &VoxelmanLibrary::get_is_textured);
	ClassDB::bind_method(D_METHOD("set_is_textured", "value"), &VoxelmanLibrary::set_is_textured);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_textured"), "set_is_textured", "get_is_textured");

	ClassDB::bind_method(D_METHOD("get_material"), &VoxelmanLibrary::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &VoxelmanLibrary::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_voxel_types_count"), &VoxelmanLibrary::get_voxel_types_count);
	ClassDB::bind_method(D_METHOD("set_voxel_types_count", "value"), &VoxelmanLibrary::set_voxel_types_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "voxel_types_count", PROPERTY_HINT_RANGE, "0," + itos(MAX_VOXEL_TYPES), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_voxel_types_count", "get_voxel_types_count");

	ClassDB::bind_method(D_METHOD("get_voxel_types_page"), &VoxelmanLibrary::get_voxel_types_page);
	ClassDB::bind_method(D_METHOD("set_voxel_types_page", "value"), &VoxelmanLibrary::set_voxel_types_page);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "voxel_types_page", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_voxel_types_page", "get_voxel_types_page");

	ClassDB::bind_method(D_METHOD("get_voxel_surface", "index"), &VoxelmanLibrary::get_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_voxel_surface", "index", "surface"), &VoxelmanLibrary::set_voxel_surface);

	for (int i = 0; i < MAX_VOXEL_TYPES; ++i) {
		ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "Voxel_" + itos(i), PROPERTY_HINT_RESOURCE_TYPE, "VoxelSurface", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_INTERNAL), "set_voxel_surface", "get_voxel_surface", i);
	}

	BIND_CONSTANT(MAX_VOXEL_TYPES);
}
