import sys
import os
# Constructs asset caches and asset enums by recursively searching through the root folder
#
# Usage (cmd line): python AssetAcquisition.py <root path> <build path> <enum path> -r <resource folder names> -i <ignore folder names>
# root path: The root path
# build path: The location of the build. Asset caches will be output to this folder
# enum path: The output location of the enums
# ignore folder names: The names of folders to ignore when searching for assets
# resource folder paths: Names of resource folders. If not empty, assets not in these folders will not be added

def split_path(a_path):
	"""Splits a path into a list of folder names"""
	a_path = a_path.replace('\\', '/')
	folders = a_path.split('/')

	return folders


def build_asset_set(a_root, a_dir, a_file_formats, a_folder_ignores, resource_folder_names, a_set=None):
	"""Construts a dictionary of assets in a root folder"""

	if a_set == None:
		a_set = []

	# Gets all the files and folders in the dir
	files_and_folders = os.listdir(a_dir)

	for f in files_and_folders:
		full_path = os.path.join(a_dir, f)

		# Recursively search through directories
		if os.path.isdir(full_path) and f not in a_folder_ignores:
			_ = build_asset_set(a_root, full_path, a_file_formats, a_folder_ignores, resource_folder_names, a_set)
		else:
			# Process file
			_, fext = os.path.splitext(f)

			if fext in a_file_formats:

				# Validate whether the asset is inside a resource folder
				# Split the path
				path_split = split_path(full_path_to_relative_path(a_root, full_path))

				# Iterate through each folder that makes up the path
				in_resource_path = False
				for i in range(len(path_split) - 1):
					# Check if it's a resource folder
					if path_split[i] in resource_folder_names:
						in_resource_path = True
						break

				if in_resource_path or len(resource_folder_names) == 0:
					a_set.append(full_path)
					print("Adding asset {0}".format(full_path))
				else:
					print(path_split)
					print(resource_folder_names)

	return a_set

def full_path_to_relative_path(a_root_path, a_full_path):
	return a_full_path.replace(a_root_path, "")

def write_asset_cache(a_cahce_save_path, a_root_path, a_assets):
	"""Writes an asset cache csv file. An asset cache is formatted: id,relative path"""
	print("Writing asset cache to {0}".format(a_cahce_save_path))

	f = open(a_cahce_save_path, "w")
	i = 0

	line = "id,path\n"
	f.write(line)

	for asset in a_assets:
		line = "{0}, {1}\n".format(i, full_path_to_relative_path(a_root_path, asset))
		f.write(line)

		i += 1

	f.close()
		
def write_asset_enum(a_enum_save_path, a_enum_name, a_mck_default_lines, a_assets):
	"""Writes an asset enum (c++)"""
	print("Writing asset enum to {0}".format(a_enum_save_path))

	f = open(a_enum_save_path, "w")
	

	# Header
	f.write("#pragma once\n")
	f.write("enum class {0}".format(a_enum_name) + "\n{\n")

	# Write the defaults
	i = 0
	for line in a_mck_default_lines:
		# Have a comma for all but the final asset
		if i == len(a_mck_default_lines) - 1 and len(a_assets) == 0:
			f.write("    {0}\n".format(line))
		else:
			f.write("    {0},\n".format(line))

		i += 1
	
	# Write seperator
	f.write("    // Negative values are reserved for engine use\n")

	# Write the found assets
	i = 0
	for asset in a_assets:
		asset_name = os.path.basename(asset)
		asset_name, _ = os.path.splitext(asset_name)
		# Have a comma for all but the final asset
		if i == len(a_assets) - 1:
			line = "    {0} = {1}\n".format(asset_name, i)
		else:
			line = "    {0} = {1},\n".format(asset_name, i)
		f.write(line)

		i += 1

	f.write("};")

	f.close()


class AssetType():
	def __init__(self, formats, cache_output, enum_output, enum_name, mck_default_lines):
		self.formats = formats
		self.cache_output = cache_output
		self.enum_output = enum_output
		self.enum_name = enum_name
		self.mck_default_lines = mck_default_lines
	
	def build(self, a_root_path, a_resource_names, a_folder_ignores):
		asset_set = build_asset_set(a_root_path, a_root_path, self.formats, a_folder_ignores, a_resource_names)
		write_asset_cache(self.cache_output, a_root_path, asset_set)
		write_asset_enum(self.enum_output, self.enum_name, self.mck_default_lines, asset_set)

# --- Script ---
# Initialise paths
root_path = sys.argv[1]
build_path = sys.argv[2]
enum_path = sys.argv[3]
resource_names = []
folder_ignores = []

print("Running Asset Acquisiton.... Root={0}".format(root_path))

if not os.path.isdir(build_path):
	os.mkdir(build_path)

if not os.path.isdir(enum_path):
	os.mkdir(enum_path)

# Parse arguments
arg_state = 0

for i in range(4, len(sys.argv)):

	# If reading ignore arguments
	if arg_state == 1:
		folder_ignores.append(sys.argv[i])
	elif arg_state == 2:
		resource_names.append(sys.argv[i])

	# -i indicates ignore
	if sys.argv[i] == "-i":
		arg_state = 1
	if sys.argv[i] == "-r":
		arg_state = 2

# Textures
texture_assets = AssetType(
	formats=[".png", ".jpg", ".jpeg"],
	cache_output=os.path.join(build_path, "textureCache.csv"),
	enum_output=os.path.join(enum_path, "TextureEnum.h"),
	enum_name="TextureEnum",
	mck_default_lines=["__MCK__DEFAULT = -1"]
	)

texture_assets.build(root_path, resource_names, folder_ignores)

# Materials
material_assets = AssetType(
	formats=[".mtl"],
	cache_output=os.path.join(build_path, "materialCache.csv"),
	enum_output=os.path.join(enum_path, "MaterialEnum.h"),
	enum_name="MaterialEnum",
	mck_default_lines=["__MCK__DEFAULT = -1"]
	)

material_assets.build(root_path, resource_names, folder_ignores)

# Meshes
mesh_assets = AssetType(
	formats=[".obj", ".glb", ".gltf"],
	cache_output=os.path.join(build_path, "meshCache.csv"),
	enum_output=os.path.join(enum_path, "MeshEnum.h"),
	enum_name="MeshEnum",
	mck_default_lines=["__MCK_DISPLAY_SCREEN = -2", "__MCK_DEFAULT = -1"]
	)

mesh_assets.build(root_path, resource_names, folder_ignores)

# Shaders
shader_assets = AssetType(
	formats=[".glsl", ".vert", ".frag"],
	cache_output=os.path.join(build_path, "shaderCache.csv"),
	enum_output=os.path.join(enum_path, "ShaderEnum.h"),
	enum_name="ShaderEnum",
	mck_default_lines=["__LIGHT_UNLIT_SHADOWS = -5", "__LIGHT_UNLIT = -4", "__FRAG_MONOCOLOUR = -3", "__MCK_FRAMEBUFFER_DISPLAY = -2", "__MCK_DEFAULT = -1"]
	)

shader_assets.build(root_path, resource_names, folder_ignores)