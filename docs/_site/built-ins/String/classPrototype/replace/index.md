# String.classPrototype.replace

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `pattern /*String*/, replacement /*String*/, [inPlace = false /*Boolean*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Number*/` 
</dd>
</dl>

### Description

The `replace` method returns a new string with the first match of a pattern replaced 
by a replacement. If `inPlace` is true, the original string is modified

### Example

```js
let sentence = "The quick brown fox jumps over the lazy dog. If the dog reacted, was it really lazy?";
console.write(sentence.replace("dog", "monkey"));
```

**Expected output**

```
The quick brown fox jumps over the lazy monkey. If the dog reacted, was it really lazy?
```

---
<sub>This page has text and code adapted from [https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replace](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replace)</sub> \
<sub>Prose content on this page is available under
([CC-BY-SA 2.5](https://creativecommons.org/licenses/by-sa/2.5/)).</sub> \
<sub>Code examples on this page and snippets are in the public domain
([CC0](https://creativecommons.org/publicdomain/zero/1.0/legalcode)).</sub>

