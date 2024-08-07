/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*-------------------------------------------------------------------------
 *
 * proclist_types.h
 *		doubly-linked lists of pgprocnos
 *
 * See proclist.h for functions that operate on these types.
 *
 * Portions Copyright (c) 2016-2020, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *		src/include/storage/proclist_types.h
 *-------------------------------------------------------------------------
 */

#ifndef PROCLIST_TYPES_H
#define PROCLIST_TYPES_H

/*
 * A node in a doubly-linked list of processes.  The link fields contain
 * the 0-based PGPROC indexes of the next and previous process, or
 * INVALID_PGPROCNO in the next-link of the last node and the prev-link
 * of the first node.  A node that is currently not in any list
 * should have next == prev == 0; this is not a possible state for a node
 * that is in a list, because we disallow circularity.
 */
typedef struct proclist_node
{
	int			next;			/* pgprocno of the next PGPROC */
	int			prev;			/* pgprocno of the prev PGPROC */
} proclist_node;

/*
 * Header of a doubly-linked list of PGPROCs, identified by pgprocno.
 * An empty list is represented by head == tail == INVALID_PGPROCNO.
 */
typedef struct proclist_head
{
	int			head;			/* pgprocno of the head PGPROC */
	int			tail;			/* pgprocno of the tail PGPROC */
} proclist_head;

/*
 * List iterator allowing some modifications while iterating.
 */
typedef struct proclist_mutable_iter
{
	int			cur;			/* pgprocno of the current PGPROC */
	int			next;			/* pgprocno of the next PGPROC */
} proclist_mutable_iter;

#endif							/* PROCLIST_TYPES_H */
