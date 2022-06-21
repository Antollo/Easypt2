# String.classPrototype.erase

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `position /*Number*/, [length = string.length() /*Number*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Erases bytes from range [position, position + length).

### Example

```js
let str = "abcdefg";
str.erase(2, 3);
console.write(str);
```

**Expected output**

```
abfg
```

