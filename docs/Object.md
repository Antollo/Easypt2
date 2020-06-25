## Name: `Object`

#### Instance of: [`Class`](Class.md)

### Description:

Base class of all Easypt classes, all objects
are instances of Object.

#### Example:

```js
let obj; // Object is default type
obj.addProperty("first", 1);
obj.addProperty("second", "a");
obj.removeProperty("second");
if (obj.hasOwnProperty("second"))
    console.write(obj.second);
else
    console.write(obj.first);
```

##### Expected output:

```
1
```

#### Members:

- [`hasOwnProperty`](Object.classPrototype.hasOwnProperty.md)
- [`removeProperty`](Object.classPrototype.removeProperty.md)
- [`addProperty`](Object.classPrototype.addProperty.md)
- [`getOwnPropertyNames`](Object.classPrototype.getOwnPropertyNames.md)
- [`readOperator`](Object.classPrototype.readOperator.md)
- [`getId`](Object.classPrototype.getId.md)
- `prototype`


### Properties:

- `classPrototype`
- `prototype`


