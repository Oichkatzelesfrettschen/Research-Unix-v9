static char *sccsid = "@(#)chmod.c	4.1 (Berkeley) 10/1/80";
/*
 * chmod [ugoa][+-=][rwxstugo] files
 *  change mode of files
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef S_ICCTYP
#define S_ICCTYP 0007000
#define S_ISYNC  0001000
#define S_IEXCL  0003000
#endif

#define	USER	04700	/* user's bits */
#define	GROUP	02070	/* group's bits */
#define	OTHER	00007	/* other's bits */
#define	ALL	01777	/* all (note absence of setuid, etc) */

#define	READ	00444	/* read permit */
#define	WRITE	00222	/* write permit */
#define	EXEC	00111	/* exec permit */
#define	SETID	06000	/* set[ug]id */

#define CONC	01000	/* concurrency control */

char	*ms;
struct	stat st;
int	cc;		/* type of concurrency control requested */

main(argc,argv)
char **argv;
{
	register i;
	register char *p;
	int status = 0;

	if (argc < 3) {
		fprintf(stderr, "Usage: chmod [ugoa][+-=][rwxstugo] file ...\n");
		exit(255);
	}
	ms = argv[1];
	newmode(0);
	for (i = 2; i < argc; i++) {
		p = argv[i];
		if (stat(p, &st) < 0) {
			fprintf(stderr, "chmod: can't access %s\n", p);
			++status;
			continue;
		}
		ms = argv[1];
		if (chmod(p, newmode(st.st_mode)) < 0) {
			fprintf(stderr, "chmod: can't change %s\n", p);
			++status;
			continue;
		}
	}
	exit(status);
}

newmode(nm)
unsigned nm;
{
	register o, m, b;

        m = abs_octal();
	if (!*ms)
		return(m);
	do {
		m = who();
		while (o = what()) {
			b = where(nm);
			if (cc || (b&SETID) && (nm&CONC))
				nm &= ~S_ICCTYP;
			switch (o) {
			case '+':
				nm |= (b & m) | cc;
				break;
			case '-':
				nm &= ~(b & m);
				break;
			case '=':
				nm &= ~m;
				nm |= (b & m) | cc;
				break;
			}
		}
	} while (*ms++ == ',');
	if (*--ms) {
		fprintf(stderr, "chmod: invalid mode\n");
		exit(255);
	}
	return(nm);
}

abs_octal()
{
	register c, i;

	i = 0;
	while ((c = *ms++) >= '0' && c <= '7')
		i = (i << 3) + (c - '0');
	ms--;
	return(i);
}

who()
{
	register m;

	m = 0;
	for (;;) switch (*ms++) {
	case 'u':
		m |= USER;
		continue;
	case 'g':
		m |= GROUP;
		continue;
	case 'o':
		m |= OTHER;
		continue;
	case 'a':
		m |= ALL;
		continue;
	default:
		ms--;
		if (m == 0)
			m = ALL;
		return m;
	}
}

what()
{
	switch (*ms) {
	case '+':
	case '-':
	case '=':
		return *ms++;
	}
	return(0);
}

where(om)
register om;
{
	register m;

	m = 0;
	switch (*ms) {
	case 'u':
		m = (om & USER) >> 6;
		goto dup;
	case 'g':
		m = (om & GROUP) >> 3;
		goto dup;
	case 'o':
		m = (om & OTHER);
	dup:
		m &= (READ|WRITE|EXEC);
		m |= (m << 3) | (m << 6);
		++ms;
		return m;
	}
	for (;;) switch (*ms++) {
	case 'y':
		cc = S_ISYNC;
		continue;
	case 'e':
		cc = S_IEXCL;
		continue;
	case 'r':
		m |= READ;
		continue;
	case 'w':
		m |= WRITE;
		continue;
	case 'x':
		m |= EXEC;
		continue;
	case 's':
		m |= SETID;
		continue;
	default:
		ms--;
		return m;
	}
}
