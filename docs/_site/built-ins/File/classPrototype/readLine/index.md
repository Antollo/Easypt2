# File.classPrototype.readLine

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*String*/` 
</dd>
</dl>

### Description

Input operation. 
Reads string terminated by new line character 
from file.

### Example

```js
let file = File();
file.create("protocols.txt");
file.writeLine("http").write(80);
file.setReadPosition(0);

let name = file.readLine();
let port = file.read();
file.close();

console.write(name, " ", port);
```

**Expected output**

```
http 80
```

