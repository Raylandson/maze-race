class_name Enemy extends CharacterBody2D

const ENEMY_SCENE: PackedScene = preload("uid://5vvlp6um5naw")

var health: float = 10
var direction: Vector2 = Vector2.ZERO
@export var speed: float = 100

var min_time_rand_dir: float = 1.0
var max_time_rand_dir: float = 2.0

var time_recalculate_dir: float = 0


static func new_enemy(glob_pos: Vector2) -> Enemy:
	var enemy: Enemy = ENEMY_SCENE.instantiate()
	enemy.global_position = glob_pos
	
	return enemy


func _ready() -> void:
	change_direction()

func _physics_process(delta: float) -> void:
	time_recalculate_dir -= delta
	for ray: RayCast2D in $Rays2.get_children():
		if ray.is_colliding() and time_recalculate_dir < 0:
			change_direction()
			time_recalculate_dir = 0.2
	
	velocity = direction * speed
	move_and_slide()


func change_direction() -> void:
	
	direction = Vector2(randf_range(-int(not %LeftRay.is_colliding()), \
	int(not %RightRay.is_colliding())), \
	randf_range(-int(not %TopRay.is_colliding()), \
	int(not %BotRay.is_colliding()))).normalized()
	randomize_timer()


func randomize_timer() -> void:
	%DirectionTimer.start(randf_range(min_time_rand_dir, max_time_rand_dir))


func take_damage(damage: float) -> void:
	health -= damage
	if health <= 0:
		queue_free()
