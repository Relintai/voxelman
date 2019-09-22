#ifndef VOXELMAN_PROP_DATA_H
#define VOXELMAN_PROP_DATA_H

#include "core/resource.h"
#include "core/math/transform.h"

class VoxelmanPropEntry : public Resource {
	GDCLASS(VoxelmanPropEntry, Resource);
	
public:

	Transform get_transform() const;
	void set_transform(const Transform value);

	VoxelmanPropEntry();
	~VoxelmanPropEntry();

protected:
	static void _bind_methods();

private:

	Transform _transform;
};

#endif
