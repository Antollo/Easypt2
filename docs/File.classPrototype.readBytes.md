## Name: `File.classPrototype.readBytes`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*Number*/`

#### Return value: `/*String*/`

### Description:

Reads string of given length
from file.

#### Example:

```js
let file = File();
file.create("abc.txt");
file.write("abbacda");
file.setReadPosition(0);
console.write(file.readBytes(4));
```

##### Expected output:

```
abba
```

### Properties:

- `prototype`


