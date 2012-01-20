
SimpleCanvas = function(canv)
{
	this.ctx = canv.getContext("2d");
	this.begin_time = (new Date()).getTime();

	var c = this.ctx;

	// canvas
	this.clear = function() {
		c.clearRect(0, 0, canv.width, canv.height);
	}
	
	// draw
	this.fillCircle = function(center, r, style) {
		c.beginPath();
		c.arc(center.x, center.y, r, 0, 2*Math.PI, false);
		c.fillStyle = style;
		c.fill();
		c.closePath();
	}

	this.line = function(v1, v2) {
		c.beginPath();
		c.moveTo(v1.x, v1.y);
		c.lineTo(v2.x, v2.y);
		c.stroke();
		c.closePath();
	}

	// util
	this.getTime = function() {
		return (new Date()).getTime() - this.begin_time;
	}

	this.resetTime = function() {
		this.begin_time = (new Date()).getTime();
	}

	this.time = function(cb, fps) {
		window.setTimeout(cb, 1000 / fps);
	}
}

