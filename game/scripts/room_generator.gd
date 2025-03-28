extends Node2D

@export var player: Player
@export var tile_map: TileMapLayer
@export var camera_limit_scene: PackedScene
@export var room_size_x: int = 5
@export var room_size_y: int = 5
@export var room_quantity: int = 15

class Room2:
	var glob_pos: Vector2i
	var tile_pos: Vector2i
	var visited: bool = false
						#up, down, left, right
	var walls: Array = [true, true, true, true]
	var directions: Array[Vector2i] = [Vector2i.UP, Vector2i.DOWN,
		Vector2i.LEFT,Vector2i.RIGHT]


var rooms: Array = []


func  _ready() -> void:
	#create_one_room(Vector2i.ZERO)
	
	await get_tree().create_timer(5.0).timeout
	
	generate_rooms()
	create_maze()
	
	await get_tree().process_frame
	player.global_position = room_quantity/2 * Vector2(room_size_x, room_size_y) * 16


func generate_rooms() -> void:
	for a in range(room_quantity):
		var row = []
		for b in range(room_quantity):
			var glob_pos: = Vector2i(a * room_size_x * 16, b * room_size_y * 16)
			var tile_pos: = Vector2i(a, b)
			var room: = create_one_room(glob_pos,tile_pos)
			row.append(room)
		rooms.append(row)


func create_one_room(glob_pos: Vector2i,tile_pos: Vector2i) -> Room2:
	var new_room: Room2 = Room2.new()
	#new_room.pos = pos
	new_room.glob_pos = glob_pos
	new_room.tile_pos = tile_pos
	
	for x in range(room_size_x + 1):
		tile_map.set_cell(glob_pos/16 + Vector2i(x, 0), 0, Vector2i(1, 1))
		tile_map.set_cell(glob_pos/16 + Vector2i(x, -room_size_y), 0, Vector2i(1, 1))
	
	
	for y in range(-room_size_y + 1, 1):
		tile_map.set_cell(glob_pos/16 + Vector2i(0, y), 0, Vector2i(1, 1))
		tile_map.set_cell(glob_pos/16 + Vector2i(room_size_x, y), 0, Vector2i(1, 1))
	
	
	var room_size_vector: Vector2i = Vector2i(room_size_x, room_size_y) + Vector2i.ONE
	var camera: CameraLimit = CameraLimit.new_camera(glob_pos, room_size_vector)
	
	get_tree().current_scene.add_child.call_deferred(camera)
	return new_room


func create_maze() -> void:
	
	var current_stack: Array[Room] = []
	var current_element: Vector2i = Vector2i(0, 0)
	current_stack.append(rooms[0][0])
	current_stack[-1].visited = true
	
	while not current_stack.is_empty():
		#var directions: Array[Vector2i] = [Vector2i.UP, Vector2i.DOWN,
		#Vector2i.LEFT,Vector2i.RIGHT]
		var dir_copy: Array[Vector2i] = current_stack[-1].directions.duplicate()
		var found_unvisited: bool = false
		dir_copy.shuffle()
		
		for dir in dir_copy:
			var neighbor: Vector2i = dir + current_element
			if neighbor.x < 0 or neighbor.y < 0 or neighbor.x >= room_quantity \
			or neighbor.y >= room_quantity:
				#print(neighbor, 'out of bounds')ddssa
				continue
			
			if not rooms[neighbor.x][neighbor.y].visited:
				#current_stack[-1]#remove wall
				remove_wall(current_stack[-1].glob_pos, dir)
				rooms[neighbor.x][neighbor.y].visited = true
				current_element = neighbor
				
				current_stack.append(rooms[neighbor.x][neighbor.y])
				found_unvisited = true
				print(dir, ' found new dir')
				await get_tree().create_timer(0.05).timeout
		
		if not found_unvisited:
			
			print(current_stack[-1])
			current_stack.pop_back()
			if not current_stack.is_empty():
				current_element = current_stack[-1].tile_pos
	
	player.global_position = Vector2.ONE * 17


func remove_wall(pos: Vector2i, dir: Vector2i) -> void:
	#var bolso: Vector2i = Vector2i(room_size_x/2, room_size_y/2)
	if dir == Vector2i.UP:
		tile_map.set_cell(pos/16 + Vector2i(room_size_x/2, -room_size_y), -1, Vector2i(1, 1))
	elif dir == Vector2i.DOWN:
		tile_map.set_cell(pos/16 + Vector2i(room_size_x/2, 0), -1, Vector2i(1, 1))
	elif dir == Vector2i.LEFT:
		tile_map.set_cell(pos/16 + Vector2i(0, -room_size_y/2), -1, Vector2i(1, 1))
	elif dir == Vector2i.RIGHT:
		tile_map.set_cell(pos/16 + Vector2i(room_size_x, -room_size_y/2), -1, Vector2i(1, 1))
