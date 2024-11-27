
# Trigonometric Expression Evaluator

**Trigonometric Expression Evaluator**, a lightweight and efficient program designed to parse and evaluate complex mathematical expressions. Built using the powerful principles of **LL(1) grammar** and **recursive descent parsing**, this evaluator is perfect for anyone delving into compiler design, numerical computations, or advanced mathematics.

## 👾 Features

- **Comprehensive Trigonometric Support**: Evaluate functions like `sin`, `cos`, and `tan` with degree input.
- **Arithmetic Operations**: Perform operations such as `+`, `-`, `*`, `/`, and exponentiation `^`.
- **Error Handling**: Detect invalid inputs, manage division by zero, and handle syntax errors gracefully.
- **Educational Value**: A practical implementation of compiler theory concepts for real-world applications.

## 🚀 Getting Started

Follow these simple steps to install, compile, and run the program.

### 1. Prerequisites

- A Linux-based operating system (tested on Linux Mint).
- GCC (GNU Compiler Collection) installed on your system.


### 2. Compiling the Code

Use the following command to compile the code:

```bash
gcc -o trig_evaluator trig_evaluator.c -lm
```

- The `-lm` flag links the math library, which is essential for operations like `sin`, `cos`, and `pow`.

### 3. Running the Program

Run the program with the expression you want to evaluate as a command-line argument:

```bash
./trig_evaluator "<expression>"
```

## 🔍 Examples

Here are some examples to help you get started:

- **Simple Arithmetic**:

  ```bash
  ./trig_evaluator "3 + 5 * 2"
  ```
  Output:
  ```
  Result: 13.000000
  ```

- **Trigonometric Evaluation**:

  ```bash
  ./trig_evaluator "sin(30) + cos(60) - tan(45)"
  ```
  Output:
  ```
  Result: 0.500000
  ```

- **Nested Expressions**:

  ```bash
  ./trig_evaluator "(2 + 3)^2 - 10 / 5"
  ```
  Output:
  ```
  Result: 23.000000
  ```

## 🛠️ How It Works

The evaluator operates in three major steps:

1. **Tokenization**: The input expression is scanned and broken into smaller, meaningful tokens (e.g., numbers, operators, functions).
2. **Parsing**: Using recursive descent parsing, the program checks the syntax and evaluates the expression according to the defined grammar rules.
3. **Evaluation**: The program computes the result by following mathematical precedence and converting degrees to radians for trigonometric calculations.

Happy coding and evaluating! 🎉
