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
 * subscriptioncmds.h
 *	  prototypes for subscriptioncmds.c.
 *
 *
 * Portions Copyright (c) 1996-2020, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/commands/subscriptioncmds.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef SUBSCRIPTIONCMDS_H
#define SUBSCRIPTIONCMDS_H

#include "catalog/objectaddress.h"
#include "nodes/parsenodes.h"

extern ObjectAddress CreateSubscription(CreateSubscriptionStmt *stmt,
										bool isTopLevel);
extern ObjectAddress AlterSubscription(AlterSubscriptionStmt *stmt);
extern void DropSubscription(DropSubscriptionStmt *stmt, bool isTopLevel);

extern ObjectAddress AlterSubscriptionOwner(const char *name, Oid newOwnerId);
extern void AlterSubscriptionOwner_oid(Oid subid, Oid newOwnerId);

#endif							/* SUBSCRIPTIONCMDS_H */
