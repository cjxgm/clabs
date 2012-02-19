$(function(){
	$('div.menu-item')
		.click(function(e){
			show_page($(this));
			e.preventDefault();
		})
		.hover(function(){
			$(this).attr('class_bak', $(this).attr('class'));
			$(this).switchClass($(this).attr('class_bak'),
								'menu-item-hover', 100);
		}, function(){
			$(this).switchClass('menu-item-hover',
								$(this).attr('class_bak'), 100);
		})
	;

	// show main page
	show_page($('div [href="home.htm"]'));
});

function show_page(d)
{
	$('#line').html('Loading...');
	$('#line').switchClass('hline-invisible', 'hline', 100);

	var url = d.attr('href');
	$.get(url, function(data){
		// when succeeded...
		$('div.menu-item-selected').switchClass(
					'menu-item-selected', 'menu-item', 100);
		d.attr('class', 'menu-item-selected');

		var main = $('#main');
		main.slideUp(500, function(){
			main.html(data);
			process_links();
			main.slideDown(500, function(){
				$('#line').switchClass('hline', 'hline-invisible', 200);
				$('#line').html('');
			});
		});
	});
}

function process_links()
{
	$('a.popup-window').click(function(e){
		show_popup_window($(this));
		e.preventDefault();
	});
}

function show_popup_window(d)
{
	$('#msg')
		.html('Loading...')
		.dialog({width: 600, height: 400});
	;

	var url = d.attr('href');
	$.get(url, function(data){
		$('#msg').html(data);
	});
}

