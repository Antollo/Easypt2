## Name: `Array.classPrototype.append`

#### Instance of: [`Function`](Function.md)

#### Parameters: `arr /*Array*/, [position = 0 /*Number*/, length = arr.length() /*Number*/]`

#### Return value: `this`

### Description:

Appends arr to this array (or optionally 
subarray starting from given position 
and of given length).

#### Example:

```js
let arr = [1, 2, 3];
let arr2 = [4, 5, 6];
arr.append(arr2);
console.write(arr);
```

##### Expected output:

```
[1, 2, 3, 4, 5, 6]
```

