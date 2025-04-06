#include "robot_tremaux.h"

void RobotTremaux::_bind_methods() {
}

RobotTremaux::RobotTremaux() {
}

void RobotTremaux::_ready() {
	RobotBase::_ready();
    if (sprite) {
		sprite->set_modulate(Color(1.0, 0.85, 0.75, 1)); // pessego
    }
}

void RobotTremaux::_process(double p_delta) {
	RobotBase::_process(p_delta);
}

void RobotTremaux::calculate_new_pos() {
	if (!room_gen || x < 0 || y < 0 || x >= room_gen->get_room_quantity_x() || y >= room_gen->get_room_quantity_y())
		return;

	if (x == (room_gen->get_room_quantity_x() - 1) && y == (room_gen->get_room_quantity_y() - 1)) {
		speed = 0.0;
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

void RobotTremaux::initialize_weight_array() {
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

void RobotTremaux::update_weights(Ref<Room> room) {
	Vector2i pos = room->get_tile_pos();
	int current_weight = weights[pos.x][pos.y];

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

}

Vector2i RobotTremaux::min_weighted_dir(Ref<Room> room) {
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