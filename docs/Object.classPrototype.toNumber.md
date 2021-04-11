## Name: `Object.classPrototype.toNumber`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*Number*/`

### Description:

Converts object to number:
- from `String` (parses the string as number)
- from `Array` (returns length of the array)
- from `Boolean` (returns 1 if true and 0 otherwise)
- from other types (if not default `toNumber` method 
  exists, calls it, otherwise returns the number of 
  properties not including prototype property)

#### Example:

```js
console.write("12.1e1".toNumber(), " ", false.toNumber());
```

##### Expected output:

```
121 0
```

