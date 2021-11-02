## Name: `File`

#### Instance of: [`Class`](Class.md)

#### Inherits from: [`Object`](Object.md)

### Description:

File class provides file input and output operations.

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

#### Members:

- [`close`](File.classPrototype.close.md)
- `constructor`
- [`create`](File.classPrototype.create.md)
- [`flush`](File.classPrototype.flush.md)
- [`getReadPosition`](File.classPrototype.getReadPosition.md)
- [`getWritePosition`](File.classPrototype.getWritePosition.md)
- [`open`](File.classPrototype.open.md)
- [`read`](File.classPrototype.read.md)
- [`readAsync`](File.classPrototype.readAsync.md)
- [`readBytes`](File.classPrototype.readBytes.md)
- [`readBytesAsync`](File.classPrototype.readBytesAsync.md)
- [`readLine`](File.classPrototype.readLine.md)
- [`readLineAsync`](File.classPrototype.readLineAsync.md)
- [`readTo`](File.classPrototype.readTo.md)
- [`readToAsync`](File.classPrototype.readToAsync.md)
- [`remove`](File.classPrototype.remove.md)
- [`setReadPosition`](File.classPrototype.setReadPosition.md)
- [`setWritePosition`](File.classPrototype.setWritePosition.md)
- [`size`](File.classPrototype.size.md)
- [`write`](File.classPrototype.write.md)
- [`writeAsync`](File.classPrototype.writeAsync.md)
- [`writeLine`](File.classPrototype.writeLine.md)
- [`writeLineAsync`](File.classPrototype.writeLineAsync.md)


#### Inherited members:

- `==`
- [`addProperty`](Object.classPrototype.addProperty.md)
- [`getId`](Object.classPrototype.getId.md)
- [`getOwnPropertyNames`](Object.classPrototype.getOwnPropertyNames.md)
- [`hasOwnProperty`](Object.classPrototype.hasOwnProperty.md)
- [`readOperator`](Object.classPrototype.readOperator.md)
- [`removeProperty`](Object.classPrototype.removeProperty.md)
- [`toArray`](Object.classPrototype.toArray.md)
- [`toBoolean`](Object.classPrototype.toBoolean.md)
- [`toNumber`](Object.classPrototype.toNumber.md)
- [`toString`](Object.classPrototype.toString.md)


### Properties:

- `classPrototype`


