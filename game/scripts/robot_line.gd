extends Line2D

@export var robot: RobotBase

var time_to_update:float = 2
@onready var default_speed: = robot.get_speed()
var distance: float = 0
var update_distance = 16

func _ready() -> void:
	print(GameEvents.room_quantity_x)
	#robot.set_speed(0)


func _process(delta: float) -> void:
	if not is_instance_valid(robot):
		return
	
	distance += robot.get_speed() * delta
	if distance >= update_distance:
		distance = 0
		if(robot.get_speed() <= 0):
			robot.set_speed(default_speed)
		add_point(robot.global_position)
