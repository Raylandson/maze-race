extends Area2D

@export var weapon: Weapon
var current_enemies: Array[Node2D]


func _physics_process(delta: float) -> void:
	
	if not current_enemies.is_empty():
		var valid_enemies: Array[Node2D] = current_enemies.filter(func(item): return is_instance_valid(item))
		if not valid_enemies.is_empty() and weapon.actual_reload_time <= 0:
			weapon.shoot(valid_enemies.pick_random().global_position)
	


func _on_body_entered(body: Node2D) -> void:
	current_enemies.append(body)


func _on_body_exited(body: Node2D) -> void:
	current_enemies.erase(body)
