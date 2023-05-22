import bpy
import math
import os
import json
from mathutils import Euler, Quaternion
from math import radians

rootFldr =  "C:/Users/Tobias/source/repos/Mackerel-Engine"
modelPrecursor = ".."
sceneFldr = "C:/Users/Tobias/source/repos/Mackerel-Engine/scenes/lvl1"
templateFldr = "C:/Users/Tobias/source/repos/Mackerel-Engine/entity_templates"
templateExtension = ".sctm"
sceneFilePath = sceneFldr + "/scene.scn"

TRANSLATE_X_MACRO = "$posX"
TRANSLATE_Y_MACRO = "$posY"
TRANSLATE_Z_MACRO = "$posZ"

ROT_X_MACRO = "$rotX"
ROT_Y_MACRO = "$rotY"
ROT_Z_MACRO = "$rotZ"
ROT_W_MACRO = "$rotW"

MODEL_PATH_MACRO = "$modPath"
NAME_MACRO = "$name"

def generate_scene_json(entities):
    sceneDict = {
        "scene":{
            "entities":entities
        }
    }

    #for e in entities:
     #   sceneDict["scene"]["entities"].append(e)
    return sceneDict

def generate_object_macros(blObject, model_path):
    
    trueEuler = blObject.rotation_euler.copy()
    quaternion_rot = trueEuler.to_quaternion()

    object_macros = {
        TRANSLATE_X_MACRO : -blObject.location.x,
        TRANSLATE_Y_MACRO : blObject.location.z,
        TRANSLATE_Z_MACRO : blObject.location.y,
        ROT_X_MACRO : quaternion_rot.x,
        ROT_Y_MACRO : quaternion_rot.y,
        ROT_Z_MACRO : quaternion_rot.z,
        ROT_W_MACRO : quaternion_rot.w,
        MODEL_PATH_MACRO : model_path.replace(rootFldr, modelPrecursor),
        NAME_MACRO : str(blObject.name)
    }
    
    for p in blObject.keys():
        object_macros["$" + p] = blObject[p]
    
    return object_macros

def load_template_as_string(full_path):
    file = open(full_path, "r")
    tmplString = file.read()
    file.close()
    
    return tmplString

def load_templates_in_folder(folder_path):
    templates = {}
    
    # Get all the files in this folder
    files_in_folder = os.listdir(folder_path)
    
    # Extract the templates
    for file_name in files_in_folder:
        
        template_name, extension = os.path.splitext(file_name)
        
        if extension == templateExtension:
            # Found a template
            tmpl = load_template_as_string(os.path.join(folder_path, file_name))
            templates[template_name] = tmpl
    return templates

def load_default_template_as_string(folder_path):
    full_path = os.path.join(folder_path, "default" + templateExtension)
    return load_template_as_string(full_path)

def substitute_template_macros(macros, template):
    """Takes in a template and a dictionary of macros and substitutes all the macros for their value. Returns substituted template """
    for macro in macros:
        template = template.replace(macro, str(macros[macro]))
        
    return template

def retrieve_object_attachment(blObject):
    attachmentData = blObject.data
    
     # If the object has multiple mesh components
    if isinstance(attachmentData, bpy.types.Collection):
        # Only supporting objects with a single mesh for now
        return attachmentData[0]
    else:
        return attachmentData

def retrieve_selected_objects():
    selectedObjects = []
    scene = bpy.context.scene
    
    # Iterate through all objects and ask if they're selected
    for blObj in scene.objects:
        if blObj.select_get():
            # Add selected
            selectedObjects.append(blObj)
    
    return selectedObjects

def export_object(blo, full_path):
    
    bpy.ops.object.select_all(action='DESELECT')
    # Select this object
    blo.select_set(True)
    
    bpy.ops.export_scene.obj(
        filepath=full_path,
        check_existing=False,
        use_selection=True,
        use_triangles=True,
        axis_up='Y',
        axis_forward='Z'
    )

def export_objects_with_instance_map(objectsToExport, folderPath):
    """Exports objects and generates instance map"""
    
    instanceMap = {}
    seenMeshes = {}
    
    # Iterate through each blender instance
    for blObject in objectsToExport:
        # Get the thing that this object holds
        attachment = retrieve_object_attachment(blObject)
        
        # If it's a mesh and it hasn't been seen
        if isinstance(attachment, bpy.types.Mesh) and not attachment.name in seenMeshes:
            
            # Add mesh to seen meshes
            seenMeshes[attachment.name] = blObject
            
            # Get mesh transform
            originalPos = blObject.location.copy()
            originalRot = blObject.rotation_euler.copy()
            
            # Set to identity
            blObject.location = (0.0, 0.0, 0.0)
            blObject.rotation_euler = (0, 0, 0)
            
            # Generate filepath
            filename = blObject.name
            filename.replace(".", "_")
            filename = filename + ".obj"
            
            full_path = folderPath + "/" + filename
            
            # Export
            export_object(blObject, full_path)
            
            # Reset transform
            blObject.location = originalPos.copy()
            blObject.rotation_euler = originalRot.copy()
            
            # Add to instance map
            instanceMap[blObject.name] = full_path
            
        elif isinstance(attachment, bpy.types.Mesh):
            # Set the model file path for this mesh
            # It's the same as the previously seen mesh
            instanceMap[blObject.name] = instanceMap[seenMeshes[attachment.name].name]
            
        else:
            # No file path for non meshes
            instanceMap[blObject.name] = ""
            
    return instanceMap

def generate_instance_json(blObject, export_filepath, default_template, template_map):
    
    # Pick the appropriate template
    chosen_template = default_template
    
    # If the object has defined a template
    if "template" in blObject:
        # Get the template it wants
        object_template_key = blObject["template"]
        
        # Check that it's in the template map
        if object_template_key in template_map:
            chosen_template = template_map[object_template_key]
    
    # Extract and substitue macros
    cur_macros = generate_object_macros(blObject, export_filepath)
    json_string = substitute_template_macros(cur_macros, chosen_template)
    
    # return json
    return json.loads(json_string)

def write_scene_json(instance_map):
    
    templates = load_templates_in_folder(templateFldr)
    defaultTemplate = load_default_template_as_string(templateFldr)
    
    entityList = []
    
    for imap_key in  instance_map:
        blObject = bpy.data.objects[imap_key]
        model_file_path = instance_map[imap_key]
        
        entityJson = generate_instance_json(blObject, model_file_path, defaultTemplate, templates)
        entityList.append(entityJson)
    
    sceneJson = generate_scene_json(entityList)

    sceneFile = open(sceneFilePath, "w")
    
    json.dump(sceneJson, sceneFile, indent=4)
        
    sceneFile.close()
        
    
    


selected_objects = retrieve_selected_objects()
imap = export_objects_with_instance_map(selected_objects, sceneFldr)
write_scene_json(imap)

#bpy.ops.test.open_filebrowser('INVOKE_DEFAULT')
        

            
            
            
            
            