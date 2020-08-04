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
	} else {
		set_cancelled(true);
	}
}

void VoxelJob::_execute() {
	ERR_FAIL_COND(!_chunk.is_valid());
	ERR_FAIL_COND(!_chunk->has_next_phase());
	//ERR_FAIL_COND(_build_step_in_progress);

	//_chunk->set_build_step_in_progress(true);

	while (!get_cancelled() && _in_tree && _chunk->has_next_phase() && _chunk->get_active_build_phase_type() == VoxelChunkDefault::BUILD_PHASE_TYPE_NORMAL) {

		int phase = _chunk->get_current_build_phase();

		_chunk->build_phase();

		print_error(String::num(get_current_execution_time()) + " phase: " + String::num(phase));

		if (should_return())
			return;

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