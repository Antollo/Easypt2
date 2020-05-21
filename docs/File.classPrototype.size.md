## Name: `File.classPrototype.size`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*Number*/`

### Description:

Returns the size of file in bytes.

#### Example:

```js
let file = File();
file.create("abc.txt");
file.write("abbacda");
file.close();
console.write(file.size());
```

##### Expected output:

```
7
```

### Properties:

- `prototype`


