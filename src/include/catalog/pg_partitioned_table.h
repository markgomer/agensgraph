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
 * pg_partitioned_table.h
 *	  definition of the "partitioned table" system catalog
 *	  (pg_partitioned_table)
 *
 *
 * Portions Copyright (c) 1996-2020, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/catalog/pg_partitioned_table.h
 *
 * NOTES
 *	  The Catalog.pm module reads this file and derives schema
 *	  information.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_PARTITIONED_TABLE_H
#define PG_PARTITIONED_TABLE_H

#include "catalog/genbki.h"
#include "catalog/pg_partitioned_table_d.h"

/* ----------------
 *		pg_partitioned_table definition.  cpp turns this into
 *		typedef struct FormData_pg_partitioned_table
 * ----------------
 */
CATALOG(pg_partitioned_table,3350,PartitionedRelationId)
{
	Oid			partrelid;		/* partitioned table oid */
	char		partstrat;		/* partitioning strategy */
	int16		partnatts;		/* number of partition key columns */
	Oid			partdefid;		/* default partition oid; InvalidOid if there
								 * isn't one */

	/*
	 * variable-length fields start here, but we allow direct access to
	 * partattrs via the C struct.  That's because the first variable-length
	 * field of a heap tuple can be reliably accessed using its C struct
	 * offset, as previous fields are all non-nullable fixed-length fields.
	 */
	int2vector	partattrs;		/* each member of the array is the attribute
								 * number of a partition key column, or 0 if
								 * the column is actually an expression */

#ifdef CATALOG_VARLEN
	oidvector	partclass;		/* operator class to compare keys */
	oidvector	partcollation;	/* user-specified collation for keys */
	pg_node_tree partexprs;		/* list of expressions in the partition key;
								 * one item for each zero entry in partattrs[] */
#endif
} FormData_pg_partitioned_table;

/* ----------------
 *		Form_pg_partitioned_table corresponds to a pointer to a tuple with
 *		the format of pg_partitioned_table relation.
 * ----------------
 */
typedef FormData_pg_partitioned_table *Form_pg_partitioned_table;

#endif							/* PG_PARTITIONED_TABLE_H */
