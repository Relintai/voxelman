#ifndef MESH_SIMPLIFIER_H
#define MESH_SIMPLIFIER_H

#include "core/reference.h"

#include "core/pool_vector.h"
#include "core/resource.h"

class MeshSimplifier : public Reference {
	GDCLASS(MeshSimplifier, Reference);

public:
	MeshSimplifier();

protected:
	static void _bind_methods() {}
};

#endif