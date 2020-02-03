#! /bin/sh
### Create one or more directory hierarchies, even on systems that lack
### support for mkdir's -p flag.
###
### Usage:
###	./mkdir.sh [-p] path-1 path-2 ... path-n
###
### Any -p option is silently ignored.
###
### [03-Jan-2000]

status=0
 
### Loop over command-line argument paths
for path in "$@"
do
	case $path in
    	-p)
		;;
	*)		
		## Change slash-separated path to space-separated parts, but
		## preserve any initial slash
		parts=`echo $path | sed -e 's@^/@:@' -e 's@/@ @'g -e 's@^:@/@' `
		top=
		for p in $parts
		do
		    if test "xx$top" = "xx"
		    then
			top=$p
			## Protect any leading hyphen in path
			case $top in
			-*)
			    top=./$top
			esac
		    else
			top=$top/$p
		    fi
		    if test ! -d $top
		    then
			mkdir $top || status=$?
		    fi
		done
		;;
	esac
done
exit $status
