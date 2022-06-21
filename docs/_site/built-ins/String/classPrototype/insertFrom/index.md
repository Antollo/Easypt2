# String.classPrototype.insertFrom

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `n /*Number*/, string /*String*/, [position = 0 /*Number*/], [length = string.length() /*Number*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Inserts bytes to string before n-th byte from substring of second
argument starting at given position and of given length.

### Example

```js
let x = "xyz";
x.insertFrom(1, "abc", 1, 2);
console.write(x);
```

**Expected output**

```
xbcyz
```

