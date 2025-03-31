extends Area2D




func _on_body_entered(body: Node2D) -> void:
	if body is Robot and !body.with_package:
		#body.set_physics_process(false)
		body.charge()
		#get_tree().create_timer(1).timeout.connect(func():
			#body.set_physics_process(true))
