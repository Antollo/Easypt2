# File.classPrototype.create

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `path /*String*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*File*/` 
</dd>
</dl>

### Description

Creates (or open and remove contents) 
file in the specified path. If the 
file object was already associated 
with existing file close it first.

### Example

```js
let file = File();
file.create("protocols.txt");
file.remove();
try {
    file.open("protocols.txt");
} catch {
    console.write("error");
}
```

**Expected output**

```
error
```

