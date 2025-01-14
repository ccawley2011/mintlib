/*
    CD-ROM Fcntl()s and structures

    Copyright (c) Julian F. Reschke (jr@ms.maus.de), 16. Mai 1994
    Free distribution and usage allowed as long as the file remains 
    unchanged.

    These Fcntl() opcodes provide a *nix-like interface to the CD-ROM     
    specific functions. They can be used either with a MiNT specific     
    CD-ROM device driver or with new MetaDOS BOS lowlevel drivers. For 
    MiNT mounted device drivers, the interface is

        Fcntl (short filehandle, short opcode, long arg)

    (with filehandle a handle to the opened CDROM device driver) or

        Dcntl (short opcode, char *name, long arg)

    (with name being the name of a file or directory on the mounted CD 
    file system).

    For MetaDOS BOS drivers, the interface is

        xbios (0x37, short device, long magic, short opcode, long arg)

    where 'device' is a MetaDOS XBIOS device number ('A'..'Z') and 'magic' 
    is 'FCTL'. With MetaDOS version >= 2.40 and the appropriate DOS 
    drivers, you can also use the GEMDOS call Dcntl (thus being compatible 
    to future MiNT drivers), because MetaDOS 2.40 implements some of the 
    new GEMDOS calls and Atari's ISO9660F.DOS passes all Dcntl calls right 
    down to the BOS driver.

    Note that there is an 'old' CD-ROM audio interface for MetaDOS, but 
    the commands depend on the custom CDAR 504 controller and can't be 
    fully implemented for SCSI drives (in fact they are, with some 
    restrictions). Consult the MetaDOS developers manual for further 
    information.

    In both cases, EINVFN (-32L) is returned for unknown opcodes.

    Note that MetaDOS drivers return EUNKNOWN (-3) if XBIOS opcode 0x37 is 
    not supported!

    Drivers supporting this interface (send updates to jr@ms.maus.de):

    MetaDOS BOS drivers (driver name, company):

    	HS-CDROM.BOS
    		Hard &Soft, Castrop-Rauxel, Germany
    		Generic driver for SCSI-CDROMs connected to ACSI or SCSI and 
    		for the CDAR504

		FX001???.BOS, CDU33???.BOS
			Gellermann & Fellmuth GbR, Berlin, Germany
			Drivers for Mitsumi drives connected to internal or parallel 
			interface.

    MiNT device drivers (driver name, company):
*/

#ifndef _CDROMIO_H
#define _CDROMIO_H

__BEGIN_DECLS

typedef union
{ 
	struct {
    	unsigned char  reserved, minute, second, frame;
    } msf;
	long lba;
} cd_ad;

/* Data structures used */

struct cdrom_msf 
{
    unsigned char    cdmsf_min0;     /* start minute */
    unsigned char    cdmsf_sec0;     /* start second */
    unsigned char    cdmsf_frame0;   /* start frame */
    unsigned char    cdmsf_min1;     /* end minute */
    unsigned char    cdmsf_sec1;     /* end second */
    unsigned char    cdmsf_frame1;   /* end frame */
};

struct cdrom_ti 
{
    unsigned char    cdti_trk0;      /* start track */
    unsigned char    cdti_ind0;      /* start index */
    unsigned char    cdti_trk1;      /* end track */
    unsigned char    cdti_ind1;      /* end index */
};

struct cdrom_tochdr     
{
    unsigned char    cdth_trk0;      /* start track */
    unsigned char    cdth_trk1;      /* end track */
};

struct cdrom_tocentry 
{
    /* input parameters */

    unsigned char    cdte_track;     /* track number or CDROM_LEADOUT */
    unsigned char    cdte_format;    /* CDROM_LBA or CDROM_MSF */
    
    /* output parameters */

    unsigned    cdte_adr:4;     /* the SUBQ channel encodes 0: nothing,
                                    1: position data, 2: MCN, 3: ISRC,
                                    else: reserved */
    unsigned    cdte_ctrl:4;    /* bit 0: audio with pre-emphasis,
                                    bit 1: digital copy permitted,
                                    bit 2: data track,
                                    bit 3: four channel */
    unsigned int cdte_datamode:8;	/* currently not set */
	unsigned	short dummy;	/* PM: what is this for ? */
	cd_ad	cdte_addr;			/* track start */
};

struct cdrom_subchnl 
{
	/* input parameters */

    unsigned char    cdsc_format;		/* CDROM_MSF or CDROM_LBA */
    
    /* output parameters */
    
    unsigned char    cdsc_audiostatus;	/* see below */
    unsigned	cdsc_resvd:	8;	/* reserved */
    unsigned	cdsc_adr:   4;	/* see above */
    unsigned	cdsc_ctrl:  4;	/* see above */
    unsigned char    cdsc_trk;			/* current track */
    unsigned char    cdsc_ind;			/* current index */
	cd_ad	cdsc_absaddr;		/* absolute address */
	cd_ad	cdsc_reladdr;		/* track relative address */
};

struct cdrom_mcn 
{
    unsigned char    mcn_audiostatus;	/* see above */
    unsigned char    mcn_mcn[23];		/* Media catalog number as ASCII string */
};

struct cdrom_tisrc
{
	/* input parameters */

    unsigned char    tisrc_track;		/* track number */
    
    /* output parameters */
    
    unsigned char    tisrc_audiostatus;	/* see above */
    unsigned char    tisrc_tisrc[23];	/* Track International Standard
    								Recording Code (ASCII) */
};

struct cdrom_volctrl
{
    unsigned char    channel0;			/* volume level 0..255 */
    unsigned char    channel1;
    unsigned char    channel2;
    unsigned char    channel3;
};

struct cdrom_audioctrl
{
	/* input parameters */

    short	set;    /* 0 == inquire only */
    
    /* input/output parameters */
    
    struct {
        unsigned char selection;
        unsigned char volume;
    } channel[4];
};

struct cdrom_read      
{
    long    cdread_lba;			/* logical block address */
    char    *cdread_bufaddr;	/* buffer pointer */
    long    cdread_buflen;		/* byte count */
};


/* CD-ROM address types */

#define CDROM_LBA   0x01
#define CDROM_MSF   0x02

/* SUB Q control bits */

#define CDROM_AUDIO_EMPHASIS    0x01
#define CDROM_COPY_PERMITTED    0x02
#define CDROM_DATA_TRACK        0x04
#define CDROM_FOUR_CHANNEL      0x08

/* The leadout track is always 0xAA, regardless of # of tracks on disc */

#define CDROM_LEADOUT   0xAA

/* return value from READ SUBCHANNEL DATA */

#define CDROM_AUDIO_INVALID     0x00    /* audio status not supported */
#define CDROM_AUDIO_PLAY        0x11    /* audio play operation in progress */
#define CDROM_AUDIO_PAUSED      0x12    /* audio play operation paused */
#define CDROM_AUDIO_COMPLETED   0x13    /* audio play successfully completed */
#define CDROM_AUDIO_ERROR       0x14    /* audio play stopped due to error */
#define CDROM_AUDIO_NO_STATUS   0x15    /* no current audio status to return */

/* CD-ROM Fcntl opcodes */

/* Get block number of first sector in last session of a multisession
   CD. Argument points to a LONG. Used by iso9660f.dos */
#define CDROMREADOFFSET     (('C'<<8)|0x00)

/* Pause audio operation */
#define CDROMPAUSE          (('C'<<8)|0x01)

/* Resume audio operation */
#define CDROMRESUME         (('C'<<8)|0x02)

/* Play audio. Argument points to cdrom_msf structure */
#define CDROMPLAYMSF        (('C'<<8)|0x03)

/* Play audio. Argument points to cdrom_ti structure */
#define CDROMPLAYTRKIND     (('C'<<8)|0x04)

/* Read header of table of contents. Argument points to cdrom_tochdr
   structure */
#define CDROMREADTOCHDR     (('C'<<8)|0x05)

/* Read a toc entry. Argument points to cdrom_tocentry structure */
#define CDROMREADTOCENTRY   (('C'<<8)|0x06)

/* Stops spindle motor */
#define CDROMSTOP           (('C'<<8)|0x07)

/* Starts spindle motor */
#define CDROMSTART          (('C'<<8)|0x08)

/* Eject medium */
#define CDROMEJECT          (('C'<<8)|0x09)

/* Sets audio playback volume. Argument points to cdrom_volctrl
   structure. Only for compatibility to Unix drivers, see also
   CDROMAUDIOCTRL */
#define CDROMVOLCTRL        (('C'<<8)|0x0a)

/* Read subchannel information. Argument points to cdrom_subchnl
   structure. */
#define CDROMSUBCHNL        (('C'<<8)|0x0b)

/* Read Mode 2 or 1 sectors. Argument points to cdrom_read
   structure. Blocks have either 2336 or 2048 bytes. */
#define CDROMREADMODE2      (('C'<<8)|0x0c)
#define CDROMREADMODE1      (('C'<<8)|0x0d)

/* Lock eject mechanism */
#define CDROMPREVENTREMOVAL (('C'<<8)|0x0e)

/* Unlock eject mechanism */
#define CDROMALLOWREMOVAL   (('C'<<8)|0x0f)

/* Control audio settings. Argument points to cdrom_audioctrl
   structure */
#define CDROMAUDIOCTRL      (('C'<<8)|0x10)

/* Read Digital Audio (red book) sectors. Argument points to
   cdrom_read structure. Blocks have 2352 bytes. */
#define CDROMREADDA         (('C'<<8)|0x11)

/* hard-reset the drive */
#define CDROMRESET          (('C'<<8)|0x12)

/* Read media catalog number. Argument points to cdrom_mcn
   structure */
#define CDROMGETMCN         (('C'<<8)|0x13)

/* Read track international standard recording code. Argument points
   to cdrom_tisrc structure */
#define CDROMGETTISRC       (('C'<<8)|0x14)

__END_DECLS

#endif  /* _CDROMIO_H */
