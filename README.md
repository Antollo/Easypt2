# Easypt

[![https://ci.appveyor.com/api/projects/status/github/Antollo/Easypt2?svg=true]( https://ci.appveyor.com/api/projects/status/github/Antollo/Easypt2?svg=true)](https://ci.appveyor.com/project/Antollo/Easypt2)

## Example:

```c
console.write("What's your name?\t");
let name = console.readLine();
console.writeLine("Hello " + name + "! Nice to meet you.");
```

Run with: `easypt2 -file my_file.ez`. Longer example:

```
console.writeLine("a\nb");
console.writeLine(1+2);
console.writeLine(1.0+2.0);
console.writeLine(0.3+0.7);
console.writeLine("a"+"b");
console.writeLine(7/2);
console.writeLine(7%2);
console.writeLine("1"+1);
console.writeLine(1+"1");
console.writeLine(1+2*3);

let f = function { console.writeLine(args[0]); };
f("function");

if (true)  
    console.writeLine(true);
if (false)  
    console.writeLine(false);

if (1=="1")  
    console.writeLine(true);
if (1==2)  
    console.writeLine(false);

console.writeLine("s"[0]);

let i = 10;
while (i)
{
    console.write(i);
    i=i-1;
}

console.newLine();

let u;
u.p = "property";
console.write(u.p);
```




## Overview

__Easypt__ (_blend of easy and script_) is cross-platform structured object-oriented interpreted programming language. Its main goal is to be easy, small and extensible. Project started on 30th August 2018. Implementation of easypt2 interpreter and started on 2nd October 2019 as well as introduction of a new language standard.
 
## Artifacts

[https://ci.appveyor.com/project/Antollo/Easypt2](https://ci.appveyor.com/project/Antollo/Easypt2)