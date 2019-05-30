gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Release/src/tohtml.d" -MT"Release/src/tohtml.d" -o "Release/src/tohtml.o" "src/tohtml.c"
gcc -s -o "tohtml" Release/src/tohtml.o  
