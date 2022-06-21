# File.classPrototype.writeLine

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `/*any number of convertible to String parameters*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*File*/` 
</dd>
</dl>

### Description

Output operation. 
Writes arguments appended with the new line 
character to file.

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

