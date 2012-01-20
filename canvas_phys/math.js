
Vec = function(x, y, z)
{
	this.x = x;
	this.y = y;
	this.z = z;

	this.add = function(v) {
		return new Vec(this.x + v.x, this.y + v.y, this.z + v.z);
	}

	this.sub = function(v) {
		return new Vec(this.x - v.x, this.y - v.y, this.z - v.z);
	}

	this.mul = function(s) {
		return new Vec(this.x * s, this.y * s, this.z * s);
	}

	this.dot = function(v) {
		return this.x*v.x + this.y*v.y + this.z*v.z;
	}

	this.cross = function(v) {
		return new Vec(this.y*v.z - v.y*this.z,
					   this.z*v.x - v.z*this.x,
					   this.x*v.y - v.x*this.y);
	}

	this.sqr = function() {
		return this.dot(this);
	}

	this.mod = function() {
		return Math.sqrt(this.sqr());
	}

	this.norm = function() {
		return this.mul(1 / this.mod());
	}

	this.classifyPlane = function(n, p) {
		var d = n.dot(this) - n.dot(p);
		return d;
	}

	this.collidePlane = function(pdst, n, p) {
		var a = pdst.sub(this);
		var d = -n.dot(p);
		var e = n.dot(this) + d;
		var f = d - n.dot(pdst);
		return a.mul(e / (e + f)).add(this);
	}
}
