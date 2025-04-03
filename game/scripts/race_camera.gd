extends Camera2D

@export var room_gen: RoomGenerator


func _ready() -> void:
	limit_top = -(room_gen.get_room_size_y() * 15)
	limit_left = 8
	
	print(1/(float(room_gen.get_room_quantity_x())/9))
	zoom = Vector2(1/(float(room_gen.get_room_quantity_x())/9),
	1/(float(room_gen.get_room_quantity_y())/5))

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("reset"):
		get_tree().reload_current_scene()
