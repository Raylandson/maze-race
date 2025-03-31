extends Area2D




func _on_body_entered(body: Node2D) -> void:
	if body is Robot and body.with_package:
		#body.with_package = false
		body.discharge()
