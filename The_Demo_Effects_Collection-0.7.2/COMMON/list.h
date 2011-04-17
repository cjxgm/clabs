/* Copyright (C) 2003 W.P. van Paassen - peter@paassen.tmfweb.nl

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#ifndef TDEC_LIST_H
#define TDEC_LIST_H

typedef struct tdec_node
{
	const void *data;
	struct tdec_node *next_node;
	struct tdec_node *previous_node;	
} TDEC_NODE;

extern TDEC_NODE* TDEC_LIST_create(const void *data);
extern void TDEC_LIST_append(TDEC_NODE *start, const void *data);
extern void TDEC_LIST_remove(TDEC_NODE *start, const void *data);
extern void TDEC_LIST_destroy(TDEC_NODE *start);
extern const void* TDEC_LIST_get_data_next(TDEC_NODE **node);

#endif
