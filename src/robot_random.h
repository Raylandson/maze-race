#pragma once

#include <room_generator.h>
#include <room.h>
#include <robot_base.h>

using namespace godot;

class RobotRandom : public RobotBase {
	GDCLASS(RobotRandom, RobotBase);
	
	protected:
    static void _bind_methods();
	
	public:
   	virtual void _ready() override;
    virtual void _process(double p_delta) override;
    void calculate_new_pos() override;
	Vector2i pick_random_dir(Ref<Room> room);
	Vector2i previous_dir = Vector2i(-1, -1);

	RobotRandom();
};
	
