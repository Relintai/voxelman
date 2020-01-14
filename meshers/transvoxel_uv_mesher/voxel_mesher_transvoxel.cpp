#include "voxel_mesher_transvoxel.h"

Vector3 VoxelMesherTransvoxel::corner_id_to_vertex(int corner_id) const {
	ERR_FAIL_COND_V(corner_id < 0 || corner_id > 8, Vector3());

	return transvoxel_vertices[corner_id];
}

int VoxelMesherTransvoxel::get_regular_cell_class(int index) const {
	return static_cast<int>(regularCellClass[index]);
}

Ref<TransvoxelCellData> VoxelMesherTransvoxel::get_regular_cell_data(int index) const {
	return _regular_cell_datas[index];
}

int VoxelMesherTransvoxel::get_regular_vertex_data(int index1, int index2) const {
	//return static_cast<int>(regularVertexData[index1][index2]);
	return regularVertexData[index1][index2];
}

//void VoxelMesherTransvoxel::set_regular_vertex_data(int index1, int index2, int value) {
//   ERR_FAIL_INDEX(index1, 256);
//    ERR_FAIL_INDEX(index2, 13);

//    regularVertexData[index1][index2] = value;
//}

int VoxelMesherTransvoxel::get_regular_vertex_data_first_vertex(int index1, int index2) const {
	int vert1 = regularVertexData[index1][index2] & 0x000F;

	return vert1;
}

int VoxelMesherTransvoxel::get_regular_vertex_data_second_vertex(int index1, int index2) const {
	int vert2 = (regularVertexData[index1][index2] & 0x00F0) >> 4;

	return vert2;
}

Vector3 VoxelMesherTransvoxel::get_regular_vertex_first_position(int index1, int index2) const {
	int vert = regularVertexData[index1][index2] & 0x000F;

	return transvoxel_vertices[vert];
}

Vector3 VoxelMesherTransvoxel::get_regular_vertex_second_position(int index1, int index2) const {
	int vert = (regularVertexData[index1][index2] & 0x00F0) >> 4;

	return transvoxel_vertices[vert];
}

Vector3 VoxelMesherTransvoxel::get_regular_vertex_direction(int index1, int index2) const {
	int vert1 = regularVertexData[index1][index2] & 0x000F;
	int vert2 = (regularVertexData[index1][index2] & 0x00F0) >> 4;

	return transvoxel_vertices[vert2] - transvoxel_vertices[vert1];
}

int VoxelMesherTransvoxel::get_transition_cell_class(int index) const {
	return static_cast<int>(transitionCellClass[index]);
}

Ref<TransvoxelCellData> VoxelMesherTransvoxel::get_transition_cell_data(int index) const {
	return _transition_cell_data[index];
}

int VoxelMesherTransvoxel::get_transition_corner_data(int index) const {
	return static_cast<int>(transitionCornerData[index]);
}

int VoxelMesherTransvoxel::get_transition_vertex_data(int index1, int index2) const {
	return static_cast<int>(transitionVertexData[index1][index2]);
}

int VoxelMesherTransvoxel::get_transition_vertex_data_first_vertex(int index1, int index2) const {
	unsigned short val = transitionVertexData[index1][index2];

	unsigned short vert = val & 0x000F;

	return static_cast<int>(vert);
}

int VoxelMesherTransvoxel::get_transition_vertex_data_second_vertex(int index1, int index2) const {
	unsigned short val = transitionVertexData[index1][index2];

	unsigned short vert = (val & 0x00F0) >> 4;

	return static_cast<int>(vert);
}

Vector3 VoxelMesherTransvoxel::get_transition_vertex_first_position(int index1, int index2) const {
	int vert = transitionVertexData[index1][index2] & 0x000F;

	return transvoxel_vertices[vert];
}

Vector3 VoxelMesherTransvoxel::get_transition_vertex_second_position(int index1, int index2) const {
	int vert = (transitionVertexData[index1][index2] & 0x00F0) >> 4;

	return transvoxel_vertices[vert];
}

Vector3 VoxelMesherTransvoxel::get_transition_vertex_direction(int index1, int index2) const {
	int vert1 = transitionVertexData[index1][index2] & 0x000F;
	int vert2 = (transitionVertexData[index1][index2] & 0x00F0) >> 4;

	return transvoxel_vertices[vert2] - transvoxel_vertices[vert1];
}

VoxelMesherTransvoxel::VoxelMesherTransvoxel() {
	for (int i = 0; i < 16; ++i) {
		_regular_cell_datas[i] = Ref<TransvoxelCellData>(memnew(TransvoxelCellData(regularCellData[i])));
	}

	for (int i = 0; i < 56; ++i) {
		_transition_cell_data[i] = Ref<TransvoxelCellData>(memnew(TransvoxelCellData(transitionCellData[i])));
	}
}

VoxelMesherTransvoxel::~VoxelMesherTransvoxel() {
}

void VoxelMesherTransvoxel::_bind_methods() {
	ClassDB::bind_method(D_METHOD("corner_id_to_vertex", "index1"), &VoxelMesherTransvoxel::corner_id_to_vertex);

	ClassDB::bind_method(D_METHOD("get_regular_cell_class", "index"), &VoxelMesherTransvoxel::get_regular_cell_class);
	ClassDB::bind_method(D_METHOD("get_regular_cell_data", "index"), &VoxelMesherTransvoxel::get_regular_cell_data);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_data", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_data);
	//  ClassDB::bind_method(D_METHOD("set_regular_vertex_data", "index1", "index2", "value"), &VoxelMesherTransvoxel::set_regular_vertex_data);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_data_first_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_data_first_vertex);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_data_second_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_data_second_vertex);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_first_position", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_first_position);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_second_position", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_second_position);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_direction", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_direction);

	ClassDB::bind_method(D_METHOD("get_transition_cell_class", "index"), &VoxelMesherTransvoxel::get_transition_cell_class);
	ClassDB::bind_method(D_METHOD("get_transition_cell_data", "index"), &VoxelMesherTransvoxel::get_transition_cell_data);
	ClassDB::bind_method(D_METHOD("get_transition_corner_data", "index"), &VoxelMesherTransvoxel::get_transition_corner_data);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_data", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_data);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_data_first_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_data_first_vertex);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_data_second_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_data_second_vertex);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_first_position", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_first_position);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_second_position", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_second_position);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_direction", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_direction);

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
