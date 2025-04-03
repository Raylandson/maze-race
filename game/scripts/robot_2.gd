class_name Robot2 extends CharacterBody2D


@export var room_gen :RoomGenerator
@export var speed: float = 600
@export var reach_offset: float = 5
@export var x: int = 0
@export var y: int = 0

var offset_vector: Vector2
var rooms_weight: Array
@onready var current_pos: Vector2 = global_position
@onready var robot_rooms: Array = room_gen.get_rooms().duplicate()


func _ready() -> void:
	var offset:int = (room_gen.get_room_size_x()) * 8
	offset_vector = Vector2(8, 8) +Vector2(offset, -offset)
	
	var pos: Vector2 = Vector2(robot_rooms[x][y].get_glob_pos()) + offset_vector
	global_position = pos
	
	for a in range(room_gen.get_room_quantity_x()):
		var row: Array[int] = []
		for b in range(room_gen.get_room_quantity_y()):
			var weight: int = abs(a - room_gen.get_room_quantity_x() + 1 ) + abs(
				b - room_gen.get_room_quantity_y() + 1
			)
			row.append(weight)
		rooms_weight.append(row)
	
	var img: Image = Image.create(32, 32, false, Image.FORMAT_RGBA8)
	img.fill(Color.RED)  # Escolha a cor que quiser (RED, BLUE, GREEN, etc.)
	var tex: ImageTexture = ImageTexture.create_from_image(img)
	var sprite:Sprite2D = Sprite2D.new()
	sprite.texture = tex
	add_child(sprite)
	
	
	calculate_new_pos()


var count: int = 0
func calculate_new_pos() -> void:
	if x < 0 or x >= room_gen.get_room_quantity_x() or y < 0\
	or y >= room_gen.get_room_quantity_y():
		return
	
	if x == room_gen.get_room_quantity_x() -1 \
	and y == room_gen.get_room_quantity_y() -1:
		set_process(false)
		return
		#queue_free()
	
	robot_rooms[x][y].set_robot_visited(true)
	
	update_weights(robot_rooms[x][y])
	
	var next_dir: Vector2i = min_weight(robot_rooms[x][y])
	x = next_dir.x
	y = next_dir.y
	#robot_rooms[x][y].set_robot_visited(true)
	var pos: Vector2 = Vector2(robot_rooms[x][y].get_glob_pos()) + offset_vector
	current_pos = pos


func update_weights(room: Room) -> void:
	var weight_update_list: Array[Room] = [room]
	
	while !weight_update_list.is_empty():
		#printt("updating weights", weight_update_list.size())
		var c_room: Room = weight_update_list.pop_front()
		var new_dir = min_weight(c_room)
		var dirs: Array = c_room.get_directions() if c_room.get_robot_visited() \
		else c_room.get_all_directions()
		
		var new_weight: int = rooms_weight[new_dir.x][new_dir.y] + 1
		if c_room.get_tile_pos().x == room_gen.get_room_quantity_x() -1 \
		and c_room.get_tile_pos().y == room_gen.get_room_quantity_y() -1:
			new_weight = 0
		
		var pos: Vector2i = c_room.get_tile_pos()
		#printt(c_room.get_tile_pos(), rooms_weight[pos.x][pos.y], new_weight)
		 
		if rooms_weight[c_room.get_tile_pos().x][c_room.get_tile_pos().y] \
		!= new_weight:
			rooms_weight[c_room.get_tile_pos().x][c_room.get_tile_pos().y] = \
			new_weight
			for i in range(len(dirs)):
				var neighbor: Room = robot_rooms[dirs[i].x][dirs[i].y]
				if weight_update_list.has(neighbor):
					continue
				weight_update_list.append(neighbor)
				
				if new_weight > 1000:
					weight_update_list.clear()


func min_weight(room: Room) -> Vector2i:
	var _min_weight: int = 0
	var min_dir: Vector2i
	var dirs: Array = room.get_directions() if room.get_robot_visited() \
	else room.get_all_directions()
	
	for a in range(len(dirs)):
		var dir: Vector2i = dirs[a]
		var current_weight = rooms_weight[dir.x][dir.y]
		if a == 0 or (current_weight < _min_weight):
			_min_weight = current_weight
			min_dir = dir
	
	return min_dir


func _process(delta: float) -> void:
	
	if global_position.distance_to(current_pos) < reach_offset:
		calculate_new_pos()
	
	var dir:Vector2 = global_position.direction_to(current_pos)
	
	global_position += speed * dir * delta
