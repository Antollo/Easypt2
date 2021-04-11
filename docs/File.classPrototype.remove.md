## Name: `File.classPrototype.remove`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*File*/`

### Description:

Deletes the recently associated file. 
If the file object was associated with 
file close it first.

#### Example:

```js
let file = File();
file.create("protocols.txt");
file.remove();
try {
    file.open("protocols.txt");
} catch {
    console.write("error");
}
```

##### Expected output:

```
error
```

