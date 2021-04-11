## Name: `Array.classPrototype.erase`

#### Instance of: [`Function`](Function.md)

#### Parameters: `position /*Number*/, [length = array.length() /*Number*/]`

#### Return value: `this`

### Description:

Erases elements from range [position, position + length).

#### Example:

```js
let arr = [1, 2, 3];
arr.erase(1, 1);
console.write(arr[0], arr[1]);
```

##### Expected output:

```
13
```

