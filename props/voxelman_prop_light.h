#ifndef VOXELMAN_PROP_LIGHT_H
#define VOXELMAN_PROP_LIGHT_H

#include "voxelman_prop_entry.h"

#include "core/color.h"

class VoxelmanPropLight : public VoxelmanPropEntry {
	GDCLASS(VoxelmanPropLight, VoxelmanPropEntry);
	
public:
	Color get_light_color() const;
	void set_light_color(const Color value);

	int get_light_size() const;
	void set_light_size(const int value);

	VoxelmanPropLight();
	~VoxelmanPropLight();

protected:
	static void _bind_methods();

private:
	Color _light_color;
	int _light_size;
};

#endif
