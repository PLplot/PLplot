These java examples are designed to mimic the results of the equivalent
C, Tcl, and Python examples.  

Here is the cookbook for compiling and running these examples.
The first part of the cookbook assumes you will be doing this from
plplot/tmp location.  The second part of the cookbook assumes you
will be doing this from the install location.

I. plplot/tmp

(1) Install a jdk (Java Development Kit).  I used the Blackdown jdk-1.2.2
for Linux, but for that platform there is also an IBM jdk if you prefer.

(2) Set environment variables (under tcsh.  For bash use the export
variable=value syntax.)
#Your location for the java jdk will vary.
setenv JAVA_HOME /home/software/java/jdk1.2.2/
setenv PATH $PATH":$JAVA_HOME/bin"
#These are relevant only when you have moved to the plplot/tmp directory
setenv CLASSPATH java
setenv LD_LIBRARY_PATH .

(3) cd plplot
# You will probably want other plplot configuration options as well
./configure --enable-java option
# Build library and core java class support
make

(4) cd tmp
# Build java demos
make jdemos

(5) Run java demos

# For newer jdk's can use the plplot/examples/x?? notation, but the dot
# notation works for all jdk's.
java plplot.examples.x01 .....

II. Install location ($prefix/lib/java/plplot/examples)

(1) Install jdk, see (1) above.

(2) set environment variables.  See (2) above except for the following:
#Your locations will differ depending on your prefix.
setenv CLASSPATH /usr/local/plplot/lib/java
setenv LD_LIBRARY_PATH /usr/local/plplot/lib

(3) Build java demos
cd /usr/local/plplot/lib/java/plplot/examples
javac -d ../../../java x01.java ...

(4) Run java demos (just like 5 above).



