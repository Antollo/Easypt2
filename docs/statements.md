# Statements

### Simple statement

An expression followed with a semicolon.

### Compound statement

A sequence of statements between braces.

### JSON statement

An expression optionally followed with a comma.

## Iteration statements

### `for` loop

Consists of three expressions between parenthesis spliced with semicolons followed with a statement. Executes the first expression and then while the value of the second expression is true executes the third expression and the statement.

### `while` loop

Consists of condition between parenthesis followed with a statement. While the condition is true executes the statement.

## Control flow

### `if` ... `else` statement

If the condition is true, then executes the statement.

### `try` ... `catch`

`try` _statement_ `catch` _statement_

Executes the first statement. If an exception is thrown, stops the execution of the first statement, declares variable `exception`, and executes the second statement.

### `break`

Exits loop. _There must be a semicolon after `break`. Usage of `break` outside the loop results in undefined behavior._
