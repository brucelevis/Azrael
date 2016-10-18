-- Generated By protoc-gen-lua Do not Edit
local protobuf = require "protobuf"
local public_pb = require("public_pb")
module('client_scene_pb')


MOVEINFO = protobuf.Descriptor();
MOVEINFO_IETTID_FIELD = protobuf.FieldDescriptor();
MOVEINFO_X_FIELD = protobuf.FieldDescriptor();
MOVEINFO_Y_FIELD = protobuf.FieldDescriptor();
MOVEINFO_SCENEID_FIELD = protobuf.FieldDescriptor();

MOVEINFO_IETTID_FIELD.name = "iEttId"
MOVEINFO_IETTID_FIELD.full_name = ".client_scene.moveInfo.iEttId"
MOVEINFO_IETTID_FIELD.number = 1
MOVEINFO_IETTID_FIELD.index = 0
MOVEINFO_IETTID_FIELD.label = 2
MOVEINFO_IETTID_FIELD.has_default_value = false
MOVEINFO_IETTID_FIELD.default_value = 0
MOVEINFO_IETTID_FIELD.type = 3
MOVEINFO_IETTID_FIELD.cpp_type = 2

MOVEINFO_X_FIELD.name = "x"
MOVEINFO_X_FIELD.full_name = ".client_scene.moveInfo.x"
MOVEINFO_X_FIELD.number = 2
MOVEINFO_X_FIELD.index = 1
MOVEINFO_X_FIELD.label = 2
MOVEINFO_X_FIELD.has_default_value = false
MOVEINFO_X_FIELD.default_value = 0
MOVEINFO_X_FIELD.type = 5
MOVEINFO_X_FIELD.cpp_type = 1

MOVEINFO_Y_FIELD.name = "y"
MOVEINFO_Y_FIELD.full_name = ".client_scene.moveInfo.y"
MOVEINFO_Y_FIELD.number = 3
MOVEINFO_Y_FIELD.index = 2
MOVEINFO_Y_FIELD.label = 2
MOVEINFO_Y_FIELD.has_default_value = false
MOVEINFO_Y_FIELD.default_value = 0
MOVEINFO_Y_FIELD.type = 5
MOVEINFO_Y_FIELD.cpp_type = 1

MOVEINFO_SCENEID_FIELD.name = "sceneId"
MOVEINFO_SCENEID_FIELD.full_name = ".client_scene.moveInfo.sceneId"
MOVEINFO_SCENEID_FIELD.number = 4
MOVEINFO_SCENEID_FIELD.index = 3
MOVEINFO_SCENEID_FIELD.label = 2
MOVEINFO_SCENEID_FIELD.has_default_value = false
MOVEINFO_SCENEID_FIELD.default_value = 0
MOVEINFO_SCENEID_FIELD.type = 5
MOVEINFO_SCENEID_FIELD.cpp_type = 1

MOVEINFO.name = "moveInfo"
MOVEINFO.full_name = ".client_scene.moveInfo"
MOVEINFO.nested_types = {}
MOVEINFO.enum_types = {}
MOVEINFO.fields = {MOVEINFO_IETTID_FIELD, MOVEINFO_X_FIELD, MOVEINFO_Y_FIELD, MOVEINFO_SCENEID_FIELD}
MOVEINFO.is_extendable = false
MOVEINFO.extensions = {}

moveInfo = protobuf.Message(MOVEINFO)

