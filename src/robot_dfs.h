#pragma once

#include <room_generator.h>
#include <room.h>
#include <robot_base.h>
#include <stack>
#include <vector>

using namespace godot;

class RobotDFS : public RobotBase {
    GDCLASS(RobotDFS, RobotBase);

protected:
    static void _bind_methods();
    
    std::stack<Vector2i> path_stack;
    Vector2i previous_pos;
    bool initialized = false;
    bool target_reached = false; // Variável adicionada
    std::vector<std::vector<bool>> visited;

    void initialize_dfs();
    void calculate_next_step();

public:
    virtual void _ready() override;
    virtual void _process(double p_delta) override;
    void calculate_new_pos() override;

    RobotDFS();
    virtual ~RobotDFS() = default;
};