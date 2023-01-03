---
sort: 2
---

# Expressions

## Functions

### `function` Function expression.

  - `function` _identifier_ `(` _parameter list_ `)` _compound statement_
  - `function` _identifier_ _compound statement_
  
  Defines function with a specified name and optional aliases for arguments.
  
  - `function` `(` _parameter list_ `)` _compound statement_
  - `function` _compound statement_

  Defines function with optional aliases for arguments.

### `=>` "Fat arrow" function expression.

  - `(` _parameter list_ `) =>` _compound statement_

  Defines function with aliases for arguments.

  - `(` _parameter list_ `) =>` _statement_

  Defines function with aliases for arguments. The return value of the function is the value of an expression that the statement consists of.

### JSON expression.

  - A sequence of JSON statements between braces.

  ```warning
  Probably not fully compatible with JSON specification yet.
  ```

## Classes

### `class` expression.

  - `class` _identifier_ _json-expression_
  - `class` _identifier_ `extends` _identifier_ _json-expression_

  Defines a new class (`Class` object) optionally with a specified base class.

## Declarations

### `let` expression.

  - `let` _identifier_

  Declares variable with the specified name. The default value is `Object()`.

# Operators

## Notation

```warning
In this document the following notation is used:
`a["operator"](b)` (where `operator` can be `+`, `-`, `==`, `await` ...). 
However, read operator (`a["property"]`) can be overloaded (e.g. `Array` overloads this operator), and the intended behavior is as following:

`Object.classPrototype.readOperator.call(a, "operator")(b)`
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

- #### `a is b` Identity operator.
  Checks if the operands refer to the same object.
  Equivalent of `a.getId() == b.getId()`.

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
  If `a` is in form _identifier_, _expression.identifier_ or _let identifier_, then operator 
  makes `a` a reference to `b` (`a` and `b` points to the same object).

- #### `a : b` JSON assignment operator.
  If `a` is _identifier_, then operator creates variable `a` and assign `b` to it 
  (if `b` is constant, assign copy of `b`).
  If `a` is `String`, then operator creates variable with a value of `a` as name and assign `b` to it
  (if `b` is constant, assign a copy of `b`).

## Call operators

- #### `f(/* ... */)` Call operator.

  If `f` is instance of `Function` calls `f`, otherwise if `f.callOperator` exists calls it.

- #### `x.m(/* ... */)` Method call operator.

  If `x.m` is instance of `Function` calls `x.m`, otherwise if `x.m.callOperator` exists calls it.

- #### `a[b]` Read operator.
  Equivalent of `a.readOperator(b)`.

- #### `a @f@ b` Binary function operator.
  Equivalent of `f(a, b)`.

- #### `@f a` Decorator.
  Equivalent of `f(a)`.


## Other operators

- #### `...a` Spread operator
  - `f(...a)` Function call, method call and read operator.

  If `a` is convertible to `Array`, pass elements of the array to the function. Example:
  ```js
  console.log(1, 2, ...[3, 4], 5, 6);
  ```

  - `{ ...a }` Object literal.

  Pass properties of `a` (excluding prototype) to the new object. Example:
  ```js
  { x: 1, ...{ y: 2, z: 3 } }
  ```

  - `[ ...a ]` Array literal.

  If `a` is convertible to `Array`, pass elements of the array to new array. Example:
  ```js
  [1, 2, ...[3, 4], 5, 6]
  ```

  - `...a ` Stack.

  Unpack properties of `a` as variables. 
  Example:
  ```js
  let a = { x: 1, y: "abc" }
  // console.log(y) Error: "y not found"
  ...a
  console.log(y) // Prints "abc"
  ```

- #### `a ? b : c` Conditional (ternary) operator.
  If `a` is true then value of conditional expression
  is `b`, otherwise it's `c`.

- #### `start..stop` or `start..stop..step` Range operator.
  Creates range.
  Example:
  ```js
  for (let i in 0..10)
      console.log(i);
  ```

- #### `a instanceOf b` "Instance of" operator.
  Checks if `b` is an instance of `a`
  (if `a` has `b.classPrototype` in its prototype chain).

- #### `delete a` Delete operator.
  If `a` is _identifier_, then operator deletes variable `a`. 
  If `a` is _expression.identifier_, then operator deletes that property.

- #### `await a` Await operator.
  Equivalent of `a["await"]()`.

- #### `a.b` Dot operator.
  Provides access to properties of object. Equivalent of `a["b"]`.
  
