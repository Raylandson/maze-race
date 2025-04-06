#include "robot_random.h"

void RobotRandom::_bind_methods() {
}

RobotRandom::RobotRandom() {
}

void RobotRandom::_ready() {
	RobotBase::_ready();
	if (sprite) {
		sprite->set_modulate(Color(0.9, 0.8, 1.0, 1)); // lavanda
	}
}

void RobotRandom::_process(double p_delta) {
	RobotBase::_process(p_delta);
}

void RobotRandom::calculate_new_pos() {
	if (!room_gen || x < 0 || y < 0 || x >= room_gen->get_room_quantity_x() || y >= room_gen->get_room_quantity_y())
		return;

	if (x == (room_gen->get_room_quantity_x() - 1) && y == (room_gen->get_room_quantity_y() - 1)) {
		speed = 0.0;
		set_process(false);
		return;
	}

	Array row = robot_rooms[x];
	Ref<Room> current_room = row[y];

	Vector2i current_pos = Vector2i(x, y);

	Vector2i next_dir = pick_random_dir(current_room);
	x = next_dir.x;
	y = next_dir.y;

	row = robot_rooms[next_dir.x];
	Ref<Room> next_room = row[next_dir.y];

	Vector2 pos = Vector2(next_room->get_glob_pos()) + offset_vector;
	target_pos = pos;
	previous_dir = current_pos;
}

Vector2i RobotRandom::pick_random_dir(Ref<Room> room) {
	Array dirs = room->get_directions();
	int dir_size = dirs.size();

	if (dir_size == 0) {
		return Vector2i(-1, -1);
	}

	if (dir_size == 1) {
		Vector2i next_dir = dirs[0].operator Vector2i();
		return next_dir;
	}

	Array valid_dirs;

	for (int i = 0; i < dir_size; i++) {
		Vector2i dir = dirs[i].operator Vector2i();
		Array row = robot_rooms[dir.x];
		Ref<Room> next_room = row[dir.y];

		if (previous_dir == next_room->get_tile_pos()) {
			continue;
		}
		valid_dirs.append(dir);
	}

	if (valid_dirs.is_empty()) {
		Vector2i next_dir = dirs[0].operator Vector2i();
		UtilityFunctions::print("Fallback: escolhendo ", next_dir);
		return next_dir;
	}

	int rand_idx = rand() % valid_dirs.size();
	Vector2i next_dir = valid_dirs[rand_idx].operator Vector2i();
	return next_dir;
}