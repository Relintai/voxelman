#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "core/resource.h"
#include "core/vector.h"

#include "data/planet_data.h"

#include "main/planet.h"


class WorldGenerator : public Resource {
	GDCLASS(WorldGenerator, Resource);
	
public:
	Ref<PlanetData> get_planet_data(const int index) const;
	void set_planet_data(const int index, const Ref<PlanetData> planet_data);
	void add_planet_data(const Ref<PlanetData> planet_data);
	void remove_planet_data(const int index);

	int get_planet_data_count() const;

	Vector<Variant> get_planet_datas();
	void set_planet_datas(const Vector<Variant> &planet_datas);

	Ref<Planet> setup_planet(int seed);

	WorldGenerator();
	~WorldGenerator();

protected:
	static void _bind_methods();

private:

	Vector<Ref<PlanetData> > _planet_datas;
};

#endif
