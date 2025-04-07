#include "robot_dfs.h"

void RobotDFS::_bind_methods() {
}

RobotDFS::RobotDFS() : target_reached(false) { 
}

void RobotDFS::_ready() {
    RobotBase::_ready();
    if (sprite) {
        //sprite->get_texture()->get_image()->fill(Color(1, 0, 1, 1)); // Magenta
        sprite->set_modulate(Color(1.0, 1.0, 0.75, 1)); // amarelo pastel
    }
    previous_pos = Vector2i(x, y);
    target_reached = false; 
}

void RobotDFS::_process(double delta) {
    RobotBase::_process(delta);
}

void RobotDFS::calculate_new_pos() {
    if (!room_gen || x < 0 || y < 0 || x >= room_gen->get_room_quantity_x() || y >= room_gen->get_room_quantity_y()) {
        return;
    }

    if (x == room_gen->get_target_x() - 1 && y == room_gen->get_target_y() - 1) {
        emit_signal("found_signal", this);
        speed = 0;
        set_process(false);
        target_reached = true;
        return;
    }

    if (target_reached) {
        return;
    }

    if (!initialized) {
        initialize_dfs();
        initialized = true;
        return;
    }

    if (get_global_position().distance_to(target_pos) <= reach_offset) {
        calculate_next_step();
    }
}

void RobotDFS::initialize_dfs() {
    while (!path_stack.empty()) {
        path_stack.pop();
    }

    visited.clear();
    visited.resize(room_gen->get_room_quantity_x());
    for (int i = 0; i < room_gen->get_room_quantity_x(); i++) {
        visited[i].resize(room_gen->get_room_quantity_y(), false);
    }

    path_stack.push(Vector2i(x, y));
    visited[x][y] = true;
    
    calculate_next_step();
}

void RobotDFS::calculate_next_step() {
    if (path_stack.empty()) return;

    Vector2i current_pos = path_stack.top();
    
    if (current_pos.x == room_gen->get_target_x() - 1 && 
        current_pos.y == room_gen->get_target_y() - 1) {
        target_reached = true;
        set_process(false);
        speed = 0;
        return;
    }

    Array row = robot_rooms[current_pos.x];
    Ref<Room> current_room = row[current_pos.y];
    Array dirs = current_room->get_directions();

    for (int i = 0; i < dirs.size(); i++) {
        Vector2i dir = dirs[i];
        if (dir.x >= 0 && dir.x < room_gen->get_room_quantity_x() &&
            dir.y >= 0 && dir.y < room_gen->get_room_quantity_y() &&
            !visited[dir.x][dir.y]) {
            
            visited[dir.x][dir.y] = true;
            path_stack.push(dir);
            
            Array new_row = robot_rooms[dir.x];
            Ref<Room> new_room = new_row[dir.y];
            target_pos = Vector2(new_room->get_glob_pos()) + offset_vector;
            
            x = dir.x;
            y = dir.y;
            return;
        }
    }

    if (!path_stack.empty()) {
        path_stack.pop();
        if (!path_stack.empty()) {
            Vector2i prev_pos = path_stack.top();
            Array prev_row = robot_rooms[prev_pos.x];
            Ref<Room> prev_room = prev_row[prev_pos.y];
            target_pos = Vector2(prev_room->get_glob_pos()) + offset_vector;
            
            x = prev_pos.x;
            y = prev_pos.y;
        }
    }
}