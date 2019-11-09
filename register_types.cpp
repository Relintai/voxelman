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
#include "world/voxel_structure.h"
#include "world/environment_data.h"
#include "world/voxel_chunk_prop_data.h"

#include "meshers/cubic_mesher/voxel_mesher_cubic.h"
#include "meshers/cubic_mesher/voxel_cube_points.h"

#include "props/prop_data.h"
#include "props/prop_data_entry.h"
#include "props/prop_data_scene.h"
#include "props/prop_data_mesh.h"
#include "props/prop_data_light.h"
#include "props/prop_data_prop.h"
#include "props/prop_data_entity.h"

#include "level_generator/voxelman_level_generator.h"

#include "world_generator/data/dungeon_room_data.h"
#include "world_generator/data/dungeon_corridor_data.h"
#include "world_generator/data/dungeon_data.h"
#include "world_generator/data/biome_data.h"
#include "world_generator/data/planet_data.h"
#include "world_generator/data/world_generator_prop_data.h"

#include "world_generator/main/biome.h"
#include "world_generator/main/dungeon.h"
#include "world_generator/main/dungeon_corridor.h"
#include "world_generator/main/dungeon_room.h"
#include "world_generator/main/planet.h"

#include "world_generator/world_generator.h"


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
	ClassDB::register_class<VoxelStructure>();
	ClassDB::register_class<EnvironmentData>();
	ClassDB::register_class<VoxelChunkPropData>();

    ClassDB::register_class<VoxelMesherCubic>();
    ClassDB::register_class<VoxelCubePoints>();

	ClassDB::register_class<PropData>();
	ClassDB::register_class<PropDataEntry>();
	ClassDB::register_class<PropDataScene>();
	ClassDB::register_class<PropDataMesh>();
	ClassDB::register_class<PropDataLight>();
	ClassDB::register_class<PropDataProp>();
	ClassDB::register_class<PropDataEntity>();
    
    ClassDB::register_class<VoxelmanLevelGenerator>();

	ClassDB::register_class<DungeonRoomData>();
	ClassDB::register_class<DungeonCorridorData>();
	ClassDB::register_class<DungeonData>();
	ClassDB::register_class<BiomeData>();
	ClassDB::register_class<PlanetData>();
	ClassDB::register_class<WorldGeneratorPropData>();

	ClassDB::register_class<DungeonRoom>();
	ClassDB::register_class<DungeonCorridor>();
	ClassDB::register_class<Dungeon>();
	ClassDB::register_class<Biome>();
	ClassDB::register_class<Planet>();

	ClassDB::register_class<WorldGenerator>();
}	

void unregister_voxelman_types() {

}

