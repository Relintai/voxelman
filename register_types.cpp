#include "register_types.h"

#include "library/voxel_surface.h"
#include "library/voxelman_library.h"

#include "data/voxel_light.h"
#include "meshers/voxel_mesher.h"
#include "meshers/transvoxel_cell_data.h"
#include "meshers/voxel_mesher_transvoxel.h"
#include "meshers/voxel_mesher_transvoxel_terrarin.h"

#include "world/voxel_buffer.h"
#include "world/voxel_world.h"
#include "world/voxel_chunk.h"

#include "meshers/cubic_mesher/voxel_mesher_cubic.h"
#include "meshers/cubic_mesher/voxel_cube_points.h"

#include "props/voxelman_prop.h"
#include "props/voxelman_prop_entry.h"
#include "props/voxelman_prop_scene.h"
#include "props/voxelman_prop_mesh.h"
#include "props/voxelman_prop_light.h"

#include "level_generator/voxelman_level_generator.h"

void register_voxelman_types() {
	ClassDB::register_class<VoxelMesher>();
	ClassDB::register_class<VoxelMesherTransvoxel>();
	ClassDB::register_class<VoxelMesherTransvoxelTerrarin>();
	ClassDB::register_class<TransvoxelCellData>();

	ClassDB::register_class<VoxelSurface>();
	ClassDB::register_class<VoxelmanLibrary>();

	ClassDB::register_class<VoxelLight>();
    
    ClassDB::register_class<VoxelBuffer>();
    ClassDB::register_class<VoxelWorld>();
    ClassDB::register_class<VoxelChunk>();

    ClassDB::register_class<VoxelMesherCubic>();
    ClassDB::register_class<VoxelCubePoints>();

	ClassDB::register_class<VoxelmanProp>();
	ClassDB::register_class<VoxelmanPropEntry>();
	ClassDB::register_class<VoxelmanPropScene>();
	ClassDB::register_class<VoxelmanPropMesh>();
	ClassDB::register_class<VoxelmanPropLight>();
    
    ClassDB::register_class<VoxelmanLevelGenerator>();
}	

void unregister_voxelman_types() {

}

