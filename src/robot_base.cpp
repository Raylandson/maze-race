#include "robot_base.h"
#include <godot_cpp/variant/utility_functions.hpp>

//#include <godot_cpp/classes/global_constants.hpp>

void RobotBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_speed", "new_value"), &RobotBase::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &RobotBase::get_speed);

	ClassDB::bind_method(D_METHOD("set_reach_offset", "new_value"), &RobotBase::set_reach_offset);
	ClassDB::bind_method(D_METHOD("get_reach_offset"), &RobotBase::get_reach_offset);

	ClassDB::bind_method(D_METHOD("set_x", "new_value"), &RobotBase::set_x);
	ClassDB::bind_method(D_METHOD("get_x"), &RobotBase::get_x);

	ClassDB::bind_method(D_METHOD("set_y", "new_value"), &RobotBase::set_y);
	ClassDB::bind_method(D_METHOD("get_y"), &RobotBase::get_y);

	ClassDB::bind_method(D_METHOD("set_room_gen", "path"), &RobotBase::set_room_gen);
	ClassDB::bind_method(D_METHOD("get_room_gen"), &RobotBase::get_room_gen);

	ClassDB::add_signal("RobotBase", MethodInfo("found_signal", PropertyInfo(Variant::OBJECT, 
		"sender", PROPERTY_HINT_RESOURCE_TYPE, "RobotBase")));

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "room_gen_path"), "set_room_gen", "get_room_gen");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "x"), "set_x", "get_x");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "y"), "set_y", "get_y");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "reach_offset"), "set_reach_offset", "get_reach_offset");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
}

RobotBase::RobotBase() {
}

void RobotBase::_ready() {
	if (room_gen_path.is_empty()) {
		UtilityFunctions::print("Erro: room_gen_path esto vazio!");
		return;
	}
	Node *node = get_node_or_null(room_gen_path);
	if (!node) {
		UtilityFunctions::print("Erro: Nenhum no encontrado em ", room_gen_path);
		return;
	}
	room_gen = Object::cast_to<RoomGenerator>(node);

	if (!room_gen) {
		UtilityFunctions::print("Erro: Node em ", room_gen_path, " nao eh um RoomGenerator!");
		return;
	}

	int offset = room_gen->get_room_size_x() * 8;

	offset_vector = Vector2(8, 8) + Vector2(offset, -offset);

	robot_rooms = room_gen->get_rooms().duplicate();

	Array row_pos = robot_rooms[x];
	Ref<Room> first_room = row_pos[y];
	Vector2 start_pos = Vector2(first_room->get_glob_pos()) + offset_vector;

	set_global_position(start_pos);
	initialize_weight_array();
	calculate_new_pos();

	Ref<Image> img = Image::create(16, 16, false, Image::FORMAT_RGBA8);
    img->fill(Color(1, 0.5, 0.5, 1));
    Ref<ImageTexture> tex = ImageTexture::create_from_image(img);
	sprite = memnew(Sprite2D);
    sprite->set_texture(tex);
    sprite->set_name("Sprite2D");
    add_child(sprite);
}

void RobotBase::_process(double p_delta) {
	if (get_global_position().distance_to(target_pos) <= reach_offset)
		calculate_new_pos();
	Vector2 dir = get_global_position().direction_to(target_pos);
	Vector2 dir_speeded = dir * Vector2(speed * p_delta, speed * p_delta);

	set_global_position(get_global_position() + dir_speeded);
}

void RobotBase::calculate_new_pos() {
}

void RobotBase::initialize_weight_array() {
}

void RobotBase::update_weights(Ref<Room> room) {

}

Vector2i RobotBase::min_weighted_dir(Ref<Room> room) {
	return Vector2i(0, 0);
}