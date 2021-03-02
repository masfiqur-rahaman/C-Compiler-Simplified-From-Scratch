bison -d -y 1505111.y
g++ -w -c -o y.o y.tab.c
flex 1505111.l
g++ -fpermissive -w -c -o l.o lex.yy.c
g++ -o a.out y.o l.o -lfl -ly
./a.out input3.txt 1505111_log.txt 1505111_error.txt 1505111_code.txt
