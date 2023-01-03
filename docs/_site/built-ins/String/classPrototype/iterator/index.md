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
let it = str.iterator()
while (!((let c <- it.next()) is Iterator.end))
    console.write(c, " ");
```

**Expected output**

```
H a r r y 
```

