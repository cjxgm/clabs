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

	// Get and show the main page.
	show_page($('div [href="main.htm"]'));
});

function show_page(d)
{
	$('#line').html('Loading...');

	var url = d.attr('href');
	$.get(url, function(data){
		$('div.menu-item-selected').switchClass(
					'menu-item-selected', 'menu-item', 100);
		d.attr('class', 'menu-item-selected');
		$('#line').switchClass('hline-invisible', 'hline', 100);

		var main = $('#main');
		main.slideUp(500, function(){
			main.html(data);
			main.slideDown(500, function(){
				$('#line').switchClass('hline', 'hline-invisible', 200);
				$('#line').html('');
			});
		});
	});
}

