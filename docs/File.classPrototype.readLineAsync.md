## Name: `File.classPrototype.readLineAsync`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*Promise*/`

### Description:

Input operation. 
Reads string terminated by new line character 
from file.

#### Example:

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

##### Expected output:

```
http 80
```

### Properties:

- `prototype`


