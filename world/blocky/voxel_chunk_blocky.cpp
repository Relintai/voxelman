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

#include "voxel_chunk_blocky.h"

#include "../../meshers/blocky/voxel_mesher_blocky.h"
#include "../../meshers/blocky/voxel_mesher_liquid_blocky.h"

#include "../../defines.h"

#include "../jobs/voxel_light_job.h"
#include "../jobs/voxel_prop_job.h"
#include "../jobs/voxel_terrarin_job.h"

VoxelChunkBlocky::VoxelChunkBlocky() {
}

VoxelChunkBlocky::~VoxelChunkBlocky() {
}

void VoxelChunkBlocky::_setup_channels() {
	set_channel_count(MAX_DEFAULT_CHANNELS);
}

void VoxelChunkBlocky::_create_meshers() {
	Ref<VoxelTerrarinJob> tj;
	tj.instance();

	Ref<VoxelLightJob> lj;
	lj.instance();

	Ref<VoxelPropJob> pj;
	pj.instance();
	pj->set_prop_mesher(Ref<VoxelMesher>(memnew(VoxelMesherBlocky)));

	tj->add_mesher(Ref<VoxelMesher>(memnew(VoxelMesherBlocky())));
	tj->add_liquid_mesher(Ref<VoxelMesher>(memnew(VoxelMesherLiquidBlocky())));

	add_job(lj);
	add_job(tj);
	add_job(pj);
}

void VoxelChunkBlocky::_bind_methods() {
	ADD_PROPERTYI(PropertyInfo(Variant::POOL_BYTE_ARRAY, "data_channel"), "set_channel_compressed", "get_channel_compressed", 0);

	//ClassDB::bind_method(D_METHOD("get_channel_compressed", "channel_index"), &VoxelChunk::get_channel_compressed);
	//ClassDB::bind_method(D_METHOD("set_channel_compressed", "channel_index", "array"), &VoxelChunk::set_channel_compressed);
}
