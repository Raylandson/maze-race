#include "robot_fill.h"
//#include <godot_cpp/classes/global_constants.hpp>

void RobotFill::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_speed", "new_value"), &RobotFill::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &RobotFill::get_speed);

	ClassDB::bind_method(D_METHOD("set_reach_offset", "new_value"), &RobotFill::set_reach_offset);
	ClassDB::bind_method(D_METHOD("get_reach_offset"), &RobotFill::get_reach_offset);

	ClassDB::bind_method(D_METHOD("set_x", "new_value"), &RobotFill::set_x);
	ClassDB::bind_method(D_METHOD("get_x"), &RobotFill::get_x);

	ClassDB::bind_method(D_METHOD("set_y", "new_value"), &RobotFill::set_y);
	ClassDB::bind_method(D_METHOD("get_y"), &RobotFill::get_y);

	ClassDB::bind_method(D_METHOD("set_room_gen", "path"), &RobotFill::set_room_gen);
	ClassDB::bind_method(D_METHOD("get_room_gen"), &RobotFill::get_room_gen);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "room_gen_path"), "set_room_gen", "get_room_gen");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "x"), "set_x", "get_x");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "y"), "set_y", "get_y");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "reach_offset"), "set_reach_offset", "get_reach_offset");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
}

void RobotFill::_ready() {
	if (room_gen_path.is_empty()) {
		UtilityFunctions::print("Erro: room_gen_path esto vazio!");
		return;
	}
	UtilityFunctions::print("get_null: Node em");
	Node *node = get_node_or_null(room_gen_path);
	if (!node) {
		UtilityFunctions::print("Erro: Nenhum no encontrado em ", room_gen_path);
		return;
	}
	UtilityFunctions::print("casntado: Node ");
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
}

void RobotFill::_process(double p_delta) {
	if (get_global_position().distance_to(target_pos) <= reach_offset)
		calculate_new_pos();
	Vector2 dir = get_global_position().direction_to(target_pos);
	Vector2 dir_speeded = dir * Vector2(speed * p_delta, speed * p_delta);

	set_global_position(get_global_position() + dir_speeded);
}

RobotFill::RobotFill() {
}

void RobotFill::calculate_new_pos() {
	if (!room_gen || x < 0 || y < 0 || x >= room_gen->get_room_quantity_x() || y >= room_gen->get_room_quantity_y())
		return;

	if (x == (room_gen->get_room_quantity_x() - 1) && y == (room_gen->get_room_quantity_y() - 1)) {
		set_process(false);
		return;
	}

	Array row = robot_rooms[x];
	Ref<Room> current_room = row[y];
	current_room->set_robot_visited(true);

	update_weights(row[y]);
	Vector2i next_dir = min_weighted_dir(row[y]);

	x = next_dir.x;
	y = next_dir.y;

	row = robot_rooms[next_dir.x];
	Ref<Room> next_room = row[next_dir.y];

	Vector2 pos = Vector2(next_room->get_glob_pos()) + offset_vector;
	target_pos = pos;
}

void RobotFill::initialize_weight_array() {
	if (!room_gen)
		return;
	weights.clear();
	int width = room_gen->get_room_quantity_x();
	int height = room_gen->get_room_quantity_y();

	weights.resize(width);
	for (int i = 0; i < width; i++) {
		weights[i].resize(height, 0);
		for (int j = 0; j < height; j++) {
			int weight = abs(i - room_gen->get_room_quantity_x() + 1) + abs(j - room_gen->get_room_quantity_y() + 1);
			weights[i][j] = weight;
		}
	}
	return;
	weights.resize(width);
	for (int i = 0; i < width; i++) {
		weights[i].resize(height, 0);
	}
}

void RobotFill::update_weights(Ref<Room> room) {
	//GODOT_PRINT_STRING_HPP("UPDATING WEIGHTS\n");
	Array weight_update_list = Array::make(room);

	while (!weight_update_list.is_empty()) {
		Ref<Room> c_room = weight_update_list.pop_front();

		Vector2i min_dir = min_weighted_dir(c_room);

		Array dirs = c_room->get_directions();

		if (!c_room->get_robot_visited())
			dirs = c_room->get_all_directions();

		int min_weight_neighbor = weights[min_dir.x][min_dir.y];

		int new_weight = min_weight_neighbor + 1;

		//when find the destination
		int x_destin = room_gen->get_room_quantity_x() - 1;
		int y_destin = room_gen->get_room_quantity_y() - 1;
		if (x_destin == x && y_destin == y)
			new_weight = 0;

		int *actual_weight = &weights[c_room->get_tile_pos().x][c_room->get_tile_pos().y];

		if (*actual_weight != new_weight) {
			*actual_weight = new_weight;
			//actual_row[c_room->get_tile_pos().y] = new_weight;

			for (int i = 0; i < dirs.size(); i++) {
				Vector2i direc = dirs[i].operator Vector2i();
				Array direc_row = robot_rooms[direc.x];
				Ref<Room> neighbor = direc_row[direc.y];
				weight_update_list.append(neighbor);
			}
		}
	}
}

Vector2i RobotFill::min_weighted_dir(Ref<Room> room) {
	int min_weight = 0;
	Vector2i min_dir;

	Array dirs = room->get_directions();

	if (!room->get_robot_visited())
		dirs = room->get_all_directions();

	for (int i = 0; i < dirs.size(); i++) {
		Vector2i dir = dirs[i].operator Vector2i();

		int current_weight = weights[dir.x][dir.y];

		if (i == 0 || current_weight <= min_weight) {
			min_weight = current_weight;
			min_dir = dir;
		}
	}
	return min_dir;
}