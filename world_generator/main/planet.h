#ifndef PLANET_H
#define PLANET_H

#include "core/reference.h"
#include "core/image.h"

#include "../../world/voxel_chunk.h"
#include "biome.h"
#include "dungeon.h"
#include "../../world/environment_data.h"

#include "../data/planet_data.h"
#include "../../library/voxel_surface.h"
#include "../../library/voxelman_library.h"

class PlanetData;

class Planet : public Reference {
	GDCLASS(Planet, Reference);
	
public:
	int get_current_seed();
	void set_current_seed(int value);

	Vector2 get_level_range();
	void set_level_range(Vector2 value);

	//Environment
	Ref<EnvironmentData> get_environment();
	void set_environment(Ref<EnvironmentData> value);

	Ref<PlanetData> get_data();
	void set_data(Ref<PlanetData> value);

	//Biomes
	Ref<Biome> get_biome(const int index) const;
	void set_biome(const int index, const Ref<Biome> biome);
	void add_biome(const Ref<Biome> biome);
	void remove_biome(const int index);
	int get_biome_count() const;

	//Dungeons
	Ref<Dungeon> get_dungeon(const int index) const;
	void set_dungeon(const int index, const Ref<Dungeon> dungeon);
	void add_dungeon(const Ref<Dungeon> dungeon);
	void remove_dungeon(const int index);
	int get_dungeon_count() const;

	void setup();
	void setup_library(Ref<VoxelmanLibrary> library);
	void _setup_library(Ref<VoxelmanLibrary> library);

	void generate_chunk(VoxelChunk *chunk, bool spawn_mobs);
	void generate_chunk_bind(Node *chunk, bool spawn_mobs);
	Ref<Image> generate_map();

	Planet();
	~Planet();

protected:
	static void _bind_methods();

private:
	int _current_seed;
	Vector2 _level_range;
	Ref<EnvironmentData> _environment;
	Ref<PlanetData> _data;
	Vector<Ref<Biome> > _biomes;
	Vector<Ref<Dungeon> > _dungeons;
};

#endif
