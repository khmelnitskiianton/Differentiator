# Differentiator

Program analyses math functions and generates formuls based on binary tree(in .tex and .dot)!
So you need to write formula to a file, enter variables and my program calculates it!

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
./differentiator.exe <file_to_read_formula>.txt
```
5. Write formula, write variables and check `tex/matan_book.pdf`!!!

## Dependent Objects

Logs using Graphviz to vizualize binary tree!(you can turn them off) 

```c
sudo apt install graphviz
```

Tex logs using TeXLive to make a math book of cringe differentiating!(you can turn them off) 

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

My program took only formuls with right grammatic.

Pattern is(in example_formula.txt):

`( ( ((_ x _)+ (_1_))ln _) ^(_x_) )`

The main rules in writing formuls are:
1. Every types of spaces is ignoring
2. All arguments must be `_` or another arguments `(...)` and have brackets! 
3. If function is unary (one arg), write it in the left and right side is `_`

**Types in formula**:

1. Double number
2. Operators(Binary, unary): `+-*/^`, `sin cos tg ctg ln`
3. Variables

**Example of generated book**:

<object data="https://github.com/khmelnitskiianton/Differentiator/blob/main/example_log_tex.pdf" type="application/pdf" width="700px" height="700px">
    <embed src="https://github.com/khmelnitskiianton/Differentiator/blob/main/example_log_tex.pdf">
        <p>Oh! This browser does not support PDFs. Please download the PDF to view it: <a href="https://github.com/khmelnitskiianton/Differentiator/blob/main/example_log_tex.pdf">Download PDF</a>.</p>
    </embed>
</object>

## Logs

`log_tex.cpp` `log_tex.h`

My program generates logs in pdf with tex and python graphic! So, you can see in `tex/` file `matan_book.pdf` - it is funny generated math book!

It write every steps in differentiating with cringe phrases, in the and it adds changings(security from overflowing formula in one line)!

`log.cpp` `log.h`

Logs use GraphViz to vizualized graphs in pictures

After generating image it includes to html file `log.html`

**Example**:

<img src="https://github.com/khmelnitskiianton/Differentiator/blob/main/example_log.svg" width=50% >
