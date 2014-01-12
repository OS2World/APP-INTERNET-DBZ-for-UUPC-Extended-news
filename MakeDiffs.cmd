/* Rexx */
origSource = "e:\uupc1.12s\source"
rc = SysFileTree("lib\lib.diff", file)
if file.0 \= 0 then
do
	rc = SysFileDelete("lib\lib.diff")
	if rc \= 0 then
	do
		say "Error removing lib\lib.diff"
		exit 1
	end
end
rc = SysFileTree("news\news.diff", file)
if file.0 \= 0 then
do
	rc = SysFileDelete("news\news.diff")
	if rc \= 0 then
	do
		say "Error removing news\news.diff"
		exit 1
	end
end

'cd lib'
rc = SysFileTree("*.h", file, "FO")
do i = 1 to file.0
	lastSlash = LastPos("\", file.i)
	curFile = Substr(file.i, lastSlash+1)
	'diff -u' origSource||'\lib\'||curFile curFile '>>lib.diff'
end /* do */
rc = SysFileTree("*.c", file, "FO")
do i = 1 to file.0
	lastSlash = LastPos("\", file.i)
	curFile = Substr(file.i, lastSlash+1)
	'diff -u' origSource'\lib\'||curFile curFile '>>lib.diff'
end /* do */
'cd ..\news'
rc = SysFileTree("*.h", file, "FO")
do i = 1 to file.0
	lastSlash = LastPos("\", file.i)
	curFile = Substr(file.i, lastSlash+1)
	'diff -u' origSource||'\news\'||curFile curFile '>>news.diff'
end /* do */
rc = SysFileTree("*.c", file, "FO")
do i = 1 to file.0
	lastSlash = LastPos("\", file.i)
	curFile = Substr(file.i, lastSlash+1)
	'diff -u' origSource||'\news\'||curFile curFile '>>news.diff'
end /* do */
'cd ..'

