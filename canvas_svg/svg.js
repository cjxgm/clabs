
var sc = new SimpleCanvas(document.getElementById("cvs"));
c = sc.ctx;



function bez_eqt3_plot(eqt3, tbegin, tend, tstep)
{
	c.beginPath();
	var p = bez_eqt3_eval(eqt3, tbegin);
	c.moveTo(p.x, p.y);
	for (tbegin+=tstep; tbegin<tend; tbegin+=tstep) {
		p = bez_eqt3_eval(eqt3, tbegin);
		c.lineTo(p[0], p[1]);
	}
	c.stroke();
	c.closePath();
}

function calc()
{
}

function draw()
{
	//sc.fillCircle(ball.phys.pos, ball.r, "#8ED6FF");
	//sc.line(ground.p, ground.p2);
	var i;
	for (i=0; i<coo.length-1; i++) {
		c.strokeStyle = "#f00";
		bez_eqt3_plot(bez_eqt3_get(coo[i]), 0, 1, 0.01);

		c.strokeStyle = "#00f";
		c.beginPath();
		c.moveTo(coo[i][0][0], coo[i][0][1]);
		c.lineTo(coo[i][1][0], coo[i][1][1]);
		c.lineTo(coo[i][2][0], coo[i][2][1]);
		c.lineTo(coo[i][3][0], coo[i][3][1]);
		c.stroke();
		c.closePath();
	}
}

function timer()
{
	//sc.dur = sc.getTime() - sc.last_time;
	sc.clear();
	draw();
	calc();
	//sc.last_time += sc.dur;
	//sc.time(timer, 30);
}
sc.time(timer, 30);

