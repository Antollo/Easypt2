## Name: `Function.classPrototype.call`

#### Instance of: [`Function`](Function.md)

#### Parameters: `caller, [...]`

### Description:

Calls function with first argument as caller
and passes following to this function.

#### Example:

```js
let x = String.classPrototype.substring.call("abc", 1, 1);
// equivalent of "abc".substring(1, 1)
console.write(x);
```

##### Expected output:

```
b
```

### Properties:

- `prototype`


