/*
Copyright (c) 2019-2022 Péter Magyar

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

#include "voxel_mesher_job_step.h"

const String VoxelMesherJobStep::BINDING_STRING_TERRAIN_TERRAIN_JOB_STEP_TYPE = "Normal,Normal LOD,Drop UV2,Merge Verts,Bake Texture,Simplify Mesh";

VoxelMesherJobStep::VoxelMesherJobStepType VoxelMesherJobStep::get_job_type() const {
	return _job_type;
}
void VoxelMesherJobStep::set_job_type(const VoxelMesherJobStep::VoxelMesherJobStepType value) {
	_job_type = value;
}

int VoxelMesherJobStep::get_lod_index() const {
	return _lod_index;
}
void VoxelMesherJobStep::set_lod_index(const int value) {
	_lod_index = value;
}

#ifdef MESH_UTILS_PRESENT
Ref<FastQuadraticMeshSimplifier> VoxelMesherJobStep::get_fqms() {
	return _fqms;
}
void VoxelMesherJobStep::set_fqms(const Ref<FastQuadraticMeshSimplifier> &val) {
	_fqms = val;
}

float VoxelMesherJobStep::get_simplification_step_ratio() const {
	return _simplification_step_ratio;
}
void VoxelMesherJobStep::set_simplification_step_ratio(const float value) {
	_simplification_step_ratio = value;
}

int VoxelMesherJobStep::get_simplification_steps() const {
	return _simplification_steps;
}
void VoxelMesherJobStep::set_simplification_steps(const int value) {
	_simplification_steps = value;
}

float VoxelMesherJobStep::get_simplification_agressiveness() const {
	return _simplification_agressiveness;
}
void VoxelMesherJobStep::set_simplification_agressiveness(const float value) {
	_simplification_agressiveness = value;
}

#endif

VoxelMesherJobStep::VoxelMesherJobStep() {
	_job_type = TYPE_NORMAL;
	_lod_index = 0;

#ifdef MESH_UTILS_PRESENT
	_simplification_step_ratio = 0.8;
	_simplification_steps = 2;
	_simplification_agressiveness = 7;
#endif
}

VoxelMesherJobStep::~VoxelMesherJobStep() {
#ifdef MESH_UTILS_PRESENT
	_fqms.unref();
#endif
}

void VoxelMesherJobStep::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_job_type"), &VoxelMesherJobStep::get_job_type);
	ClassDB::bind_method(D_METHOD("set_job_type", "value"), &VoxelMesherJobStep::set_job_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "job_type", PROPERTY_HINT_ENUM, VoxelMesherJobStep::BINDING_STRING_TERRAIN_TERRAIN_JOB_STEP_TYPE), "set_job_type", "get_job_type");

	ClassDB::bind_method(D_METHOD("get_lod_index"), &VoxelMesherJobStep::get_lod_index);
	ClassDB::bind_method(D_METHOD("set_lod_index", "value"), &VoxelMesherJobStep::set_lod_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_index"), "set_lod_index", "get_lod_index");

#ifdef MESH_UTILS_PRESENT
	ClassDB::bind_method(D_METHOD("get_fqms"), &VoxelMesherJobStep::get_fqms);
	ClassDB::bind_method(D_METHOD("set_fqms", "value"), &VoxelMesherJobStep::set_fqms);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fqms", PROPERTY_HINT_RESOURCE_TYPE, "FastQuadraticMeshSimplifier"), "set_fqms", "get_fqms");

	ClassDB::bind_method(D_METHOD("get_simplification_step_ratio"), &VoxelMesherJobStep::get_simplification_step_ratio);
	ClassDB::bind_method(D_METHOD("set_simplification_step_ratio", "value"), &VoxelMesherJobStep::set_simplification_step_ratio);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "simplification_step_ratio"), "set_simplification_step_ratio", "get_simplification_step_ratio");

	ClassDB::bind_method(D_METHOD("get_simplification_steps"), &VoxelMesherJobStep::get_simplification_steps);
	ClassDB::bind_method(D_METHOD("set_simplification_steps", "value"), &VoxelMesherJobStep::set_simplification_steps);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "simplification_steps"), "set_simplification_steps", "get_simplification_steps");

	ClassDB::bind_method(D_METHOD("get_simplification_agressiveness"), &VoxelMesherJobStep::get_simplification_agressiveness);
	ClassDB::bind_method(D_METHOD("set_simplification_agressiveness", "value"), &VoxelMesherJobStep::set_simplification_agressiveness);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "simplification_agressiveness"), "set_simplification_agressiveness", "get_simplification_agressiveness");
#endif

	BIND_ENUM_CONSTANT(TYPE_NORMAL);
	BIND_ENUM_CONSTANT(TYPE_NORMAL_LOD);
	BIND_ENUM_CONSTANT(TYPE_DROP_UV2);
	BIND_ENUM_CONSTANT(TYPE_MERGE_VERTS);
	BIND_ENUM_CONSTANT(TYPE_BAKE_TEXTURE);
	BIND_ENUM_CONSTANT(TYPE_SIMPLIFY_MESH);
	BIND_ENUM_CONSTANT(TYPE_OTHER);
}
