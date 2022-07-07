/*-------------------------------------------------------------------------
 *
 * parse_clause.h
 *	  handle clauses in parser
 *
 *
 * Portions Copyright (c) 1996-2017, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/parser/parse_clause.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PARSE_CLAUSE_H
#define PARSE_CLAUSE_H

#include "parser/parse_node.h"

extern void transformFromClause(ParseState *pstate, List *frmList);
extern RangeTblEntry *transformCTEReference(ParseState *pstate, RangeVar *r,
					CommonTableExpr *cte, Index levelsup);
extern int setTargetTable(ParseState *pstate, RangeVar *relation,
			   bool inh, bool alsoSource, AclMode requiredPerms);
extern bool interpretOidsOption(List *defList, bool allowOids);

extern Node *transformWhereClause(ParseState *pstate, Node *clause,
					 ParseExprKind exprKind, const char *constructName);
extern Node *transformLimitClause(ParseState *pstate, Node *clause,
					 ParseExprKind exprKind, const char *constructName);
extern List *transformGroupClause(ParseState *pstate, List *grouplist,
					 List **groupingSets,
					 List **targetlist, List *sortClause,
					 ParseExprKind exprKind, bool useSQL99);
extern List *transformSortClause(ParseState *pstate, List *orderlist,
					List **targetlist, ParseExprKind exprKind,
					bool useSQL99);

extern List *transformWindowDefinitions(ParseState *pstate,
						   List *windowdefs,
						   List **targetlist);

extern List *transformDistinctClause(ParseState *pstate,
						List **targetlist, List *sortClause, bool is_agg);
extern List *transformDistinctOnClause(ParseState *pstate, List *distinctlist,
						  List **targetlist, List *sortClause);
extern void transformOnConflictArbiter(ParseState *pstate,
						   OnConflictClause *onConflictClause,
						   List **arbiterExpr, Node **arbiterWhere,
						   Oid *constraint);

extern List *addTargetToSortList(ParseState *pstate, TargetEntry *tle,
					List *sortlist, List *targetlist, SortBy *sortby);
extern Index assignSortGroupRef(TargetEntry *tle, List *tlist);
extern bool targetIsInSortList(TargetEntry *tle, Oid sortop, List *sortList);

extern Query *transformCypherMatchClause(ParseState *pstate,
										 CypherClause *clause);
extern Query *transformCypherReturnClause(ParseState *pstate,
										  CypherClause *clause);

extern List *generateGroupClause(ParseState *pstate, List **targetlist,
								 List *sortClause);

#endif   /* PARSE_CLAUSE_H */
