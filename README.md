# The-LOOP-LANG
https://en.wikipedia.org/wiki/LOOP_(programming_language)

An attempt to build an interpreter for the theoretical LOOP language.

### Grammar:
```
program   -> statement ( ";" statement )* ;

statement -> loop
             | assign ;

loop    -> "LOOP" var "DO" program "END" ;
assign  -> var ":=" expr ;

expr    -> add ;
add     -> primary ( ("+"|"-") primary )* ;
primary -> var | number ;

var     -> {A,...,Z} x {0,...,99} ;
primary -> {0,...} ;

