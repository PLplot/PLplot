#!/usr/bin/env python

# Programme to convert all single-line or multi-line "/* ... */"
# comments to "//" form in C or C++ code.  Input source code file is
# stdin, output file is stdout.
import sys
import re
ifsingleline = True

for line in sys.stdin.readlines():
    end_comment = line.find("*/")
    # len(line) includes new line character at end.
    if end_comment >=0 and end_comment !=  len(line) - 3:
        sys.stderr.write(line)
        raise RuntimeError, "Trailing garbage after */"

    if ifsingleline :
        start_comment = line.find("/*")
        if start_comment <0 and end_comment >=0:
            sys.stderr.write(line)
            raise RuntimeError, "Trailing */ for a line which is not a comment"

        # convert single-line comment.
        line = re.sub(r'^(.*)(/\*)(.*) *\*/$', "\\1//\\3", line)

        start_comment = line.find("/*")
	if start_comment >= 0:
            # Convert first line of multiline comment.
            line = line.replace("/*", "//")
            ifsingleline = False
    else:
        end_comment = line.find("*/")
        if end_comment < 0:
            # Convert multiline comment line that is not start line
            # or end line of that comment.  Look for leading blank asterisk
            # standard form of multiline commend line and get rid of it.
            line = "//" + re.sub(r'^ \*', "", line)
        else:
            # Convert last line of multiline comment.
            line = "//" + re.sub(r'^ *(.*)\*/', "\\1", line)
            ifsingleline = True

    sys.stdout.write(line)
sys.exit()
