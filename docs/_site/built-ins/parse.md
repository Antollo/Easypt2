# parse

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `code /*String*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Function*/` 
</dd>
</dl>

### Description

Parses code from string to `Function` object. It is dangerous
(it could parse malicious code) function.

### Example

```js
let fun = parse("
    let x = 7;
    let y = 3;
    return x*y + y;");
let result = fun();
console.write(result);
```

**Expected output**

```
24
```

