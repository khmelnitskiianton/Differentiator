# Differentiator

Program analyzes math functions and generates formulas based on binary tree (in .tex and .dot)!
So you need to write formula to a file, enter variables and my program calculates it!

System: x86-64 Linux

**Example of differentiating and generated math book**:

<img src="https://github.com/khmelnitskiianton/Differentiator/blob/main/example.gif" width=100%>

## Table of Contents

- [Installation](#installation)
- [Dependent Objects](#dependent-objects)
- [Using](#using)
- [Logs](#logs)

## Installation

To start program you need to use CMake and run program

1.  Clone repository
2.  Compile CMake, write command in main directory in repo
3.  Run CMake (compile program), write command in main directory in repo
4.  Run program(before you need to create this file) 

```c
git clone https://github.com/khmelnitskiianton/Differentiator.git
cd ./Differentiator
cmake .
make
./diff.elf <file_to_read_formula>.txt
```
1. Write formula, write variables and check `tex/math_book.pdf`!!!

## Dependent Objects

Logs using Graphviz to visualize binary tree!(you can turn them off) 

```c
sudo apt install graphviz
```

Logs using TeXLive to make a math book of cringe differentiation!(you can turn them off)
```c
sudo apt install texlive
```

Also logs using Python3 and Matplotlib to generate graphic in math book!

```c
sudo apt install python3
python3 -m pip install -U pip
python3 -m pip install -U matplotlib
```

It uses command `pdflatex ...` `python3 ...`

## Using

My program took only formulas with correct grammar.

The pattern is (in example_formula.txt):

`( ( ((_ x _)+ (_1_))ln _) ^(_x_) )`

The main rules for writing formulas are
1. All types of spaces are ignored.
2. All arguments must be `_` or other arguments `(...)` and have parentheses! 
3. If function is unary (one arg), write it in left and right side is `_`.

**Types in formula**:

1. Double number
2. Operators(Binary, unary): `+-*/^`, `sin cos tg ctg ln`
3. Variables

## Logs

`log_tex.cpp` `log_tex.h`

My program generates logs in pdf with tex and python graphic! So, you can see in `tex/` file `math_book.pdf` - it is funny generated math book!

It write every steps in differentiating with cringe phrases, in the and it adds changing(security from overflowing formula in one line)!

`log.cpp` `log.h`

Logs use GraphViz to visualize graphs in images

After creating the image it is included in the html file `log.html`

**Example**:

<img src="https://github.com/khmelnitskiianton/Differentiator/blob/main/example_log.svg" width=50% >
