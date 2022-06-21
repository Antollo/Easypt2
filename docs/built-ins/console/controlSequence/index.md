# console.controlSequence

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `/*String*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `console` 
</dd>
</dl>

### Description

Writes ANSI escape code starting with `ESC` and `[` 
(CSI sequence) to the standard output stream.

### Example

```js
console.controlSequence("7m").write("special").controlSequence("0m");
```

