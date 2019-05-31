#include "register_types.h"

#include "library/voxel_surface.h"
#include "library/voxelman_library.h"

#include "data/voxel.h"
#include "data/voxel_light.h"
#include "meshers/voxel_mesher.h"
#include "meshers/voxel_mesher_transvoxel.h"

#include "utility/marching_cubes_voxel_query.h"

#include "world/voxel_buffer.h"

void register_voxelman_types() {
	ClassDB::register_class<VoxelMesher>();
	ClassDB::register_class<VoxelMesherTransvoxel>();
	ClassDB::register_class<TransvoxelRegularCellData>();
	ClassDB::register_class<TransvoxelTransitionCellData>();

	ClassDB::register_class<VoxelSurface>();
	ClassDB::register_class<VoxelmanLibrary>();

	ClassDB::register_class<Voxel>();
	ClassDB::register_class<VoxelLight>();

	ClassDB::register_class<MarchingCubesVoxelQuery>();
    
    ClassDB::register_class<VoxelBuffer>();
}	

void unregister_voxelman_types() {

}

