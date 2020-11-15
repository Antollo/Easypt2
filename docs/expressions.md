# Expressions

## Functions

### `function` expression

- `function` _identifier_ `(`_parameter_list_`)` _compound_statement_
- `function` _identifier_ _compound_statement_

 Defines function with a specified name and optional aliases for arguments.

- `function` `(`_parameter_list_`)` _compound_statement_
- `function` _compound_statement_

Defines function with optional aliases for arguments.

### `=>` "fat arrow" function

- `(`_parameter_list_`) => ` _compound_statement_

Defines function with aliases for arguments.

- `(`_parameter_list_`) => ` statement

Defines function with aliases for arguments. The return value of the function is the value of an expression that the statement consists of.

## JSON expression

A sequence of JSON statements between braces.

_Probably not fully compatible with JSON specification yet._

## Classes

### `class` expression

- `class` _identifier_ _json-expression_
- `class` _identifier_ `extends` _identifier_ _json-expression_

Defines a new class (`Class` object) optionally with a specified base class.

## Declarations

### `let` expression

`let` _identifier_

Declares variable with the specified name. The default value is `Object()`.

## Control flow

### `return` keyword

`return` _expression_

Ends execution of functions. The value of the current function call is `a`.

### `throw` keyword

`throw` _expression_

Stops execution of current context. Throws `a`.

# Operators

## Notation

In this document the following notation is used:
`a["operator"](b)` (where `operator` can be `+`, `&`, `>=` ...).
However, read operator (`a["property"]`) can be overloaded (e.g. `Array` overloads this operator), and the intended behavior is as following:

```js
Object.classPrototype.readOperator.call(a, "operator")(b)
```

## Arithmetic Operators

- #### `a + b` Addition operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator adds them.
  If `a` is a `String` and `b` is convertible to `String`, then operator concatenates them.
  If `a` is an `Array` and `b` is convertible to `Array`, then operator merges them.
  Otherwise, operator calls `a["+"](b)`.

- #### `a - b` Subtraction operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator subtracts the second operand from the first.
  Otherwise, operator calls `a["-"](b)`.

- #### `a * b` Multiplication operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator multiplies them.
  If `a` is a `String` and `b` is convertible to `Number`, then operator concatenates `b` copies of `a` (e.g. `"ab" * 2` is `"abab"`).
  If `a` is an `Array` and `b` is convertible to `Number`, then operator merges `b` copies of `a`
  Otherwise, operator calls `a["*"](b)`.

- #### `a / b` Division operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator divides the first operand by the second.
  Otherwise, operator calls `a["/"](b)`.

- #### `a % b` Modulo operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator
  finds the remainder after division of the first operand by the second,
  both operands are rounded downward before operation.
  Otherwise, operator calls `a["%"](b)`.

- #### `a++` Increment operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator 
  increments the operand.
  Otherwise, operator calls `a["++"]()`.

- #### `a--` Decrement operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator decrements the operand.
  Otherwise, operator calls `a["--"]()`.

## Relational Operators

- #### `a == b` Equality operator.
  If `a` is of type `T` and `b` is convertible to type `T`, then operator 
  checks if the values of two operands are equal
  (where `T` is `Number`, `String`, `Array` or `Boolean`).
  Otherwise, operator calls `a["=="](b)`.

- #### `a != b` Inquality operator.
  If `a` is of type `T` and `b` is convertible to type `T`, then operator 
  checks if the values of two operands are not equal
  (where `T` is `Number`, `String`, `Array` or `Boolean`).
  Otherwise, operator calls `a["!="](b)`.

- #### `a > b` Greater than operator.
  If `a` is of type `T` and `b` is convertible to type `T`, then operator 
  checks if the value of the first operand is greater than the value 
  of the second operand
  (where `T` is `Number`, `String` or `Array` or `Boolean`).
  Otherwise, operator calls `a[">"](b)`.

- #### `a >= b` Greater than or equal operator.
  If `a` is of type `T` and `b` is convertible to type `T`, then operator 
  checks if the value of the first operand is greater or equal than the value 
  of the second operand
  (where `T` is `Number`, `String` or `Array` or `Boolean`).
  Otherwise, operator calls `a[">="](b)`.

- #### `a < b` Less than operator.
  If `a` is of type `T` and `b` is convertible to type `T`, then operator 
  checks if the value of the first operand is less than the value 
  of the second operand
  (where `T` is `Number`, `String` or `Array` or `Boolean`).
  Otherwise, operator calls `a["<"](b)`.

- #### `a <= b` Less than or equal operator.
  If `a` is of type `T` and `b` is convertible to type `T`, then operator 
  checks if the value of the first operand is less or equal than the value 
  of the second operand
  (where `T` is `Number`, `String` or `Array` or `Boolean`).
  Otherwise, operator calls `a["<="](b)`.

## Logical Operators

- #### `a && b` Logical AND operator.
  If `a` is convertible to `Boolean` and `b` is convertible to `Boolean`, then operator 
  is true if both of its operands are true and false if at least one operand is false.
  Otherwise, operator calls `a["&&"](b)`.

- #### `a || b` Logical OR operator.
  If `a` is convertible to `Boolean` and `b` is convertible to `Boolean`, then operator 
  is `a` if the first operand is true and `b` otherwise.
  Otherwise, operator calls `a["||"](b)`.

- #### `!a` Logical NOT operator.
  If `a` is convertible to `Boolean`, then operator 
  negates the value of the operand.
  Otherwise, operator calls `a["!"]()`.

## Bitwise Operators

- #### `a & b` Binary AND operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator
  puts a `1` bit in each bit position if it exists in this position in both operands.
  Otherwise, operator calls `a["&"](b)`.

- #### `a | b` Binary OR operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator
  puts a `1` bit in each bit position if it exists in this position in at least one operand.
  Otherwise, operator calls `a["|"](b)`.

- #### `a ^ b` Binary XOR operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator
  puts a `1` bit in each bit position if it exists in this position in exactly one operand.
  Otherwise, operator calls `a["^"](b)`.

- #### `a << b` Binary left shift operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator
  shifts the value of the first operand to the left by the number of bits specified by the second operand.
  Otherwise, operator calls `a["<<"](b)`.

- #### `a >> b` Binary right shift operator.
  If `a` is a `Number` and `b` is convertible to `Number`, then operator
  shifts the value of the first operand to the right by the number of bits specified by the second operand.
  Otherwise, operator calls `a["<<"](b)`.

- #### `~a` Binary complement operator.
  If `a` is `Number`, then operator 
  flips the bits of its operand.
  Otherwise, operator calls `a["~"]()`.

## Assignment Operators

- #### `a = b` Assignment operator.
  If `a` is not a constant assign the copy of `b` to `a`.

- #### `a <- b` Initialization assignment operator.
  If `a` is in form _identifier_, _identifier.property_ or _let identifier_, then operator 
  makes `a` a reference to `b` (`a` and `b` points to the same object).

- #### `a : b` JSON assignment operator.
  If `a` is _identifier_, then operator creates variable `a` and assign `b` to it 
  (if `b` is constant, assign copy of `b`).
  If `a` is `String`, then operator creates variable with a value of `a` as name and assign `b` to it
  (if `b` is constant, assign a copy of `b`).

## Call operators

- #### `f(/* ... */)` Call operator.

- #### `x.m(/* ... */)` Method call operator.

- #### `a[b]` Read operator.
  Equivalent of `a.readOperator(b)`.

- #### `a @f@ b` User operator.
  Equivalent of `f(a, b)`.


## Other operators

- #### `a instanceOf b` "Instance of" operator.
  Operators check if `b` is an instance of `a`
  (if `a` has `b.classPrototype` in its prototype chain).

- #### `delete a` Delete operator.
  If `a` is _identifier_, then operator deletes variable `a`. 
  If `a` is _identifier.property_, then operator deletes that property.

- #### `await a` Await operator.
  Equivalent of `a["await"]()`.

- #### `a.b` Dot operator.
  Provides access to properties of object. Equivalent of `a["b"]`.
  
