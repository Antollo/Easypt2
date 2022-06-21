# Timeout

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Class`]({{site.baseurl}}\built-ins\Class) 
</dd>
<dt> Inherits from </dt><dd markdown="1">
 [`Promise`]({{site.baseurl}}\built-ins\Promise) 
</dd>
</dl>

### Description

Asynchronously waits for the given amount 
of time (in milliseconds). Completes after 
finishing waiting.

### Example

```js
// Wait for ms
Timeout(1).then(() => console.write(1););
console.write(0);
await Timeout(23);
console.write(23);
```

**Expected output**

```
0123
```

### Members

- [`constructor`]({{site.baseurl}}\built-ins\Timeout\classPrototype\constructor\index)


### Inherited members

- [`await`]({{site.baseurl}}\built-ins\Promise\classPrototype\await\index)
- `constructor`
- [`then`]({{site.baseurl}}\built-ins\Promise\classPrototype\then\index)


### Properties

- `classPrototype`


