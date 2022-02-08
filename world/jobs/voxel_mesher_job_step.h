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

#ifndef VOXEL_MESHER_JOB_STEP_H
#define VOXEL_MESHER_JOB_STEP_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/ref_counted.h"
#ifndef Reference
#define Reference RefCounted
#endif
#else
#include "core/reference.h"
#endif

#ifdef MESH_UTILS_PRESENT
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

class VoxelMesherJobStep : public Reference {
	GDCLASS(VoxelMesherJobStep, Reference);

public:
	//todo add:
	//type generate lighting,
	//type skip (this would leave the mesh empty)
	//type previous mesh (this would set the previous mesh's rid to the current lod level)
	enum VoxelMesherJobStepType {
		TYPE_NORMAL = 0,
		TYPE_NORMAL_LOD,
		TYPE_DROP_UV2,
		TYPE_MERGE_VERTS,
		TYPE_BAKE_TEXTURE,
		TYPE_SIMPLIFY_MESH,
		TYPE_OTHER,
	};

	static const String BINDING_STRING_TERRAIN_TERRAIN_JOB_STEP_TYPE;

	VoxelMesherJobStepType get_job_type() const;
	void set_job_type(const VoxelMesherJobStepType value);

	int get_lod_index() const;
	void set_lod_index(const int value);

	#ifdef MESH_UTILS_PRESENT
		Ref<FastQuadraticMeshSimplifier> get_fqms();
		void set_fqms(const Ref<FastQuadraticMeshSimplifier> &val);

		float get_simplification_step_ratio() const;
		void set_simplification_step_ratio(const float value);

		int get_simplification_steps() const;
		void set_simplification_steps(const int value);

		float get_simplification_agressiveness() const;
		void set_simplification_agressiveness(const float value);
	#endif

	VoxelMesherJobStep();
	~VoxelMesherJobStep();

protected:
	static void _bind_methods();

	VoxelMesherJobStepType _job_type;
	int _lod_index;

	#ifdef MESH_UTILS_PRESENT
		Ref<FastQuadraticMeshSimplifier> _fqms;
		float _simplification_step_ratio;
		int _simplification_steps;
		float _simplification_agressiveness;
	#endif
};

VARIANT_ENUM_CAST(VoxelMesherJobStep::VoxelMesherJobStepType);

#endif
