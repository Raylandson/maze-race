class_name Bullet extends Area2D

var damage: float = 20
var direction: Vector2 = Vector2.ZERO
var speed: float = 300
var max_distance: float = 16 * 50

const BULLET_SCENE = preload("uid://d0aedtl3i6aa0")

static func new_bullet(dir: Vector2, glob_pos: Vector2) -> Bullet:
	var bullet: Bullet = BULLET_SCENE.instantiate()
	bullet.global_position = glob_pos
	bullet.direction = dir
	bullet.rotation = atan2(dir.y, dir.x)
	
	return bullet


func _physics_process(delta: float) -> void:
	self.global_position += speed * direction * delta
	max_distance -= speed * delta
	
	if max_distance < 0:
		print('limpado')
		self.queue_free()


func _on_body_entered(body: Node2D) -> void:
	return
	#if body is Enemy:
		#body.take_damage(damage)
		#queue_free()
