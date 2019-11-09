#ifndef VOXEL_SURFACE_MERGER_H
#define VOXEL_SURFACE_MERGER_H

#include "voxel_surface.h"

#include "scene/resources/texture.h"

#include "../../texture_packer/texture_packer.h"

class VoxelSurfaceMerger : public VoxelSurface {
	GDCLASS(VoxelSurfaceMerger, VoxelSurface)

public:
	Ref<AtlasTexture> get_region(const VoxelSurfaceSides side);
	void set_region(const VoxelSurfaceSides side, Ref<AtlasTexture> texture);

	Ref<Texture> get_texture(const VoxelSurfaceSides side);
	void set_texture(const VoxelSurfaceSides side, Ref<Texture> texture);

	void refresh_rects();

	VoxelSurfaceMerger();
	~VoxelSurfaceMerger();

protected:
	static void _bind_methods();

private:
	Ref<AtlasTexture> _regions[VOXEL_SIDES_COUNT];
	Ref<Texture> _textures[VOXEL_SIDES_COUNT];
};

#endif
