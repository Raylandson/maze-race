extends Node2D

@export var simple_bot_res: SimpleBotResource

var coins: int = 0:
	set(value):
		coins = value
		#updating button enable
		%SpeedButton.set_disabled(coins < speed_up_cost)
		%NewRobotBu.set_disabled(coins < buy_robot_cost)

var label_coin: float = float(coins)
@onready var coin_count_label:Label = $CanvasLayer/Control2/MarginContainer/VBoxContainer/Label
@export var click_gains: int = 1

@export var speed_up_cost: int = 5:
	set(value):
		speed_up_cost = value
		%SpeedLabel.set_text(str("Cost: %d" % speed_up_cost))


@export var buy_robot_cost: int = 500:
	set(value):
		buy_robot_cost = value
		%RobotSLabel.set_text(str("Cost: %d" % buy_robot_cost))


func _ready() -> void:
	#updating prices
	buy_robot_cost = buy_robot_cost
	speed_up_cost = speed_up_cost
	coins = coins
	
	GameEvents.add_coin.connect(add_coin)


func add_coin(value: int):
	coins += value


func _process(delta: float) -> void:
	label_coin = lerp(label_coin, float(coins), 9 * delta)
	
	coin_count_label.text = str(int(round(label_coin)))


func _on_ch_spd_button_pressed() -> void:
	simple_bot_res.charge_time -= simple_bot_res.charge_time * 0.1


func _on_coin_button_pressed() -> void:
	coins += click_gains
	%ClickSound.play()


func _on_speed_button_pressed() -> void:
	#simple_bot_res.speed += simple_bot_res.speed * 0.1
	if coins < speed_up_cost:
		return
	coins -= speed_up_cost
	speed_up_cost += 5
	simple_bot_res.speed += 50
	
	if simple_bot_res.speed >= simple_bot_res.speed_max:
		%SpeedCon.hide()


func _on_new_robot_pressed() -> void:
	
	if coins < buy_robot_cost:
		return
	
	coins -= buy_robot_cost
	buy_robot_cost += 200
	#for a in range(10):
		#await get_tree().create_timer(0.2).timeout
	var robot_pos: Vector2 = Vector2(16, -24)
	get_tree().current_scene.add_child(Robot.new_simple_bot(robot_pos))
	
	if Robot.quantity >= 150:
		%NewRobotCon.hide()
