## Name: `Class`

#### Instance of: [`Class`](Class.md)

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
        // TODO: base class destructor calling
        console.write("destructor ");
    }
};
let obj <- MyClass();
obj.method();

```

##### Expected output:

```
baseConstructor baseDestructor | baseConstructor constructor method destructor 
```

#### Members:

- [`constructor`](Class.classPrototype.constructor.md)
- `prototype`


### Properties:

- `classPrototype`
- `prototype`


