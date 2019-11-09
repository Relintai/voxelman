#ifndef WORLD_GENERATOR_PROP_DATA_H
#define WORLD_GENERATOR_PROP_DATA_H

#include "core/resource.h"

#include "../../props/prop_data.h"

class WorldGeneratorPropData : public Resource {
	GDCLASS(WorldGeneratorPropData, Resource);
	
public:

	bool can_spawn(int seed);
	Ref<PropData> get_prop(int seed);

	WorldGeneratorPropData();
	~WorldGeneratorPropData();

protected:
	static void _bind_methods();

};

#endif
