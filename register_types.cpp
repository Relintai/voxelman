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

#include "library/voxel_surface.h"
#include "library/voxel_surface_simple.h"

#include "library/voxel_library.h"
#include "library/voxel_library_simple.h"
#include "library/voxel_material_cache.h"

#ifdef TEXTURE_PACKER_PRESENT
#include "library/voxel_library_merger.h"
#include "library/voxel_library_merger_pcm.h"
#include "library/voxel_material_cache_pcm.h"
#include "library/voxel_surface_merger.h"
#endif

#include "data/voxel_light.h"
#include "meshers/voxel_mesher.h"

#include "meshers/marching_cubes/marching_cubes_cell_data.h"
#include "meshers/marching_cubes/voxel_mesher_marching_cubes.h"

#include "world/block_voxel_structure.h"
#include "world/environment_data.h"
#include "world/voxel_chunk.h"
#include "world/voxel_structure.h"
#include "world/voxel_world.h"

#include "world/default/voxel_chunk_default.h"
#include "world/default/voxel_world_default.h"

#include "meshers/cubic/voxel_cube_points.h"
#include "meshers/cubic/voxel_mesher_cubic.h"

#include "level_generator/voxel_level_generator.h"
#include "level_generator/voxel_level_generator_flat.h"

#include "areas/world_area.h"

#include "world/voxel_world_editor.h"

#include "meshers/blocky/voxel_mesher_blocky.h"
#include "meshers/blocky/voxel_mesher_liquid_blocky.h"

#include "world/blocky/voxel_chunk_blocky.h"
#include "world/blocky/voxel_world_blocky.h"

#include "world/cubic/voxel_chunk_cubic.h"
#include "world/cubic/voxel_world_cubic.h"

#include "world/marching_cubes/voxel_chunk_marching_cubes.h"
#include "world/marching_cubes/voxel_world_marching_cubes.h"

#include "nodes/voxel_light_node.h"

#include "world/jobs/voxel_job.h"
#include "world/jobs/voxel_light_job.h"
#include "world/jobs/voxel_prop_job.h"
#include "world/jobs/voxel_terrain_job.h"
#include "world/jobs/voxel_mesher_job_step.h"

void register_voxelman_types() {
	ClassDB::register_class<VoxelMesher>();
	ClassDB::register_class<VoxelMesherDefault>();

	ClassDB::register_class<VoxelMesherMarchingCubes>();
	ClassDB::register_class<MarchingCubesCellData>();

	ClassDB::register_class<VoxelSurface>();
	ClassDB::register_class<VoxelSurfaceSimple>();

	ClassDB::register_class<VoxelLibrary>();
	ClassDB::register_class<VoxelLibrarySimple>();

	ClassDB::register_class<VoxelMaterialCache>();

#ifdef TEXTURE_PACKER_PRESENT
	ClassDB::register_class<VoxelSurfaceMerger>();
	ClassDB::register_class<VoxelLibraryMerger>();
	ClassDB::register_class<VoxelLibraryMergerPCM>();
	ClassDB::register_class<VoxelMaterialCachePCM>();
#endif

	ClassDB::register_class<VoxelLight>();
	ClassDB::register_class<VoxelLightNode>();

	ClassDB::register_class<VoxelWorld>();
	ClassDB::register_class<VoxelChunk>();
	ClassDB::register_class<VoxelStructure>();
	ClassDB::register_class<BlockVoxelStructure>();
	ClassDB::register_class<EnvironmentData>();

	ClassDB::register_class<VoxelChunkDefault>();
	ClassDB::register_class<VoxelWorldDefault>();

	ClassDB::register_class<VoxelMesherCubic>();
	ClassDB::register_class<VoxelCubePoints>();

	ClassDB::register_class<VoxelMesherBlocky>();
	ClassDB::register_class<VoxelWorldBlocky>();
	ClassDB::register_class<VoxelChunkBlocky>();
	ClassDB::register_class<VoxelMesherLiquidBlocky>();

	ClassDB::register_class<VoxelWorldMarchingCubes>();
	ClassDB::register_class<VoxelChunkMarchingCubes>();

	ClassDB::register_class<VoxelWorldCubic>();
	ClassDB::register_class<VoxelChunkCubic>();

	ClassDB::register_class<VoxelLevelGenerator>();
	ClassDB::register_class<VoxelLevelGeneratorFlat>();

	ClassDB::register_class<WorldArea>();

	ClassDB::register_class<VoxelJob>();
	ClassDB::register_class<VoxelTerrainJob>();
	ClassDB::register_class<VoxelMesherJobStep>();
	ClassDB::register_class<VoxelLightJob>();
	ClassDB::register_class<VoxelPropJob>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<VoxelWorldEditorPlugin>();
#endif
}

void unregister_voxelman_types() {
}
