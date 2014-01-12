/* dbz.c
 *
 * Try to emulate C-News dbz.  Since I've never seen a working copy of the
 * program, I'm really just guessing here.
 *
 * Returns:
 *    0 for no error
 *    1 for key "not found", could not add or not missing (if using -m)
 *    49 for possible usage error
 *    98 for UUPC system error (probably configuration)
 *
 * Based upon genhist.c (by Kai Rummel) for UUPC/Extended, since
 * it gleefully reads and writes to the history files.
 *
 * Author:	John D. Carmack <MarchHare@momeraths.org
 * 21 July 97 (date not Y2K compliant :) )
 *
 * Change log:
 *
 * Version .003:
 *
 * 11 Aug 97:	Finally got around to fixing the dbz log so that it
 *             actually writes to "Spool\dbz.log" instead of the
 *             default "UUPC\extended.log".	Improved logging so
 *             that debug level 5 will log all of the transactions
 *             that dbz could/could not make.
 *
 * Version .002:
 *
 * 9 Aug 97:	Added lock file mechanism so program would not run when
 *             UUXQT is already running.	Changed and added more exit
 *             codes:  1 means not found (-x and -m switches only).
 *             A "not found" for -m really means that it is not missing.
 *             When using -x, the program now spits back the key and the
 *             entries!  This is not a minor change, and may break some
 *             programs.
 *
 */

#include "uupcmoah.h"
#ifndef __GCC__
#include <direct.h>
#endif

/*-------------------------------------------------------------------*/
/*                  System include files										*/
/*-------------------------------------------------------------------*/

#include <ctype.h>
#include <limits.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

/*-------------------------------------------------------------------*/
/*               UUPC/extended include files                         */
/*-------------------------------------------------------------------*/

#include "active.h"
#include "dater.h"
#include "getopt.h"
#include "hdbm.h"
#include "history.h"
#include "hostable.h"
#include "import.h"
#include "importng.h"
#include "lock.h"
#include "logger.h"
#include "uundir.h"
#include "pushpop.h"
#include "stater.h"
#include "timestmp.h"

/* dbz include file */
#include "dbz.h"

currentfile();

short int option = 0;
void *history;
char *logname = "dbz.log";

void BackupHistory(void)
{
	FILE *dirFile, *pagFile;
	FILE *oldDirFile, *oldPagFile;
	char dirFileOld[FILENAME_MAX];
	char pagFileOld[FILENAME_MAX];
	char dirFileOrig[FILENAME_MAX];
	char pagFileOrig[FILENAME_MAX];
	int c;

	printmsg(5, "Backing up history files...");

	mkfilename(dirFileOrig, E_newsdir, "history.dir");
	mkfilename(pagFileOrig, E_newsdir, "history.pag");
	mkfilename(dirFileOld, E_newsdir, "oldhist.dir");
	mkfilename(pagFileOld, E_newsdir, "oldhist.pag");

	dirFile = fopen(dirFileOrig, "rb");
	pagFile = fopen(pagFileOrig, "rb");

	if(dirFile == NULL || pagFile == NULL)
	{
		printmsg(0, "Error opening history file");
		panic();
	}

	oldDirFile = fopen(dirFileOld, "wb");
	oldPagFile = fopen(pagFileOld, "wb");

	if(oldDirFile == NULL || oldPagFile == NULL)
	{
		printmsg(0, "Error opening backup history file");
		panic();
	}

	while((c = fgetc(dirFile)) != EOF)
		fputc(c, oldDirFile);
	while((c = fgetc(pagFile)) != EOF)
		fputc(c, oldPagFile);

	fclose(dirFile);
	fclose(pagFile);
	fclose(oldDirFile);
	fclose(oldPagFile);
	printmsg(5, "\n");
}

/*-------------------------------------------------------------------*/
/*   m a i n																			*/
/*                                                                   */
/*   Main program                                                    */
/*-------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	extern char *optarg;
	char histEntry[FILENAME_MAX + DBM_BUFSIZ +1];
	int c;
	int rc = 0;           /* Return code */
	KWBoolean locked;

	debuglevel = 1;
	optind = 0;

// Check configuration and initialize
	if(!configure(B_DBZ))
	{
		printmsg(0, "%s - configuration failure", argv[0]);
		exit(98);	/* system configuration failed */
	}

	checkreal(E_nodename);	 /* Force Host Table to initialize			*/
	openlog(logname);
	printmsg(20, "Log file opened.\n");
	PushDir(E_newsdir);			 /* Save drive, directory name,
											creates directory if needed  */

	while((c = getopt(argc, argv, "xacmd:")) !=	EOF)
		switch(c)
		{
			case 'a':
				if(option)
				{
					printmsg(0, "only one of -x -a -c -m allowed");
					exit(49);
				}
				else
					option = 'a';
				break;

			case 'c':
				if(option)
				{
					printmsg(0, "only one of -x -a -c -m allowed");
					exit(49);
				}
				else
					option = 'c';
				break;

			case 'm':
				if(option)
				{
					printmsg(0, "only one of -x -a -c -m allowed");
					exit(49);
				}
				else
					option = 'm';
				break;

			case 'x':
				if(option)
				{
					printmsg(0, "only one of -x -a -c -m allowed");
					exit(49);
				}
				else
					option = 'x';
				break;

			case 'd':
				debuglevel = atoi(optarg);
				break;

			case '?':
				Usage(argv[0]);
				break;

			default:
				printmsg(0, "%s - invalid option -%c", argv[0], c);
				Usage(argv[0]);
				break;
		}

	if(!option)
	{
		printmsg(0, "%s - no options given", argv[0]);
		Usage(argv[0]);
	}
	else
		printmsg(5, "option is %c.", option);

	if(argc > optind)
	{
		strcpy(histEntry, argv[optind]); // Overwrite any prev value
		optind++;
		while(argc > optind)
		{
			strcat(histEntry, " ");
			strcat(histEntry, argv[optind]);
			optind++;
		}
		printmsg(5, "histEntry = %s.\n", histEntry);
	}
	else // Not enough arguments to program
	{
		printmsg(0,"Insufficient arguments.");
		Usage(argv[0]);
	}

	if(LockSystem(E_nodename , B_UUXQT ))
	{
		history = open_history("history");
		locked = KWTrue;
	}
	else
	{
		locked = KWFalse;
		printmsg(0, "Error, cannot get system lock");
		rc = 98;
	}

	if(locked)
	{
		switch(option)
		{
			case 'a':	/* append to existing table */
// Note: This is too time-consuming to put in the program
// and it is counter-productive.  The real answer is to
// back up the files _before_ running this program!
#ifdef DEBUG
				BackupHistory();
#endif
				rc = AppendHistEntry(histEntry);
				break;

			case 'c':	/* check existing table */
				printmsg(0,"Unimplemented function, sorry.");
				rc = 49;
				break;

			case 'm':	/* extract missing from existing table (-x complement) */
				rc = MissingEntry(histEntry);
				break;

			case 'x':	/* extract from existing table (-m complement) */
				rc = XtractEntry(histEntry);
				break;
		} /* endswitch */
		/* Close it only if it was opened */
		close_history(history);
		UnlockSystem();
	} /* endif locked */

	if(rc)
		printmsg(10, "Entry not found.");
	else
		printmsg(10, "Entry found.");
	fflush(logfile);
	PopDir();
	exit(rc);
} /* main() */

// AppendHistEntry() -- Add entire record to database.
//
int AppendHistEntry(char *histEntry)
{
	char messageID[FILENAME_MAX], newHistEntry[DBM_BUFSIZ];
	char *entryP;
	int i, rc;

	entryP = histEntry;
	sscanf(histEntry, "%s", messageID);
	for(i=0; i<=strlen(messageID); i++)
		entryP++;
	strcpy(newHistEntry, entryP);

	rc = add_histentry(history, messageID, newHistEntry);
	if(!rc) /* Could not add entry */
		printmsg(0,"AppendHistEntry failure: %s", newHistEntry);
	return rc;
} /* AppendHistEntry() */

// MissingEntry() -- Input a key, then print it back out if
//       it does not exist in the database.
//
int MissingEntry(char *histEntry)
{
	char *value = NULL;
	int rc = 0;

	value = get_histentry(history, histEntry);
	if(value == NULL)
	{
		printf("%s\n", histEntry);
		printmsg(2, "%s\n", histEntry);
	}
	else
		rc = 1; /* entry is not missing */
	return rc;
} /* MissingEntry() */

// XtractEntry() -- If the key is found, print out the values.
//
int XtractEntry(char *histEntry)
{
	char *value;
	int rc = 0;

	value = get_histentry(history, histEntry);
	if(value != NULL)
	{
		printf("%s %s\n", histEntry, value);
		printmsg(2, "%s %s\n", histEntry, value);
	}
	else /* Entry not found */
		rc = 1;
	return rc;
} /* XtractEntry() */

// Usage() -- Print usage of program
//
static void Usage(char *program)
{
	printmsg(0, "Usage:   %s\t<-a | -c | -m | -x> [-d debuglevel]\n", program);
	fflush(logfile);
	exit(49);
} /* Usage */

