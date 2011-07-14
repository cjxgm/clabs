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

#include <stdlib.h>
#include "list.h"

TDEC_NODE* TDEC_LIST_create(const void *data)
{
	TDEC_NODE *new_node = (TDEC_NODE*)malloc(sizeof(TDEC_NODE));
	
	new_node->data = data;
	new_node->next_node = (TDEC_NODE*)0;
	new_node->previous_node = (TDEC_NODE*)0;
	
	return new_node;
}

void TDEC_LIST_append(TDEC_NODE *start, const void *data)
{
	TDEC_NODE *n = start;
	TDEC_NODE *new_node = (TDEC_NODE*)malloc(sizeof(TDEC_NODE));
	new_node->data = data;
	new_node->next_node = (TDEC_NODE*)0;
		
	while(n->next_node != (TDEC_NODE*)0)
	{
		n = n->next_node;
	}

	n->next_node = new_node;
	new_node->previous_node = n;		
}

void TDEC_LIST_remove(TDEC_NODE *start, const void *data)
{
	TDEC_NODE *n = start;
	
	if (start->data == data)
	{
		/* remove first node, so second node becomes first */
		if (n->next_node)
		{
			TDEC_NODE *second = start->next_node;
			start->data = start->next_node->data;
			start->next_node = start->next_node->next_node;
			free(second);
		}
		else
		{
			/* destroy list */
			free(start);
		}
	}
	else
	{		
		while(n->data != data)
		{
			n = n->next_node;
		}
	
		n->previous_node->next_node = n->next_node;
		n->next_node->previous_node = n->previous_node;
		free(n);	
	}
}

void TDEC_LIST_destroy(TDEC_NODE *start)
{
	if (start->next_node != (TDEC_NODE*)0)
	{
		TDEC_LIST_destroy(start->next_node);
	}

	free(start);
}

const void* TDEC_LIST_get_data_next(TDEC_NODE **node)
{
	const void *data = (*node)->data;
	*node = (*node)->next_node;
	return data;
}
