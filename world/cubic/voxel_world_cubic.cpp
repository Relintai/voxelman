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

#include "voxel_world_cubic.h"

#include "voxel_chunk_cubic.h"

#include "../../meshers/cubic/voxel_mesher_cubic.h"
#include "../jobs/voxel_light_job.h"
#include "../jobs/voxel_prop_job.h"
#include "../jobs/voxel_terrarin_job.h"

Ref<VoxelChunk> VoxelWorldCubic::_create_chunk(int x, int y, int z, Ref<VoxelChunk> chunk) {

	if (!chunk.is_valid()) {
		chunk = Ref<VoxelChunk>(memnew(VoxelChunkCubic));
	}

	if (chunk->job_get_count() == 0) {
		Ref<VoxelTerrarinJob> tj;
		tj.instance();

		Ref<VoxelLightJob> lj;
		lj.instance();

		Ref<VoxelPropJob> pj;
		pj.instance();
		pj->set_prop_mesher(Ref<VoxelMesher>(memnew(VoxelMesherCubic)));

		Ref<VoxelMesher> m = Ref<VoxelMesher>(memnew(VoxelMesherCubic()));
		m->set_channel_index_type(VoxelChunkDefault::DEFAULT_CHANNEL_TYPE);
		m->set_channel_index_isolevel(VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);

		tj->add_mesher(m);
		//add_liquid_mesher(Ref<VoxelMesher>(memnew(VoxelMesherLiquidMarchingCubes())));

		chunk->job_add(lj);
		chunk->job_add(tj);
		chunk->job_add(pj);
	}

	return VoxelWorld::_create_chunk(x, y, z, chunk);
}

VoxelWorldCubic::VoxelWorldCubic() {
	set_data_margin_start(1);
	set_data_margin_end(1);
}

VoxelWorldCubic ::~VoxelWorldCubic() {
}

void VoxelWorldCubic::_bind_methods() {
}