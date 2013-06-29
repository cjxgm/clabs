# elm\_ui preprocessor - part of lacuna preprocessor project
## processing flow

	cpp -fpreprocessed file | elm\_ui\_pp | gcc -xc -

## how to use?

	object:+object_type	parameters(which will be directly copied into c)
	object:set_method	parameters(which will be directly copied into c)
	object::get_method	parameters(which will be directly copied into c)
	object:-event_name	parameters(which will be directly copied into c)


	check <object_type>, if has, do it;
	if not:
		check <object_type> as "object", if has, do it;
		if not:
			apply default rule as original <object_type>, that is
			<prefix>_<object_type>_<method>_<set|get>

## configuration file
example:

	evas object
		weight
			size_hint_weight_set
			size_hint_weight_get
			EXPAND	EVAS_HINT_EXPAND

		align
			size_hint_align_set
			size_hint_align_get
			FILL	EVAS_HINT_FILL


	elm table
		pack
			pack
			/


	elm box
		pack
			pack_end
			/

		pack_end
			pack_end
			/
		
		

In the configuration file, blank lines is very important.
They tell the parser when a section ends.

	prefix object_type
		method
			set_func
			get_func
			what_to_replace <BLANKS> replacement
			what_to_replace <BLANKS> replacement
			what_to_replace <BLANKS> replacement
			what_to_replace <BLANKS> replacement
			...
		<BLANK_LINE>
		method
			set_func
			get_func
			what_to_replace <BLANKS> replacement
		<BLANK_LINE>
		method
			set_func
			get_func
			what_to_replace <BLANKS> replacement
		<BLANK_LINE>
		method
			set_func
			get_func
			what_to_replace <BLANKS> replacement
		<BLANK_LINE>
		...
	<BLANK_LINE>
	prefix object_type
		method
			set_func
			get_func
			what_to_replace <BLANKS> replacement
		<BLANK_LINE>
	<BLANK_LINE>
	...



