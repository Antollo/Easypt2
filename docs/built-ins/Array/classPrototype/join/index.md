# Array.classPrototype.join

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `[separator = "," /*String*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*String*/` 
</dd>
</dl>

### Description

The `join` method creates and returns a new string by concatenating all of the elements in an 
array, separated by commas or a specified separator string. If the array has only one item, 
then that item will be returned without using the separator.

### Example

```js
let elements = ["Fire", "Air", "Water"];
console.write(elements.join());
console.write("\n");
console.write(elements.join(""));
console.write("\n");
console.write(elements.join("-"));
```

**Expected output**

```
Fire,Air,Water
FireAirWater
Fire-Air-Water
```

---
<sub>This page has text and code adapted from [https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/join](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/join)</sub> \
<sub>Prose content on this page is available under
([CC-BY-SA 2.5](https://creativecommons.org/licenses/by-sa/2.5/)).</sub> \
<sub>Code examples on this page and snippets are in the public domain
([CC0](https://creativecommons.org/publicdomain/zero/1.0/legalcode)).</sub>

