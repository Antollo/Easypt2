# execute

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `code /*String*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Object*/` 
</dd>
</dl>

### Description

Parses code from string and executes it (using current stack - variable declared in
string is accessible outside the execute function). It is dangerous (it could execute
malicious code) function.

### Example

```js
let result = execute("
    let x = 7;
    let y = 3;
    let z = x*y + y;
    return z; 
");
console.write(result, " ", z);
```

**Expected output**

```
24 24
```

