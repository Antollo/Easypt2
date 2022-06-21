# Class

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`obj`]({{site.baseurl}}\built-ins\obj) 
</dd>
<dt> Inherits from </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
</dl>

### Description

Takes part in class creation.

### Example

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
        console.write("destructor ");
        // TODO: auto base class destructor calling
        this.prototype.prototype.destructor.call(this);
    }
};
let obj <- MyClass();
obj.method();

```

**Expected output**

```
baseConstructor baseDestructor | baseConstructor constructor method destructor baseDestructor 
```

### Members

- `constructor`


### Inherited members

- [`call`]({{site.baseurl}}\built-ins\Function\classPrototype\call\index)


### Properties

- `classPrototype`


