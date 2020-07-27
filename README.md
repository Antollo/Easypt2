# Easypt

[![https://ci.appveyor.com/api/projects/status/github/Antollo/Easypt2?svg=true]( https://ci.appveyor.com/api/projects/status/github/Antollo/Easypt2?svg=true)](https://ci.appveyor.com/project/Antollo/Easypt2)

### Easypt VS Code extension

[![vs marketplace](https://img.shields.io/vscode-marketplace/v/Antollo.easypt.svg?label=vs%20marketplace)](https://marketplace.visualstudio.com/items?itemName=Antollo.easypt)
[![downloads](https://img.shields.io/vscode-marketplace/d/Antollo.easypt.svg)](https://marketplace.visualstudio.com/items?itemName=Antollo.easypt)
[![rating](https://img.shields.io/vscode-marketplace/r/Antollo.easypt.svg)](https://marketplace.visualstudio.com/items?itemName=Antollo.easypt)

## Overview

__Easypt__ (_blend of easy and script_) is cross-platform structured object-oriented interpreted programming language. Its main goal is to be easy, small and extensible. Project started on 30th August 2018. Implementation of easypt2 interpreter and started on 2nd October 2019 as well as introduction of a new language standard and grammar. Significant improvement of parser was done in May 2020.

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
class Myclass {
    constructor: function {
        this.sth = args[0];
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

## Documentation

See the [language reference](https://antollo.github.io/Easypt2/).
 
## Downloads

- [For Windows](https://ci.appveyor.com/api/projects/antollo/Easypt2/artifacts/packages%2FEasypt-0.0.1-win32.exe?branch=master&job=Image%3A%20Visual%20Studio%202017)

- [For Linux](https://ci.appveyor.com/api/projects/antollo/Easypt2/artifacts/packages%2FEasypt-0.0.1-Linux.sh?branch=master&job=Image%3A%20Ubuntu)

## Compiling

#### Windows

```ps1
./init.ps1
cmake .
cmake --build . --config Release
ctest --output-on-failure
```

#### Linux

```sh
./init.sh
cmake .
cmake --build . --config Release
ctest --output-on-failure
```