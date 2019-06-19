#!/usr/bin/env python3

import struct

import serial

def score_bcd_to_int(bcd_int):
	try:
		return int('%x' % bcd_int)
	except:
		return None

def event_nvram_dump(s):
	data = s.read(7)
	score, initials = struct.unpack('>I3s', data)
	score = score_bcd_to_int(score)
	print('nvram_dump=%s,%s' % (score, initials))

def event_wave_change(s):
	data = s.read(1)
	wave = struct.unpack('>B', data)[0]
	print('wave_change=%d' % wave)

def event_enforcer_shot(s):
	print('enforcer_shot')

def event_score_changed(s):
	data = s.read(4)
	score = struct.unpack('>I', data)[0]
	score = score_bcd_to_int(score)
	print('score_changed=%s' % score)

def event_game_over(s):
	print('game_over')

def event_grunt_killed_by_electrode(s):
	print('grunt_killed_by_electrode')

def event_human_killed(s):
	print('human_killed')

def event_human_saved(s):
	print('human_saved')

def event_still_trying(s):
	print('still_trying')

def event_player_death(s):
	print('player_death')

def event_game_start(s):
	print('game_start')

def event_boot(s):
	print('boot')

handler_map = {
	0xfb: event_nvram_dump,
	0xfa: event_wave_change,
	0xf9: event_enforcer_shot,
	0xf8: event_score_changed,
	0xf7: event_game_over,
	0xf6: event_grunt_killed_by_electrode,
	0xf5: event_human_killed,
	0xf4: event_human_saved,
	0xf3: event_still_trying,
	0xf2: event_player_death,
	0xf1: event_game_start,
	0xf0: event_boot,
}

s = serial.Serial('/dev/ttyUSB0', 115200)

while True:
	c = s.read(1)[0]
	print('%02x' % c)

	if c in handler_map:
		handler_map[c](s)
