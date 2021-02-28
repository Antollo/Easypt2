## Name: `Object.classPrototype.toString`

#### Instance of: [`Function`](Function.md)

### Description:

Converts object to number:
- from `Number` (serializes the number to string)
- from `Array` (serializes the array to string)
- from `Boolean` (serializes the boolean to string)
- from other types (if not default `toString` method 
  exists, calls it, otherwise serializes the object 
  to JSON)

#### Example:

```js
console.write(12.1e1.toString(), "
", { x: 1 }.toString());
```

##### Expected output:

```
121
{
    "x": 1
}
```

### Properties:



