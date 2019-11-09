#include "voxel_surface_merger.h"

#include "voxelman_library_merger.h"

int VoxelSurfaceMerger::get_atlas_x(const VoxelSurfaceSides side) const {
	int indx = (side * 2);

	return _atlas_positions[indx];
}
void VoxelSurfaceMerger::set_atlas_x(const VoxelSurfaceSides side, int value) {
	int indx = (side * 2);

	_atlas_positions[indx] = value;
}

int VoxelSurfaceMerger::get_atlas_y(const VoxelSurfaceSides side) const {
	int indx = (side * 2) + 1;

	return _atlas_positions[indx];
}
void VoxelSurfaceMerger::set_atlas_y(const VoxelSurfaceSides side, int value) {
	int indx = (side * 2) + 1;

	_atlas_positions[indx] = value;
}

void VoxelSurfaceMerger::refresh_rects() {
	VoxelmanLibraryMerger *lib = Object::cast_to<VoxelmanLibraryMerger>(_library);

	ERR_FAIL_COND(!ObjectDB::instance_validate(lib));

	for (int i = 0; i < VOXEL_SIDES_COUNT; ++i) {
		float culomn = 1.0 / static_cast<float>(lib->get_atlas_columns());
		float row = 1.0 / static_cast<float>(lib->get_atlas_rows());

		Rect2 r;

		r.position.x = _atlas_positions[i * 2] * culomn;
		r.position.y = _atlas_positions[i * 2 + 1] * row;

		r.size.x = culomn;
		r.size.y = row;

		_rects[i] = r;
	}
}

VoxelSurfaceMerger::VoxelSurfaceMerger() {
	for (int i = 0; i < VOXEL_SIDES_ARRAY_SIZE; ++i) {
		_atlas_positions[i] = 0;
	}
}

VoxelSurfaceMerger::~VoxelSurfaceMerger() {
}

void VoxelSurfaceMerger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_atlas_x", "side"), &VoxelSurfaceMerger::get_atlas_x);
	ClassDB::bind_method(D_METHOD("set_atlas_x", "side", "value"), &VoxelSurfaceMerger::set_atlas_x);
	
	ClassDB::bind_method(D_METHOD("get_atlas_y", "side"), &VoxelSurfaceMerger::get_atlas_y);
	ClassDB::bind_method(D_METHOD("set_atlas_y", "side", "value"), &VoxelSurfaceMerger::set_atlas_y);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "top_atlas_x"), "set_atlas_x", "get_atlas_x", VOXEL_SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "top_atlas_y"), "set_atlas_y", "get_atlas_y", VOXEL_SIDE_TOP);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "bottom_atlas_x"), "set_atlas_x", "get_atlas_x", VOXEL_SIDE_BOTTOM);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "bottom_atlas_y"), "set_atlas_y", "get_atlas_y", VOXEL_SIDE_BOTTOM);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "side_atlas_x"), "set_atlas_x", "get_atlas_x", VOXEL_SIDE_SIDE);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "side_atlas_y"), "set_atlas_y", "get_atlas_y", VOXEL_SIDE_SIDE);
}
