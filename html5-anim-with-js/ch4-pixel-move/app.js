
function app()
{
	var canvas  = utils.get("canvas");
	var context = canvas.getContext("2d");
	var mouse   = utils.captureMouse(canvas);
	var video   = utils.get("video");

	(function drawFrame () {
		window.requestAnimationFrame(drawFrame, canvas);

		context.drawImage(video, 0, 0);

		var imagedata = context.getImageData(0, 0,
		 								canvas.width, canvas.height),
			pixels = imagedata.data;

		 //pixel iteration
		for (var y = 0; y < imagedata.height; y += 1) {
			for (var x = 0; x < imagedata.width; x += 1) {
				var dx = x - mouse.x,
					dy = y - mouse.y,
				dist = Math.sqrt(dx * dx + dy * dy),
				offset = (x + y * imagedata.width) * 4,
				r = pixels[offset],
				g = pixels[offset + 1],
				b = pixels[offset + 2];
				pixels[offset    ] = Math.cos(r * dist *  0.001) * 256;
				pixels[offset + 1] = Math.sin(g * dist *  0.001) * 256;
				pixels[offset + 2] = Math.cos(b * dist * 0.0005) * 256;
			}
		}
		context.putImageData(imagedata, 0, 0);
	}());
}

