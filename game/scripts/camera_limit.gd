class_name CameraLimit extends Area2D

var shape: RectangleShape2D = null
const CAMERA_SCENE: PackedScene = preload('uid://ctdejr8pqexha')
#var room: Room = null
var visited: bool = false


static func new_camera(pos: Vector2i, shape_size: Vector2i) -> CameraLimit:
	var camera: CameraLimit = CAMERA_SCENE.instantiate()
	var collision: CollisionShape2D = CollisionShape2D.new()
	collision.shape = RectangleShape2D.new()
	
	collision.shape.size = shape_size * 16 - Vector2i(10, 10)
	#collision.shape.size.y -= 10ds
	camera.add_child(collision)
	camera.shape = collision.shape
	camera.global_position = pos + (shape_size -
	Vector2i(0, (shape_size.y - 1)*2)) * 16/2
	return camera


func _on_body_entered(body: Node2D) -> void:
	#return
	if body is Player:
		if not is_instance_valid(shape):
			return
		if not visited:
			print('novo lugar visitado')
			visited = true
		
		body.update_camera_limits(global_position, shape.size)
