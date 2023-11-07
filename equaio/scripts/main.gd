extends HBoxContainer

enum TYPE {BASIC, VALUE, FRAC}
const Block  = preload("res://block.tscn")

# Called when the node enters the scene tree for the first time.
func _ready():
	var ctx = EqContext.new()
	ctx.set(["x","y","z"], ["+","-","="], ["_"], false)
	
	var expr = EqExpression.new()
	expr.parse("(x + y) = z", ctx)
	print(expr.to_string())
	
	var block = EqBlockDisplay.new()
	block.from_expression(expr, ctx)
	print(block.to_string())
	
	do_block(self, block)

#	for n in range(5):
#		var b = Block.instantiate()
#		b.set_text(str(n))
#		self.add_child(b)

func do_block(root : Node, block : EqBlockDisplay):
	for i in range(block.get_child_count()):
		var c = EqBlockDisplay.new()
		block.get_child_at(i, c)
		var type = c.get_type()
		if type == TYPE.VALUE:
			var b = Block.instantiate()
			b.set_text(c.get_value())
			root.add_child(b)
		elif type == TYPE.BASIC:
			var container = HBoxContainer.new()
			root.add_child(container)
			do_block(container, c)
		elif type == TYPE.FRAC:
			pass
	pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
