# String.classPrototype.iterator

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Iterator*/` 
</dd>
</dl>

### Description

Returns an iterator that iterates over characters in the string, 
characters are represented as new strings.

### Example

```js
let str = "Harry";
for (let c in str)
    console.write(c, " ");
```

**Expected output**

```
H a r r y 
```

