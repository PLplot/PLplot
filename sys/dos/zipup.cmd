cd ..
zip plplot plplot/lib/*.fnt plplot/tmp/.dummy plplot/private/.dummy
zip -r plplot plplot/* -x plplot/tmp plplot/lib plplot/private
