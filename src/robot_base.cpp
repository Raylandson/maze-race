#include "robot_base.h"
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

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "room_gen_path"), "set_room_gen", "get_room_gen");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "x"), "set_x", "get_x");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "y"), "set_y", "get_y");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "reach_offset"), "set_reach_offset", "get_reach_offset");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
}

RobotBase::RobotBase() {
}

void RobotBase::_ready() {
	//initialize_weight_array();
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

	if (!room_gen_path.is_empty()) {
		room_gen = get_node<RoomGenerator>(room_gen_path); 
		if (!room_gen) {
			return;
		}
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

void RobotBase::_process(double p_delta) {
	if (get_global_position().distance_to(target_pos) <= reach_offset)
		calculate_new_pos();
	Vector2 dir = get_global_position().direction_to(target_pos);
	Vector2 dir_speeded = dir * Vector2(speed * p_delta, speed * p_delta);

	set_global_position(get_global_position() + dir_speeded);
}

void RobotBase::calculate_new_pos() {
	if (!room_gen || x < 0 || y < 0 || x >= room_gen->get_room_quantity_x() || y >= room_gen->get_room_quantity_y())
		return;

	if (x == (room_gen->get_room_quantity_x() - 1) && y == (room_gen->get_room_quantity_y() - 1)) {
		set_process(false);
		return;
	}

	Array row = robot_rooms[x];
	Ref<Room> current_room = row[y];

	update_weights(row[y]);
	Vector2i next_dir = min_weighted_dir(row[y]);
	x = next_dir.x;
	y = next_dir.y;

	row = robot_rooms[next_dir.x];
	Ref<Room> next_room = row[next_dir.y];

	Vector2 pos = Vector2(next_room->get_glob_pos()) + offset_vector;
	target_pos = pos;
}

void RobotBase::initialize_weight_array() {
	if (!room_gen)
		return;
	weights.clear();
	int width = room_gen->get_room_quantity_x();
	int height = room_gen->get_room_quantity_y();

	weights.resize(width);
	for (int i = 0; i < width; i++) {
		weights[i].resize(height, 0);
	}
	return;
}

void RobotBase::update_weights(Ref<Room> room) {
	Vector2i pos = room->get_tile_pos();
	int current_weight = weights[pos.x][pos.y];
	UtilityFunctions::print("weight before in (", pos.x, ",", pos.y, "): ", current_weight);

	Array dirs = room->get_directions();
	int dir_size = dirs.size();

	// Contar quantos vizinhos sao fins (INT_MAX)
	int dead_end_count = 0;
	for (int i = 0; i < dir_size; i++) {
		Vector2i dir = dirs[i].operator Vector2i();
		if (dir.x >= 0 && dir.x < weights.size() && dir.y >= 0 && dir.y < weights[0].size()) {
			if (weights[dir.x][dir.y] == INT_MAX) {
				dead_end_count++;
			}
		}
	}

	if (dir_size == 1) {
		weights[pos.x][pos.y] = INT_MAX;
	} else if (dir_size == 2) {
		if (dead_end_count == 1) {
			weights[pos.x][pos.y] = INT_MAX;
		} else {
			weights[pos.x][pos.y] = current_weight + 1;
		}
	} else if (dir_size >= 3) {
		if (dead_end_count >= dir_size - 1) {
			weights[pos.x][pos.y] = INT_MAX;
		} else {
			weights[pos.x][pos.y] = current_weight + 1;
		}
	}

	UtilityFunctions::print("weight after in (", pos.x, ",", pos.y, "): ", weights[pos.x][pos.y]);
}

Vector2i RobotBase::min_weighted_dir(Ref<Room> room) {
	int min_weight = INT_MAX;
	Vector2i min_dir;

	Array dirs = room->get_directions();

	for (int i = 0; i < dirs.size(); i++) {
		Vector2i dir = dirs[i].operator Vector2i();

		int current_weight = weights[dir.x][dir.y];
		if (current_weight < min_weight && current_weight < INT_MAX - 1) {
			min_weight = current_weight;
			min_dir = dir;
		}
	}
	return min_dir;
}