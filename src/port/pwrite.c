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
 * pwrite.c
 *	  Implementation of pwrite(2) for platforms that lack one.
 *
 * Portions Copyright (c) 1996-2020, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *	  src/port/pwrite.c
 *
 * Note that this implementation changes the current file position, unlike
 * the POSIX function, so we use the name pg_pwrite().
 *
 *-------------------------------------------------------------------------
 */


#include "c.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

ssize_t
pg_pwrite(int fd, const void *buf, size_t size, off_t offset)
{
#ifdef WIN32
	OVERLAPPED	overlapped = {0};
	HANDLE		handle;
	DWORD		result;

	handle = (HANDLE) _get_osfhandle(fd);
	if (handle == INVALID_HANDLE_VALUE)
	{
		errno = EBADF;
		return -1;
	}

	overlapped.Offset = offset;
	if (!WriteFile(handle, buf, size, &result, &overlapped))
	{
		_dosmaperr(GetLastError());
		return -1;
	}

	return result;
#else
	if (lseek(fd, offset, SEEK_SET) < 0)
		return -1;

	return write(fd, buf, size);
#endif
}
