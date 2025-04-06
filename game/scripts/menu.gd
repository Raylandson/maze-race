extends Control


func _ready() -> void:
	$"../../RoomGenerator".set_room_quantity_x(GameEvents.room_quantity_x)
	$"../../RoomGenerator".set_room_quantity_y(GameEvents.room_quantity_y)
	$"../../RoomGenerator".set_percent_remove_existent(GameEvents.percent_remove)
	%Xedit.value = GameEvents.room_quantity_x
	%Yedit.value = GameEvents.room_quantity_y
	%RemoveWall.value = GameEvents.percent_remove
	%SpeedEdit.value = GameEvents.robot_speed
	%OffsetEdit.value = GameEvents.reach_offset
	
	for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
		robot.set_reach_offset(GameEvents.reach_offset)
		printt(robot.name, GameEvents.reach_offset)
	

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("reset"):
		get_tree().reload_current_scene()
	if event.is_action_pressed("confirm"):
		for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
				#get_tree().create_timer(0.5).timeout.connect(func():
					#robot.set_speed(GameEvents.robot_speed)
					#robot.set_speed(GameEvents.robot_speed))
				#continue
			
			robot.set_speed(GameEvents.robot_speed)
			#if robot is RobotDFS:
				#robot.set_speed(700)
			printt(robot.name, robot.speed)


func _on_hide_button_pressed() -> void:
	for control: Control in %VerticalCon.get_children():
		if control is Button:
			continue
		control.visible = not control.visible


var time_to_print: float = 1.0

func _process(delta: float) -> void:
	time_to_print -= delta
	if time_to_print > 0:
		return
	for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
		printt(robot.name, robot.speed)
		time_to_print = 1.0


#func update_robots() -> void:
	#for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
		#robot


func _on_speed_edit_value_changed(value: float) -> void:
	GameEvents.robot_speed = value


func _on_yedit_value_changed(value: float) -> void:
	GameEvents.room_quantity_y = int(value)


func _on_xedit_value_changed(value: float) -> void:
	GameEvents.room_quantity_x = int(value)


func _on_offset_edit_value_changed(value: float) -> void:
	GameEvents.reach_offset = value


func _on_remove_wall_value_changed(value: float) -> void:
	GameEvents.percent_remove = value
