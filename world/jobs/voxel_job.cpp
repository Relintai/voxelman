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

#include "../default/voxel_chunk_default.h"

#include "../../../opensimplex/open_simplex_noise.h"

void VoxelJob::set_chunk(const Ref<VoxelChunk> &chunk) {
	_chunk = chunk;

	_in_tree = true;
}

void VoxelJob::chunk_exit_tree() {

	_in_tree = false;

	if (get_complete()) {
		_chunk.unref();
	} else {
		set_cancelled(true);
	}
}

void VoxelJob::_execute() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<VoxelChunkDefault> chunk = _chunk;

	ERR_FAIL_COND(!chunk.is_valid());

	if (!chunk->has_next_phase()) {
		//_chunk->set_build_step_in_progress(false);

		//if (!_in_tree) {
		//	_chunk.unref();
		//}

		set_complete(true);
	}

	ERR_FAIL_COND(!chunk->has_next_phase());
	//ERR_FAIL_COND(_build_step_in_progress);

	//_chunk->set_build_step_in_progress(true);

	while (!get_cancelled() && _in_tree && chunk->has_next_phase() && chunk->get_active_build_phase_type() == VoxelChunkDefault::BUILD_PHASE_TYPE_NORMAL) {

		//int phase = _chunk->get_current_build_phase();

		chunk->build_phase();

		//print_error(String::num(get_current_execution_time()) + " phase: " + String::num(phase));

		if (chunk->get_active_build_phase_type() == VoxelChunkDefault::BUILD_PHASE_TYPE_NORMAL && should_return())
			return;

		if (!chunk->get_build_phase_done())
			break;
	}

	chunk->set_build_step_in_progress(false);

	if (!_in_tree) {
		chunk.unref();
	}

	set_complete(true);
}

//Data Management functions
void VoxelJob::generate_ao() {
	ERR_FAIL_COND(!_chunk.is_valid());

	int data_size_x = _chunk->get_data_size_x();
	int data_size_y = _chunk->get_data_size_y();
	int data_size_z = _chunk->get_data_size_z();

	ERR_FAIL_COND(data_size_x == 0 || data_size_y == 0 || data_size_z == 0);

	int margin_start = _chunk->get_margin_start();
	int margin_end = _chunk->get_margin_end();

	int ssize_x = _chunk->get_size_x();
	int ssize_y = _chunk->get_size_y();
	int ssize_z = _chunk->get_size_z();

	int size_x = ssize_x + margin_end;
	int size_y = ssize_y + margin_end;
	int size_z = ssize_z + margin_end;

	for (int y = margin_start - 1; y < size_y - 1; ++y) {
		for (int z = margin_start - 1; z < size_z - 1; ++z) {
			for (int x = margin_start - 1; x < size_x - 1; ++x) {
				int current = _chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);

				int sum = _chunk->get_voxel(x + 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);
				sum += _chunk->get_voxel(x - 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);
				sum += _chunk->get_voxel(x, y + 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);
				sum += _chunk->get_voxel(x, y - 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);
				sum += _chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);
				sum += _chunk->get_voxel(x, y, z - 1, VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);

				sum /= 6;

				sum -= current;

				if (sum < 0)
					sum = 0;

				_chunk->set_voxel(sum, x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO);
			}
		}
	}
}

void VoxelJob::generate_random_ao(int seed, int octaves, int period, float persistence, float scale_factor) {
	ERR_FAIL_COND(!_chunk.is_valid());

	int margin_start = _chunk->get_margin_start();
	int margin_end = _chunk->get_margin_end();

	int size_x = _chunk->get_size_x();
	int size_y = _chunk->get_size_y();
	int size_z = _chunk->get_size_z();

	int position_x = _chunk->get_position_x();
	int position_y = _chunk->get_position_y();
	int position_z = _chunk->get_position_z();

	Ref<OpenSimplexNoise> noise;
	noise.instance();

	noise->set_seed(seed);
	noise->set_octaves(octaves);
	noise->set_period(period);
	noise->set_persistence(persistence);

	for (int x = -margin_start; x < size_x + margin_end; ++x) {
		for (int z = -margin_start; z < size_z + margin_end; ++z) {
			for (int y = -margin_start; y < size_y + margin_end; ++y) {
				float val = noise->get_noise_3d(x + (position_x * size_x), y + (position_y * size_y), z + (position_z * size_z));

				val *= scale_factor;

				if (val > 1)
					val = 1;

				if (val < 0)
					val = -val;

				_chunk->set_voxel(int(val * 255.0), x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
			}
		}
	}
}

VoxelJob::VoxelJob() {
	_in_tree = false;

#if !THREAD_POOL_PRESENT
	_complete = true;
	_cancelled = false;

	_max_allocated_time = 0;
	_start_time = 0;

	_current_run_stage = 0;
	_stage = 0;
#endif
}

VoxelJob::~VoxelJob() {
	_chunk.unref();
}

void VoxelJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_execute"), &VoxelJob::_execute);

#if !THREAD_POOL_PRESENT
	ClassDB::bind_method(D_METHOD("get_complete"), &VoxelJob::get_complete);
	ClassDB::bind_method(D_METHOD("set_complete", "value"), &VoxelJob::set_complete);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "complete"), "set_complete", "get_complete");

	ClassDB::bind_method(D_METHOD("get_start_time"), &VoxelJob::get_start_time);
	ClassDB::bind_method(D_METHOD("set_start_time", "value"), &VoxelJob::set_start_time);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "start_time"), "set_start_time", "get_start_time");

	ClassDB::bind_method(D_METHOD("get_current_run_stage"), &VoxelJob::get_current_run_stage);
	ClassDB::bind_method(D_METHOD("set_current_run_stage", "value"), &VoxelJob::set_current_run_stage);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_run_stage"), "set_current_run_stage", "get_current_run_stage");

	ClassDB::bind_method(D_METHOD("get_stage"), &VoxelJob::get_stage);
	ClassDB::bind_method(D_METHOD("set_stage", "value"), &VoxelJob::set_stage);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stage"), "set_stage", "get_stage");

	ClassDB::bind_method(D_METHOD("get_current_execution_time"), &VoxelJob::get_current_execution_time);

	ClassDB::bind_method(D_METHOD("should_do", "just_check"), &VoxelJob::should_do, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("should_return"), &VoxelJob::should_return);

	BIND_VMETHOD(MethodInfo("_execute"));
	ClassDB::bind_method(D_METHOD("execute"), &VoxelJob::execute);

	ADD_SIGNAL(MethodInfo("completed"));
#endif
}

#if !THREAD_POOL_PRESENT
bool VoxelJob::get_complete() const {
	return _complete;
}
void VoxelJob::set_complete(const bool value) {
	_complete = value;
}

bool VoxelJob::get_cancelled() const {
	return _cancelled;
}
void VoxelJob::set_cancelled(const bool value) {
	_cancelled = value;
}

float VoxelJob::get_max_allocated_time() const {
	return _max_allocated_time;
}
void VoxelJob::set_max_allocated_time(const float value) {
	_max_allocated_time = value;
}

int VoxelJob::get_start_time() const {
	return _start_time;
}
void VoxelJob::set_start_time(const int value) {
	_start_time = value;
}

int VoxelJob::get_current_run_stage() const {
	return _current_run_stage;
}
void VoxelJob::set_current_run_stage(const int value) {
	_current_run_stage = value;
}

int VoxelJob::get_stage() const {
	return _stage;
}
void VoxelJob::set_stage(const int value) {
	_stage = value;
}

float VoxelJob::get_current_execution_time() {
	return 0;
}

bool VoxelJob::should_do(const bool just_check) {
	return true;
}
bool VoxelJob::should_return() {
	if (_cancelled)
		return true;

	return false;
}

void VoxelJob::execute() {
	ERR_FAIL_COND(!has_method("_execute"));

	call("_execute");
}

#endif