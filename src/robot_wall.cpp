#include "robot_wall.h"

void RobotWall::_bind_methods() {
}

RobotWall::RobotWall() :
		current_dir(Vector2i(1, 0)) {
}

void RobotWall::_ready() {
	RobotBase::_ready();
	if (sprite) {
		sprite->set_modulate(Color(0.75, 1.0, 0.85, 1)); // verde menta
	}
}

void RobotWall::_process(double p_delta) {
	RobotBase::_process(p_delta);
}

void RobotWall::calculate_new_pos() {
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
	Array dirs = current_room->get_directions();

	Vector2i current_tile_pos(x, y);
	Array movement_dirs;

	for (int i = 0; i < dirs.size(); ++i) {
		Vector2i neighbor_tile_pos = dirs[i];
		Vector2i movement_dir = neighbor_tile_pos - current_tile_pos;
		movement_dirs.append(movement_dir);
	}

	Vector2i right_dir(current_dir.y, -current_dir.x);
	Vector2i straight_dir = current_dir;
	Vector2i left_dir(-current_dir.y, current_dir.x);
	Vector2i back_dir = -current_dir;

	Array priority_order;
	priority_order.append(right_dir);
	priority_order.append(straight_dir);
	priority_order.append(left_dir);
	priority_order.append(back_dir);

	Vector2i next_dir = current_dir;
	bool found = false;
	for (int i = 0; i < priority_order.size(); ++i) {
		Vector2i dir = priority_order[i];
		for (int j = 0; j < movement_dirs.size(); ++j) {
			Vector2i movement_dir = movement_dirs[j];
			if (movement_dir == dir) {
				next_dir = dir;
				found = true;
				break;
			}
		}
		if (found)
			break;
	}

	current_dir = next_dir;
	x += next_dir.x;
	y += next_dir.y;

	Array new_row = robot_rooms[x];
	Ref<Room> new_room = new_row[y];
	target_pos = Vector2(new_room->get_glob_pos()) + offset_vector;
}