
function app()
{
	var canvas = utils.get("canvas");
	utils.listen(canvas, "mousedown", function(evt) {
		console.log("mouse down");
	});
	utils.listen(canvas, "mouseup", function(evt) {
		console.log("mouse up");
	});
}

