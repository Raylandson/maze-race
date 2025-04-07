#include "robot_fill.h"
//#include <godot_cpp/classes/global_constants.hpp>

void RobotFill::_bind_methods() {
}

void RobotFill::_ready() {
	RobotBase::_ready();
	if (sprite){
		//sprite->set_modulate(Color(1, 1, 0, 1));
		sprite->set_modulate(Color(0.7, 0.85, 1.0, 1)); // azul claro
	}
}

void RobotFill::_process(double p_delta) {
	RobotBase::_process(p_delta);
}

RobotFill::RobotFill() {
}

void RobotFill::calculate_new_pos() {
	if (!room_gen || x < 0 || y < 0 || x >= room_gen->get_room_quantity_x() || y >= room_gen->get_room_quantity_y())
		return;

	if (x == (room_gen->get_target_x() - 1) && y == (room_gen->get_target_y() - 1)) {
		emit_signal("found_signal", this);
		speed = 0.0;
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
			int weight = abs(i - room_gen->get_target_x() + 1) + abs(j - room_gen->get_target_y() + 1);
			weights[i][j] = weight;
		}
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
		int x_destin = room_gen->get_target_x() - 1;
		int y_destin = room_gen->get_target_y() - 1;
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