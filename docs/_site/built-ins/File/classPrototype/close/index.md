# File.classPrototype.close

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*File*/` 
</dd>
</dl>

### Description

Performs flush and closes the associated file.

### Example

```js
let file = File();
file.create("abc.txt");
file.write("abbacda");
console.write(file.size());
file.close();
```

**Expected output**

```
7
```

