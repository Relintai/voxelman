#ifndef MARCHING_CUBES_VOXEL_QUERY_H
#define MARCHING_CUBES_VOXEL_QUERY_H

#include "core/reference.h"

#include "../data/voxel.h"

class MarchingCubesVoxelQuery : public Reference {
	GDCLASS(MarchingCubesVoxelQuery, Reference);

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

	Ref<Voxel> get_entry(int index);
	void set_entry(int index, Ref<Voxel> voxel);

	int get_size();
	void set_size(int size);

	Vector3i get_position();
	void set_position(Vector3i position);

	Vector3 get_position_bind();
	void set_position_bind(Vector3 position);

	int get_case_code();
	void update_case_code();
	bool is_triangulation_trivial();

	void set_entries(Ref<Voxel> voxel000, Ref<Voxel> voxel100, Ref<Voxel> voxel010, Ref<Voxel> voxel110, Ref<Voxel> voxel001, Ref<Voxel> voxel101, Ref<Voxel> voxel011, Ref<Voxel> voxel111);

	MarchingCubesVoxelQuery();
	~MarchingCubesVoxelQuery();

protected:
	static void _bind_methods();

private:
	int _case_code;
	int _size;
	Vector3i _position;
	Ref<Voxel> _voxels[VOXEL_ENTRIES_SIZE];
};

VARIANT_ENUM_CAST(MarchingCubesVoxelQuery::VoxelEntryIndices);
VARIANT_ENUM_CAST(MarchingCubesVoxelQuery::VoxelEntryMask);

#endif
