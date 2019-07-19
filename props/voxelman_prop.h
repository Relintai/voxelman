#ifndef VOXELMAN_PROP_H
#define VOXELMAN_PROP_H

#include "core/reference.h"
#include "core/vector.h"

#include "voxelman_prop_data.h"

class VoxelmanProp : public Resource {
	GDCLASS(VoxelmanProp, Resource);

public:
	Ref<VoxelmanPropData> get_prop(const int index) const;
	void set_prop(const int index, const Ref<VoxelmanPropData> prop);

	int get_prop_count() const;
	void set_prop_count(const int size);

	VoxelmanProp();
	~VoxelmanProp();

	enum {
		MAX_PROPS = 36
	};

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &property) const;

private:
	Vector<Ref<VoxelmanPropData> > _props;
};

#endif
