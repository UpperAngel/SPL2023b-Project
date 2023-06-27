# notes
* ### making an assembler for a dummy assembly language
    * each word is 12 bits 
    * 7 registers. r0 - r7

## types of lines:
* comment: start with a `;`. the assembler ignores this line. <br> 
**I.E.** `; this is a comment`

* instruction: tells to the assembler what it needs to do in *"compile time"*. <br> 
these lines don't include actions that need to be done in runtime. <br>
**I.E.** `NUM: .data -7, 2, +15`.


* command line: generates assembly instructions. <br>
these instructions are executed in runtime. <br>
this line is composed of: instruction name, and the operands of that instruction. <br>
**I.E.** `mov @r1 @r2`.


# input
**AS** file that *can* have  macros, comments, and white lines.

# output
4 files 
* AM:
    - deletes white lines 
    - deletes comments 
    - spreads the macros
 
* ENT:
    - includes 