#ifndef VOXEL_H
#define VOXEL_H

#include "core/reference.h"
#include "../collections/vector3i.h"
#include "core/math/quat.h"
#include "core/resource.h"
#include "core/vector.h"
#include "sub_voxel_points.h"
#include "voxel_data.h"
#include "voxel_enums.h"
#include "voxel_face.h"

#include "../library/voxel_surface_definition.h"

using namespace voxelman;

namespace voxelman {

class SubVoxelPoints;

class Voxel : public Reference {
	GDCLASS(Voxel, Reference);

public:
	
	static const unsigned int FACE_COUNT = 6;

	int get_neighbours();
	void set_neighbours(int value);

	bool get_has_sub_voxel_points();
	SubVoxelPoints *get_sub_voxel_points();

	void set_voxel_datas(Ref<VoxelData> VP000, Ref<VoxelData> VP100, Ref<VoxelData> VP010, Ref<VoxelData> VP001, Ref<VoxelData> VP110, Ref<VoxelData> VP011, Ref<VoxelData> VP101, Ref<VoxelData> VP111);
	void refresh_sub_voxel_points();

protected:
	static void _bind_methods() {}

private:
	int _neighbours;
	SubVoxelPoints *_sub_voxel_points;
};

} // namespace voxelman

#endif
