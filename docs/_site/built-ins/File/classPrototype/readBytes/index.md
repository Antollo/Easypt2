# File.classPrototype.readBytes

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `/*Number*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*String*/` 
</dd>
</dl>

### Description

Input operation. 
Reads string of given length (in bytes)
from file.

### Example

```js
let file = File();
file.create("abc.txt");
file.write("abbacda");
file.setReadPosition(0);
console.write(file.readBytes(4));
```

**Expected output**

```
abba
```

