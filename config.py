

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "WorldArea",

        "VoxelLight",

        "VoxelmanLevelGenerator",

        "VoxelSurfaceMerger",
        "VoxelSurfaceSimple",
        "VoxelSurface",
        "VoxelmanLibraryMerger",
        "VoxelmanLibrarySimple",
        "VoxelmanLibrary",

        "VoxelCubePoints",
        "VoxelMesherCubic",
        "TransvoxelCellData",
        "VoxelMeshData",
        "VoxelMesherTransvoxel",
        "VoxelMesher",

        "EnvironmentData",
        "VoxelChunkPropData",
        "VoxelChunk",
        "VoxelChunkDefault",
        "VoxelStructure",
        "BlockVoxelStructure",
        "VoxelWorld",

        "VoxelMesherBlocky",
	    "VoxelWorldBlocky",
	    "VoxelChunkBlocky",
    ]

def get_doc_path():
    return "doc_classes"

