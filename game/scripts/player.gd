class_name Player extends CharacterBody2D

var direction: Vector2 = Vector2.ZERO
var speed: float = 400


#min sizex 36
#min sizey 20


func _ready() -> void:
	await get_tree().process_frame
	
	
	#var zoom: float = 1/(room_gen.get_room_size_x() * room_gen.get_room_quantity())
	#%Camera.zoom = Vector2(1, 1)
	#%Camera.limit_left = int(0)
	#%Camera.limit_right = int(room_gen.get_room_size_x() * \
	#room_gen.get_room_quantity() * 16 + 16)
	##print(get_viewport_rect().size.x * (1/%Camera.zoom.x))
	#%Camera.limit_top = int(-room_gen.get_room_size_y() * 16)
	#%Camera.limit_bottom = int(room_gen.get_room_size_y() * (room_gen.get_room_quantity() - 1) * 16 + 16)


func _physics_process(delta: float) -> void:
	direction = Input.get_vector("left", "right", "up", "down")
	velocity = direction.normalized() * speed
	
	if Input.is_action_just_pressed("reset"):
		get_tree().reload_current_scene()
	
	
	if Input.is_action_pressed("zoom_in"):
		%Camera.zoom += Vector2.ONE * 2 * delta * %Camera.zoom.length()

	if Input.is_action_pressed("zoom_out"):
		%Camera.zoom -= Vector2.ONE * 2 * delta * %Camera.zoom.length()

	move_and_slide()


func update_camera_limits(pos: Vector2, size: Vector2) -> void:
	global_position += 33 * global_position.direction_to(pos)
	
	%Camera.limit_bottom = int(pos.y + size.y/2)
	%Camera.limit_top = int(pos.y - size.y/2)
	%Camera.limit_right = int(pos.x + size.x/2)
	%Camera.limit_left = int(pos.x - size.x/2)
