# coding: utf-8
#print '<div class="mag_con">',
lines = [line for line in file("/tmp/mag")]
print '<div class="mag_title">%s</div>' % lines[0].strip(),
del lines[0]
print '<div class="mag_author">%s</div>' % lines[0].strip(),
del lines[0]
for line in lines:
	if line.startswith('  '): # paragraph
		print '<div class="mag_text">　　%s</div>' % line.strip(),
	elif line.startswith('-'): # sub-title
		print '<div class="mag_subtitle">%s</div>' % 		line.replace('-', ''),
	elif line.startswith('~'): # cap
		print '<div class="mag_cap">%s</div>' % 		line.replace('~', ''),
	elif line.startswith('*'): # comment
		pass
	else: print line,
#print '</div>',
