#ifndef PROP_MESH_DATA_REOURCE_H
#define PROP_MESH_DATA_REOURCE_H

#include "core/resource.h"
#include "core/array.h"
#include "scene/resources/mesh.h"

class PropMeshDataResource : public Resource {

	GDCLASS(PropMeshDataResource, Resource);
	RES_BASE_EXTENSION("pmdres");

public:
	Array get_array();
	void set_array(const Array &p_arrays);

	PropMeshDataResource();

protected: 
	static void _bind_methods();

private:
	Array _arrays;

};

#endif
