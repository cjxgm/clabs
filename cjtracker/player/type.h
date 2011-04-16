/** type.h
 * CJTM Player: Type
 */

#ifndef __CJTMPLAY_TYPE__
#define __CJTMPLAY_TYPE__

typedef unsigned char	byte;
typedef unsigned int	uint;

typedef struct _mod		mod_t;
typedef struct _pat		pat_t;
typedef struct _chn		chn_t;
typedef struct _note	note_t;

struct _note {
	byte	note;
	byte	ins_id;		// Instrument ID
	byte	vol;		// Volumn

	// FOLLOWINGS ONLY APPEAR IN MEMORY
	float	freq;		// Frequency
	float	frame;
};

// Channel
struct _chn {
	note_t	notes[64];
};

// Pattern
struct _pat {
	chn_t	chns[4];
};

// Module
struct _mod {
	byte	bpm;
	pat_t	pats[1];
};

#endif
