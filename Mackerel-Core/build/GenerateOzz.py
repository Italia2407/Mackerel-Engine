import os
import shutil
import subprocess
import sys



asset_path = sys.argv[1]
asset_name = os.path.splitext(os.path.basename(asset_path))[0]
json_path = os.path.join(sys.argv[1], os.pardir, "{0}.json".format(asset_name))
bat_path = os.path.join(sys.argv[0], os.pardir, "gltf2ozz.exe")
json = "{\"skeleton\" : {\"filename\" : \"" + asset_name +  ".ozz\"},\"animations\" : [{\"clip\" : \"*\",\"filename\" : \"" + asset_name +  "_*.ozz\"}]}"

json_file = open(json_path, "w")
json_file.write(json)
json_file.close()

exe_path = "\"{0}\"".format(os.path.join(bat_path))
file_arg = "--file=\"{0}\"".format(os.path.join(asset_path))
config_arg = "--config_file=\"{0}\"".format(os.path.join(json_path))
cmd = "\"" + exe_path + " " + file_arg + " " + config_arg + "\""
# print(cmd)
os.system(cmd)

files = os.listdir(os.getcwd())
for file_name in files:
    if (os.path.splitext(file_name)[1] == ".ozz"):
        ozz_path = os.path.join(os.getcwd(), file_name)
        shutil.copy(ozz_path, os.path.join(sys.argv[1], os.pardir))
        os.remove(ozz_path)