# argv

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Array`]({{site.baseurl}}\built-ins\Array) 
</dd>
</dl>

### Description

Array of the interpreter's command-line arguments.

### Example

```js
// easypt -file f.ez
// f.ez:
for (let i = 0; i < argc; i++)
    console.writeLine(argv[i]);
```

