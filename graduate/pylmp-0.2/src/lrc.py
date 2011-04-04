#!/usr/bin/env python
#-*- coding: utf8 -*-
#
#	lrc.py
#
#	Copyleft 2010-2011 cjxgm (兰威举) <cjxgm@126.com>
#
#	This file is part of pylmp.
#
#	Pylmp is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	pylmp is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with pylmp.  If not, see <http://www.gnu.org/licenses/>.

from os import path
import ui_cn as ui

class g:  # Global Vars
	lrcds = {}  # LRC datas
	timeline = []  # LRC timeline

def changeExt(p):  # change extname from '.mp3' to '.lrc'
	return path.splitext(p)[0] + '.lrc'

def loadLrc(lrcFile):
	g.lrcds = {}  # Clear last datas
	g.timeline = []
	if not path.exists(lrcFile):
		return False
	lf = open(lrcFile)
	while True:
		line = lf.readline()
		if line == '':
			break
		line = iconv(line).strip()
		if line == '':  # Skip blank line
			continue
		if not line.startswith('['):
			ui.throw('未知的 LRC 节：' + line)
			continue
		line = line[1:]
		line = line.split(']', 1)
		if len(line) != 2:
			ui.throw('LRC 节异常：' + str(line))
			continue
		line[0] = line[0].split(':')
		# For LRC section: "[a:b]c"
		# line = [[a, b], c]
		if not line[0][0].isdigit():  # LRC data	line[1] == '' and 
			g.lrcds[line[0][0]] = line[0][1]
		else:
			offset = 0
			if g.lrcds.has_key('offset'):
				offset = int(g.lrcds['offset'])
			time = int((int(line[0][0]) * 60 + float(line[0][1])) * 1000)
			g.timeline.append([time + offset, line[1]])
	g.timeline.sort()
	return True

def getLrc(current_time):
	i = 0
	for lrc_time, lyric in g.timeline:  # Why enumerate doesn't work?
		if current_time < lrc_time:
			break
		i += 1
	i -= 1
	lyrics = ['', '', '']
	if i > 0:
		lyrics[0] = g.timeline[i - 1][1]
	lyrics[1] = g.timeline[i][1]
	if i < len(g.timeline) - 1:
		lyrics[2] = g.timeline[i + 1][1]
	return lyrics

def iconv(string):
	codings = ('utf8', 'gbk')
	for c in codings:
		error = False
		try:
			string.decode(c)
		except:
			error = True
		if not error:
			return string.decode(c).encode('utf8')
	return string

def main():
	
	return 0

if __name__ == '__main__': main()
