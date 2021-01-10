## Name: `File.classPrototype.write`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*any number of convertible to String parameters*/`

#### Return value: `/*File*/`

### Description:

Output operation. 
Writes arguments to file.

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

### Properties:

- `prototype`


