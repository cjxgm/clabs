// vim: noet ts=4 sw=4 sts=0

// compile and run: ./test_elm.pl grid_test.c

$_(ctn, elm_table_add(win));	// ctn		= container
evas_object_size_hint_weight_set(ctn, 1, 1);
elm_win_resize_object_add(win, ctn);
evas_object_show(ctn);

$_(grid, elm_grid_add(win));	// grid		= grid
evas_object_size_hint_weight_set(grid, 1, 1);
evas_object_size_hint_align_set(grid, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_table_pack(ctn, grid, 0, 0, 1, 1);
evas_object_show(grid);

$_(btn, elm_button_add(win));	// btn		= button
evas_object_size_hint_weight_set(btn, 1, 1);
evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_grid_pack(grid, btn, 10, 10, 80, 80);
evas_object_show(btn);

$_(slh, elm_slider_add(win));	// slh		= slider horizontal
elm_slider_min_max_set(slh, 100, 200);
evas_object_size_hint_weight_set(slh, 1, 0);
evas_object_size_hint_align_set(slh, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_table_pack(ctn, slh, 0, 1, 1, 1);
evas_object_show(slh);

$_(slv, elm_slider_add(win));	// slb		= slider vertical
elm_slider_min_max_set(slv, 100, 200);
elm_slider_horizontal_set(slv, EINA_FALSE);
evas_object_size_hint_weight_set(slv, 0, 1);
evas_object_size_hint_align_set(slv, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_table_pack(ctn, slv, 1, 0, 1, 1);
evas_object_show(slv);

$_(ck, elm_check_add(win));		// ck		= check
evas_object_size_hint_align_set(ck, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_table_pack(ctn, ck, 1, 1, 1, 1);
evas_object_show(ck);

static int grid_w = 100;
static int grid_h = 100;

void on_slider_change(int * size, Evas_Object * sl)
{
	if (elm_check_state_get(ck)) {
		grid_w = grid_h = elm_slider_value_get(sl);
		elm_slider_value_set(slh, grid_w);
		elm_slider_value_set(slv, grid_w);
	}
	else *size = elm_slider_value_get(sl);
	elm_grid_size_set(grid, grid_w, grid_h);
}
$$$$(slh, "changed", (void *)&on_slider_change, &grid_w);
$$$$(slv, "changed", (void *)&on_slider_change, &grid_h);

