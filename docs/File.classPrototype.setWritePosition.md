## Name: `File.classPrototype.setWritePosition`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*Number*/`

### Description:

Sets write position of file
(counted from begining in bytes).

#### Example:

```js
let file = File();
file.create("ab.txt");
file.write("aaaaa");
file.setWritePosition(2);
file.write("bb");
file.setReadPosition(1);
console.write(file.readBytes(4), " ");
console.write(file.getReadPosition(), " ");
console.write(file.getWritePosition());
```

##### Expected output:

```
abba 5 5
```

### Properties:

- `prototype`


