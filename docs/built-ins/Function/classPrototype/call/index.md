# Function.classPrototype.call

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `caller, [...]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `return value of function /*Object*/` 
</dd>
</dl>

### Description

Calls function with the first argument as caller
and passes following arguments to this function.

### Example

```js
let x = String.classPrototype.substring.call("abc", 1, 1);
// equivalent of "abc".substring(1, 1)
console.write(x);
```

**Expected output**

```
b
```

