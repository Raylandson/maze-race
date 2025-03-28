
#pragma once

#include <godot_cpp/classes/node2d.hpp>

using namespace godot;

class Room : public RefCounted {
	GDCLASS(Room, RefCounted);
    Vector2i glob_pos;
    Vector2i tile_pos;
    bool visited = false;
    Array directions;
    int64_t a = 24;


protected:
	static void _bind_methods();

public:
	//virtual void _physics_process(double p_delta) override;
    void set_glob_pos(Vector2i new_pos){glob_pos = new_pos;}
    Vector2i get_glob_pos() const {return glob_pos;}
    void set_tile_pos(Vector2i new_pos){tile_pos = new_pos;}
    Vector2i get_tile_pos() const {return tile_pos;}
    void set_visited(bool new_value){visited = new_value;}
    bool get_visited() const {return visited;}

	Room();
    Room(Vector2i glob_pos, Vector2i tile_pos);
};