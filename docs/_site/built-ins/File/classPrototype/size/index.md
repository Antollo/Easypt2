# File.classPrototype.size

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Number*/` 
</dd>
</dl>

### Description

Returns the size of file in bytes.

### Example

```js
let file = File();
file.create("abc.txt");
file.write("abbacda");
console.write(file.size());
file.close();
```

**Expected output**

```
7
```

