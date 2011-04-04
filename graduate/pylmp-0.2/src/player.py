#!/usr/bin/env python
#-*- coding: utf8 -*-
#
#	player.py
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

import mad, ao, time  # import system modules
from os import path
import ui_cn as ui  # import custom modules
import lrc

class g:  # Glabal Vars
	paused = False
	skip = False

def play(f):  # f: Filename
	if not path.exists(f):
		ui.throw('文件不存在：' + f)
		return
	mf = mad.MadFile(f)
	dev = ao.AudioDevice('alsa', rate = mf.samplerate())
	ui.setInfo(f, mf.total_time())
	ui.g.lrcFile = lrc.changeExt(f)
	if not lrc.loadLrc(ui.g.lrcFile):  # No LRC file
		ui.g.lrcFile = ''
	g.skip = False
	while not g.skip:
		if not g.paused:
			buf = mf.read()
			if buf is None:
				break
			dev.play(buf, len(buf))
			ui.setTime(mf.current_time())
		else:
			time.sleep(0.1)
		ui.draw()
	
def main():
	
	return 0

if __name__ == '__main__': main()
