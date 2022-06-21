# File.classPrototype.readTo

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `/*String*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*String*/` 
</dd>
</dl>

### Description

Input operation. 
Reads string terminated by given character
from file.

### Example

```js
let file = File();
file.create("abc.txt");
file.write("abbacda");
file.setReadPosition(0);
console.write(file.readTo("c"));
```

**Expected output**

```
abba
```

