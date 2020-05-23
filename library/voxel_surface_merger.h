/*
Copyright (c) 2019-2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef VOXEL_SURFACE_MERGER_H
#define VOXEL_SURFACE_MERGER_H

#include "voxel_surface.h"

#include "scene/resources/texture.h"

#include "../../texture_packer/texture_packer.h"

#include "../defines.h"

#if GODOT4
#define Texture Texture2D
#endif

class VoxelSurfaceMerger : public VoxelSurface {
	GDCLASS(VoxelSurfaceMerger, VoxelSurface)

public:
	Ref<AtlasTexture> get_region(const VoxelSurfaceSides side);
	void set_region(const VoxelSurfaceSides side, const Ref<AtlasTexture> &texture);

	Ref<Texture> get_texture(const VoxelSurfaceSides side);
	void set_texture(const VoxelSurfaceSides side, const Ref<Texture> &texture);

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
