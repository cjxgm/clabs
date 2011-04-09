var pages = new Array();
var images = new Array();
var ps = new Array(); // Particles
/** images
 * [0] Logo
 * [1] CC Logo
 * [2] Cover image
 * [3..] Other bakgrounds
 */
var cur_image = 2;
var new_image = 2;
var sel_item = -1;
var from_item = -1;
var menu_ready = false;
var interval_main;
var stat_main = { step: 0, frame: 0 };
var stat_btn = { step: 0, frame: 0 }; // Button "exit"
var stat_sw = { step: 0, frame: 0 }; // switching
var effect = 0;
var effect_title = 0;
var grad_over; // Gradient

var TIME_SPC = 2;

var m = Math;
var cxt = c.getContext('2d');

function item_sel(item)
{
	pages[sel_item].div.style.visibility = 'hidden';
	pages[sel_item].item.className = 'item';
	pages[item].div.style.visibility = 'visible';
	pages[item].item.className = 'item_sel';
	sel_item = item;
}

function new_page(title, txt)
{
	pages[pages.length] = {
		title: title,
		html: txt,
		div: document.createElement("div"),
		title: title,
		step: 0,
		frame: 0,
		x: 0,
		y: 0
	};
}

function ready()
{
	cxt.clearRect(0, 0, c.width, c.height);
	cxt.textAlign = "center";
	font(32, '');
	cxt.fillText("Loading", c.width/2, c.height/2);

	// Load images
	for (var i=0; i<13; i++){
		var img = new Image();
		img.src = "images/"+i+".jpg";
		images[images.length] = img;
	}
	
	// Create gradient
	grad_over = cxt.createLinearGradient(0, 0, 230, 20);
	grad_over.addColorStop(0, "RGBA(255, 255, 255, 0.2)");
	grad_over.addColorStop(1, "RGBA(100, 160, 255, 0.8)");
	
	// Register sth...
	c.onmousemove = function(e){ mouse_move(e.offsetX, e.offsetY); }
	c.onmousedown = function(e){ mouse_down(e.offsetX, e.offsetY); }
	c.onmouseup = function(e){ mouse_up(e.offsetX, e.offsetY); }
	c.onclick = function(e){ mouse_click(e.offsetX, e.offsetY); }
	interval_main = setInterval(fresh, 33.3);
}

function fresh()
{
	cxt.clearRect(0, 0, c.width, c.height);
	switch(stat_main.step){
		case 0: // Wait
			if (stat_main.frame == 5){
				stat_main.step++;
				stat_main.frame = 0;
			}
			stat_main.frame++;
			break;
		case 1: // Logo
			if (stat_main.frame<30){
				cxt.globalAlpha = map_range(stat_main.frame, 0, 29, 0, 1);
				cxt.fillStyle = "RGB("+ parseInt(map_range(stat_main.frame, 0, 29, 0, 128))
						+ ',' + parseInt(map_range(stat_main.frame, 0, 29, 0, 196))
						+ ',' + parseInt(map_range(stat_main.frame, 0, 29, 0, 255)) + ")";
			}
			else if (stat_main.frame >=60 && stat_main.frame<90){
				cxt.globalAlpha = map_range(stat_main.frame, 60, 89, 1, 0);
				cxt.fillStyle = "RGB("+ parseInt(map_range(stat_main.frame, 60, 89, 128, 0))
						+ ',' + parseInt(map_range(stat_main.frame, 60, 89, 196, 0))
						+ ',' + parseInt(map_range(stat_main.frame, 60, 89, 255, 0)) + ")";
			}
			
			var img = images[0];
			cxt.drawImage(img, (c.width-img.width)/2, 
							(c.height-img.height)/2, img.width, img.height);
			var img = images[1];
			cxt.drawImage(img, 80, 100, img.width, img.height);

			cxt.textAlign = "center";
			font(32, '');
			cxt.fillText("兰威举产品", 760, 400);
			cxt.textAlign = "left";
			font(16, 'bold');
			cxt.fillText('This work is licensed under a Creative Commons '
				+ 'Attribution-NonCommercial 3.0 Unported License.', 180, 120);

			stat_main.frame++;
			
			if (stat_main.frame == 90){
				stat_main.step++;
				stat_main.frame = 0;
			} else break;
		case 2: // Draw the magazine!
			if (stat_main.frame == 0){
				new_effect_ps_0(700, 400, 100, 1);
			}
			
			// 背景 及 退出按钮
			if (stat_main.frame >= 10){
				if (stat_main.frame < 20)
					cxt.globalAlpha = map_range(stat_main.frame, 10, 19, 0, 1);
				else cxt.globalAlpha = 1;
				
				var img = images[cur_image];
				cxt.drawImage(img, 0, 0, c.width, c.height);
				
				if (stat_sw.step == 1){
					if (stat_sw.frame < 10){
						var img = images[new_image];
						cxt.globalAlpha = map_range(stat_sw.frame, 0, 9, 0, 1);
						cxt.drawImage(img, 0, 0, c.width, c.height);
					}
					if (stat_sw.frame == 9)
						cur_image = new_image;
				}
				
				draw_button();
			}
			
			if (sel_item == -1){
				// "素年锦时" 字样
				if (stat_main.frame >=5){
					if (stat_main.frame < 30){
						cxt.fillStyle = "RGB(0,0,"
								+ parseInt(map_range(stat_main.frame,
											5, 29, 0, 255)) + ')';
					} else cxt.fillStyle = "RGB(0, 0, 255)";
					cxt.textAlign = "center";
					font(64, '');
					cxt.fillText("素年锦时", 700, 400);
				}
			
				// "浙江省衢州二中高一(4)班优秀作文选" 字样
				if (stat_main.frame >= 10){
					cxt.fillStyle = "black";
					cxt.textAlign = "center";
					font(16, 'italic');
					cxt.fillText("浙江省衢州二中高一(4)班优秀作文选", 700, 440);
				}
			}
			
			// 画菜单
			if (stat_main.frame >= 20){
				if ((stat_main.frame-20)%TIME_SPC == 0){
					if ((stat_main.frame-20)/TIME_SPC < pages.length){
						pages[(stat_main.frame-20)/TIME_SPC].step = 1;
						pages[(stat_main.frame-20)/TIME_SPC].y = 160
										+ (stat_main.frame-20)/TIME_SPC * 20;
					} else menu_ready = true;
				}
				
				for (var i=0; i<pages.length; i++){
					switch(pages[i].step){
						case 0:
							continue;
						case 1:
							var x;
							if (pages[i].frame < TIME_SPC*5+1)
								x = map_range(m.sin(map_range(
										pages[i].frame,
										0, TIME_SPC*5, 0, m.PI/2)),
									0, 1, -100, 25);
							
							cxt.fillStyle = "black";
							cxt.textAlign = "left";
							cxt.globalAlpha = 1;
							font(14, '');
							cxt.fillText(pages[i].title, x, pages[i].y);
							pages[i].frame++;
							if (pages[i].frame == TIME_SPC*5+1){
								pages[i].step++;
								pages[i].frame = 0;
							}
							break;
						case 2:
							if (pages[i].frame > 0){
								cxt.globalAlpha = map_range(pages[i].frame,
													0, 4, 0, 1);
								pages[i].frame--;
							}
							else cxt.globalAlpha = 0;
							cxt.fillStyle = grad_over;
							cxt.fillRect(10, pages[i].y - 15, 230, 20);
							
							cxt.globalAlpha = 1;
							cxt.fillStyle = "black";
							cxt.textAlign = "left";
							font(14, '');
							cxt.fillText(pages[i].title, 18, pages[i].y);
							break;
						case 3:
							if (pages[i].frame < 5){
								cxt.globalAlpha = map_range(pages[i].frame,
													0, 4, 0, 1);
								pages[i].frame++;
							}
							else cxt.globalAlpha = 1;
							cxt.fillStyle = grad_over;
							cxt.fillRect(10, pages[i].y - 15, 230, 20);
							
							cxt.globalAlpha = 1;
							cxt.fillStyle = "black";
							cxt.textAlign = "left";
							font(14, '');
							cxt.fillText(pages[i].title, 18, pages[i].y);
							break;
						case 4:
							if (pages[i].frame < 5){
								cxt.globalAlpha = map_range(pages[i].frame,
													0, 4, 1, 0.4);
								pages[i].frame++;
							}
							else cxt.globalAlpha = 0.4;
							cxt.fillStyle = "RGB(100, 160, 255)";
							cxt.fillRect(
									map_range(pages[i].frame, 0, 4, 10, 2),
									pages[i].y - 15,
									map_range(pages[i].frame, 0, 4, 230, 246),
									20);
							
							cxt.globalAlpha = 1;
							cxt.fillStyle = "black";
							cxt.textAlign = "left";
							font(14, '');
							cxt.fillText(pages[i].title, 18, pages[i].y);
							break;
					}
				}
			}
			
			// 转场
			if (stat_sw.step == 1){
				if (from_item == -1){
					if (stat_sw.frame<5){
						// "素年锦时" 字样
						cxt.fillStyle = "RGB("
							+ parseInt(map_range(stat_sw.frame, 0, 4, 0, 255))
							+ ','
							+ parseInt(map_range(stat_sw.frame, 0, 4, 0, 255))
							+ ', 255)';
						cxt.textAlign = "center";
						font(64, '');
						cxt.fillText("素年锦时", 700, 400);
			
						// "浙江省衢州二中高一(4)班优秀作文选" 字样
						cxt.fillStyle = "RGB("
							+ parseInt(map_range(stat_sw.frame, 0, 4, 0, 255))
							+ ','
							+ parseInt(map_range(stat_sw.frame, 0, 4, 0, 255))
							+ ','
							+ parseInt(map_range(stat_sw.frame, 0, 4, 0, 255))
							+ ')';
						font(16, 'italic');
						cxt.fillText("浙江省衢州二中高一(4)班优秀作文选", 700, 440);
					}
				}
				else if (stat_sw.frame < 5) {
					// Title gone...
					cxt.textAlign = "center";
					cxt.fillStyle = "#00f";
					font(map_range(stat_sw.frame, 0, 4, 32, 0), 'bold');
					cxt.fillText(pages[from_item].title, 640, 
						map_range(stat_sw.frame, 0, 4, 50, 0));
				}
				
				if (stat_sw.frame == 0){
					switch(effect){
						default:
						case 0:
							new_effect_ps_0(600 + (m.random()-0.5)*200,
											200 + (m.random()-0.5)*200,
											300 + m.random() * 100, 5);
							new_effect_ps_0(900 + (m.random()-0.5)*200,
											600 + (m.random()-0.5)*200,
											310 + m.random() * 100, 6);
							new_effect_ps_0(600 + (m.random()-0.5)*200,
											500 + (m.random()-0.5)*200,
											390 + m.random() * 100, 7);
							break;
						case 1:
						case 2:
							new_effect_ps_2(200 + m.random() * 100, 5);
							break;
						case 3:
						case 4:
							new_effect_ps_3(200 + m.random() * 100, 5);
							break;
						case 5:
						case 6:
							new_effect_ps_6(600, 360, 100 + m.random() * 50, 24);
							break;
						case 7:
						case 8:
							new_effect_ps_7(100 + m.random() * 50, 40 + m.random() * 10);
							break;
					}
				}
				
				if (stat_sw.frame >= 0 && stat_sw.frame < 22)
					ds.style.opacity = map_range(stat_sw.frame, 0, 21, 1, 0);
				else if (stat_sw.frame == 22){
					ds.innerHTML = pages[sel_item].html;
					ds.style.visibility = "visible";
				}
				else if (stat_sw.frame >= 22 && stat_sw.frame < 30)
					ds.style.opacity = map_range(stat_sw.frame, 22, 29, 0, 1);

				// 标题特效
				if (stat_sw.frame >= 30){
					cxt.textAlign = "center";
					var title = pages[sel_item].title;
					
					switch (effect_title) {
						default:
						case 0:
						case 1:
							for (var i=0; i<title.length; i++){
								var k = (stat_sw.frame - 30) / 5;
								if (i > k) break;
								
								var size = 32;
								if (i >= k - 1)
									size = map_range(k - i, 0, 1, 0, 32);
								
								var x = map_range(i, 0, title.length,
									640 - size * title.length / 2 + size / 2,
									640 + size * title.length / 2 + size / 2);
									
								if (i >= k - 1)
									new_effect_ps_4(x+16, 60, x+16, 30, 8 + m.random()*4);
								
								cxt.fillStyle = "#00f";
								font(size, 'bold');
								cxt.fillText(title[i], x, 50);
							}
							break;
						case 2:
						case 3:
							for (var i=0; i<title.length; i++){
								var k = (stat_sw.frame - 30) / 5;
								
								var x = map_range(i, 0, title.length,
									640 - 32 * title.length / 2 + 16,
									640 + 32 * title.length / 2 + 16);
								
								if (i >= k - 3){
									for (var j=0; j<32; j+=32/5){
										var y = 30 + j;
										var sy = j * 4 - 20;
										new_effect_ps_4(
											x + 100 + (k-i)*32 + (m.random()-0.5)*30,
											sy + (m.random()-0.5)*30, 
											x - 16 + (k-i)*32 + (m.random()-0.5)*30,
											y + (m.random()-0.5)*30,
											8 + m.random()*4);
									}
								}
								
								if (i > k - 2) break;
								var size = 32;
								if (i >= k - 2){
									size = map_range(k - i - 1, 0, 1, 0, 32);
								}
								
								cxt.fillStyle = "#00f";
								font(size, 'bold');
								
								cxt.fillText(title[i], x, 50);
							}
							break;
					}
				}
				
				stat_sw.frame++;
				if (stat_sw.frame == 30 + pages[sel_item].title.length * 5)
					stat_sw.step = 0;
			}
			// 画标题
			else if (sel_item != -1) {
				cxt.textAlign = "center";
				cxt.fillStyle = "#00f";
				font(32, 'bold');
				cxt.fillText(pages[sel_item].title, 640, 50);
			}
			
			// 绘制粒子
			draw_ps();
			
			stat_main.frame++;
			break;
		case 3: // the End
			// 背景
			if (stat_main.frame < 10){
				cxt.globalAlpha = map_range(stat_main.frame, 0, 9, 1, 0);
				
				var img = images[cur_image];
				cxt.drawImage(img, 0, 0, c.width, c.height);
			}
			if (stat_main.frame >= 10){
				if (stat_main.frame < 120){
					cxt.textAlign = "left";
				
					var x;
					if (stat_main.frame < 25){
						cxt.fillStyle = "RGB("
								+ parseInt(map_range(stat_main.frame, 10, 24, 0, 128))
								+ ','
								+ parseInt(map_range(stat_main.frame, 10, 24, 0, 196))
								+ ','
								+ parseInt(map_range(stat_main.frame, 10, 24, 0, 255))
								+ ')';
						x = map_range(stat_main.frame, 10, 24, 0, 20);
						cxt.globalAlpha = map_range(stat_main.frame, 10, 24, 0, 1);
					}
					else if (stat_main.frame >= 100){
						cxt.fillStyle = "RGB("
								+ parseInt(map_range(stat_main.frame, 100, 119, 128, 0))
								+ ','
								+ parseInt(map_range(stat_main.frame, 100, 119, 196, 0))
								+ ','
								+ parseInt(map_range(stat_main.frame, 100, 119, 255, 0))
								+ ')';
						x = map_range(stat_main.frame, 100, 119, 20, 40);
						cxt.globalAlpha = map_range(stat_main.frame, 100, 119, 1, 0);
					}
					else{
						cxt.fillStyle = "RGB(128,196,255)";
						x = 20;
						cxt.globalAlpha = 1;
					}
					font(32, '');
					cxt.fillText("代码", 100 + x, 100);
					cxt.fillText("总设计", 100 + x, 200);
					cxt.fillText("特效设计", 100 + x, 300);
					cxt.fillText("制作", 100 + x, 420);
					cxt.fillText("鸣谢所有投稿者、审稿者及其他相关人员的支持！", 120 + x, 600);
					cxt.fillText("特别鸣谢", 500 + x, 100);
					
					var img = images[1];
					cxt.drawImage(img, 440 + x, 620, img.width, img.height);
					font(16, 'bold');
					cxt.fillText('This work is licensed under a Creative '
								+ 'Commons Attribution-NonCommercial 3.0 '
								+ 'Unported License.', 120 + x, 680);
					
					if (stat_main.frame >= 20){
						var y;
						if (stat_main.frame < 40){
							cxt.fillStyle = "RGB("
									+ parseInt(map_range(stat_main.frame, 20, 39, 0, 255))
									+ ','
									+ parseInt(map_range(stat_main.frame, 20, 39, 0, 255))
									+ ','
									+ parseInt(map_range(stat_main.frame, 20, 39, 0, 255))
									+ ')';
							y = map_range(stat_main.frame, 20, 39, 20, 0);
						}
						else if (stat_main.frame >= 100){
						cxt.fillStyle = "RGB("
								+ parseInt(map_range(stat_main.frame, 100, 119, 255, 0))
								+ ','
								+ parseInt(map_range(stat_main.frame, 100, 119, 255, 0))
								+ ','
								+ parseInt(map_range(stat_main.frame, 100, 119, 255, 0))
								+ ')';
						y = map_range(stat_main.frame, 100, 119, 0, -20);
						}
						else{
							cxt.fillStyle = "white";
							y = 0;
						}
						font(24, '');
						cxt.fillText("兰威举", 140 + x, 140 + y);
						cxt.fillText("兰威举", 140 + x, 240 + y);
						cxt.fillText("徐乾", 140 + x, 340 + y);
						cxt.fillText("兰威举", 140 + x, 370 + y);
						cxt.fillText("兰威举", 140 + x, 450 + y);
						cxt.fillText("Google (谷歌，不只是搜索引擎……)", 540 + x, 140 + y);
						cxt.fillText("Ubuntu Linux (Canonical, Inc.)", 540 + x, 170 + y);
						cxt.fillText("GNU", 540 + x, 200 + y);
						cxt.fillText("W3C", 540 + x, 230 + y);
						cxt.fillText("Llnux Torvalds", 540 + x, 280 + y);
						cxt.fillText("Richard Stallman", 540 + x, 310 + y);
						cxt.fillText("文泉驿", 540 + x, 340 + y);
						cxt.fillText("Creative Commons", 540 + x, 390 + y);
					}
				}
			}
			if (stat_main.frame == 0)
					new_effect_ps_6(c.width/2, c.height/2, 100, 40);
			if (stat_main.frame == 86) new_effect_ps_3(100, 5);
			if (stat_main.frame == 86) new_effect_ps_0(160, 300, 300, 1);
			draw_ps();
			stat_main.frame++;
			if (stat_main.frame == 150)
				window.close();
			break;
	}
}

function map_range(value, src_from, src_to, dest_from, dest_to)
{
	return (value-src_from) / (src_to-src_from)
			* (dest_to-dest_from) + dest_from;
}

function font(size, style)
{
	cxt.font = style + ' ' + size +'px 文泉驿正黑';
}

function new_effect_ps_0(x, y, rho, speed) // White-liquid...
{
	ps[ps.length] = {
		type: 0,
		x: x,
		y: y,
		rho: rho,
		speed: speed,
		frame: 0
	};
	return ps.length-1;
}

function new_effect_ps_1(x, y, vx, vy, ax, ay, size) // Moving particle...
{
	ps[ps.length] = {
		type: 1,
		x: x,
		y: y,
		vx: vx,
		vy: vy,
		ax: ax,
		ay: ay,
		size: (size == undefined ? 20 : size),
		frame: 0
	};
	return ps.length-1;
}

function new_effect_ps_2(rho, speed) // White-liquid-up-down...
{
	ps[ps.length] = {
		type: 2,
		rho: rho,
		speed: speed,
		frame: 0
	};
	return ps.length-1;
}

function new_effect_ps_3(rho, speed) // White-liquid-down-up...
{
	ps[ps.length] = {
		type: 3,
		rho: rho,
		speed: speed,
		frame: 0
	};
	return ps.length-1;
}

function new_effect_ps_4(sx, sy, dx, dy, size) // Moving particle 2...
{
	ps[ps.length] = {
		type: 4,
		sx: sx,
		sy: sy,
		dx: dx,
		dy: dy,
		size: (size == undefined ? 20 : size),
		frame: 0
	};
	return ps.length-1;
}

function new_effect_ps_5(x, y, vx, vy, ax, ay, size) // One tadpole
{
	ps[ps.length] = {
		type: 5,
		x: x, cx: x,
		y: y, cy: y,
		vx: vx,
		vy: vy,
		ax: ax,
		ay: ay,
		size: (size == undefined ? 20 : size),
		frame: 0
	};
	return ps.length-1;
}

function new_effect_ps_6(x, y, rho, speed) // Tadpoles
{
	ps[ps.length] = {
		type: 6,
		x: x,
		y: y,
		rho: rho,
		speed: speed,
		frame: 0
	};
	return ps.length-1;
}

function new_effect_ps_7(rho, speed) // Tadpoles from one corner
{
	ps[ps.length] = {
		type: 7,
		rho: rho,
		speed: speed,
		frame: 0
	};
	return ps.length-1;
}

function draw_ps()
{
	for (var i=0; i<ps.length; i++){
		if (ps[i] == undefined) continue;
		switch(ps[i].type){
			case 0: // 12 frames
				if (ps[i].frame < 5)
					cxt.globalAlpha = map_range(ps[i].frame, 0, 4, 0, 1);
				else cxt.globalAlpha = 1;
				cxt.fillStyle = "white";
				cxt.beginPath();
				circle(ps[i].x, ps[i].y, 
						map_range(ps[i].frame, 0, 11, 500, 20));
				cxt.closePath();
				cxt.fill();
				
				ps[i].frame++;
				
				if (ps[i].frame == 12){
					var step = 2*m.PI / ps[i].rho;
					for (var j=0; j<ps[i].rho; j++){
						var vx = ps[i].speed*m.cos(j*step)
								 + (m.random()-0.5)*2;
						var vy = ps[i].speed*m.sin(j*step)
								 + (m.random()-0.5)*2;
						new_effect_ps_1(ps[i].x, ps[i].y, vx, vy,
								(m.random()-0.5)*10, (m.random()-0.5)*10);
					}
					delete ps[i--];
				}
				
				break;
			case 1:
				cxt.globalAlpha = map_range(ps[i].frame, 0, 39, 1, 0);
				var size = map_range(ps[i].frame, 0, 39,
								ps[i].size, ps[i].size * 3 / 4);
				cxt.fillStyle = "white";
				cxt.beginPath();
				circle(ps[i].x, ps[i].y, size);
				cxt.closePath();
				cxt.fill();
				
				ps[i].vx += ps[i].ax;
				ps[i].vy += ps[i].ay;
				ps[i].x += ps[i].vx;
				ps[i].y += ps[i].vy;
				
				ps[i].frame++;
				if (ps[i].frame == 40)
					delete ps[i--];
				break;
			case 2:
				if (ps[i].frame%2 == 0){
					var step = 650 / ps[i].rho;
					for (var j=0; j<ps[i].rho; j++){
						new_effect_ps_1(300 + j*step + (m.random()-0.5)*10,
										ps[i].frame * 40,
										(m.random()-0.5)*ps[i].speed,
										(m.random()-0.5)*ps[i].speed,
										(m.random()-0.5)*ps[i].speed,
										(m.random()-0.5)*ps[i].speed);
					}
				}
				ps[i].frame++;
				if (ps[i].frame == 16)
					delete ps[i--];
				break;
			case 3:
				if (ps[i].frame%2 == 0){
					var step = 650 / ps[i].rho;
					for (var j=0; j<ps[i].rho; j++){
						new_effect_ps_1(300 + j*step + (m.random()-0.5)*10,
										c.height - ps[i].frame * 40,
										(m.random()-0.5)*ps[i].speed,
										(m.random()-0.5)*ps[i].speed,
										(m.random()-0.5)*ps[i].speed,
										(m.random()-0.5)*ps[i].speed);
					}
				}
				ps[i].frame++;
				if (ps[i].frame == 16)
					delete ps[i--];
				break;
			case 4:
				if (ps[i].frame < 5)
					cxt.globalAlpha = map_range(ps[i].frame, 0, 4, 0, 1);
				else if (ps[i].frame >= 10)
					cxt.globalAlpha = map_range(ps[i].frame, 10, 14, 1, 0);
				else cxt.globalAlpha = 1;
				cxt.fillStyle = "white";
				cxt.beginPath();
				if (ps[i].frame < 10)
					circle(map_range(ps[i].frame, 0, 9, ps[i].sx, ps[i].dx),
							map_range(ps[i].frame, 0, 9, ps[i].sy, ps[i].dy),
							ps[i].size);
				else circle(ps[i].dx, ps[i].dy, ps[i].size);
				cxt.closePath();
				cxt.fill();
				
				ps[i].frame++;
				if (ps[i].frame == 15)
					delete ps[i--];
				break;
			case 5:
				cxt.globalAlpha = map_range(ps[i].frame, 0, 29, 1, 0);
				cxt.fillStyle = "white";
				cxt.strokeStyle = "white";
				cxt.lineWidth = 8;
				cxt.beginPath();
				cxt.moveTo(ps[i].cx, ps[i].cy);
				cxt.lineTo(ps[i].x, ps[i].y);
				cxt.closePath();
				cxt.stroke();
				cxt.beginPath();
				circle(ps[i].x, ps[i].y, 
						map_range(ps[i].frame, 0, 29,
								ps[i].size, ps[i].size * 3 / 4));
				cxt.closePath();
				cxt.fill();
				
				ps[i].vx += ps[i].ax;
				ps[i].vy += ps[i].ay;
				ps[i].x += ps[i].vx;
				ps[i].y += ps[i].vy;
				
				ps[i].frame++;
				if (ps[i].frame == 30)
					delete ps[i--];
				break;
			case 6: // 12 frames
				if (ps[i].frame == 0 || ps[i].frame == 12){
					var step = 2*m.PI / ps[i].rho;
					for (var j=0; j<ps[i].rho; j++){
						var vx = ps[i].speed*m.cos(j*step)
								 + (m.random()-0.5)*0.5;
						var vy = ps[i].speed*m.sin(j*step)
								 + (m.random()-0.5)*0.5;
						new_effect_ps_5(ps[i].x, ps[i].y, vx, vy,
								(m.random()-0.5)*0.1, (m.random()-0.5)*0.1, 10);
					}
				}
				ps[i].frame++;
				if (ps[i].frame > 12)
					delete ps[i--];
				break;
			case 7: // 12 frames
				var angle = map_range(ps[i].frame, 0, 6, m.PI, m.PI/2);
				if (ps[i].frame >= 6)
					angle = map_range(ps[i].frame, 6, 11, m.PI/2, m.PI);
				
				for (var j=0; j<ps[i].rho/6; j++){
					new_effect_ps_5(c.width, c.height,
									ps[i].speed * m.cos(angle) + (m.random()-0.5)*2,
									-ps[i].speed * m.sin(angle)*2 + (m.random()-0.5)*2,
									-ps[i].speed * m.cos(angle)/80,
									ps[i].speed * m.sin(angle)*2/80,
									10);
					angle += m.PI / ps[i].rho;
				}
				ps[i].frame++;
				if (ps[i].frame == 12)
					delete ps[i--];
				break;
		}
	}
}

function circle(x, y, r)
{
	cxt.arc(x, y, r, 0, m.PI*2, true);
}

function draw_button()
{
	var temp_alpha = cxt.globalAlpha;
	
	switch(stat_btn.step){
		case 0:
			cxt.fillStyle = "white";
			cxt.globalAlpha *= map_range(stat_btn.frame, 0, 9, 0.5, 1);
			if (stat_btn.frame > 0) stat_btn.frame--;
			break;
		case 1:
			cxt.fillStyle = "white";
			cxt.globalAlpha *= map_range(stat_btn.frame, 0, 9, 0.5, 1);
			if (stat_btn.frame < 9) stat_btn.frame++;
			break;
		case 2:
			cxt.fillStyle = "#7cf";
			break;
	}

	
	cxt.beginPath();
	circle(50, 50, 30);
	cxt.closePath();
	cxt.fill();
	
	cxt.fillStyle = "black";
	cxt.textAlign = "center";
	font(16, '');
	cxt.fillText("离开", 50, 55);
	
	cxt.globalAlpha = temp_alpha;
}

function mouse_move(x, y)
{
	if (stat_main.step == 2 && stat_main.frame >= 30){
		// 退出按钮
		if (stat_btn.step != 2){
			var x2 = x-50, y2 = y-50;
			if (x2*x2 + y2*y2 < 900 /* 30*30 */) stat_btn.step = 1;
			else stat_btn.step = 0;
		}
		
		// 菜单
		if (menu_ready){
			for (var i=0; i<pages.length; i++){
				if (pages[i].step == 4) continue;
				if (x >= 10 && x <= 240
						&& y >= pages[i].y-15 && y < pages[i].y+5)
					pages[i].step = 3;
				else pages[i].step = 2;
			}
		}
	}
}

function mouse_down(x, y)
{
	if (stat_main.step == 2 && stat_main.frame >= 30){
		// 退出按钮
		if (stat_btn.step == 1) stat_btn.step = 2;
	}
}

function mouse_up(x, y)
{
	if (stat_main.step == 2 && stat_main.frame >= 30){
		if (stat_btn.step == 2) stat_btn.step = 1;
	}
}

function mouse_click(x, y)
{
	if (stat_main.step == 2 && stat_main.frame >= 30){
		// 退出按钮
		if (stat_btn.step == 1 && stat_sw.step == 0){
			ds.style.visibility = "hidden";
			stat_main.step++;
			stat_main.frame = 0;
		}
		
		// 菜单
		if (menu_ready && stat_sw.step == 0){
			for (var i=0; i<pages.length; i++){
				if (pages[i].step != 4
						&& x >= 10 && x <= 240
						&& y >= pages[i].y-15 && y < pages[i].y+5){
					if (sel_item != -1){
						pages[sel_item].step = 2;
						pages[sel_item].frame = 0;
					}
					from_item = sel_item;
					sel_item = i;
					pages[i].step = 4;
					pages[i].frame = 0;
					stat_sw.step = 1;
					stat_sw.frame = 0;
					new_image = parseInt((images.length-2) * (m.random()-0.001) + 2); 
					effect = parseInt(m.random()*9);
					effect_title = parseInt(m.random()*4);
					break;
				}
			}
		}
	}
}
