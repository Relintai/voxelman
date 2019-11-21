# voxelman

A voxel engine module for godot, focusing on editor integration, rapid prototyping, gameplay-related features, speed, and extendability (even from gdscript).

Be warned! API breakages still happen a lot, only use thes module, if that's okay with you!

This module have dependencies to some of my other engine modules, these will be made optional later, for now you will need to install them as well.

For now, my main game project has the implementation of the transvoxel mesher. The cubic mesher should be usable out of the box.

The cubic mesher is just an algorithm that I made up, to have voxel cubes, that can use the actual 3d fill values from the chunk.

Multi threading is really new, it still has a bunch of crashes, for now I don't recommend turning it on (unless you know what you are doing ofc).

The module supports baking lights into vertex colors, it can also merge texture atlases, and meshes.

It has also Props, these are combinations of lights, meshes, and scenes. The addon that can create these these are in a separate repository.

In gles2 the mesh data isn't accessible, so mergeable meshes has to be in a different format (MeshDataResource). They have an import plugin.
You have to set mergeable meshes to this format in godot's import tab.

Note, that this module was created for my game, at the moment only the things that I directly needed (or thought I needed) are implemented!

This will change over time!
