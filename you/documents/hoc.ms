.TL
Hoc - An Interactive Language For Floating Point Arithmetic
.AU
Brian Kernighan
Rob Pike
.AB
.I Hoc
is a simple programmable interpreter
for floating point expressions.
It has C-style control flow,
function definition and the usual
numerical built-in functions
such as cosine and logarithm.
.AE
.NH
Expressions
.PP
.I Hoc
is an expression language,
much like C:
although there are several control-flow statements,
most statements such as assignments
are expressions whose value is disregarded.
For example, the assignment operator
.CW =
assigns the value of its right operand
to its left operand, and yields the value,
so multiple assignments work.
The expression grammar is:
.DS
.I
expr:        number
        |    variable
        |    ( expr )
        |    expr binop expr
        |    unop expr
        |    function ( arguments )
.R
.DE
Numbers are floating point.

