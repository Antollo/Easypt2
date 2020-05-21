## Name: `File.classPrototype.close`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*File*/`

### Description:

Closes the file, makes sure all
input/output operations are
saved to disk.

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


