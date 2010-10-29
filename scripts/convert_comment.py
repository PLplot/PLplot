#!/usr/bin/env python

# Programme to convert all single-line or multi-line "/* ... */"
# comments to "//" form in C or C++ code.  Input source code file is
# stdin, output file is stdout.
import sys
import re
# Restore default signal handling for SIGPIPE as suggested by
# http://www.velocityreviews.com/forums/t332662-ioerror-errno-32-broken-pipe.html
# so that stdout can be sent (unix-only) to a pipelined process that terminates
# early such as cmp.
import signal
signal.signal(signal.SIGPIPE, signal.SIG_DFL)

ifsingleline = True

for line in sys.stdin.readlines():
    start_comment = line.find("/*")
    # FIXME?: the simple rules below to ignore all special strings
    # in a line where the first instance is quoted obviously
    # need generalization.  However, second instances
    # of special strings are unlikely in practice so we will go with
    # this simple rule until it is proved something more is required.
    # Ignore all "/*" instances on a line where the first one is
    # quoted.
    if start_comment >=0 and re.search(r'^[^"]*("[^"]*"[^"]*)*"[^"]*/\*[^"]*"', line):
        start_comment = -1

    start_special = line.find("//*")
    # FIXME?
    # Ignore all "//*" instances on a line where the first one is
    # quoted.
    if start_special >= 0 and re.search(r'^[^"]*("[^"]*"[^"]*)*"[^"]*//\*[^"]*"', line):
        start_special = -1

    # if start_special corresponds to start_comment, then ignore start_comment
    # to deal with issue of recursive changes to an original line of the
    # form "/******..."
    if start_special >= 0 and start_special == start_comment -1:
        start_comment = -1

    end_comment = line.find("*/")
    # FIXME?
    # Ignore all "*/" instances on a line where the first one is
    # quoted.
    if end_comment >= 0 and re.search(r'^[^"]*("[^"]*"[^"]*)*"[^"]*\*/[^"]*"', line):
        end_comment = -1

    start_special = line.find("//")
    # FIXME?
    # Ignore all "//" instances on a line where the first one is
    # quoted.
    if start_special >= 0 and re.search(r'^[^"]*("[^"]*"[^"]*)*"[^"]*//[^"]*"', line):
        start_special = -1
    # If unquoted leading //, then ignore any old-style comments beyond.
    if start_special >= 0 and start_special <= start_comment -1:
        start_comment = -1
    if start_special >= 0 and start_special <= end_comment:
        end_comment = -1

    # Note trailing "\n" has not (yet) been removed from line so
    # that the next to last character is at position len(line) - 3.
    if end_comment >=0 and end_comment !=  len(line) - 3:
        if ifsingleline and start_comment >=0:
            # Skip most further processing for a line with embedded
            # comments outside of multiline blocks of comments.
            start_comment = -1
            end_comment = -1
        else:
            sys.stderr.write(line)
            raise RuntimeError, "Cannot interpret trailing character(s) after */ for this line"

    if ifsingleline and (start_comment >=0 or end_comment >=0):
        # strip trailing "\n" (Unix line endings, only) since that might
        # cause trouble with stripping trailing white space below.
        # This trailing "\n" will be added back at the end of this block.
        line = re.sub(r'^(.*)\n$', "\\1", line)
        if start_comment <0 and end_comment >=0:
            sys.stderr.write(line + "\n")
            raise RuntimeError, "Trailing */ for a line which is not a comment"

        # Convert single-line comment (if it exists).
        # \1 corresponds to zero or more leading characters before "/*".
        # \3 corresponds to zero or more characters between "/*" and
        # "*/".
        # N.B. preserves indentation.
        line = re.sub(r'^(.*)(/\*)(.*)\*/$', "\\1//\\3", line)
        # strip trailing white space (if any).
        line = line.rstrip()

        # Deal with "/**" case which would be changed to "//*" above.
        start_comment = line.find("/*")
        # FIXME?
        # Ignore all "/*" instances on a line where the first one is
        # quoted.
        if start_comment >=0 and re.search(r'^[^"]*("[^"]*"[^"]*)*"[^"]*/\*[^"]*"', line):
            start_comment = -1

        start_special = line.find("//*")
        # FIXME?
        # Ignore all "//*" instances on a line where the first one is
        # quoted.
        if start_special >= 0 and re.search(r'^[^"]*("[^"]*"[^"]*)*"[^"]*//\*[^"]*"', line):
            start_special = -1

        if start_special < 0 and start_comment >= 0:
            # Convert first line of multiline comment.
            # N.B. preserves indentation.
            line = line.replace("/*", "//", 1)
            ifsingleline = False

        # Add back (Unix-only) line ending.
        line = line + "\n"

    elif not ifsingleline:
        if start_comment >=0:
            sys.stderr.write(line)
            raise RuntimeError, "*/ in the middle of a comment block"

        # strip trailing "\n" (Unix line endings, only) since that might
        # cause trouble with stripping trailing white space below.
        # This trailing "\n" will be added back at the end of this block.
        line = re.sub(r'^(.*)\n$', "\\1", line)
        if end_comment < 0:
            # Convert multiline comment line that is not start line
            # or end line of that comment.  
            # Replace " *" after zero or more blanks (the standard form
            # produced by uncrustify) if it is there by "//".
            # N.B. preserves indentation.
            line = re.sub(r'^( *) \*', "\\1//", line)
            start_newcomment = line.find("//")
            if start_newcomment < 0:
                # If all previous conversion attempts failed....
                line = "//" + line

        else:
            # Convert last line of multiline comment.
            # Try converting vacuous form (initial blanks + " */")
            # to initial blanks (if any) + "//".
            # This consumes the blank in " */" that is customary as
            # the last line of a multi-line block.
            # N.B. preserves indentation.
            # \1 contains the initial blanks
            line = re.sub(r'^( *) \*/$', "\\1//", line)

            # Try converting non-vacuous standard form produced by
            # uncrustify (initial blanks + " *" + any string + "*/")
            # to initial blanks + "//" + any string.
            # N.B. preserves indentation.
            # \1 contains the initial blanks
            # \2 contains the "any" string preceding "*/".
            line = re.sub(r'^( *) \*(.*)\*/$', "\\1//\\2", line)

            # If all previous conversion attempts failed....
            # N.B. Does not preserve indentation.
            line = re.sub(r'^(.*)\*/$', "//\\1", line)

            # strip trailing white space (if any).
            line = line.rstrip()
            ifsingleline = True

        # Add back (Unix-only) line ending.
        line = line + "\n"

    # End with some special processing for all lines which previously
    # or now include "//".
    start_special = line.find("//")
    if start_special >= 0:
        # Special transforms to get rid of left-over "\*" and "*\"
        # forms which have historically been used to frame
        # multi-block comments by some PLplot developers.
        # Replace leading "// \*" ==> "//"
        line = re.sub(r'^// \\\*(.*)$', "//\\1", line)
        # Remove "*\" from end of comment lines
        line = re.sub(r'^//(.*)\*\\$', "//\\1", line)
        # Convert long "horizontal line" comment forms to standard form.
        line = re.sub(r'^// *[-*= ]{50,200}$', "//--------------------------------------------------------------------------", line)
        # Look for trailing continuation after comment lines and
        # complain if you find any.
        start_special = line.rfind("\\")
        # Note line has trailing "\n" so that the last character
        # is at position len(line) - 2.
        if start_special >=0 and start_special == len(line) -2:
            sys.stderr.write(line)
            raise RuntimeError, "Continuation not allowed for comments"

    sys.stdout.write(line)

sys.exit()
