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
 * pg_range.c
 *	  routines to support manipulation of the pg_range relation
 *
 * Portions Copyright (c) 1996-2020, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/catalog/pg_range.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/genam.h"
#include "access/htup_details.h"
#include "access/table.h"
#include "catalog/dependency.h"
#include "catalog/indexing.h"
#include "catalog/pg_collation.h"
#include "catalog/pg_opclass.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_range.h"
#include "catalog/pg_type.h"
#include "utils/fmgroids.h"
#include "utils/rel.h"


/*
 * RangeCreate
 *		Create an entry in pg_range.
 */
void
RangeCreate(Oid rangeTypeOid, Oid rangeSubType, Oid rangeCollation,
			Oid rangeSubOpclass, RegProcedure rangeCanonical,
			RegProcedure rangeSubDiff)
{
	Relation	pg_range;
	Datum		values[Natts_pg_range];
	bool		nulls[Natts_pg_range];
	HeapTuple	tup;
	ObjectAddress myself;
	ObjectAddress referenced;

	pg_range = table_open(RangeRelationId, RowExclusiveLock);

	memset(nulls, 0, sizeof(nulls));

	values[Anum_pg_range_rngtypid - 1] = ObjectIdGetDatum(rangeTypeOid);
	values[Anum_pg_range_rngsubtype - 1] = ObjectIdGetDatum(rangeSubType);
	values[Anum_pg_range_rngcollation - 1] = ObjectIdGetDatum(rangeCollation);
	values[Anum_pg_range_rngsubopc - 1] = ObjectIdGetDatum(rangeSubOpclass);
	values[Anum_pg_range_rngcanonical - 1] = ObjectIdGetDatum(rangeCanonical);
	values[Anum_pg_range_rngsubdiff - 1] = ObjectIdGetDatum(rangeSubDiff);

	tup = heap_form_tuple(RelationGetDescr(pg_range), values, nulls);

	CatalogTupleInsert(pg_range, tup);
	heap_freetuple(tup);

	/* record type's dependencies on range-related items */

	myself.classId = TypeRelationId;
	myself.objectId = rangeTypeOid;
	myself.objectSubId = 0;

	referenced.classId = TypeRelationId;
	referenced.objectId = rangeSubType;
	referenced.objectSubId = 0;
	recordDependencyOn(&myself, &referenced, DEPENDENCY_NORMAL);

	referenced.classId = OperatorClassRelationId;
	referenced.objectId = rangeSubOpclass;
	referenced.objectSubId = 0;
	recordDependencyOn(&myself, &referenced, DEPENDENCY_NORMAL);

	if (OidIsValid(rangeCollation))
	{
		referenced.classId = CollationRelationId;
		referenced.objectId = rangeCollation;
		referenced.objectSubId = 0;
		recordDependencyOn(&myself, &referenced, DEPENDENCY_NORMAL);
	}

	if (OidIsValid(rangeCanonical))
	{
		referenced.classId = ProcedureRelationId;
		referenced.objectId = rangeCanonical;
		referenced.objectSubId = 0;
		recordDependencyOn(&myself, &referenced, DEPENDENCY_NORMAL);
	}

	if (OidIsValid(rangeSubDiff))
	{
		referenced.classId = ProcedureRelationId;
		referenced.objectId = rangeSubDiff;
		referenced.objectSubId = 0;
		recordDependencyOn(&myself, &referenced, DEPENDENCY_NORMAL);
	}

	table_close(pg_range, RowExclusiveLock);
}


/*
 * RangeDelete
 *		Remove the pg_range entry for the specified type.
 */
void
RangeDelete(Oid rangeTypeOid)
{
	Relation	pg_range;
	ScanKeyData key[1];
	SysScanDesc scan;
	HeapTuple	tup;

	pg_range = table_open(RangeRelationId, RowExclusiveLock);

	ScanKeyInit(&key[0],
				Anum_pg_range_rngtypid,
				BTEqualStrategyNumber, F_OIDEQ,
				ObjectIdGetDatum(rangeTypeOid));

	scan = systable_beginscan(pg_range, RangeTypidIndexId, true,
							  NULL, 1, key);

	while (HeapTupleIsValid(tup = systable_getnext(scan)))
	{
		CatalogTupleDelete(pg_range, &tup->t_self);
	}

	systable_endscan(scan);

	table_close(pg_range, RowExclusiveLock);
}
