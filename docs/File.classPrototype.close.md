## Name: `File.classPrototype.close`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*File*/`

### Description:

Performs flush and closes the associated file.

#### Example:

```js
let file = File();
file.create("abc.txt");
file.write("abbacda");
console.write(file.size());
file.close();
```

##### Expected output:

```
7
```

