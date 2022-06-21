# console.writeLineAsync

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `/*any number of convertible to String parameters*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Promise*/` 
</dd>
</dl>

### Description

Writes arguments appended with the new line character or 
characters sequence (platform dependent) to standard 
output stream.

### Example

```js
await console.writeLineAsync("seven: ", 7);
```

**Expected output**

```
seven: 7

```

