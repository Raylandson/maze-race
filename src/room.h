
#pragma once

#include <godot_cpp/classes/node2d.hpp>

using namespace godot;

class Room : public RefCounted {
	GDCLASS(Room, RefCounted);

    Vector2i glob_pos;
    Vector2i tile_pos;
    bool visited = false;
    
    
    
    
    protected:
	static void _bind_methods();
    
    public:
    Array directions;
    Array all_directions;
    bool robot_visited = false;
	//virtual void _physics_process(double p_delta) override;
    void set_glob_pos(Vector2i new_pos){glob_pos = new_pos;}
    Vector2i get_glob_pos() const {return glob_pos;}
    void set_tile_pos(Vector2i new_pos){tile_pos = new_pos;}
    Vector2i get_tile_pos() const {return tile_pos;}
    void set_visited(bool new_value){visited = new_value;}
    bool get_visited() const {return visited;}

    Array get_directions() const {return directions;}
    void set_directions(Array new_dirs) {directions = new_dirs;}

    Array get_all_directions() const {return all_directions;}
    void set_all_directions(Array new_dirs) {all_directions = new_dirs;}

    bool get_robot_visited() const {return robot_visited;}
    void set_robot_visited(bool new_value){ robot_visited = new_value;}
	
    Room();
    Room(Vector2i glob_pos, Vector2i tile_pos);
};