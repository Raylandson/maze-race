extends Node2D

enum BotType {GDSCRIPT, CPP}

@export var room_gen: RoomGenerator
@export var bot_quantity: int = 700
@export var type: BotType:
	set(value):
		type = value
		var _tex: String = "C++" if value == BotType.CPP else "GDSCRIPT"
		$"../CanvasLayer/Control2/Label".text = _tex
@export var time_to_spawn: float = 0.1
@export var robot_speed: float = 600.0
@export var alternate: bool = false
var robo_quantity: int = 0


func _ready() -> void:
	#await get_tree().create_timer(1).timeout
	#spawn_bots()
	pass


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("confirm"):
		spawn_bots()
		time_to_spawn = 0.02
		#type = BotType.CPP if type == BotType.GDSCRIPT else BotType.GDSCRIPT
	
	if event.is_action_pressed("zoom_out"):
		type = BotType.CPP if type == BotType.GDSCRIPT else BotType.GDSCRIPT
		time_to_spawn = 0
		delete_bots()

func delete_bots() -> void:
	var childs: Array = get_children().duplicate()
	for child in childs:
		child.queue_free()
		robo_quantity -= 1

func spawn_bots() -> void:
	for a in range(bot_quantity):
		if type == BotType.GDSCRIPT:
			create_robot_pito()
			type = BotType.CPP if alternate else type
			#print("c++ robot")
		else:
			create_robot_cpp()
			type = BotType.GDSCRIPT if alternate else type
			#print("gdscript robot")
		robo_quantity += 1
		await get_tree().create_timer(time_to_spawn).timeout


func create_robot_cpp() -> void:
	var r: RobotFill = RobotFill.new()
	r.set_room_gen(room_gen.get_path())
	r.set_x(0)           # Posição inicial x
	r.set_y(0)           # Posição inicial y
	r.set_speed(robot_speed)   # Velocidade
	r.set_reach_offset(5.0)  # Offset de alcance
	var img: Image = Image.create(32, 32, false, Image.FORMAT_RGBA8)
	img.fill(Color.WHITE)  # Escolha a cor que quiser (RED, BLUE, GREEN, etc.)
	var tex: ImageTexture = ImageTexture.create_from_image(img)
	var sprite:Sprite2D = Sprite2D.new()
	sprite.texture = tex
	r.add_child(sprite)
	
	add_child(r)


func create_robot_pito() -> void:
	var r: Robot2 = Robot2.new()
	r.room_gen = room_gen
	r.x = 0           # Posição inicial x
	r.y = 0           # Posição inicial y
	r.speed = robot_speed   # Velocidade
	r.reach_offset = 5.0  # Offset de alcance
	
	add_child(r)
