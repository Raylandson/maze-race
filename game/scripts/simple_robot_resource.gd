class_name SimpleBotResource extends Resource

#above this, break the bot
@export var speed_max:float = 600
@export var speed: float = 0:
	set(value):
		speed = min(value, speed_max)
		#print(value)
		#if value == 0:
			#speed = 10

@export var charge_time: float = 2
