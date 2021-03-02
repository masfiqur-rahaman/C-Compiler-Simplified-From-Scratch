%option noyywrap

%{
#include<stdio.h>
#include<stdlib.h>

int line_count=1;
/*In pattern-action part these should be included


{SINGLECHAR}	 	{printf("line:%d %s is a character.\n",line_count,yytext);}

WHITESPACE		{printf("line:%d %s mathced in whitespace rule.\n",line_count,yytext);}

*/
%}
WHITESPACE [ \t\f\r\v]+
ALPHANUMERIC [a-zA-Z0-9]
SINGLECHAR .
NEWLINE \n
%%

{NEWLINE}		{printf("line:%d  mathced in Newline rule.\n",line_count);line_count++;}
{ALPHANUMERIC}+ 	{printf("line:%d %s mathced in Aphanumeric rule.\n",line_count,yytext);}
[a-z]{-}[jv]		{printf("line:%d %s is in a-z excluding j,v.\n",line_count,yytext);}

%%




int main(int argc,char *argv[]){

	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}

	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}


	yyin= fin;
	yylex();
	printf("Number of lines: %d",line_count);
	fclose(yyin);
	return 0;
}
