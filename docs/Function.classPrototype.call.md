## Name: `Function.classPrototype.call`

#### Instance of: [`Function`](Function.md)

#### Parameters: `caller, [...]`

#### Return value: `return value of function /*Object*/`

### Description:

Calls function with the first argument as caller
and passes following arguments to this function.

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

