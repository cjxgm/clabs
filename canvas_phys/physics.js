
var sc = new SimpleCanvas(document.getElementById("cvs"));
sc.dur = 1;			// duration
sc.last_time = 0;
c = sc.ctx;


Physics = function() {
	this.a = new Vec(0.1, 1, 0);
	this.v = new Vec(-10, 0, 0);
	this.pos = new Vec(400, 0, 0);
	this.newpos = new Vec(0, 0, 0);

	this.apply = function(t) {
		this.newpos = this.pos.add(this.v.mul(t).add(this.a.mul(t*t/2)));
		this.v = this.v.add(this.a.mul(t));
	}
};

function calcPlane(p)
{
	p.n = p.p2.sub(p.p).cross(new Vec(0, 0, 1)).norm();
}



Ball = function(r) {
	this.r = r;
	this.phys = new Physics();
}

var ball = new Ball(10);
var ground = { p: new Vec(0, 300, 0), p2: new Vec(640, 100, 0) };
calcPlane(ground);
var ttt = 0;

function calc()
{
	ball.phys.apply(sc.dur*0.01);
	sc.line(ball.phys.pos,
		ball.phys.newpos.sub(ball.phys.pos).mul(10).add(ball.phys.pos));

	var last = ball.phys.pos.classifyPlane(ground.n,
								ground.p.add(ground.n.mul(ball.r)));
	var curr = ball.phys.newpos.classifyPlane(ground.n,
								ground.p.add(ground.n.mul(ball.r)));
	if (last * curr < 0) {
		ball.phys.pos = ball.phys.pos.collidePlane(ball.phys.newpos,
								ground.n,
								ground.p.add(ground.n.mul(ball.r)));
		var v = ball.phys.v;
		var n = ground.n;
		ball.phys.v = v.add(n.mul(-n.dot(v)*2)).mul(0.6);
	}
	else {
		ball.phys.pos = ball.phys.newpos;
		ground.p2 = new Vec(640, 300 + Math.sin(ttt+=sc.dur*0.002)*100, 0);
		calcPlane(ground);
	}

	curr = ball.phys.pos.classifyPlane(ground.n,
								ground.p.add(ground.n.mul(ball.r)));
	if (ball.phys.pos.x<0) ball.phys.pos.x += 640;
	if (ball.phys.pos.x>640) ball.phys.pos.x -= 640;
	if (curr<-0.1) ball.phys.pos.y = 0;
}

function draw()
{
	sc.fillCircle(ball.phys.pos, ball.r, "#8ED6FF");
	sc.line(ground.p, ground.p2);
}

function timer()
{
	sc.dur = sc.getTime() - sc.last_time;
	sc.clear();
	draw();
	calc();
	sc.last_time += sc.dur;
	sc.time(timer, 30);
}
sc.time(timer, 30);

