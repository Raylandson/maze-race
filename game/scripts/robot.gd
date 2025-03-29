extends CharacterBody2D

var direction: Vector2 = Vector2.ZERO
var speed: float = 50

@export var room_gen: RoomGenerator


func _ready() -> void:
	await get_tree().process_frame
	#print(room_gen.get_rooms().size())
	#print(room_gen.rooms)
	#var zoom: float = 1/(room_gen.get_room_size_x() * room_gen.get_room_quantity())
	%Camera.zoom = Vector2(1, 1)
	%Camera.limit_left = int(0)
	%Camera.limit_right = int(max(room_gen.get_room_size_x() * \
	room_gen.get_room_quantity_x() * 16 + 16, get_viewport_rect().size.x * (1/%Camera.zoom.x)))
	#print(get_viewport_rect().size.x * (1/%Camera.zoom.x))
	%Camera.limit_top = int(-room_gen.get_room_size_y() * 16)
	%Camera.limit_bottom = int(room_gen.get_room_size_y() * (room_gen.get_room_quantity_y() - 1) * 16 + 16)

var once: bool = true
var time_giro: float = 24/speed
var dis: float = 0


func ri_colliding() -> bool:
	return $Right.is_colliding() or $Right2.is_colliding()

func lef_colliding() -> bool:
	return $Esq.is_colliding() or $Esq2.is_colliding()


func _physics_process(delta: float) -> void:
	direction = Input.get_vector("left", "right", "up", "down")
	
	direction = Vector2.DOWN
	
	#print($LeftArea.get_overlapping_bodies())
	
	
	#if $Bot.is_colliding() and not lef_colliding():
		#self.rotation_degrees += 90
	#
	#elif $Bot.is_colliding() and lef_colliding():
		#self.rotation_degrees -= 90
	#
	#elif not lef_colliding() and not ri_colliding():
		#self.rotation_degrees += 90
	
	
	
	#var none_left: bool = false
	#
	#for ray in [$Left, $Left2, $Left3]:
		#if ray.is_colliding():
			#none_left = false
			#break
		#none_left = true
	#
		#
	#time_giro -= delta
	if not left_colliding():
		#print(222)
		self.rotation_degrees += 90
	
	
	elif $Bot.is_colliding() and not $Left.is_colliding():
		#for a in range(20):
			#print('caso2')
		self.rotation_degrees -= 90
		dis = 0
	
	elif left_colliding() and not $Bot.is_colliding():
		#for a in range(20):
			#print('caso3')
		#direction = Vector2.DOWN
		dis = 0
	
	elif left_colliding() and $Bot.is_colliding():
		self.rotation_degrees -= 90
		dis = 0
	
	#elif not left_colliding() :
		##for a in range(20):
			##print('caso5')
		##print(1111)
		#dis += delta * speed
		#time_giro -= delta
	
	
	
	if Input.is_action_pressed("zoom_in"):
		%Camera.zoom += Vector2.ONE * 2 * delta * %Camera.zoom.length()

	if Input.is_action_pressed("zoom_out"):
		%Camera.zoom -= Vector2.ONE * 2 * delta * %Camera.zoom.length()

	if Input.is_action_pressed("confirm"):
		speed += 10
	
	velocity = direction.rotated(rotation).normalized() * speed
	
	if Input.is_action_just_pressed("reset"):
		get_tree().reload_current_scene()
	move_and_slide()


func left_colliding() -> bool:
	return not $LeftArea.get_overlapping_bodies().is_empty()
	
	return not (not $Left.is_colliding() and not $Left2.is_colliding() \
	and not $Left3.is_colliding())
	


func update_camera_limits(pos: Vector2, size: Vector2) -> void:
	global_position += 16 * global_position.direction_to(pos)
	
	%Camera.limit_bottom = int(pos.y + size.y/2)
	%Camera.limit_top = int(pos.y - size.y/2)
	%Camera.limit_right = int(pos.x + size.x/2)
	%Camera.limit_left = int(pos.x - size.x/2)
