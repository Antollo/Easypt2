# String.classPrototype.split

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `delimeter /*String*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Array*/` 
</dd>
</dl>

### Description

Returns array of substrings delimited by 
the delimeter in the original string.

### Example

```js
let arr = "abcd abcde abcdef bcdefg".split("cd");
for (let i = 0; i<arr.length(); i++)
    console.write(arr[i], ", ");
```

**Expected output**

```
ab,  ab, e ab, ef b, efg, 
```

