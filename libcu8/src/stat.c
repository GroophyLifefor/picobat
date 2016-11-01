/*

 libcu8 - A wrapper to fix msvcrt utf8 incompatibilities issues
 Copyright (c) 2014, 2015, 2016 Romain GARBI

 All rights reserved.
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * Neither the name of the name of Romain Garbi nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY ROMAIN GARBI AND CONTRIBUTORS ``AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL ROMAIN GARBI AND CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES
 LOSS OF USE, DATA, OR PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <windows.h>
#include <assert.h>
#include <sys/stat.h>

#include "internals.h"
#include "libcu8.h"

/* build libcu8_stat32 */
#ifdef HAVE__STAT32
#define __libcu8_stat libcu8_stat32
#define __libcu8_stat_t _stat32
#define __libcu8_wstat _wstat32
#include "stat_gen.c"
#undef __libcu8_stat
#undef __libcu8_stat_t
#undef __libcu8_wstat
#endif

#ifdef HAVE__STAT32I64
/* build libcu8_stat32i64 */
#define __libcu8_stat libcu8_stat32i64
#define __libcu8_stat_t _stat32i64
#define __libcu8_wstat _wstat32i64
#include "stat_gen.c"
#undef __libcu8_stat
#undef __libcu8_stat_t
#undef __libcu8_wstat
#endif

#ifdef HAVE__STAT64
/* build libcu8_stat64 */
#define __libcu8_stat libcu8_stat64
#define __libcu8_stat_t __stat64
#define __libcu8_wstat _wstat64
#include "stat_gen.c"
#undef __libcu8_stat
#undef __libcu8_stat_t
#undef __libcu8_wstat
#endif

#ifdef HAVE__STAT64I32
/* build libcu8_stat64i32 */
#define __libcu8_stat libcu8_stat64i32
#define __libcu8_stat_t _stat64i32
#define __libcu8_wstat _wstat64i32
#include "stat_gen.c"
#undef __libcu8_stat
#undef __libcu8_stat_t
#undef __libcu8_wstat
#endif

#ifdef HAVE__STAT
/* build libcu8_stat */
#define __libcu8_stat libcu8_stat
#define __libcu8_stat_t _stat
#define __libcu8_wstat _wstat
#include "stat_gen.c"
#endif
