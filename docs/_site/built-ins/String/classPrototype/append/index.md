# String.classPrototype.append

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `str /*String*/, [position = 0 /*Number*/, length = str.length() /*Number*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Appends str to this string (or optionally 
substring starting from given position 
and of given length).

### Example

```js
let str = "Roses are ";
let str2 = "red, Violets are blue.";
str.append(str2);
console.write(str);
str = "Roses are ";
str.append(str2, 17, 5);
console.write(" | ", str);
```

**Expected output**

```
Roses are red, Violets are blue. | Roses are blue.
```

