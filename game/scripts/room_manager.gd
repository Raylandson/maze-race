extends Node2D

@export var room_gen: RoomGenerator


func _ready() -> void:
	await get_tree().process_frame
	create_cameras()


func create_cameras() -> void:
	
	for row in room_gen.get_rooms():
		for room: Room in row:
			var room_size_vector: Vector2i = Vector2i(room_gen.get_room_size_x(), \
			room_gen.get_room_size_y()) + Vector2i.ONE
			var new_cam: CameraLimit = CameraLimit.new_camera(room.get_glob_pos(), 
			room_size_vector)
			get_tree().current_scene.add_child(new_cam)
