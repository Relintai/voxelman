#include "voxel_mesher_transvoxel.h"

int TransvoxelRegularCellData::get_vertex_index(int index) const {
	return static_cast<int>(_cell_data.vertexIndex[index]);
}

int TransvoxelRegularCellData::get_vertex_count() const {
	return _cell_data.GetVertexCount();
}

int TransvoxelRegularCellData::get_triangle_count() const {
	return _cell_data.GetTriangleCount();
}

TransvoxelRegularCellData::TransvoxelRegularCellData() {
}

TransvoxelRegularCellData::TransvoxelRegularCellData(RegularCellData cell_data) {
	_cell_data = cell_data;
}

TransvoxelRegularCellData::~TransvoxelRegularCellData() {
}

void TransvoxelRegularCellData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_vertex_index", "index"), &TransvoxelRegularCellData::get_vertex_index);
	ClassDB::bind_method(D_METHOD("get_vertex_count"), &TransvoxelRegularCellData::get_vertex_count);
	ClassDB::bind_method(D_METHOD("get_triangle_count"), &TransvoxelRegularCellData::get_triangle_count);
}


int TransvoxelTransitionCellData::get_vertex_index(int index) const {
	return static_cast<int>(_cell_data.vertexIndex[index]);
}

int TransvoxelTransitionCellData::get_vertex_count() const {
	return _cell_data.GetVertexCount();
}

int TransvoxelTransitionCellData::get_triangle_count() const {
	return _cell_data.GetTriangleCount();
}

TransvoxelTransitionCellData::TransvoxelTransitionCellData() {
}

TransvoxelTransitionCellData::TransvoxelTransitionCellData(TransitionCellData cell_data) {
	_cell_data = cell_data;
}

TransvoxelTransitionCellData::~TransvoxelTransitionCellData() {
}

void TransvoxelTransitionCellData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_vertex_index", "index"), &TransvoxelTransitionCellData::get_vertex_index);
	ClassDB::bind_method(D_METHOD("get_vertex_count"), &TransvoxelTransitionCellData::get_vertex_count);
	ClassDB::bind_method(D_METHOD("get_triangle_count"), &TransvoxelTransitionCellData::get_triangle_count);
}




int VoxelMesherTransvoxel::get_regular_cell_class(int index) const {
	return static_cast<int>(regularCellClass[index]);
}

Ref<TransvoxelRegularCellData> VoxelMesherTransvoxel::get_regular_cell_data(int index) const {
	return _regular_cell_datas[index];
}

int VoxelMesherTransvoxel::get_regular_vertex_data(int index1, int index2) const {
	//return static_cast<int>(regularVertexData[index1][index2]);
	return regularVertexData[index1][index2];
}

int VoxelMesherTransvoxel::get_regular_vertex_data_first_vertex(int index1, int index2) const {
	int vert1 = regularVertexData[index1][index2] & 0x000F;

	return vert1;
}

int VoxelMesherTransvoxel::get_regular_vertex_data_second_vertex(int index1, int index2) const {
	int vert2 = (regularVertexData[index1][index2] & 0x00F0) >> 4;

	return vert2;
}

Vector3 VoxelMesherTransvoxel::get_regular_vertex_start_position(int index1, int index2) const {
	int vert1 = regularVertexData[index1][index2] & 0x000F;

	return transvoxel_vertices[vert1];
}

Vector3 VoxelMesherTransvoxel::get_regular_vertex_direction(int index1, int index2) const {
	int vert1 = regularVertexData[index1][index2] & 0x000F;
	int vert2 = (regularVertexData[index1][index2] & 0x00F0) >> 4;

	return transvoxel_vertices[vert2] - transvoxel_vertices[vert1];
}

int VoxelMesherTransvoxel::get_transition_cell_class(int index) const {
	return static_cast<int>(transitionCellClass[index]);
}

Ref<TransvoxelTransitionCellData> VoxelMesherTransvoxel::get_transition_cell_data(int index) const {
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

Vector3 VoxelMesherTransvoxel::get_transition_vertex_start_position(int index1, int index2) const {
	int vert1 = regularVertexData[index1][index2] & 0x000F;

	return transvoxel_vertices[vert1];
}

Vector3 VoxelMesherTransvoxel::get_transition_vertex_direction(int index1, int index2) const {
	int vert1 = regularVertexData[index1][index2] & 0x000F;
	int vert2 = (regularVertexData[index1][index2] & 0x00F0) >> 4;

	return transvoxel_vertices[vert2] - transvoxel_vertices[vert1];
}


VoxelMesherTransvoxel::VoxelMesherTransvoxel() {
	for (int i = 0; i < 16; ++i) {
		_regular_cell_datas[i] = Ref<TransvoxelRegularCellData>(memnew(TransvoxelRegularCellData(regularCellData[i])));
	}

	for (int i = 0; i < 56; ++i) {
		_transition_cell_data[i] = Ref<TransvoxelTransitionCellData>(memnew(TransvoxelTransitionCellData(transitionCellData[i])));
	}

}

VoxelMesherTransvoxel::~VoxelMesherTransvoxel() {

}

void VoxelMesherTransvoxel::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_regular_cell_class", "index"), &VoxelMesherTransvoxel::get_regular_cell_class);
	ClassDB::bind_method(D_METHOD("get_regular_cell_data", "index"), &VoxelMesherTransvoxel::get_regular_cell_data);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_data", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_data);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_data_first_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_data_first_vertex);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_data_second_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_data_second_vertex);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_start_position", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_start_position);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_direction", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_direction);

	ClassDB::bind_method(D_METHOD("get_transition_cell_class", "index"), &VoxelMesherTransvoxel::get_transition_cell_class);
	ClassDB::bind_method(D_METHOD("get_transition_cell_data", "index"), &VoxelMesherTransvoxel::get_transition_cell_data);
	ClassDB::bind_method(D_METHOD("get_transition_corner_data", "index"), &VoxelMesherTransvoxel::get_transition_corner_data);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_data", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_data);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_data_first_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_data_first_vertex);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_data_second_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_data_second_vertex);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_start_position", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_start_position);
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
