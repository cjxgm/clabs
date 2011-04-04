#!/usr/bin/env python
#-*- coding: utf8 -*-
#
#	pylmp.py
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

import sys, traceback  # import system modules
import ui_cn as ui  # import custom modules
import player

filelist = sys.argv[1:]
###########################  D  E  B  U  G  ############################
#if len(filelist) == 0:  
#	filelist = ['a.mp3','b.mp3']
########################################################################
def main():
	global filelist
	noIntro = False
	while True:
		if len(filelist) == 0:
			ui.throw('没有提供任何文件！', True)
		if filelist[0] == '-s':
			noIntro = True
			del filelist[0]
		elif filelist[0] == '-c':
			del filelist[0]
			if len(filelist) == 0:
				ui.throw('错误的参数：-c。', True)
			ui.g.cText = filelist[0]
			del filelist[0]
		elif filelist[0] == '--osd':
			ui.g.enable_osd = True
			del filelist[0]
		elif filelist[0] == '--help':
			ui.showCmdHelp()
			exit()
		else:
			break
	if not noIntro:
		ui.showIntro()
	while True:
		ui.g.step = 0
		for f in filelist:
			player.play(f)
		ui.g.step = 1
		ui.showRoll()
########################################################################
ui.initScreen()
try:
	main()
	ui.restoreScreen()
except:
	ui.restoreScreen()
	traceback.print_exc()


