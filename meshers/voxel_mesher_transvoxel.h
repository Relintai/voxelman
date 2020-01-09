#ifndef VOXEL_MESHER_TRANSVOXEL_H
#define VOXEL_MESHER_TRANSVOXEL_H

#include "core/reference.h"
#include "voxel_mesher.h"

#include "transvoxel_cell_data.h"

#include "transvoxel_tables.h"

using namespace Transvoxel;

class VoxelMesherTransvoxel : public VoxelMesher {
	GDCLASS(VoxelMesherTransvoxel, VoxelMesher)

public:
	static const String BINDING_STRING_VOXEL_ENTRY_INDICES;
	static const String BINDING_STRING_VOXEL_ENTRY_MASK;

	enum VoxelEntryIndices {
		VOXEL_ENTRY_INDEX_000 = 0,
		VOXEL_ENTRY_INDEX_100 = 1,
		VOXEL_ENTRY_INDEX_001 = 2,
		VOXEL_ENTRY_INDEX_101 = 3,

		VOXEL_ENTRY_INDEX_010 = 4,
		VOXEL_ENTRY_INDEX_110 = 5,
		VOXEL_ENTRY_INDEX_011 = 6,
		VOXEL_ENTRY_INDEX_111 = 7,

		VOXEL_ENTRIES_SIZE = 8,
	};

	enum VoxelEntryMask {
		VOXEL_ENTRY_MASK_000 = 1 << 0,
		VOXEL_ENTRY_MASK_100 = 1 << 1,
		VOXEL_ENTRY_MASK_001 = 1 << 2,
		VOXEL_ENTRY_MASK_101 = 1 << 3,

		VOXEL_ENTRY_MASK_010 = 1 << 4,
		VOXEL_ENTRY_MASK_110 = 1 << 5,
		VOXEL_ENTRY_MASK_011 = 1 << 6,
		VOXEL_ENTRY_MASK_111 = 1 << 7,
	};

	Vector3 corner_id_to_vertex(int corner_id) const;

	int get_regular_cell_class(int index) const;

	Ref<TransvoxelCellData> get_regular_cell_data(int index) const;
	int get_regular_vertex_data(int index10, int index2) const;
	// void set_regular_vertex_data(int index1, int index2, int value);
	int get_regular_vertex_data_first_vertex(int index1, int index2) const;
	int get_regular_vertex_data_second_vertex(int index1, int index2) const;
	Vector3 get_regular_vertex_first_position(int index1, int index2) const;
	Vector3 get_regular_vertex_second_position(int index1, int index2) const;
	Vector3 get_regular_vertex_direction(int index1, int index2) const;

	int get_transition_cell_class(int index) const;

	Ref<TransvoxelCellData> get_transition_cell_data(int index) const;
	int get_transition_corner_data(int index) const;
	int get_transition_vertex_data(int index1, int index2) const;
	int get_transition_vertex_data_first_vertex(int index1, int index2) const;
	int get_transition_vertex_data_second_vertex(int index1, int index2) const;
	Vector3 get_transition_vertex_first_position(int index1, int index2) const;
	Vector3 get_transition_vertex_second_position(int index1, int index2) const;
	Vector3 get_transition_vertex_direction(int index1, int index2) const;

	VoxelMesherTransvoxel();
	~VoxelMesherTransvoxel();

protected:
	static void _bind_methods();
	Ref<TransvoxelCellData> _regular_cell_datas[16];
	Ref<TransvoxelCellData> _transition_cell_data[56];
};

VARIANT_ENUM_CAST(VoxelMesherTransvoxel::VoxelEntryIndices);
VARIANT_ENUM_CAST(VoxelMesherTransvoxel::VoxelEntryMask);

#endif // VOXEL_MESHER_SMOOTH_H
