#ifndef VOXEL_WORLD_H
#define VOXEL_WORLD_H

#include "scene/3d/spatial.h"
#include "core/hash_map.h"

#include "../math/vector3i.h"
#include "../library/voxelman_library.h"
#include "voxel_buffer.h"

class VoxelWorld : public Spatial {
	GDCLASS(VoxelWorld, Spatial);

public:
    int get_chunk_size_x() const;
    void set_chunk_size_x(const int value);
    
    int get_chunk_size_y() const;
    void set_chunk_size_y(const int value);
    
    int get_chunk_size_z() const;
    void set_chunk_size_z(const int value);
    
    Ref<VoxelmanLibrary> get_library() const;
    void set_library(const ref<VoxelmanLibrary> library);
    
	NodePath get_player_path();
	void set_player_path(NodePath player_path);
    
   

	VoxelWorld();
	~VoxelWorld();

protected:
	static void _bind_methods();

private:
    Vector3i _chunk_size;
    Ref<VoxelmanLibrary> _library;
    
	HashMap<Vector3i, Ref<VoxelBuffer> > _chunks; 
    Vector<Ref<VoxelBuffer> > _chunks_vector;

	NodePath _player_path;
	Spatial *_player;
};

#endif
