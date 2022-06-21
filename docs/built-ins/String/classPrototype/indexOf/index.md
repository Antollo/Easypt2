# String.classPrototype.indexOf

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `key /*String*/, [position /*Number*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Number*/` 
</dd>
</dl>

### Description

Returns the index of the first occurrence of key 
(optionaly starting from given position) 
or -1 if the key is not found.

### Example

```js
let str = "Roses are red, Violets are blue, If you're reading this, I'm sorry for you.";
let a = str.indexOf("are");
let b = str.indexOf("are", a + 1);
let c = str.indexOf("yellow");
console.write(a, " ", b, " ", c);
```

**Expected output**

```
6 23 -1
```

