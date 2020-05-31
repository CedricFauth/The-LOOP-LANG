# The-LOOP-LANG
https://en.wikipedia.org/wiki/LOOP_(programming_language)

### Installing and usage: [CLICK HERE](INSTALL.md)

An attempt to build an interpreter for the theoretical LOOP language.


### Default grammar of loop:
```
P := Xi := Xj + c
   | Xi := Xj - c
   | P;P
   | "LOOP" Xi "DO" P "END"

(where i,j are natural numbers)
```

### Features of the interpreter:
The interpreter has some additional features which may simplify writing programs:
 - Arbitrary long expressions when assigning variables like: X4 := X3 - 2 + A42 + 123 + 314159
 - The interpreter supports variable names starting with a capital letter followed by 1 to 2 digits, e.g.: X12, A2, G34, ...
   - Not allowed are names like: X, R342, HE, %§&,  ...
 - As in the original language, programs need to be separated by a semicolon
 - Variables are initialized to 0 by default
 - Output variable is always X0
 - Values of input variables (starting with X1, X2, ...) need to be specified by the input parameter: -in=VALUE1,VALUE2,...
 - LOOP statements can handle expressions as condition like: LOOP 4 + X1 DO ...

 ### Upcoming features:
 ### ! CURRENTLY THE PROJECT IS DISCONTINUED !
 - Custom definition of new keywords and reuse of programs
 - Library support
 - Debugging mode where a program is interpreted step-by-step

### Internal grammar of this interpreter:
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
number -> {0,...} ;

