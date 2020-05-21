## Name: `parse`

#### Instance of: [`Function`](Function.md)

#### Parameters: `code /*String*/`

#### Return value: `/*Function*/`

### Description:

Parses code from string to Function object. It is dangerous (it could parse malicious 
code) function.

#### Example:

```js
let fun = parse("
    let x = 7;
    let y = 3;
    return x*y + y;");
let result = fun();
console.write(result);
```

##### Expected output:

```
24
```

### Properties:

- `prototype`


