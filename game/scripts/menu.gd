extends Control

@onready var traces: Array[Line2D] = [$"../../RobotFillLine",
$"../../RobotTremauxLine",$"../../RobotRandomLine",
$"../../RobotDFSLine", $"../../RobotWallLine"]
@onready var lights: Array[PointLight2D] = [
	$"../../RobotWall/PointLight2D2",
	$"../../RobotFill/PointLight2D",
	$"../../RobotTremaux/PointLight2D2",
	$"../../RobotRandom/PointLight2D3",
	$"../../RobotDFS/PointLight2D4",
	$"../../FinalLight"
]

@export var result_container_scene: PackedScene
var race_started: bool = false
var current_time: float = 0.0

func _ready() -> void:
	$"../../RoomGenerator".set_room_quantity_x(GameEvents.room_quantity_x)
	$"../../RoomGenerator".set_room_quantity_y(GameEvents.room_quantity_y)
	
	$"../../RoomGenerator".set_target_x(GameEvents.room_target_x)
	$"../../RoomGenerator".set_target_y(GameEvents.room_target_y)
	
	$"../../RoomGenerator".set_percent_remove_existent(GameEvents.percent_remove)
	
	%Xedit.value = GameEvents.room_quantity_x
	%Yedit.value = GameEvents.room_quantity_y
	
	%TargetY.value = GameEvents.room_target_y
	%TargetX.value = GameEvents.room_target_x
	
	%RemoveWall.value = GameEvents.percent_remove
	%SpeedEdit.value = GameEvents.robot_speed
	%OffsetEdit.value = GameEvents.reach_offset
	
	for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
		printt(robot.name, GameEvents.reach_offset)
		var reach_offset: float = (GameEvents.robot_speed/3000) * 22
		reach_offset = clamp(reach_offset, 5, 22)
		print(reach_offset)
		robot.found_signal.connect(chegou)
		%OffsetEdit.value = reach_offset
		GameEvents.reach_offset = reach_offset
		robot.set_reach_offset(GameEvents.reach_offset)


func _on_hide_button_pressed() -> void:
	for control: Control in %VerticalCon.get_children():
		if control is Button:
			continue
		control.visible = not control.visible


func _process(delta: float) -> void:
	if race_started:
		current_time += delta


func _on_speed_edit_value_changed(value: float) -> void:
	GameEvents.robot_speed = value
	var reach_offset: float = (value/3000) * 22
	reach_offset = clamp(reach_offset, 5, 22)
	%OffsetEdit.value = reach_offset
	GameEvents.reach_offset = reach_offset
	
	for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
		if robot.get_speed() > 0:
			robot.set_speed(value)
		robot.set_reach_offset(GameEvents.reach_offset)


func update_target_value_x(value: float) -> void:
	%TargetX.max_value = value
	#%TargetX.value = value
	#GameEvents.room_target_x = int(value)


func update_target_value_y(value: float) -> void:
	%TargetY.max_value = value
	#%TargetY.value = value
	#GameEvents.room_target_y = int(value)


func _on_yedit_value_changed(value: float) -> void:
	GameEvents.room_quantity_y = int(value)
	update_target_value_y(value)


func _on_xedit_value_changed(value: float) -> void:
	GameEvents.room_quantity_x = int(value)
	update_target_value_x(value)


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


func chegou(robot: RobotBase):
	printt("chegou", robot.name, current_time)
	var result_instance: PanelContainer = result_container_scene.instantiate()
	for child in result_instance.get_children():
		if child is Label:
			child.text = "%s %.3f" % [robot.name, current_time]
			break
	
	%BoxResults.add_child(result_instance)


func _on_start_race_pressed() -> void:
	for robot: RobotBase in get_tree().get_nodes_in_group('robot'):
		robot.set_speed(GameEvents.robot_speed)
		printt(robot.name, robot.speed)
	
	race_started = true


func _on_target_x_value_changed(value: float) -> void:
	GameEvents.room_target_x = int(value)


func _on_target_y_value_changed(value: float) -> void:
	GameEvents.room_target_y = int(value)


func _on_restart_regen_maze_pressed() -> void:
	get_tree().reload_current_scene()
