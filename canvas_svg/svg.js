
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

function line(x1, y1, x2, y2)
{
	c.beginPath();
	c.moveTo(x1, y1);
	c.lineTo(x2, y2);
	c.stroke();
	c.closePath();
}

function bez(x1, y1, x2, y2, x3, y3, x4, y4)
{
	bez_eqt3_plot(
		bez_eqt3_get([[x1, y1, 0],
					  [x2, y2, 0],
					  [x3, y3, 0],
					  [x4, y4, 0]]),
				0, 1, 0.01);
}

function draw()
{
	c.strokeStyle = "#f00";
	draw_svg();
}

