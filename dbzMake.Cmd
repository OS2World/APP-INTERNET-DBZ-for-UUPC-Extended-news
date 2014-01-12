/* Rexx */
/* dbzMake.Cmd -- Make sure you are in the correct directory, */
/* then make sure that library and include paths are setup. */
/*If not, then call dbz_mksetup.cmd.  Finally, run dmake. */

parse arg opts

rc = SysFileTree('.\lib', 'libDir', 'OD')
if rc \= 0 then
	signal NoMemory
if libDir.0 \= 1 then
	signal WrongDirectory

rc = SysFileTree('.\news', 'newsDir', 'OD')
if rc \= 0 then
	signal NoMemory
if newsDir.0 \= 1 then
	signal WrongDirectory

needSetup = 0
libPath = value('LIBRARY_PATH',, 'OS2ENVIRONMENT')
incPath = value('C_INCLUDE_PATH',, 'OS2ENVIRONMENT')
libDir.1 = Translate(libDir.1, '/', '\')
newsDir.1 = Translate(newsDir.1, '/', '\')

rc = Pos(libDir.1, libPath)
if rc = 0 then
	needSetup = 1

rc = Pos(libDir.1, incPath)
if rc = 0 then
	needSetup = 1

rc = Pos(newsDir.1, libPath)
if rc = 0 then
	needSetup = 1

rc = Pos(newsDir.1, incPath)
if rc = 0 then
	needSetup = 1

if needSetup = 1 then
do
	/* Make sure there is a semi-colon on the end first. */
	/* Do not assume that merely because one of the variables */
	/* does not end with a semi-colon, that the other does not */
	/* as well. */
	lastSemi = LastPos(';', libPath)
	strLength = Length(libPath)
	if lastSemi < strLength then
	do
		libPath = libPath||';'
		libPath = value('LIBRARY_PATH',libPath, 'OS2ENVIRONMENT')
	end

	lastSemi = LastPos(';', incPath)
	strLength = Length(incPath)
	if lastSemi < strLength then
	do
		incPath = incPath||';'
		incPath = value('C_INCLUDE_PATH',incPath, 'OS2ENVIRONMENT')
	end

	/* Add semi-colon to the end to make this easier */
	call dbz_mksetup libDir.1||';'||newsDir.1||';'

end /* if needSetup = 1 */

parse upper var opts stuff
rc = Pos("PACKAGE", stuff)
if rc \= 0 then
do
/*   'dmake clean -f Makefile.dbz' */
   Call MakeDiffs
end
'dmake' opts '-f Makefile.dbz'
/* Return whatever dmake returns */
exit rc
/* End of main program */

NoMemory:
say "SysFileTree() error, not enough memory?"
exit 1

WrongDirectory:
say "Cannot find specified directory."
say "Be sure to run this from the same directory"
say "that the makefile is in."
exit 2

