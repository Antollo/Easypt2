# argc

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Number`]({{site.baseurl}}\built-ins\Number) 
</dd>
</dl>

### Description

Number of the interpreter's command-line arguments.

### Example

```js
// easypt -file f.ez
// f.ez:
for (let i = 0; i < argc; i++)
    console.writeLine(argv[i]);
```

