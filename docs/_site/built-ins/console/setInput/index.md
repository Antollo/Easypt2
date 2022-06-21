# console.setInput

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `/*String*/, /*Function*/, [...]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `console` 
</dd>
</dl>

### Description

Calls provided function (forwards arguments) and pass the 
string as input to this function. Useful for testing.

### Example

```js
console.setInput("abc", () => {
    let input = console.read();
    console.write(input);
});
```

**Expected output**

```
abc
```

