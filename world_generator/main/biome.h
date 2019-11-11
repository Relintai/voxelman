#ifndef BIOME_H
#define BIOME_H

#include "core/reference.h"

#include "../../world/voxel_chunk.h"
#include "../data/world_generator_prop_data.h"
#include "dungeon.h"
#include "../../world/environment_data.h"
#include "../../../entity_spell_system/entities/data/entity_data.h"

#include "../data/biome_data.h"
#include "../../library/voxelman_library.h"

class BiomeData;

class Biome : public Reference {
	GDCLASS(Biome, Reference);
	
public:
	int get_current_seed();
	void set_current_seed(int value);

	Vector2 get_level_range();
	void set_level_range(Vector2 value);

	//Environment
	Ref<EnvironmentData> get_environment();
	void set_environment(Ref<EnvironmentData> value);

	Ref<BiomeData> get_data();
	void set_data(Ref<BiomeData> value);

	//WorldGeneratorPropData
	Ref<WorldGeneratorPropData> get_prop_data(const int index) const;
	void set_prop_data(const int index, const Ref<WorldGeneratorPropData> prop_data);
	void add_prop_data(const Ref<WorldGeneratorPropData> prop_data);
	void remove_prop_data(const int index);

	int get_prop_data_count() const;

	//Entities
	Ref<EntityData> get_entity_data(const int index) const;
	void set_entity_data(const int index, const Ref<EntityData> entity_data);
	void add_entity_data(const Ref<EntityData> entity_data);
	void remove_entity_data(const int index);
	int get_entity_data_count() const;

	//Dungeons
	Ref<Dungeon> get_dungeon(const int index) const;
	void set_dungeon(const int index, const Ref<Dungeon> dungeon);
	void add_dungeon(const Ref<Dungeon> dungeon);
	void remove_dungeon(const int index);
	int get_dungeon_count() const;

	void generate_chunk(VoxelChunk *chunk, bool spawn_mobs);
	void generate_chunk_bind(Node *chunk, bool spawn_mobs);
	void generate_stack(VoxelChunk *chunk, int x, int z, bool spawn_mobs);
	void generate_stack_bind(Node *chunk, int x, int z, bool spawn_mobs);

	void setup();
	void setup_library(Ref<VoxelmanLibrary> library);
	void _setup_library(Ref<VoxelmanLibrary> library);

	Biome();
	~Biome();

protected:
	static void _bind_methods();

private:
	int _current_seed;

	Vector2 _level_range;

	Ref<EnvironmentData> _environment;
	Ref<BiomeData> _data;
	Vector<Ref<WorldGeneratorPropData> > _prop_datas;
	Vector<Ref<EntityData> > _entity_datas;
	Vector<Ref<Dungeon> > _dungeons;
};

#endif
