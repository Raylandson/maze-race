extends Node2D

@export var room_gen: RoomGenerator

@export var min_enemy_qnt: int = 2
@export var max_enemy_qnt: int = 5
@export var create_enemies: bool = true
@export var create_camera: bool = true

@export var create_final_area_bool: bool = true
func _ready() -> void:
	await get_tree().process_frame
	create_cameras()
	create_final_area()

func create_final_area() -> void:
	#return
	var roms: Array = room_gen.get_rooms()
	
	var last_room: Room = roms[room_gen.get_room_quantity_x() -1][room_gen.get_room_quantity_y() - 1]
	
	
	#$"../Final_area".global_position = last_room.get_glob_pos()



func create_cameras() -> void:
	return
	for row in room_gen.get_rooms():
		for room: Room in row:
			if not create_camera:
				continue
			var room_size_vector: Vector2i = Vector2i(room_gen.get_room_size_x(), \
			room_gen.get_room_size_y()) + Vector2i.ONE
			var new_cam: CameraLimit = CameraLimit.new_camera(room.get_glob_pos(), 
			room_size_vector)
			get_tree().current_scene.add_child(new_cam)
			
			if not create_enemies:
				continue
			if randf() >= 0.5:
				var qnt: int = randi_range(min_enemy_qnt, max_enemy_qnt)
				
				
				for e in range(qnt):
					var random_pos: Vector2i = Vector2i(
						randi_range(room_gen.get_room_size_x()/2,
						room_gen.get_room_size_x() - 1), 
						randi_range(room_gen.get_room_size_y()/2,
						room_gen.get_room_size_y() - 1))
					create_enemy(room.get_glob_pos() + random_pos * 16)



func create_enemy(glob_pos: Vector2) -> void:
	return
	#get_tree().current_scene.add_child(Enemy.new_enemy(glob_pos))
