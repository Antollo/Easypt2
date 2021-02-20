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
console.write("What's your name?\t");
let name = console.readLine();
console.writeLine("Hello " + name + "! Nice to meet you.");
```

```js
let arr = "ab: cd\n\re: f\n\rg: hij".split("\n\r");
for (let i = 0; i<arr.length(); i++)
    console.writeLine(arr[i]);
```

```js
function sum(a, b) { return a + b; };
console.log(sum(1, 3));
```

```js
function sum {
    let s = args[0];
    for (let i = 1; i < args.length(); i++)
        s = s + args[i];
    return s;
};
console.log(sum(1, 3, 5, 7));
```

```js
class Myclass {
    constructor: function (sth) {
        this.sth = sth;
        console.log("constructor");
    },
    writeSth: function { console.writeLine(this.sth); },
    destructor: function { console.log("destructor"); }
};

let a <- Myclass("something");
a.writeSth();
```

```js
let obj = {
    name: "Abc.xyz",
    arr: [1, 2, 3]
};

try {
    console.log(obj.name);
    console.log(obj.arr[10]); // Throws exception!
} catch {
    console.error(exception);
}
```

Run with `easypt -file my_file.ez`.

Run `easypt -help` to see other options.

## Benchmark

### Platform:

  - System: Microsoft Windows [Version 10.0.19041.630]
  - Processor: Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz
  - RAM: 8GB DDR4 2133MHz

Easypt:

```js
function gcd {
    let a = args[0];
    let b = args[1];
    while (a != b) {
        if (a < b)
            b <- b - a;
        else
            a <- a - b;
    }
    return a;
};
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

1. Easypt (local compilation from 15.11.2020)
1. Python (version 3.9.0)
1. Node.js¹ (version v15.0.1, JIT compilation disabled - `%NeverOptimizeFunction(gcd)`) 
1. Node.js² (version v15.0.1) 

## Documentation

### See the [language reference](https://antollo.github.io/Easypt2/).
 
## Downloads

- [For Windows](https://ci.appveyor.com/api/projects/antollo/Easypt2/artifacts/packages%2FEasypt-0.0.1-win32.exe?branch=master&job=Image%3A%20Visual%20Studio%202019)

- [For Linux](https://ci.appveyor.com/api/projects/antollo/Easypt2/artifacts/packages%2FEasypt-0.0.1-Linux.sh?branch=master&job=Image%3A%20Ubuntu)

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
