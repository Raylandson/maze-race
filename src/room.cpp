
#include "room.h"

#include <godot_cpp/classes/input.hpp>

void Room::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_glob_pos", "new_pos"), &Room::set_glob_pos);
    ClassDB::bind_method(D_METHOD("get_glob_pos"), &Room::get_glob_pos);
    ClassDB::bind_method(D_METHOD("set_tile_pos", "new_pos"), &Room::set_tile_pos);
    ClassDB::bind_method(D_METHOD("get_tile_pos"), &Room::get_tile_pos);
    ClassDB::bind_method(D_METHOD("set_visited", "new_value"), &Room::set_visited);
    ClassDB::bind_method(D_METHOD("get_visited"), &Room::get_visited);
    ClassDB::bind_method(D_METHOD("get_directions"), &Room::get_directions);
    ClassDB::bind_method(D_METHOD("set_directions", "new_dirs"), &Room::set_directions);

    ClassDB::bind_method(D_METHOD("get_all_directions"), &Room::get_all_directions);
    ClassDB::bind_method(D_METHOD("set_all_directions", "new_dirs"), &Room::set_all_directions);
    ClassDB::bind_method(D_METHOD("get_robot_visited"), &Room::get_robot_visited);
    ClassDB::bind_method(D_METHOD("set_robot_visited", "new_value"), &Room::set_robot_visited);

}

Room::Room() {
}

Room::Room(Vector2i glob_pos, Vector2i tile_pos){
    set_glob_pos(glob_pos);
    set_tile_pos(tile_pos);
    //this.glob_pos = glob_pos; aiaaiai
}
