## Name: `File.classPrototype.read`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*String*/`

### Description:

Input operation. 
Reads word terminated by whitespace from file.

#### Example:

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

##### Expected output:

```
http 80
```

