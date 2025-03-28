class_name Weapon extends Node2D

@export var enabled: bool = true
var reload_time: float = 0.5
var actual_reload_time: float = 0


func _physics_process(delta: float) -> void:
	actual_reload_time = max(actual_reload_time - delta, 0)
	if Input.is_action_pressed("left_click")and actual_reload_time <= 0:
		shoot(get_global_mouse_position())


func shoot(enemy_pos: Vector2) -> void:
	if not enabled:
		return
	actual_reload_time = reload_time
	var direction = global_position.direction_to(enemy_pos)
	get_tree().current_scene.add_child(Bullet.new_bullet(direction, global_position))
