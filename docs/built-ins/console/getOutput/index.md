# console.getOutput

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `/*Function*/, [...]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*String*/` 
</dd>
</dl>

### Description

Calls provided function (forwards arguments) and intercept 
output written by that function. Useful for testing.

### Example

```js
function f (a) {
    console.write(a);
};
let output = console.getOutput(f, "abc");
console.write(output);
```

**Expected output**

```
abc
```

