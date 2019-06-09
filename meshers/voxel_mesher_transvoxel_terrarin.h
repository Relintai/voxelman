#ifndef VOXEL_MESHER_TRANSVOXEL_TERRARIN_H
#define VOXEL_MESHER_TRANSVOXEL_TERRARIN_H

#include <scene/resources/mesh.h>

#include "voxel_mesher.h"
#include "transvoxel_tables.h"


class VoxelMesherTransvoxelTerrarin : public VoxelMesher {
	GDCLASS(VoxelMesherTransvoxelTerrarin, VoxelMesher)

public:
	static const int MINIMUM_PADDING = 2;

	void _add_buffer(Ref<VoxelBuffer> buffer);
	//int get_minimum_padding() const override;

	VoxelMesherTransvoxelTerrarin();

protected:
	static void _bind_methods();

private:
	struct ReuseCell {
		int vertices[4];
		int case_index;
		ReuseCell();
	};

	void build_internal(Ref<VoxelBuffer> buffer, unsigned int channel);
	ReuseCell &get_reuse_cell(Vector3i pos);
	void emit_vertex(Vector3 primary, Vector3 normal);

private:
	int padding;
	const Vector3i PAD = Vector3i(1, 1, 1);

	Vector<ReuseCell> m_cache[2];
	Vector3i m_block_size;
};

#endif // VOXEL_MESHER_TRANSVOXEL_TERRARIN_H
