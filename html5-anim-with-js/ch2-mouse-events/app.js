
function app()
{
	var canvas = utils.get("canvas");
	var mouse  = utils.captureMouse(canvas);

	function onMouse(evt)
	{
		console.log(evt.type + " at: " + mouse.x + ", " + mouse.y);
	}

	utils.listen(canvas, "mousedown",  onMouse);
	utils.listen(canvas, "mouseup",    onMouse);
	utils.listen(canvas, "click",      onMouse);
	utils.listen(canvas, "dblclick",   onMouse);
	utils.listen(canvas, "mousewheel", onMouse);
	utils.listen(canvas, "mousemove",  onMouse);
	utils.listen(canvas, "mouseover",  onMouse);
	utils.listen(canvas, "mouseout",   onMouse);
}

