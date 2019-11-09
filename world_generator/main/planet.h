#ifndef PLANET_H
#define PLANET_H

#include "core/reference.h"
#include "core/image.h"

#include "../../world/voxel_chunk.h"
#include "biome.h"
#include "../../world/environment_data.h"

class Planet : public Reference {
	GDCLASS(Planet, Reference);
	
public:
	int get_seed();
	void set_seed(int value);

	Vector2 get_level_range();
	void set_level_range(Vector2 value);

	//Environment
	Ref<EnvironmentData> get_environment();
	void set_environment(Ref<EnvironmentData> value);

	Ref<Biome> get_biome(const int index) const;
	void set_biome(const int index, const Ref<Biome> biome);
	void add_biome(const Ref<Biome> biome);
	void remove_biome(const int index);

	int get_biome_count() const;

	void setup();
	void generate_chunk(VoxelChunk *chunk, bool spawn_mobs);
	void generate_chunk_bind(Node *chunk, bool spawn_mobs);
	Ref<Image> generate_map();

	Planet();
	~Planet();

protected:
	static void _bind_methods();

private:
	int _seed;
	Vector2 _level_range;
	Ref<EnvironmentData> _environment;
	Vector<Ref<Biome> > _biomes;
};

#endif
