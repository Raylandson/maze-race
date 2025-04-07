extends Control

@onready var traces: Array[Line2D] = [$"../../RobotFillLine",
$"../../RobotTremauxLine",$"../../RobotRandomLine",
$"../../RobotDFSLine", $"../../RobotWallLine"]
@onready var lights: Array[PointLight2D] = [
	$"../../RobotWall/PointLight2D2",
	$"../../RobotFill/PointLight2D",
	$"../../RobotTremaux/PointLight2D2",
	$"../../RobotRandom/PointLight2D3",
	$"../../RobotDFS/PointLight2D4"
]

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
		printt(robot.name, GameEvents.reach_offset)
		var reach_offset: float = (GameEvents.robot_speed/3000) * 22
		reach_offset = clamp(reach_offset, 5, 22)
		print(reach_offset)
		%OffsetEdit.value = reach_offset
		GameEvents.reach_offset = reach_offset
		robot.set_reach_offset(GameEvents.reach_offset)
	

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("reset"):
		get_tree().reload_current_scene()
	if event.is_action_pressed("confirm"):
		for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
			robot.set_speed(GameEvents.robot_speed)
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


func _on_speed_edit_value_changed(value: float) -> void:
	GameEvents.robot_speed = value
	for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
		if robot.get_speed() > 0:
			robot.set_speed(value)
			var reach_offset: float = (value/3000) * 22
			reach_offset = clamp(reach_offset, 5, 22)
			print(reach_offset)
			%OffsetEdit.value = reach_offset
			GameEvents.reach_offset = reach_offset
			robot.set_reach_offset(GameEvents.reach_offset)


func _on_yedit_value_changed(value: float) -> void:
	GameEvents.room_quantity_y = int(value)


func _on_xedit_value_changed(value: float) -> void:
	GameEvents.room_quantity_x = int(value)


func _on_offset_edit_value_changed(value: float) -> void:
	GameEvents.reach_offset = value
	for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
		robot.set_reach_offset(GameEvents.reach_offset)


func _on_remove_wall_value_changed(value: float) -> void:
	GameEvents.percent_remove = value


func _on_line_button_pressed() -> void:
	for line in traces:
		line.visible = not line.visible


func _on_light_button_pressed() -> void:
	for light in lights:
		light.visible = not light.visible
