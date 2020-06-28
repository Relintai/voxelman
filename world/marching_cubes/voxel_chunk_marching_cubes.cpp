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

#include "voxel_chunk_marching_cubes.h"

#include "../../meshers/marching_cubes/voxel_mesher_marching_cubes.h"

#include "../../defines.h"

VoxelChunkMarchingCubes::VoxelChunkMarchingCubes() {
}

VoxelChunkMarchingCubes::~VoxelChunkMarchingCubes() {
}

void VoxelChunkMarchingCubes::_setup_channels() {
	set_channel_count(MAX_DEFAULT_CHANNELS);
}

void VoxelChunkMarchingCubes::_create_meshers() {
	set_prop_mesher(Ref<VoxelMesher>(memnew(VoxelMesherMarchingCubes)));
	add_mesher(Ref<VoxelMesher>(memnew(VoxelMesherMarchingCubes())));
	//add_liquid_mesher(Ref<VoxelMesher>(memnew(VoxelMesherLiquidMarchingCubes())));

	for (int i = 0; i < _meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_lod_size(get_lod_size());
		mesher->set_voxel_scale(get_voxel_scale());

		Ref<VoxelMesherDefault> md = mesher;

		if (md.is_valid()) {
			md->set_build_flags(get_build_flags());
		}
	}

	/*
	for (int i = 0; i < _liquid_meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_lod_size(get_lod_size());
		mesher->set_voxel_scale(get_voxel_scale());

		Ref<VoxelMesherDefault> md = mesher;

		if (md.is_valid()) {
			md->set_build_flags(get_build_flags());
		}
	}*/
}

void VoxelChunkMarchingCubes::_bind_methods() {
	ADD_PROPERTYI(PropertyInfo(Variant::POOL_BYTE_ARRAY, "data_channel"), "set_channel_compressed", "get_channel_compressed", 0);
	ADD_PROPERTYI(PropertyInfo(Variant::POOL_BYTE_ARRAY, "isolevel_channel"), "set_channel_compressed", "get_channel_compressed", 1);

	//ClassDB::bind_method(D_METHOD("get_channel_compressed", "channel_index"), &VoxelChunk::get_channel_compressed);
	//ClassDB::bind_method(D_METHOD("set_channel_compressed", "channel_index", "array"), &VoxelChunk::set_channel_compressed);
}
