## Name: `File.classPrototype.flush`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*File*/`

### Description:

Makes sure all output operations are
saved to the underlaying storage.

#### Example:

```js
let file = File();
file.create("abc.txt");
file.write("abbacda");
console.write(file.read());
file.flush();
// File contents are now saved o disk
file.close();
```

##### Expected output:

```
abbacda
```

