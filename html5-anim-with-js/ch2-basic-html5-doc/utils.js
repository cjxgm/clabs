var utils = {};

if (!window.requestAnimationFrame) { 
	window.requestAnimationFrame = (
			window.webkitRequestAnimationFrame || 
			window.mozRequestAnimationFrame    || 
			window.oRequestAnimationFrame      || 
			window.msRequestAnimationFrame     || 
			function (callback) { 
				return window.setTimeout(callback, 1000/60); 
			}); 
}

utils.listen = function(elem, evt, handler) {
	elem.addEventListener(evt, handler, false);
};

utils.get = function(id) {
	return document.getElementById(id);
};

utils.captureMouse = function(elem) {
	var mouse = { x: 0, y: 0 };

	utils.listen(elem, "mousemove", function(evt) {
		var x, y;

		if (evt.pageX) {
			x = evt.pageX;
			y = evt.pageY;
		} else {
			x = event.clientX + document.body.scrollLeft + 
						document.documentElement.scrollLeft; 
			y = event.clientY + document.body.scrollTop + 
						document.documentElement.scrollTop;
		}

		x -= elem.offsetLeft; 
		y -= elem.offsetTop; 

		mouse.x = x;
		mouse.y = y;
	});

	return mouse;
};

