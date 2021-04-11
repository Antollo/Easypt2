## Name: `File.classPrototype.readTo`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*String*/`

#### Return value: `/*String*/`

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
console.write(file.readTo("c"));
```

##### Expected output:

```
abba
```

