## Name: `Class`

#### Instance of: [`Class`](Class.md)

#### Inherits from: [`Function`](Function.md)

### Description:

Takes part in class creation.

#### Example:

```js
class MyBaseClass {
    function constructor {
        console.write("baseConstructor ");
    },
    function method {
        console.write("baseMethod ");
    },
    function destructor {
        console.write("baseDestructor ");
    }
};
MyBaseClass();

console.write("| ");

class MyClass extends MyBaseClass {
    function constructor {
        this.super();
        console.write("constructor ");
    },
    function method {
        console.write("method ");
    },
    function destructor {
        // TODO: auto base class destructor calling
        this.prototype.prototype.destructor();
        console.write("destructor ");
    }
};
let obj <- MyClass();
obj.method();

```

##### Expected output:

```
baseConstructor baseDestructor | baseConstructor constructor method baseDestructor destructor 
```

#### Members:

- `constructor`
- `prototype`


### Properties:

- `classPrototype`
- `prototype`


