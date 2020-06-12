

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "WorldArea",

        "VoxelLight",

        "VoxelmanLevelGenerator",
        "VoxelmanLevelGeneratorFlat",

        "VoxelSurfaceMerger",
        "VoxelSurfaceSimple",
        "VoxelSurface",
        "VoxelmanLibraryMerger",
        "VoxelmanLibrarySimple",
        "VoxelmanLibrary",

        "VoxelCubePoints",
        "VoxelMesherCubic",
        "VoxelMeshData",

        "MarchingCubesCellData",
        "VoxelMesherMarchingCubes",

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
        "VoxelMesherLiquidBlocky",

        "VoxelWorldMarchingCubes",
	      "VoxelChunkMarchingCubes",

        "VoxelMesherCubic",
	      "VoxelWorldCubic",

        "VoxelMesherDefault",
	      "VoxelWorldDefault",
	      "VoxelChunkBlocky",
    ]

def get_doc_path():
    return "doc_classes"

