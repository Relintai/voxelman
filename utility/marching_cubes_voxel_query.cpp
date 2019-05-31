#include "marching_cubes_voxel_query.h"

const String MarchingCubesVoxelQuery::BINDING_STRING_VOXEL_ENTRY_INDICES = "Index_000, Index_100, Index_010, Index_110, Index_001, Index_101, Index_011, Index_111";
const String MarchingCubesVoxelQuery::BINDING_STRING_VOXEL_ENTRY_MASK = "Index_000, Index_100, Index_010, Index_110, Index_001, Index_101, Index_011, Index_111";

Ref<Voxel> MarchingCubesVoxelQuery::get_entry(int index) {
	return _voxels[index];
}
void MarchingCubesVoxelQuery::set_entry(int index, Ref<Voxel> voxel) {
	_voxels[index] = voxel;
}

int MarchingCubesVoxelQuery::get_size() {
	return _size;
}
void MarchingCubesVoxelQuery::set_size(int size) {
	_size = size;
}

Vector3i MarchingCubesVoxelQuery::get_position() {
	return _position;
}
void MarchingCubesVoxelQuery::set_position(Vector3i position) {
	_position = position;
}

Vector3 MarchingCubesVoxelQuery::get_position_bind() {
	return _position.to_vec3();
}
void MarchingCubesVoxelQuery::set_position_bind(Vector3 position) {
	_position = position;
}

int MarchingCubesVoxelQuery::get_case_code() {
	return _case_code;
}

void MarchingCubesVoxelQuery::update_case_code() {
	int entry_mask = 0;

	if (_voxels[VOXEL_ENTRY_INDEX_000].is_valid()) {
		entry_mask = entry_mask | VOXEL_ENTRY_MASK_000;
	}

	if (_voxels[VOXEL_ENTRY_INDEX_100].is_valid()) {
		entry_mask = entry_mask | VOXEL_ENTRY_MASK_100;
	}

	if (_voxels[VOXEL_ENTRY_INDEX_010].is_valid()) {
		entry_mask = entry_mask | VOXEL_ENTRY_MASK_010;
	}

	if (_voxels[VOXEL_ENTRY_INDEX_110].is_valid()) {
		entry_mask = entry_mask | VOXEL_ENTRY_MASK_110;
	}

	if (_voxels[VOXEL_ENTRY_INDEX_001].is_valid()) {
		entry_mask = entry_mask | VOXEL_ENTRY_MASK_001;
	}

	if (_voxels[VOXEL_ENTRY_INDEX_101].is_valid()) {
		entry_mask = entry_mask | VOXEL_ENTRY_MASK_101;
	}

	if (_voxels[VOXEL_ENTRY_INDEX_011].is_valid()) {
		entry_mask = entry_mask | VOXEL_ENTRY_MASK_011;
	}

	if (_voxels[VOXEL_ENTRY_INDEX_111].is_valid()) {
		entry_mask = entry_mask | VOXEL_ENTRY_MASK_111;
	}

	_case_code = entry_mask;
}

bool MarchingCubesVoxelQuery::is_triangulation_trivial() {
	int corner = 0;

	if (_voxels[VOXEL_ENTRY_INDEX_111].is_valid()) {
		corner = 0xFF;
	}

	if ((_case_code ^ corner) != 0) {
		// Cell has a nontrivial triangulation.
		return false;
	} 

	return true;
}

void MarchingCubesVoxelQuery::set_entries(Ref<Voxel> voxel000, Ref<Voxel> voxel100, Ref<Voxel> voxel010, Ref<Voxel> voxel110, Ref<Voxel> voxel001,
											Ref<Voxel> voxel101, Ref<Voxel> voxel011, Ref<Voxel> voxel111) {
	_voxels[VOXEL_ENTRY_INDEX_000] = voxel000;
	_voxels[VOXEL_ENTRY_INDEX_100] = voxel100;
	_voxels[VOXEL_ENTRY_INDEX_010] = voxel010;
	_voxels[VOXEL_ENTRY_INDEX_110] = voxel110;
	_voxels[VOXEL_ENTRY_INDEX_001] = voxel001;
	_voxels[VOXEL_ENTRY_INDEX_101] = voxel101;
	_voxels[VOXEL_ENTRY_INDEX_011] = voxel011;
	_voxels[VOXEL_ENTRY_INDEX_111] = voxel111;

	update_case_code();
}

MarchingCubesVoxelQuery::MarchingCubesVoxelQuery() {
	_size = 1;
}

MarchingCubesVoxelQuery::~MarchingCubesVoxelQuery() {

}

void MarchingCubesVoxelQuery::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_entry", "index"), &MarchingCubesVoxelQuery::get_entry);
	ClassDB::bind_method(D_METHOD("set_entry", "index", "voxel"), &MarchingCubesVoxelQuery::set_entry);

	ClassDB::bind_method(D_METHOD("get_size"), &MarchingCubesVoxelQuery::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &MarchingCubesVoxelQuery::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("get_position"), &MarchingCubesVoxelQuery::get_position_bind);
	ClassDB::bind_method(D_METHOD("set_position", "spositionize"), &MarchingCubesVoxelQuery::set_position_bind);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "position"), "set_position", "get_position");

	ClassDB::bind_method(D_METHOD("get_case_code"), &MarchingCubesVoxelQuery::get_case_code);
	ClassDB::bind_method(D_METHOD("update_case_code"), &MarchingCubesVoxelQuery::update_case_code);
	ClassDB::bind_method(D_METHOD("is_triangulation_trivial"), &MarchingCubesVoxelQuery::is_triangulation_trivial);

	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_000);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_100);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_010);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_110);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_001);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_101);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_011);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_111);

	BIND_ENUM_CONSTANT(VOXEL_ENTRIES_SIZE);

	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_000);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_100);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_010);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_110);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_001);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_101);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_011);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_111);

}
