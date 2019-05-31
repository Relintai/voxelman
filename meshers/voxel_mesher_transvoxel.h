#ifndef VOXEL_MESHER_TRANSVOXEL_H
#define VOXEL_MESHER_TRANSVOXEL_H

#include "core/reference.h"
#include "voxel_mesher.h"

#include "transvoxel_tables.h"

using namespace Transvoxel;

class TransvoxelRegularCellData : public Reference {
	GDCLASS(TransvoxelRegularCellData, Reference)

public:
	int get_vertex_index(int index) const;
	int get_vertex_count() const;
	int get_triangle_count() const;

	TransvoxelRegularCellData();
	TransvoxelRegularCellData(RegularCellData cell_data);
	~TransvoxelRegularCellData();

protected:
	static void _bind_methods();

private:
	RegularCellData _cell_data;
};

class TransvoxelTransitionCellData : public Reference {
	GDCLASS(TransvoxelTransitionCellData, Reference)

public:
	int get_vertex_index(int index) const;
	int get_vertex_count() const;
	int get_triangle_count() const;

	TransvoxelTransitionCellData();
	TransvoxelTransitionCellData(TransitionCellData cell_data);
	~TransvoxelTransitionCellData();

protected:
	static void _bind_methods();

private:
	TransitionCellData _cell_data;
};



class VoxelMesherTransvoxel : public VoxelMesher {
	GDCLASS(VoxelMesherTransvoxel, VoxelMesher)

public:
	int get_regular_cell_class(int index) const;

	Ref<TransvoxelRegularCellData> get_regular_cell_data(int index) const;
	int get_regular_vertex_data(int index10, int index2) const;
	int get_regular_vertex_data_first_vertex(int index1, int index2) const;
	int get_regular_vertex_data_second_vertex(int index1, int index2) const;
	Vector3 get_regular_vertex_start_position(int index1, int index2) const;
	Vector3 get_regular_vertex_direction(int index1, int index2) const;


	int get_transition_cell_class(int index) const;

	Ref<TransvoxelTransitionCellData> get_transition_cell_data(int index) const;
	int get_transition_corner_data(int index) const;
	int get_transition_vertex_data(int index1, int index2) const;
	int get_transition_vertex_data_first_vertex(int index1, int index2) const;
	int get_transition_vertex_data_second_vertex(int index1, int index2) const;
	Vector3 get_transition_vertex_start_position(int index1, int index2) const;
	Vector3 get_transition_vertex_direction(int index1, int index2) const;

	VoxelMesherTransvoxel();
	~VoxelMesherTransvoxel();

protected:
	static void _bind_methods();
	Ref<TransvoxelRegularCellData> _regular_cell_datas[16];
	Ref<TransvoxelTransitionCellData> _transition_cell_data[56];
};

#endif // VOXEL_MESHER_SMOOTH_H
