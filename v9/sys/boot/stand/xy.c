/*
 * @(#)xy.c 1.1 86/02/03 Copyright (c) 1986 by Sun Microsystems, Inc.
 */

/* Standalone driver for Xylogics 440/450 */

#include "saio.h"
#include "param.h"
#include "../h/dkbad.h"
#include "../sun/dklabel.h"
#include "../sun/dkio.h"
#include "../ufs/fsdir.h"
#include "../sundev/xycreg.h"
#include "../sundev/xyreg.h"

extern char	msg_nolabel[];
/*
 * Magic constants used by the older driver code.  Defining them
 * symbolically clarifies their purpose.
 */
#define XY_ERR_UNDEFINED 0x1F   /* undocumented error code used during probe */
#define XY_BADCNT       126    /* number of entries in bad block table */


struct xyparam {
	unsigned short	xy_boff;	/* Cyl # starting partition */
 	unsigned short	xy_nsect;	/* Sect/track */
 	unsigned short	xy_ncyl;	/* Cyl/disk */
 	unsigned short	xy_nhead;	/* Heads/cyl */
 	unsigned short	xy_bhead;	/* Base head # for removable disk */
 	unsigned short	xy_drive;	/* Xylogics drive type ID */
	int	xy_nblk;
	int	xy_badaddr;		/* Disk block # of bad-block table */
	struct dkbad xy_bad;
	unsigned char	xy_unit;
};

#define CYL(p)  (p*xyp->xy_nsect*xyp->xy_nhead)     /* cyl to block conv */

#define NSTD 	2
unsigned long xystd[NSTD] = { 0xee40, 0xee48 };

/*
 * Structure of our DMA area
 */
struct xydma {
	struct xyiopb	xyiopb;
	char		xyblock[MAXBSIZE];	/* R/W data */
};

/*
 * What resources we need to run
 */
struct devinfo xyinfo = {
	sizeof (struct xydevice), 
	sizeof (struct xydma),
	sizeof (struct xyparam),
	NSTD,
	xystd,
	MAP_MBIO,
#ifdef BOOTBLOCK
	DEV_BSIZE,
#else
	DEV_BSIZE,	/* MAXBSIZE, */
#endif
};

/*
 * What facilities we export to the world
 */
int	xyopen(), xystrategy();
extern int	xxboot(), xxprobe();
extern int	nullsys();

struct boottab xydriver = {
	"xy",	xxprobe, xxboot, xyopen, nullsys, xystrategy,
	"xy: Xylogics 440/450 disk", &xyinfo,
}; 


/*
 * Open a xylogics disk.
 */
xyopen(sip)
	register struct saioreq *sip;
{
	register struct xyparam *xyp;
	register struct xydevice *xyaddr;
	register struct xyiopb *xy;
	struct dk_label *label;
	register int i, t;
	u_short ppart;
#ifndef BOOTBLOCK
	int xyspin();
#endif !BOOTBLOCK

	xyp = (struct xyparam *)sip->si_devdata;
	xyaddr = (struct xydevice *)sip->si_devaddr;
	xy = &((struct xydma *)sip->si_dmaaddr)->xyiopb;

	xyp->xy_unit = sip->si_unit & 0x03;
	ppart = (sip->si_unit >> 2) & 1;

	/* reset controller */
	i = xyaddr->xy_resupd;
#ifdef lint
	i = i;		/* Avoid complaint about "set but not used" */
#endif
	DELAY(100);	/* Allow for controller recovery time */

	xyp->xy_boff  = 0;		/* Don't offset block numbers */
	xyp->xy_nsect = 2;
	xyp->xy_nhead = 2;
	xyp->xy_ncyl = 2;
	xyp->xy_bad.bt_mbz = -1;

#ifndef BOOTBLOCK
	/*
	 * Wait for disk to spin up, if necessary.
	 */
	if (!isspinning(xyspin, (char *)sip, 0))
		return (-1);
#endif !BOOTBLOCK

	for (xyp->xy_drive = 0; xyp->xy_drive < NXYDRIVE; xyp->xy_drive++) {
		label = (struct dk_label *)
			((struct xydma *)sip->si_dmaaddr)->xyblock;
		label->dkl_magic = 0;
		if (xycmd(XY_READ, sip, 0, (char *)label, 1))
			continue;
		if (chklabel(label))
			continue;
		if (ppart != label->dkl_ppart)
			continue;
		goto foundlabel;
	}
	printf(msg_nolabel);
	return (-1);

foundlabel:
	xyp->xy_nhead = label->dkl_nhead;
	xyp->xy_nsect = label->dkl_nsect;
	xyp->xy_ncyl  = label->dkl_ncyl + label->dkl_acyl;
	xyp->xy_boff  = label->dkl_map[sip->si_boff].dkl_cylno;
	xyp->xy_nblk  = label->dkl_map[sip->si_boff].dkl_nblk;

	bzero((char *)xy, sizeof (struct xyiopb));
	xy->xy_reloc = 1;
	xy->xy_cmd = XY_INIT;
	xy->xy_drive = xyp->xy_drive;
	xy->xy_unit = xyp->xy_unit & 3;
	xy->xy_head = xyp->xy_nhead - 1;
	xy->xy_cylinder = xyp->xy_ncyl - 1;
	xy->xy_sector = xyp->xy_nsect - 1;
	xy->xy_bhead = label->dkl_bhead;

	t = XYREL(xyaddr, MB_DMA_ADDR(xy));
	xyaddr->xy_iopbrel[0] = t >> 8;
	xyaddr->xy_iopbrel[1] = t;
	xyaddr->xy_iopboff[0] = MB_DMA_ADDR(xy) >> 8;
	xyaddr->xy_iopboff[1] = MB_DMA_ADDR(xy);
	xyaddr->xy_csr = XY_GO;

	do {
		DELAY(30);
	} while (xyaddr->xy_csr & XY_BUSY);
	if (xy->xy_iserr)
		printf("xy: init error %x\n", xy->xy_errno);
	/*
	 * Fetch bad block info.
	 */
	xyp->xy_badaddr = ((int)xyp->xy_ncyl * xyp->xy_nhead - 1)
			  * xyp->xy_nsect;
	if (xycmd(XY_READ, sip, xyp->xy_badaddr, (char *)&xyp->xy_bad, 1))
		printf("xy: no bad block info\n");
	return (0);
}

xystrategy(sip, rw)
	struct saioreq *sip;
	int rw;
{
	register int cmd = (rw == WRITE) ? XY_WRITE : XY_READ;
	register int boff;
	register struct xyparam *xyp = (struct xyparam *)sip->si_devdata;
	register short nsect;
#ifndef BOOTBLOCK
	char *ma;
	int i, bn;
#endif BOOTBLOCK

	boff = CYL(xyp->xy_boff);
#ifdef BOOTBLOCK
	/* assert si_cc == DEV_BSIZE */
	if (xycmd(cmd, sip, sip->si_bn + boff, sip->si_ma, 1))
		return (-1);
	return (sip->si_cc);
#else BOOTBLOCK
	nsect = sip->si_cc / DEV_BSIZE;
	if (sip->si_bn + nsect > xyp->xy_nblk)
		nsect = xyp->xy_nblk - sip->si_bn;
	if (nsect == 0)
		return (0);
	if (xycmd(cmd, sip, sip->si_bn+boff, sip->si_ma, nsect) == 0)
		return (nsect*DEV_BSIZE);
	/*
	 * Large transfer failed, now do one at a time
	 */
	bn = sip->si_bn + boff;
	ma = sip->si_ma;
	for (i = 0; i < nsect; i++) {
		if (xycmd(cmd, sip, bn, ma, 1))
			return (-1);
		bn++;
		ma += DEV_BSIZE;
	}
	return (nsect*DEV_BSIZE);
#endif	BOOTBLOCK
}

#ifndef BOOTBLOCK
/*
 * This routine is called from isspinning() as the test condition.
 */
int
xyspin(sip, dummy)
	struct saioreq *sip;
	int dummy;
{
	register struct xyiopb *xy = &((struct xydma *)sip->si_dmaaddr)->xyiopb;
 
	(void) xycmd(XY_STATUS, sip, 0, (char *)0, 0);
	return ((xy->xy_status & XY_READY) ? 0 : 1);
}
#endif !BOOTBLOCK

xycmd(cmd, sip, bno, buf, nsect)
	int cmd;
	struct saioreq *sip;
	register int bno;
	char *buf;
	int nsect;
{
	register int i, t;
	register struct xyparam *xyp = (struct xyparam *)sip->si_devdata;
	register struct xyiopb *xy = &((struct xydma *)sip->si_dmaaddr)->xyiopb;
	register struct xydevice *xyaddr = (struct xydevice *)sip->si_devaddr;
	register char *bp = ((struct xydma *)sip->si_dmaaddr)->xyblock;
	unsigned short cylno, sect, head;
	int error, errcnt = 0;

	cylno = bno / CYL(1);
	sect = bno % xyp->xy_nsect;
	head = (bno / xyp->xy_nsect) % xyp->xy_nhead;

	if (cmd == XY_WRITE && buf != bp)	/* Many just use common buf */
		bcopy(buf, bp, nsect*DEV_BSIZE);

retry:
	bzero((char *)xy, sizeof (struct xyiopb));
	xy->xy_reloc = 1;
	xy->xy_autoup = 1;
	xy->xy_cmd = cmd;
	xy->xy_drive = xyp->xy_drive;
	xy->xy_unit = xyp->xy_unit & 3;
	xy->xy_eccmode = 2;	/* means 0 on 440 */
	xy->xy_throttle = XY_THROTTLE;
	xy->xy_sector = sect;
	xy->xy_head = head;
	xy->xy_cylinder = cylno;
	xy->xy_nsect = nsect;
	xy->xy_bufoff = XYOFF(MB_DMA_ADDR(bp));
	xy->xy_bufrel = XYREL(xyaddr, MB_DMA_ADDR(bp));

	t = XYREL(xyaddr, MB_DMA_ADDR(xy));
	xyaddr->xy_iopbrel[0] = t >> 8;
	xyaddr->xy_iopbrel[1] = t;
	xyaddr->xy_iopboff[0] = MB_DMA_ADDR(xy) >> 8;
	xyaddr->xy_iopboff[1] = MB_DMA_ADDR(xy);
	xyaddr->xy_csr = XY_GO;

	do {
		DELAY(30);
	} while (xyaddr->xy_csr & XY_BUSY);

	if (xyaddr->xy_csr & XY_ERROR) {
		xyaddr->xy_csr = XY_ERROR;
		DELAY(100);
		if (bno != 0)
			printf("xy: hard error bno %d\n", bno);
		if (cmd != XY_RESET)
			(void) xycmd(XY_RESET, sip, 0, (char *)0, 0);
		return (-1);
	}
	if (xy->xy_iserr && xy->xy_errno != XY_ERR_UNDEFINED) {	
		if (nsect != 1)		/* only try hard on single sectors */
			return (-1);
		error = xy->xy_errno;
		if ((i = xyfwd(xyp, cylno, head, sect)) != 0)
			return xycmd(cmd, sip, i, buf, 1);
		/* Attempt to reset the error condition */
		if (cmd != XY_RESET)
			(void) xycmd(XY_RESET, sip, 0, (char *)0, 0);
		if (++errcnt < 5)
			goto retry;
		if (bno != 0 || error != 5)	/* drive type probe */
			printf("xy: error %x bno %d\n", error, bno);
		return (-1);			/* Error */
	}

	if (cmd == XY_READ && buf != bp)	/* Many just use common buf */
		bcopy(bp, buf, nsect*DEV_BSIZE);

	return (0);
}

xyfwd(xyp, cn, tn, sn)
	register struct xyparam *xyp;
	int cn, tn, sn;
{
	register struct dkbad *bt = &xyp->xy_bad;
	register int i;

	if (bt->bt_mbz != 0)	/* not initialized */
		return (0);
	for (i=0; i<XY_BADCNT; i++)		
		if (bt->bt_bad[i].bt_cyl == cn &&
		    bt->bt_bad[i].bt_trksec == (tn<<8)+sn) {
			return (xyp->xy_badaddr - i - 1);
		}
	return (0);
}
