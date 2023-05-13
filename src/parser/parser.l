/*** Definition Section has one variable
which can be accessed inside yylex()
and main() ***/
%{

#include <linux/limits.h>
#include <string.h>
char current_field[128];
char token[PATH_MAX];
char buf[PATH_MAX];
char output[PATH_MAX];
char braces_flag= 0;
char size_flag=0;
char first_flag=0;
char current_operation[20];
%}

tags   \{[^\}]+\}
name (\"[^\"]+\")


%%
"!" {
    strcat(current_operation, "-not ");
    }

"&" {
    size_flag=0;
    if(first_flag){
         if(braces_flag)strcat(output,"\\) ");
        strcpy(current_operation,"-and ");
    }
   
}
"|" {
    size_flag=0;
    if(first_flag){
        if(braces_flag)strcat(output, "\\) ");
        strcpy(current_operation,"-o ");
    }

}

.  {}
\n    {return 0;}
"{size}" {
    yytext[0] = '-';
    sscanf(yytext, "%[^}]",current_field);
    token[0]=0;
    braces_flag=0;
    size_flag=1;
}
{tags} {
    
    yytext[0] = '-';
    sscanf(yytext, "%[^}]",current_field);
    token[0]=0;
    braces_flag=0;
}
{name} {
    first_flag=1;
    if(!braces_flag){
        strcat(output, current_operation);
        strcpy(current_operation," ");
    }
   
    if(token[0] != 0 && !size_flag){
        strcat(output, "-o ");
    }
    else if(token[0] != 0 && size_flag){
        strcat(output, "-and ");
    }
    else strcat(output,"\\( ");
    sprintf(token,"%s %s ",current_field, yytext);
    strcat(output,token);
    braces_flag=1;
}
%%
 
int yywrap(){}
int main(int argc, char** argv){
 

// yywrap() - wraps the above rule section
/* yyin - takes the file pointer
          which contains the input*/
/* yylex() - this is the main flex function
          which runs the Rule Section*/
// yytext is the text in the buffer
 

yy_scan_string(argv[1]);
yylex();
if(braces_flag)strcat(output,"\\) ");
printf("%s", output);
return 0;
}
