/*
Copyright (c) 2019-2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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

#include "world/voxel_world_editor.h"

#include "prop_tool/prop_tool_light.h"
#include "prop_tool/prop_tool_mesh.h"
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

	ClassDB::register_class<PropToolMesh>();
	ClassDB::register_class<PropToolLight>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<VoxelWorldEditorPlugin>();
#endif

	//#ifdef TOOLS_ENABLED
	//	EditorPlugins::add_by_type<PropToolEditorPlugin>();
	//#endif
}

void unregister_voxelman_types() {
}
