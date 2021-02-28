## Name: `Array`

#### Instance of: [`Class`](Class.md)

#### Inherits from: [`Object`](Object.md)

### Description:

Array lists provides methods for manipulating arrays (list-like
objects). Array elements are indexed with non-negative integers.

#### Example:

```js
let arr = [36.6, "Harry", true];
arr.forEach(function {
    console.write(args[0], " is element with index ", args[1], ", ");
});
```

##### Expected output:

```
36.600000 is element with index 0, Harry is element with index 1, true is element with index 2, 
```

#### Members:

- `constructor`
- [`erase`](Array.classPrototype.erase.md)
- [`forEach`](Array.classPrototype.forEach.md)
- [`insert`](Array.classPrototype.insert.md)
- [`insertFrom`](Array.classPrototype.insertFrom.md)
- [`length`](Array.classPrototype.length.md)
- [`readOperator`](Array.classPrototype.readOperator.md)
- [`subarray`](Array.classPrototype.subarray.md)


#### Inherited members:

- [`addProperty`](Object.classPrototype.addProperty.md)
- [`getId`](Object.classPrototype.getId.md)
- [`getOwnPropertyNames`](Object.classPrototype.getOwnPropertyNames.md)
- [`hasOwnProperty`](Object.classPrototype.hasOwnProperty.md)
- [`readOperator`](Object.classPrototype.readOperator.md)
- [`removeProperty`](Object.classPrototype.removeProperty.md)
- [`toArray`](Object.classPrototype.toArray.md)
- [`toBoolean`](Object.classPrototype.toBoolean.md)
- [`toNumber`](Object.classPrototype.toNumber.md)
- [`toString`](Object.classPrototype.toString.md)


### Properties:

- `classPrototype`
- [`readOperator`](Array.readOperator.md)


