/*
** ATOP - System & Process Monitor
**
** The program 'atop' offers the possibility to view the activity of
** the system on system-level as well as process-level.
**
** Include-file describing process-level counters maintained and functions
** to access the process-database.
** ================================================================
** Author:      Gerlof Langeveld
** E-mail:      gerlof.langeveld@atoptool.nl
** Date:        November 1996
** LINUX-port:  June 2000
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 2, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
*/

#define	PNAMLEN		15
#define	CMDLEN		150

/* 
** structure containing only relevant process-info extracted 
** from kernel's process-administration
*/
struct pstat {
	/* GENERAL PROCESS INFO 					*/
	struct gen {
		int	pid;		/* process identification 	*/
		int	ppid;           /* parent process identification*/
		int	ruid;		/* real  user  identification 	*/
		int	euid;		/* eff.  user  identification 	*/
		int	suid;		/* saved user  identification 	*/
		int	fsuid;		/* fs    user  identification 	*/
		int	rgid;		/* real  group identification 	*/
		int	egid;		/* eff.  group identification 	*/
		int	sgid;		/* saved group identification 	*/
		int	fsgid;		/* fs    group identification 	*/
		int	jid;		/* jail process identification 	*/
		int	nthr;		/* number of threads in tgroup 	*/
		char	name[PNAMLEN+1];/* process name string       	*/
		char 	state;		/* process state ('E' = exited)	*/
		int	excode;		/* process exit status		*/
		time_t 	btime;		/* process start time (epoch)	*/
		time_t 	elaps;		/* process elaps time (hertz)	*/
		char	cmdline[CMDLEN+1];/* command-line string       	*/
		int	nthrslpi;	/* # threads in state 'S'       */
		int	nthrslpu;	/* # threads in state 'D'       */
		int	nthrrun;	/* # threads in state 'R'       */
		int	ifuture[1];     /* reserved                     */
	} gen;

	/* CPU STATISTICS						*/
	struct cpu {
		count_t	utime;		/* time user   text (ticks) 	*/
		count_t	stime;		/* time system text (ticks) 	*/
		int	nice;		/* nice value                   */
		int	prio;		/* priority                     */
		int	rtprio;		/* realtime priority            */
		int	policy;		/* scheduling policy            */
		int	curcpu;		/* current processor            */
		int	sleepavg;       /* sleep average percentage     */
		int	ifuture[4];	/* reserved for future use	*/
		count_t	cfuture[4];	/* reserved for future use	*/
	} cpu;

	/* DISK STATISTICS						*/
	struct dsk {
		count_t	rio;		/* number of read requests 	*/
		count_t	rsz;		/* cumulative # sectors read	*/
		count_t	wio;		/* number of write requests 	*/
		count_t	wsz;		/* cumulative # sectors written	*/
		count_t	cwsz;		/* cumulative # written sectors */
					/* being cancelled              */
		count_t	cfuture[4];	/* reserved for future use	*/
	} dsk;

	/* MEMORY STATISTICS						*/
	struct mem {
		count_t	minflt;		/* number of page-reclaims 	*/
		count_t	majflt;		/* number of page-faults 	*/
		count_t	shtext;		/* text     memory (Kb)         */
		count_t	vmem;		/* virtual  memory (Kb)		*/
		count_t	rmem;		/* resident memory (Kb)		*/
		count_t vgrow;		/* virtual  growth (Kb)    	*/
		count_t rgrow;		/* resident growth (Kb)     	*/
		count_t	cfuture[4];	/* reserved for future use	*/
	} mem;

	/* NETWORK STATISTICS						*/
	struct net {
		count_t tcpsnd;		/* number of TCP-packets sent	*/
		count_t tcpssz;		/* cumulative size packets sent	*/
		count_t	tcprcv;		/* number of TCP-packets recved	*/
		count_t tcprsz;		/* cumulative size packets rcvd	*/
		count_t	udpsnd;		/* number of UDP-packets sent	*/
		count_t udpssz;		/* cumulative size packets sent	*/
		count_t	udprcv;		/* number of UDP-packets recved	*/
		count_t udprsz;		/* cumulative size packets sent	*/
		count_t	rawsnd;		/* number of raw packets sent	*/
		count_t	rawrcv;		/* number of raw packets recved	*/
		count_t	cfuture[4];	/* reserved for future use	*/
	} net;
};

struct pinfo {
	struct pinfo	*phnext;	/* next process in hash    chain */
	struct pinfo	*prnext;	/* next process in residue chain */
	struct pinfo	*prprev;	/* prev process in residue chain */

	struct pstat	pstat;		/* per-process statistics        */
};

/*
** prototypes of process-database functions
*/
int		pdb_getproc(int, time_t, struct pinfo **);
void		pdb_addproc(int, struct pinfo *);
int		pdb_delproc(int);
int		pdb_newproc(struct pinfo **);
int		pdb_makeresidue(void);
int		pdb_cleanresidue(void);
int		pdb_srchresidue(struct pstat *, struct pinfo **);

/*
** prototypes for raw process-statistics functions
*/
int		deviatproc(struct pstat *, int, struct pstat *, int, int,
				struct pstat *, struct sstat *,
				int *, int *, int *, int *);
int		photoproc(struct pstat *, int);
unsigned int	countprocs(void);
