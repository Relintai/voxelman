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

#include "voxel_light_job.h"

#include "../../defines.h"

#include "../../library/voxel_surface.h"
#include "../../library/voxel_library.h"

#include "../../meshers/voxel_mesher.h"
#include "../default/voxel_chunk_default.h"

void VoxelLightJob::phase_light() {
	Ref<VoxelChunkDefault> chunk = _chunk;

	if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
		if (!chunk->channel_get(VoxelChunkDefault::DEFAULT_CHANNEL_AO))
			generate_ao();

	bool gr = (chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_AUTO_GENERATE_RAO) != 0;

	if (!gr && (chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) == 0) {
		next_phase();
		return;
	}

	bool bl = (chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_BAKE_LIGHTS) != 0;

	if (bl && should_do()) {
		chunk->clear_baked_lights();

		if (should_return())
			return;
	}

	if (gr && should_do()) {
		generate_random_ao(chunk->get_voxel_world()->get_current_seed());

		if (should_return())
			return;
	}

	if (bl && should_do()) {
		chunk->bake_lights();

		if (should_return())
			return;
	}

	reset_stages();
	next_phase();
}

void VoxelLightJob::_execute_phase() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<VoxelLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	phase_light();
	set_complete(true);
	next_job();
}

VoxelLightJob::VoxelLightJob() {
}

VoxelLightJob::~VoxelLightJob() {
}

void VoxelLightJob::_bind_methods() {
}
