extends Control


func _ready() -> void:
	if $"../../RobotGenerator".type == $"../../RobotGenerator".BotType.GDSCRIPT:
		$"../Control2/Label".text = "GDSCRIPT"
	else:
		$"../Control2/Label".text = "C++"


func _process(delta: float) -> void:
	$Label.text = "FPS: %d" % int(Engine.get_frames_per_second())
	$"../Control/Label".text = "Robos: %d" % $"../../RobotGenerator".robo_quantity
