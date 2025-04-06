#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include <room.h>
#include <room_generator.h>
#include <my_player.h>
#include <robot_fill.h>
#include <robot_tremaux.h>
#include <robot_base.h>
#include <robot_random.h>
#include <robot_dfs.h>
#include <robot_wall.h>

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_RUNTIME_CLASS(Room);
	GDREGISTER_RUNTIME_CLASS(RoomGenerator);
	GDREGISTER_RUNTIME_CLASS(MyPlayer);
	GDREGISTER_RUNTIME_CLASS(RobotBase);
	GDREGISTER_RUNTIME_CLASS(RobotFill);
	GDREGISTER_RUNTIME_CLASS(RobotTremaux);
	GDREGISTER_RUNTIME_CLASS(RobotRandom);
	GDREGISTER_RUNTIME_CLASS(RobotDFS);
	GDREGISTER_RUNTIME_CLASS(RobotWall);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT graphgen_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}