#pragma once

#include <room_generator.h>
#include <room.h>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

using namespace godot;

class RobotFill : public CharacterBody2D {
	GDCLASS(RobotFill, CharacterBody2D);

	//export vars
	NodePath room_gen_path;
	float speed = 600.0;
	float reach_offset = 5.0;
	int x = 0;
	int y = 0;

private:
	//ready vars
	RoomGenerator* room_gen = nullptr;
	Array robot_rooms;
	Array rooms_weight;
	Vector2 target_pos;
	Vector2 offset_vector = Vector2(0, 0);
	bool calculated_ready = false;
	void calculate_new_pos();
	void update_weights(Ref<Room> room);
	Vector2i min_weighted_dir(Ref<Room> room);
	void initialize_weight_array();

protected:
	static void _bind_methods();

public:
	virtual void _ready() override;
	virtual void _process(double p_delta) override;

	float get_speed() const { return speed; }
	void set_speed(float new_value) { speed = new_value; }

	float get_reach_offset() const { return reach_offset; }
	void set_reach_offset(float new_value) { reach_offset = new_value; }

	int get_x() const { return x; }
	void set_x(int new_value) { x = new_value; }

	int get_y() const { return y; }
	void set_y(int new_value) { y = new_value; }


	void set_room_gen(const NodePath &room_path) { room_gen_path = room_path; }
	NodePath get_room_gen() const { return room_gen_path; }

	RobotFill();
};
