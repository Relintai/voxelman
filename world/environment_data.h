#ifndef ENVIRONMENT_DATA_H
#define ENVIRONMENT_DATA_H

#include "core/color.h"
#include "core/resource.h"
#include "scene/3d/light.h"
#include "scene/3d/world_environment.h"
#include "scene/main/node.h"

class EnvironmentData : public Resource {
	GDCLASS(EnvironmentData, Resource);

public:
	Ref<Environment> get_environment();
	void set_environment(Ref<Environment> value);

	Color get_color(int index);
	void set_color(int index, Color value);
	float get_energy(int index);
	void set_energy(int index, float value);
	float get_indirect_energy(int index);
	void set_indirect_energy(int index, float value);

	void setup(WorldEnvironment *world_environment, DirectionalLight *primary_light, DirectionalLight *secondary_light);
	void setup_bind(Node *world_environment, Node *primary_light, Node *secondary_light);

	EnvironmentData();
	~EnvironmentData();

public:
	enum {
		LIGHT_COUNT = 2,
	};

protected:
	static void _bind_methods();

private:
	Ref<Environment> _environment;

	Color _colors[LIGHT_COUNT];
	float _energies[LIGHT_COUNT];
	float _indirect_energies[LIGHT_COUNT];
};

#endif
