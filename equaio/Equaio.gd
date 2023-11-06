extends GDEquaio


# Called when the node enters the scene tree for the first time.
func _ready():
	var ctx = EqContext.new()
	ctx.set(["x","y"], ["+","-"], ["_"], false)
	print(ctx.get())
	
	var expr = EqExpression.new()
	expr.parse("x + y", ctx)
	print(expr.to_string())


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
