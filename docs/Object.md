## Name: `Object`

#### Instance of: [`Class`](Class.md)

#### Inherits from: [`Object`](Object.md)

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

- `==`
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


#### Inherited members:

- `==`
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


