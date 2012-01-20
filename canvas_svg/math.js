/*
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
		if (d > 0.0001) return 1;
		else if (d < -0.0001) return -1;
		else return 0;
	}

	this.collidePlane = function(pdst, n, p) {
		var a = pdst.sub(this);
		var d = -n.dot(p);
		var e = n.dot(this) + d;
		var f = d - n.dot(pdst);
		return a.mul(e / (e + f)).add(this);
	}
}
*/

function bez_eqt_get(v)
{
	return [ v[0],
			 3*(v[1]-v[0]),
			 3*(v[2]-2*v[1]+v[0]),
			 v[3]-3*(v[2]-v[1])-v[0] ];
}

function bez_eqt_eval(eqt, t)
{
	return eqt[0] + eqt[1]*t + eqt[2]*t*t + eqt[3]*t*t*t;
}

function bez_eqt3_get(ps)
{
	return [ bez_eqt_get([ps[0][0], ps[1][0], ps[2][0], ps[3][0]]),
			 bez_eqt_get([ps[0][1], ps[1][1], ps[2][1], ps[3][1]]),
			 bez_eqt_get([ps[0][2], ps[1][2], ps[2][2], ps[3][2]]) ];
}

function bez_eqt3_eval(eqt3, t)
{
	return [ bez_eqt_eval(eqt3[0], t),
			 bez_eqt_eval(eqt3[1], t),
			 bez_eqt_eval(eqt3[2], t) ];
}

