	.data
	.asciz	"@(#)srt0.s	1.1 86/02/03	Copyr 1985 Sun Micro";
	.even
| This variable is used in the DELAY macro.  5 is the right value for
| 68010's running 10MHz.  3 is the right value for 68020's running 16MHz
| with cache on.  (4x as fast at doing the delay loop.)  Other values
| should be empirically determined as needed.  Srt0.s sets this value
| based on the actual runtime environment encountered.
|
| It's critical that the value be no SMALLER than required, e.g. the
| DELAY macro guarantees a MINIMUM delay, not a maximum.
	.globl	_cpudelay
_cpudelay:
	.long	5			| Multiplier for DELAY macro.

	.text

|	Copyright (c) 1985 by Sun Microsystems, Inc.

|
| Startup code for standalone system
|

	.globl	_end
	.globl	_edata
	.globl	_main
	.globl	__exit
	.globl	__exitto

HIGH = 0x2700

CACR_CLEAR = 0x8
CACR_ENABLE = 0x1

	.globl	entry
entry:
	movw	#HIGH,sr		| just in case
leax:	lea	pc@(entry-(leax+2)),a0	| True current location of "entry"
	lea	entry:l,a1		| Desired      location of "entry"
	cmpl	a0,a1
	jeq	gobegin			| If the same, just go do it.
	movl	#_end,d0		| Desired end of program
	subl	a1,d0			| Calculate length, round up.
	lsrl	#2,d0
movc:	movl	a0@+,a1@+		| Move it where it belongs.
	dbra	d0,movc

|	Test if 68020: Use the 68020 scaled indexing mode, which is ignored by
|	68000/68010.
gobegin:
	moveq	#is020-is010,d0		| Set up jump offset
|	jmp	pc@((is010-.)-(is020-is010),d0:w*2)	| Jump if 68010
	.word	0x4EFB
J:	.word	0x0200+(is010-J)-(is020-is010)
is020:
| Turn on the cache...
	moveq	#CACR_CLEAR+CACR_ENABLE,d0
	.long	0x4e7b0002		| movc	d0,cacr
	moveq	#3,d0			| Set the delay factor for 68020
	movl	d0,_cpudelay
is010:
	jmp	begin:l			| Force non-PCrel jump

begin:
	movl	sp,entry-4:l		| Save old stack pointer value
	lea	entry-4:l,sp		| Set up new stack below load point
	movl	#_edata,a0
clr:
	clrl	a0@+
	cmpl	#_end,a0
	ble	clr
#ifdef SUN2
	jsr	_setkcontext
#endif
	clrl	sp@-			| argv = 0 for now.
	clrl	sp@-			| argc = 0 for now.
	jsr	_main
	addqw	#8,sp
	jsr	_exit			| after main() returns, call exit().
| Just fall thru into __exit if exit() ever returns.

__exit:
	movl	entry-4:l,sp		| Restore caller's stack pointer
	rts				| Return to caller (PROM probably)

| Transfer control to a new program, passing on the same arguments that
| we (srt0.s) received when we were started.  Note that this does NOT call
| exit() -- you'd better close your files yourself.
__exitto:
	movl	sp@(4),a0		| Address to call
	movl	entry-4:l,sp		| Restore caller's stack pointer
	jra	a0@			| Jump to callee using caller's stk
