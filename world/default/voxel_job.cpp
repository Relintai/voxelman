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

#include "voxel_job.h"

#include "voxel_chunk_default.h"

void VoxelJob::set_chunk(const Ref<VoxelChunkDefault> &chunk) {
	_chunk = chunk;

	_in_tree = true;
}

void VoxelJob::chunk_exit_tree() {
	_in_tree = false;

	if (get_complete()) {
		_chunk.unref();
	}
}

void VoxelJob::_execute() {
	ERR_FAIL_COND(!_chunk.is_valid());
	ERR_FAIL_COND(!_chunk->has_next_phase());
	//ERR_FAIL_COND(_build_step_in_progress);

	//_chunk->set_build_step_in_progress(true);

	while (!get_cancelled() && _in_tree && _chunk->has_next_phase() && _chunk->get_active_build_phase_type() == VoxelChunkDefault::BUILD_PHASE_TYPE_NORMAL) {
		if (should_return())
			return;

		_chunk->build_phase();

		if (!_chunk->get_build_phase_done())
			break;
	}

	_chunk->set_build_step_in_progress(false);

	if (!_in_tree) {
		_chunk.unref();
	}

	set_complete(true);
}

VoxelJob::VoxelJob() {
	_build_step_in_progress = false;
	_in_tree = false;
}

VoxelJob::~VoxelJob() {
	_chunk.unref();
}

void VoxelJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_execute"), &VoxelJob::_execute);
}
