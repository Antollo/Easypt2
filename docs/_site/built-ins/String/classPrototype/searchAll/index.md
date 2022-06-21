# String.classPrototype.searchAll

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `regex /*String*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Array*/` 
</dd>
</dl>

### Description

Returns array of object in form (position, length)
representing all substrings that matches regex.

### Example

```js
let arr = "Roses are red.".searchAll("[a-z]+");
for (let i = 0; i < arr.length(); i++)
    console.write(arr[i].position, " ", arr[i].length, ", ");
```

**Expected output**

```
1 4, 6 3, 10 3, 
```

