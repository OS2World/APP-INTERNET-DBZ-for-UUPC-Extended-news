/* Rexx */
/* makedbz.cmd -- set environment variables so that dmake can */
/* find the UUPC headers and libraries. */

parse arg addPath

if addPath = "" then
do
	say ""
	say "Error in arguments!"
	say ""
	say "Usage:  makedbz additional-path"
	say "		where the additional-path is the one"
	say "		you wish to add to the C_INCLUDE_PATH"
	say "		and LIBRARY_PATH environment variables"
	say ""
	exit(1);
end

libPath = value('LIBRARY_PATH',, 'OS2ENVIRONMENT')
incPath = value('C_INCLUDE_PATH',, 'OS2ENVIRONMENT')
/* Tack the new path onto the old path */
newLibPath = libPath||addPath
newIncPath = incPath||addPath

/* Oops, let's check for '\' and translate them */
newLibPath = Translate(newLibPath, '/', '\')
newIncPath = Translate(newIncPath, '/', '\')

/* Now let's set them */
libPath = value('LIBRARY_PATH', newLibPath, 'OS2ENVIRONMENT')
say "LIBRARY_PATH was" libPath
libPath = value('LIBRARY_PATH',, 'OS2ENVIRONMENT')
say "LIBRARY_PATH now is" libPath

incPath = value('C_INCLUDE_PATH', newIncPath, 'OS2ENVIRONMENT')
say "C_INCLUDE_PATH was" incPath
incPath = value('C_INCLUDE_PATH',, 'OS2ENVIRONMENT')
say "C_INCLUDE_PATH now is" incPath
