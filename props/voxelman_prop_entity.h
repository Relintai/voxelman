#ifndef VOXELMAN_PROP_ENTITY_H
#define VOXELMAN_PROP_ENTITY_H

#include "voxelman_prop_entry.h"

class VoxelmanPropEntity : public VoxelmanPropEntry {
	GDCLASS(VoxelmanPropEntity, VoxelmanPropEntry);
	
public:
	int get_entity_data_id() const;
	void set_entity_data_id(const int value);

	int get_level() const;
	void set_level(const int value);

	VoxelmanPropEntity();
	~VoxelmanPropEntity();

protected:
	static void _bind_methods();

private:
	int _level;
	int _entity_data_id;
};

#endif
