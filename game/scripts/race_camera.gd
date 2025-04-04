extends Camera2D

@export var room_gen: RoomGenerator
var speeds: Array[float] = []

func _ready() -> void:
	limit_top = -(room_gen.get_room_size_y() * 15)
	limit_left = 8
	for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
		speeds.append(robot.get_speed())
		robot.set_speed(0)
	
	print(1/(float(room_gen.get_room_quantity_x())/9))
	zoom = Vector2(1/(float(room_gen.get_room_quantity_x())/9),
	1/(float(room_gen.get_room_quantity_y())/5))

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("reset"):
		get_tree().reload_current_scene()
	if event.is_action_pressed("confirm"):
		var idx: int = 0
		for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
			if idx >= speeds.size():
				continue
			robot.set_speed(speeds[idx])
		
