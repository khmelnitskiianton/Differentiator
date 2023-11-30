# Differentiator

Program analyses math functions and generates formuls based on binary tree(in .tex and .dot)!
So you need to write formula to a file, enter variables and my program calculates it!

## Table of Contents

- [Installation](#installation)
- [Extra programs](#extra-programs)
- [Using](#using)
- [Options of compiling](#options-of-compiling)
- [Myassert](#myassert)
- [Verificator](#verificator)
- [Logs](#logs)

## Installation

To start program you need to use CMake and run program

1.  Clone repository
2.  Compile CMake, write command in main directory in repo
3.  Run CMake (compile program), write command in main directory in repo
4.  Run program(before you need to create this file) 
```c
git clone https://github.com/khmelnitskiianton/Akinator.git
cd ./Differentiator
cmake .
make
./differentiator.exe <file_to_read_formula>.txt
```
5. Write formula, write variables and check logs!!!

## Extra programs

Logs using Graphviz to vizualize binary tree!(you can turn them off) 

```c
$ sudo apt install graphviz
```

## Using

My program took only formuls with right grammatic.

Pattern is(in example_formula.txt):

`( ( (_aboba_) / ((_1_)sin_)) * (_suka_) )`

The main rules in writing formuls are:
1. Every types of spaces is ignoring
2. All arguments must be `_` or another arguments `(...)` and have brackets! 
3. If function is unary (one arg), write it in the left and right side is `_`

Types in formula:

1. Double number
2. Operators(Binary, unary): `+-*/^`, `sin cos tg ctg sqrt`
3. Variables

## Options of compiling

DEBUG_LOG 

DEBUG_MYASSERT 

DEBUG_VERIFY 

## Myassert

`myassert.h`

Now it relized like MYASSERT() that take all errors with enum and write it to console

## Verificator

`verificator.cpp` `verificator.h` 

Checking tree for loops and other errors. Functions returns EnumOfErrors 

## Logs

`log.cpp` `log.h`

Logs use GraphViz to vizualized graphs in pictures

After generating image it includes to html file `log.html`

![Пример графического лога!](https://github.com/khmelnitskiianton/Differentiator/blob/main/example_log.svg)