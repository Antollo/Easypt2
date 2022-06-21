# File.classPrototype.constructor

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `[path /*String*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Optionally opens file specified by the path.

### Example

```js
File().create("abc.txt").write("abc");
console.write(File("abc.txt").read());
```

**Expected output**

```
abc
```

