#include "register_types.h"

#include "containers/voxelman_queue.h"
#include "containers/voxelman_unbounded_queue.h"

#include "library/voxel_surface.h"
#include "library/voxel_surface_merger.h"
#include "library/voxel_surface_simple.h"

#include "library/voxelman_library.h"
#include "library/voxelman_library_merger.h"
#include "library/voxelman_library_simple.h"

#include "data/voxel_light.h"
#include "meshers/transvoxel_uv_mesher/transvoxel_cell_data.h"
#include "meshers/transvoxel_uv_mesher/voxel_mesher_transvoxel.h"
#include "meshers/voxel_mesher.h"

#include "world/environment_data.h"
#include "world/voxel_chunk.h"
#include "world/voxel_chunk_prop_data.h"
#include "world/voxel_structure.h"
#include "world/voxel_world.h"

#include "meshers/cubic_mesher/voxel_cube_points.h"
#include "meshers/cubic_mesher/voxel_mesher_cubic.h"

#include "props/prop_data.h"
#include "props/prop_data_entity.h"
#include "props/prop_data_entry.h"
#include "props/prop_data_light.h"
#include "props/prop_data_mesh.h"
#include "props/prop_data_prop.h"
#include "props/prop_data_scene.h"

#include "level_generator/voxelman_level_generator.h"

#include "areas/world_area.h"

#include "clutter/ground_clutter.h"
#include "clutter/ground_clutter_foliage.h"

//#include "prop_tool/prop_tool_editor_plugin.h"

void register_voxelman_types() {
	ClassDB::register_class<VoxelmanQueue>();
	ClassDB::register_class<VoxelmanUnboundedQueue>();

	ClassDB::register_class<VoxelMesher>();
	ClassDB::register_class<VoxelMesherTransvoxel>();
	ClassDB::register_class<TransvoxelCellData>();

	ClassDB::register_class<VoxelSurface>();
	ClassDB::register_class<VoxelSurfaceSimple>();
	ClassDB::register_class<VoxelSurfaceMerger>();

	ClassDB::register_class<VoxelmanLibrary>();
	ClassDB::register_class<VoxelmanLibrarySimple>();
	ClassDB::register_class<VoxelmanLibraryMerger>();

	ClassDB::register_class<VoxelLight>();

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

	ClassDB::register_class<WorldArea>();

	ClassDB::register_class<GroundClutterFoliage>();

//#ifdef TOOLS_ENABLED
//	EditorPlugins::add_by_type<PropToolEditorPlugin>();
//#endif
}

void unregister_voxelman_types() {
}
