#include <machine/sunromvec.h>

/*
 * Boot device configuration table.  Each entry points to a
 * driver that can be used by the standalone boot program.
 */

extern struct boottab xydriver;
extern struct boottab sddriver;
extern struct boottab stdriver;
extern struct boottab mtdriver;
extern struct boottab xtdriver;

/*
 * The device table 
 */
struct boottab *(devsw[]) = {
	&sddriver,
	&stdriver,
	(struct boottab *)0,
};
