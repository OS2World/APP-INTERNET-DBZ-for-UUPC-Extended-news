This is the README documentation for the Mome Raths BBS package dbz for
UUPC/Extended.  This package is not part of the UUPC distribution, but 
is based upon their work.  UUPC/Extended is copyrighted, trademarked 
software, and every effort has been made to not knowingly infringe upon
either one.

The patched copy of expire.exe in this package have had only minor
modifications made to it.  It is covered under the above copyright.

However, the dbz.c and dbz.h that are in this distribution are original
works.  It has been made public domain by myself, the author.


These are the files you should have:

README -- this file, which explains the prerequisites, what dbz is, 
what it is used for, what NoCEM is, why dbz is but isn't public domain 
and why I wrote it.

Install.doc -- tells you how to install and use dbz.  How it may differ
from other dbz programs.  How to recompile it using emx.

Makefile.dbz -- for using dmake to recompile dbz.

dbz.exe -- a compiled executable, ready-to-go (assuming you already
have the other prerequites setup).

expire.exe -- another compiled executable.  This patched version
works around a bug in the UUPC 1.12s expire.exe, where expire
would try to delete a non-existent file, thus crashing.

dbzMake.Cmd -- a REXX script that sets up the correct paths to compile 
dbz.

dbz_mksetup.cmd -- a supporting REXX script called by dbzMake.Cmd, if 
necessary.

makediffs.cmd -- a small batch file used to create the diffs.

patchme.cmd -- a rather brainless batch file that can be used to patch 
the sources.

lib/lib.diff -- diff file for library files.

news/news.diff -- diff file for news files.

news/dbz.c -- source file for dbz executable.

news/dbz.h -- header file for dbz.


Overview:

This dbz program is based upon the same concept as the C-News program of 
the same name. I actually downloaded C-News and had a look at it.  The
dbz program compiled without any problems whatsoever.  However, it
bombed very quickly.  Looking at the program provided very few clues as
to how they actually expected it to work, in actuality.  So, while it is
intended to act similarly, I really don't know since I've never seen
it actually work.  Comments on how it is supposed to work can be sent to
MarchHare@momeraths.org.

The code itself is based upon, not surprisingly, genhist, which comes 
with the UUPC/Extended package.  Portions of the code are copyright of
Kew, and they are not being distributed with this package.  If you wish
to modify and recompile it, you will need the source code that
accompanies the UUPC/Extended package.  The source code packages are
usually designated by the letter 'S' and should be available wherever
you got UUPC.  If all else fails, try ftp.kew.com.  See section on
Recompiling dbz in Install.doc, which also lists how to patch the 
source code if you wish to recompile any of UUPC under OS/2's emx.


Prerequisites:

An installed, running, working UUPC machine - If you don't have one of 
these, you probably just wasted a small chunk of time.  The current 
version of UUPC is 1.12s.  If you are using an older version, please 
upgrade it.  Drew gets questions on a regular basis from people who 
have run into a bug that has been fixed several revisions ago.  This 
has not been tested on anything earlier, nor do I have the time or 
desire to do so.  If you need to upgrade, check out http://www.kew.com 
for the latest.

emx 0.9c development package, or equivalent runtime version - Please 
note that several packages come with DLL's for emx.  This package does 
not.  You should always make sure you only have one copy in your PATH 
and LIBPATH statements, and that you are using the latest.  IMO, 
distributing these files with the software is counter-productive, as 
they become out-of-date so quickly.

zip/unzip - which I assume you have or you would not be reading this.


Further prerequisites for compiling (this assumes you are compiling 
with emx):

HPFS - the diff files are named with .diff as the extension.  I'm not 
about to apologize for this.  One of the Rexx scripts is also above 
the 8.3 format.  If you are running news on FAT, on OS/2 at least, you 
get what you deserve.

emx development system 0.9c or later.

OS/2 port of GNU patch.

dmake, version 4 or higher


What is dbz?

dbz is a program which can be used to test, extract information from, 
add information to or check the existence of data within a BSD-type 
database.  These databases are usually associated with news programs, 
which track the history of incoming news messages or (in the case of 
newsreaders) already read news messages.  The messages are inserted 
into the database and indexed on the message ID number.


Why would I want to use dbz?

There can be several reasons.  The most obvious is if you simply want 
to test the functionality of the database.  However, it is unlikely 
that you would want the package for this for various reasons.

The most likely reason you need dbz, and perhaps the least obvious, is 
for NoCEM on spool.  Since you probably downloaded this package because
you are running UUPC/Extended, then you can appreciate the difficulties
involved in trying to run NoCEM.


I'm using NNTP, can I still use this package?

Very unlikely.  Most NoCEM packages come with alternatives for NNTP.
I would suggest looking at those instead.  Note that there are 
actually two history database formats:  C-News format and the older
B-News format.  From what I can ascertain, UUPC/Extended uses the
older format.


I'm using Waffle/Gigo/etc., not UUPC/Extended, can I still use this
package?

Unknown and untested.  If you try it, back up your history files and/or
have a way to rebuild them first.


What is NoCEM?

NoCEM is a way to either mark as read or remove articles from a news
spool which have been determined to be spam, Make-Money-Fast schemes or
binary-in-discussion-group articles.  NoCEM just marks the messages as
read, and is a user-oriented program.  NoCEM on spool is run by a
newsadmin and actually removes the articles from the news spool.  A
better explanation is at http://www.cm.org, the home of the Cancelmoose.


Does this mean this is free?

Parts of this program are under copyright of UUPC/Extended.  The
conditions of that package currently state that it is free, but a (very)
small fee can get you telephone support.  Obviously, this is subject to
change, although it is unlikely to change.  Please read the UUPC 
documentation for specifics.

The part of this program written by myself is public domain.  You can do
whatever you wish to do with it.  Those parts are the only ones included
in this package.


Public domain?  Does this mean it is worthless?

No.  It means that I want people to use it.  I want other programmers to
include it (or parts thereof) into their programs.  I want people to
realize, as well, that OS/2 and UUCP are not dead, and NNTP is not the
only protocol affected by spam.  In fact, UUCP has more problems because
of the time delays.


So, you are porting NoCEM to OS/2?

I already have a working REXX script, which I am still debugging
and some cosmetic changes (such as at least one misleading error
message) and about another week of testing.  I was going to do 
the Perl script first, but I found it was harder than I 
anticipated.  So, I will actually be doing a "port of a port", 
by porting the REXX script into Perl.  The Perl port, as well
is almost completed, with development now being done in parallel
(or, perhaps back-and-forth would be more accurate :) ).


How much longer will it take?

About another three weeks, I estimate.  As soon as I am satisfied
that there are no bugs that I can find, I will unleash it upon
the world.  Of course, I'm really learning Perl, so that is the
main reason it will still take that long.  A Perl expert could
probably do it in a week (in their spare time, that is).  Also,
getting side-tracked on the expire bug added some time to this
project.


That's fine for news, but what about email?

Stay tuned...


Will the OS/2 NoCEM be public domain as well?

Yes.  Use it for yourself.  Use it in your programs.  All I
really ask is that you don't ABUSE it.

