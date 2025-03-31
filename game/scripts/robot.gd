class_name Robot extends CharacterBody2D


const ROBOT_SCENE: PackedScene = preload("uid://civven5hg8eew") 

@export var resource: SimpleBotResource
var direction: Vector2 = Vector2.DOWN


@export var room_gen: RoomGenerator

var charge_qnt: float = 0.0
var charging: bool = false
var time_to_charge: float = 2

static var quantity: int = 1

static func new_simple_bot(pos: Vector2) -> Robot:
	var new_robot: Robot = ROBOT_SCENE.instantiate()
	new_robot.global_position = pos
	quantity += 1
	print(quantity)
	return new_robot


func _ready() -> void:
	await get_tree().process_frame
	#print(room_gen.get_rooms().size())
	#print(room_gen.rooms)
	#var zoom: float = 1/(room_gen.get_room_size_x() * room_gen.get_room_quantity())
	%Camera.zoom = Vector2(1, 1)
	if not is_instance_valid(room_gen):
		%Camera.enabled = false
		#set_process(false)
		return 
	%Camera.limit_left = int(0)
	%Camera.limit_right = int(max(room_gen.get_room_size_x() * \
	room_gen.get_room_quantity_x() * 16 + 16, get_viewport_rect().size.x * (1/%Camera.zoom.x)))
	#print(get_viewport_rect().size.x * (1/%Camera.zoom.x))
	%Camera.limit_top = int(-room_gen.get_room_size_y() * 16)
	%Camera.limit_bottom = int(room_gen.get_room_size_y() * (room_gen.get_room_quantity_y() - 1) * 16 + 16)

var once: bool = true
var dis: float = 0



func ri_colliding() -> bool:
	return $Right.is_colliding() or $Right2.is_colliding()

func lef_colliding() -> bool:
	return $Esq.is_colliding() or $Esq2.is_colliding()


func _physics_process(delta: float) -> void:
	#direction = Input.get_vector("left", "right", "up", "down")
	
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
	
	
	elif left_colliding() and $Bot.is_colliding():
		self.rotation_degrees -= 90
	
	#elif $Bot.is_colliding() and not $Left.is_colliding():
		#self.rotation_degrees -= 90
	
	
	
	#print(resource.speed)
	
	

	#if Input.is_action_pressed("confirm"):
		#resource.speed += 10
	
	velocity = direction.rotated(rotation).normalized() * resource.speed
	
	move_and_slide()

var with_package: bool = false

func update_progress_charge(_delta: float) -> void:
		charge_qnt += _delta * (%ChargingBar.max_value/time_to_charge)
		charge_qnt = clamp(charge_qnt, %ChargingBar.min_value,%ChargingBar.max_value)
		%ChargingBar.value = charge_qnt


func _process(delta: float) -> void:
	if charging:
		update_progress_charge(delta)
		
	if charge_qnt >= %ChargingBar.max_value and charging:
		%ChargingBar.hide()
		charging = false
		set_physics_process(true)
	
	if discharging:
		update_progress_charge(-delta)
	
	if charge_qnt <= %ChargingBar.min_value and discharging:
		%ChargingBar.hide()
		discharging = false
		set_physics_process(true)
		GameEvents.add_coin.emit(200)
	
	
	if not %Camera.is_enabled():
		return
	
	if Input.is_action_pressed("zoom_in"):
		%Camera.zoom += Vector2.ONE * 2 * delta * %Camera.zoom.length()
	
	if Input.is_action_pressed("zoom_out"):
		%Camera.zoom -= Vector2.ONE * 2 * delta * %Camera.zoom.length()
	
	if Input.is_action_just_pressed("reset"):
		get_tree().reload_current_scene()




func left_colliding() -> bool:
	return not $LeftArea.get_overlapping_bodies().is_empty()
	
	return not (not $Left.is_colliding() and not $Left2.is_colliding() \
	and not $Left3.is_colliding())


func charge() -> void:
	%ChargingBar.show()
	set_physics_process(false)
	charge_qnt = 0.0
	charging = true
	with_package = true

var discharging: bool = false

func discharge() -> void:
	%ChargingBar.show()
	set_physics_process(false)
	discharging = true
	with_package = false


func update_camera_limits(pos: Vector2, size: Vector2) -> void:
	global_position += 16 * global_position.direction_to(pos)
	
	%Camera.limit_bottom = int(pos.y + size.y/2)
	%Camera.limit_top = int(pos.y - size.y/2)
	%Camera.limit_right = int(pos.x + size.x/2)
	%Camera.limit_left = int(pos.x - size.x/2)
