## Name: `File.classPrototype.setReadPosition`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*Number*/`

### Description:

Input operation. 
Sets read position of file
(counted from begining in bytes). 
All input operations set the write 
position to 0, and consistently all 
output operations set the read 
position to 0.

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
abba 5 0
```

### Properties:

- `prototype`


