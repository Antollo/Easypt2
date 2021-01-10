## Name: `File.classPrototype.readToAsync`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*String*/`

#### Return value: `/*Promise*/`

### Description:

Input operation. 
Reads string terminated by given character
from file.

#### Example:

```js
let file = File();
file.create("abc.txt");
file.write("abbacda");
file.setReadPosition(0);
console.write(await file.readToAsync("c"));
```

##### Expected output:

```
abba
```

### Properties:

- `prototype`


