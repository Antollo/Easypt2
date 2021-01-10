## Name: `File.classPrototype.writeLineAsync`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*any number of convertible to String parameters*/`

#### Return value: `/*Promise*/`

### Description:

Output operation. 
Writes arguments appended with the new line 
character to file.

#### Example:

```js
let file = File();
file.create("protocols.txt");
await file.writeLineAsync("http");
await file.writeAsync(80);
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


