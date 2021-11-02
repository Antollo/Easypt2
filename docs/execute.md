## Name: `execute`

#### Instance of: [`Function`](Function.md)

#### Parameters: `code /*String*/`

#### Return value: `/*Object*/`

### Description:

Parses code from string and executes it (using current stack - variable declared in
string is accessible outside the execute function). It is dangerous (it could execute
malicious code) function.

#### Example:

```js
let result = execute("
    let x = 7;
    let y = 3;
    let z = x*y + y;
    return z; 
");
console.write(result, " ", z);
```

##### Expected output:

```
24 24
```

