
#include "my_player.h"

#include <godot_cpp/classes/input.hpp>

void MyPlayer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_speed", "speed"), &MyPlayer::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &MyPlayer::get_speed);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
}

void MyPlayer::_physics_process(double p_delta) {
	Input *input = Input::get_singleton();

	direction = input->get_vector("ui_left", "ui_right", "ui_up", "ui_down");

	set_velocity(direction.normalized() * speed);

	move_and_slide();
}

void MyPlayer::set_speed(float p_speed) {
	speed = p_speed;
}

float MyPlayer::get_speed() const {
	return speed;
}

MyPlayer::MyPlayer() {
}
