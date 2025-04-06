#pragma once

#include <room_generator.h>
#include <room.h>
#include <robot_base.h>

using namespace godot;

class RobotWall : public RobotBase {
    GDCLASS(RobotWall, RobotBase);

protected:
    static void _bind_methods();
    Vector2i current_dir;

public:
    virtual void _ready() override;
    virtual void _process(double p_delta) override;
    void calculate_new_pos() override;

    RobotWall();
};