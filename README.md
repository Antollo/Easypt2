# Easypt

[![https://ci.appveyor.com/api/projects/status/github/Antollo/Easypt2?svg=true]( https://ci.appveyor.com/api/projects/status/github/Antollo/Easypt2?svg=true)](https://ci.appveyor.com/project/Antollo/Easypt2)

### Easypt VS Code extension

[![vs marketplace](https://img.shields.io/vscode-marketplace/v/Antollo.easypt.svg?label=vs%20marketplace)](https://marketplace.visualstudio.com/items?itemName=Antollo.easypt)
[![downloads](https://img.shields.io/vscode-marketplace/d/Antollo.easypt.svg)](https://marketplace.visualstudio.com/items?itemName=Antollo.easypt)
[![rating](https://img.shields.io/vscode-marketplace/r/Antollo.easypt.svg)](https://marketplace.visualstudio.com/items?itemName=Antollo.easypt)

## Overview

**Easypt** (_blend of easy and script_) is a cross-platform structured object-oriented prototype-based interpreted programming language. Its main goal is to be easy, small and extensible. The project started on 30th August 2018. Implementation of easypt2 interpreter started on 2nd October 2019 as well as the introduction of a new language standard and grammar. A significant improvement of the parser was done in May 2020.

## Examples

```js
console.write("What's your name? ")
let name = console.readLine()
console.writeLine("Hello " + name + "! Nice to meet you.")
```

```js
let arr = "ab c de fgh".split(" ")
for (let i = 0; i < arr.length(); i++)
    console.writeLine(arr[i])
```

```js
function sum(a, b) {
    return a + b
}
console.log(sum(2, 3))

let mul = (a, b) => a * b // Fat arrow syntax
console.log(mul(2, 3))
```

```js
// Parenthesis can be omitted in the function definition
function sum {
    let s = args[0] // "args" is an array of all arguments passed to the function
    for (let i = 1; i < args.length(); i++)
        s = s + args[i]
    return s
}
console.log(sum(1, 3, 5, 7))
```

```js
class MyClass {
    constructor: function (sth) {
        this.sth = sth
        console.log("constructor")
    },
    writeSth: () => console.writeLine(this.sth),
    destructor: () => console.log("destructor")
}

/*
"=" is the assignment operator - it assigns a copy of the right operand to the left operand
"<-" is the initialization assignment operator - if the left operand is in form of "identifier", 
     "identifier.property" or "let identifier", then operator makes left operand a reference to 
     right operand (left operand and right operand points to the same object)

In summary, using "=" in the following code would create two MyClass instances (one temporary
and one copy assigned to a)
*/

let a <- MyClass("something")
a.writeSth()
```

```js
let obj = {
    name: "Abc.xyz",
    arr: [1, 2, 3]
}

try {
    console.log(obj.name)
    console.log(obj.arr[10]) // Throws exception
} catch {
    console.error(exception)
}
```

```js
let seven = 7
/*
A semicolon is needed when the following line starts with a parenthesis "(" or a bracket "["
(otherwise, the following code would be interpreted as seven[1, 2, 3])
*/
let copyOfSeven = seven;
[1, 2, 3].forEach((element, index, array) => console.log(element))
```

```js
// Spread operator:
let arr = [...[1, 2, 3], ...[4, 5, 6]] // Unpacks arrays to the array
console.log(arr)

console.log(...[1, 2, 3]) // Unpacks array to the arguments

let obj = {...{x: 1}, ...{y: 2}} // Unpacks object's properties to the object
console.log(obj)

let math = {
    sum: (a, b) => a + b,
    div: (a, b) => a / b,
}

...math // Unpacks objects to the stack
console.log(sum(1, 2))

```

```js
let server = SslClient() // Many IO APIs (for files, TCP sockets, subprocesses, etc.)
Timeout(500).then(() => console.log("500 ms has elapsed")) // Promise
await server.connect("example.com", 443) // Asynchronous call and await keyword
console.log("Connected")
await server.send("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n")
let response = await server.receive()
console.log(response)
/*
Possible output:
01-01-2022 12:00:00 Connected
01-01-2022 12:00:01 500 ms has elapsed
01-01-2022 12:00:01 HTTP/1.1 200 OK
...
*/
```

Run with `easypt -file my_file.ez`.

Run `easypt -help` to see other options.

## Benchmark

### Platform:

  - System: Microsoft Windows [Version 10.0.19042.1288]
  - Processor: Intel(R) Core(TM) i7-11700KF @ 3.60GHz
  - RAM: 16GB DDR4 3200MHz

Easypt:

```js
function gcd {
    let a = args[0]
    let b = args[1]
    while (a != b) {
        if (a < b)
            b <- b - a
        else
            a <- a - b
    }
    return a
}
```

Python:

```py
def gcd(a, b):
    while a != b:
        if a < b:
            b = b - a
        else:
            a = a - b
    return a
```

JavaScript:

```js
function gcd(a, b) {
    while (a != b) {
        if (a < b)
            b = b - a
        else
            a = a - b
    }
    return a
}
```

Test:

```js
gcd(144, 1836311903)
```

The full code of the benchmark can be found in `/benchmarks`.

![Alt text](benchmarks/test.svg?raw=true "Title")

1. Easypt (local compilation from 02.11.2021)
1. Python (version 3.9.7)
1. Node.js¹ (version v16.10.0, JIT compilation disabled - `%NeverOptimizeFunction(gcd)`) 
1. Node.js² (version v16.10.0) 

## Documentation

### See the [language reference](https://antollo.github.io/Easypt2/).
 
## Downloads

- [For Windows](https://ci.appveyor.com/api/projects/antollo/Easypt2/artifacts/packages%2FEasypt-0.0.1-win64.exe?branch=master&job=Image%3A%20Visual%20Studio%202022)

- [For Linux](https://ci.appveyor.com/api/projects/antollo/Easypt2/artifacts/packages%2FEasypt-0.0.1-Linux.sh?branch=master&job=Image%3A%20Ubuntu2004)

## Compiling

#### Windows

```sh
./init.ps1
cmake .
cmake --build . --config Release
ctest --output-on-failure --build-config Release
```

#### Linux

```sh
./init.sh
cmake .
cmake --build . --config Release
ctest --output-on-failure --build-config Release
```
