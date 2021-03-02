%option noyywrap yylineno

%{
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include "1505111_SymbolTable.h"
//#define YYSTYPE SymbollInfo*
#include "y.tab.h"

using namespace std;
extern FILE *errorFile;

extern YYSTYPE yylval;
extern SymbollTable table;
void yyerror(const char *);

int error_count=0;
//SymbollTable tables(20);


%}
SPECIAL_CHAR \\n|\\t|\\\\|\\a|\\f|\\r|\\b|\\v|\\0
DIGIT [0-9]
LETTER [a-zA-Z]
INTEGER {DIGIT}+
FLOATING_POINT (({DIGIT}+\.{DIGIT}*)|(\.{DIGIT}+)) 

HEX_NUM 0x([ABCDEF]|[0-9])+
TOO_MANY_DEC_ERROR {DIGIT}*\.{DIGIT}*\.{DIGIT}*
ILL_FORMED_NUMBER_ERROR ({INTEGER}|{FLOATING_POINT})([eE][0-9]*\.[0-9]*)
INVALID_SUFFIX_PREFIX_ERROR {DIGIT}+[A-DF-Za-df-z_]+
MULTI_CHAR_CONST_ERROR \'([ -&(-[\]-~]|{SPECIAL_CHAR})([ -&(-[\]-~]|{SPECIAL_CHAR})+\'
UNFINISHED_CHAR_ERROR (\')|(\'([ -&(-[\]-~]|{SPECIAL_CHAR}))|(\'\\\')
UNFINISHED_STRING_ERROR \"([ -!#-[\]-~]|{SPECIAL_CHAR}|\\\\|\\\n)*
UNRECOGNIZED_CHAR_ERROR [^ -~\n\t\a\f\r\b\v\0]
UNFINISHED_COMMENT \/\*([^*]|\*+[^\/*])*
EMPTY_CHAR_ERROR \'\'

STRING \"([ -!#-[\]-~]|\\\"|\\n|\\t|\\a|\\f|\\r|\\b|\\v|\\0|\\\\|\\\n)+\"
FLOATING_POINT_WITH_EXP ({FLOATING_POINT}([eE][+-]?{DIGIT}+)?)|({INTEGER}[eE][+-]?{DIGIT}+)
CHAR_LITERAL \'([ -&(-[\]-~]|\\n|\\t|\\\\|\\a|\\f|\\r|\\b|\\v|\\0)\'
NEWLINE (\r\n|\r|\n)
WHITESPACE [ \t\f\r\v]+ 
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
COMMENT \/\/(\\\n|[^\n])*|(\/\*([^*]|\*+[^\/*])*\*\/)

%% 


{TOO_MANY_DEC_ERROR}	{
			error_count++;
			fprintf(errorFile,"Error at Line no %d:Too many decimal point in %s\n\n",yylineno, yytext);
}
{ILL_FORMED_NUMBER_ERROR}	{
			error_count++;
			fprintf(errorFile,"Error at Line no %d:Ill formed number in %s\n\n",yylineno, yytext);
}
{INVALID_SUFFIX_PREFIX_ERROR}	{
			error_count++;
			fprintf(errorFile,"Error at Line no %d:Invalid suffix on numberic constant, or invalid prefix on identifier %s\n\n",yylineno, yytext);
}
{MULTI_CHAR_CONST_ERROR} {
			error_count++;
			fprintf(errorFile,"Error at Line no %d:Multi character constant %s\n\n",yylineno, yytext);
}
{UNFINISHED_CHAR_ERROR}	{
			error_count++;
			fprintf(errorFile,"Error at Line no %d:Unterminated character %s\n\n",yylineno, yytext);
}
{UNFINISHED_STRING_ERROR} {
			error_count++;
			fprintf(errorFile,"Error at Line no %d:Unterminated string %s\n\n",yylineno, yytext);
			for(int i=0; i<strlen(yytext); i++){
				if(i+1<strlen(yytext) && yytext[i] == '\\' && yytext[i+1] == '\n'){
					//line_count++;
				}
			}
}
{UNRECOGNIZED_CHAR_ERROR} {
			error_count++;
			fprintf(errorFile,"Error at Line no %d:Unrecognized character %s\n\n",yylineno, yytext);
}
{UNFINISHED_COMMENT} {
			error_count++;
			fprintf(errorFile,"Error at Line no %d:Unfinished comment %s\n\n",yylineno, yytext);
			for(int i=0; i<strlen(yytext); i++){
				if(i+1<strlen(yytext) && yytext[i] == '\\' && yytext[i+1] == '\n'){
					//line_count++;
				}
			}
}
{EMPTY_CHAR_ERROR} {
			error_count++;
			fprintf(errorFile,"Error at Line no %d:Empty char constant %s\n\n",yylineno, yytext);
			
}

"if"	{
	return IF;}
		
"for"	{
	return FOR;}
		
"do"	{ 
	return DO;}
		
"int"	{ 
	return INT;}
		
"float"	{ 
	return FLOAT;}
		
"void"	{
	 return VOID;}
		
"switch"	{ 
	return SWITCH;}
		
"default"	{
	 return DEFAULT;}
"else"	{
	 return ELSE;}
		
"while"	{
	 return WHILE;}
		
"break"	{
	 return BREAK;}
		
"char"	{
	 return CHAR;}		
"double"	{ 
	return DOUBLE;}
		
"return"	{
	 return RETURN;}
		
"case"	{
	 return CASE;}
		
"continue"	{
	 return CONTINUE;}
"println"	{return PRINTLN;}

{INTEGER} 	{
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"CONST_INT");
			yylval = (YYSTYPE)s;
			return CONST_INT;	
}

{FLOATING_POINT_WITH_EXP}	{
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"CONST_FLOAT");
			yylval = (YYSTYPE)s;
			return CONST_FLOAT;	
}
{CHAR_LITERAL}		{
			char ch;
			if(strlen(yytext) == 3){
				ch = yytext[1];
			}
			else if(strlen(yytext) == 4){
				if(yytext[2] == 'n'){ch='\n';}
				else if(yytext[2] == 't'){ch='\t';}
				else if(yytext[2] == '\\'){ch='\\';}
				else if(yytext[2] == 'a'){ch='\a';}
				else if(yytext[2] == 'f'){ch='\f';}
				else if(yytext[2] == 'r'){ch='\r';}
				else if(yytext[2] == 'b'){ch='\b';}
				else if(yytext[2] == 'v'){ch='\v';}
				else if(yytext[2] == '\0'){ch='\0';}
			}	
			SymbollInfo *s=new SymbollInfo(string(1,ch), (char *)"CONST_CHAR");
			yylval = (YYSTYPE)s;
			return CONST_CHAR;	
}


{NEWLINE}		{}
{WHITESPACE}		{}
"+"			|

"-"			{
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"ADDOP");
			yylval = (YYSTYPE)s;
			return ADDOP;
}
[*/%]			{
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"MULOP");
			yylval = (YYSTYPE)s;
			return MULOP;
}
"++" 		{
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"INCOP");
			yylval = (YYSTYPE)s;
			return INCOP;	
}
"--" 		{
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"DECOP");
			yylval = (YYSTYPE)s;
			return DECOP;	
}

"<"	|
"<="	|
">"	|
">="	|
"=="	|
"!="			{
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"RELOP");
			yylval = (YYSTYPE)s;
			return RELOP;
}
"="			{
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"ASSIGNOP");
			yylval = (YYSTYPE)s;
			return ASSIGNOP;	
}
"&&"	|
"||"			{
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"LOGICOP");
			yylval = (YYSTYPE)s;
			return LOGICOP;	
}
"&"	|
"|"	|
"^"	|
"<<"	|
">>"			{
			return BITOP;	
}
"!"			{
			return NOT;	
}
"("			{
			return LPAREN;
}
")"			{
			return RPAREN;	
}
"{"			{
				//scope enter
			table.EnterScope();
			return LCURL;	
}
"}"			{
			
			return RCURL;
}
"["			{
			return LTHIRD;	
}
"]"			{
			return RTHIRD;	
}
","			{
			return COMMA;
}
";"			{
			return SEMICOLON;	
}
{IDENTIFIER}		{
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"ID");
			yylval = (YYSTYPE)s;
			return ID;	
}
{STRING}		{
			string str="";
			for(int i=0; i<strlen(yytext); i++){
				if(yytext[i] == '\\'){
					if(yytext[i+1] == '\n'){
						i++;
						//line_count++;
						//str+='\t';
					}
					else if(yytext[i+1] == 't'){
						i++;
						str+='\t';
					}
					else if(yytext[i+1] == 'n'){
						i++;
						str+='\n';
					}
					else if(yytext[i+1] == '"'){
						i++;
						str+='"';
					}
				}
				else{
					str+=yytext[i];
				}
			}
			SymbollInfo *s=new SymbollInfo(yytext, (char *)"STRING");
			yylval = (YYSTYPE)s;
			return STRING;
}
{COMMENT}	{
			//return COMMENT;
}		
			

%%






