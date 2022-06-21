# File.classPrototype.readAsync

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Promise of String*/` 
</dd>
</dl>

### Description

Input operation. 
Reads word terminated by whitespace from file.

### Example

```js
let file = File();
file.create("protocols.txt");
file.writeLine("http").write(80);
file.setReadPosition(0);

let name = await file.readLineAsync();
let port = await file.readAsync();
file.close();

console.write(name, " ", port);
```

