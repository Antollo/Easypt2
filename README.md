# Easypt

[![https://ci.appveyor.com/api/projects/status/github/Antollo/Easypt2?svg=true]( https://ci.appveyor.com/api/projects/status/github/Antollo/Easypt2?svg=true)](https://ci.appveyor.com/project/Antollo/Easypt2)

## Example:

```js
console.write("What's your name?\t");
let name = console.readLine();
console.writeLine("Hello " + name + "! Nice to meet you.");
```

Run with: `easypt -file my_file.ez`. Another example:

```js
let arr = "ab: cd\n\re: f\n\rg: hij".split("\n\r");

for(let i = 0; i<arr.length(); i++)
    console.writeLine(arr[i]);
```




## Overview

__Easypt__ (_blend of easy and script_) is cross-platform structured object-oriented interpreted programming language. Its main goal is to be easy, small and extensible. Project started on 30th August 2018. Implementation of easypt2 interpreter and started on 2nd October 2019 as well as introduction of a new language standard and grammar. Significant improvement of parser was done in May 2020.
 
## Artifacts

[https://ci.appveyor.com/project/Antollo/Easypt2](https://ci.appveyor.com/project/Antollo/Easypt2)