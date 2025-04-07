
#pragma once

#include <room.h>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class RoomGenerator : public Node2D {
	GDCLASS(RoomGenerator, Node2D);

private:
	NodePath player_path;
	NodePath tile_map_path;
	Ref<PackedScene> camera_limit_scene;

	int room_size_x = 7;
	int room_size_y = 7;
	int room_quantity_x = 15;
	int room_quantity_y = 15;
	int target_x = 0;
	int target_y = 0;
	bool remove_entire_wall = false;
	float percent_remove_existent = 0.0;
	Array rooms;

protected:
	static void _bind_methods();

public:
	virtual void _ready() override;
	RoomGenerator();
	void generate_rooms();
	void create_maze();
	void remove_wall(Vector2i pos, Vector2i dir);
	void create_one_room(Vector2i glob_pos, Vector2i tile_pos);

	void set_player(const NodePath &p_path) { player_path = p_path; }
	NodePath get_player() const { return player_path; }

	void set_tile_map(const NodePath &p_path) { tile_map_path = p_path; }
	NodePath get_tile_map() const { return tile_map_path; }

	void set_room_size_x(int p_size) { room_size_x = p_size; }
	int get_room_size_x() const { return room_size_x; }

	void set_room_size_y(int p_size) { room_size_y = p_size; }
	int get_room_size_y() const { return room_size_y; }
	
	void set_target_x(int n_value) { target_x = n_value; }
	int get_target_x() const { return target_x; }
	
	void set_target_y(int n_value) { target_y = n_value; }
	int get_target_y() const { return target_y; }

	void set_room_quantity_x(int p_quantity) { room_quantity_x = p_quantity; }
	int get_room_quantity_x() const { return room_quantity_x; }
	
	void set_room_quantity_y(int p_quantity) { room_quantity_y = p_quantity; }
	int get_room_quantity_y() const { return room_quantity_y; }
	
	void set_percent_remove_existent(float n_value) { percent_remove_existent = n_value; }
	float get_percent_remove_existent() const { return percent_remove_existent; }

	void set_remove_entire_wall(bool new_value) { remove_entire_wall = new_value; }
	bool get_remove_entire_wall() const { return remove_entire_wall; }

	Array get_rooms() const { return rooms; }
};