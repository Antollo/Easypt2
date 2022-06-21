# import

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `file /*String or File*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Object*/` 
</dd>
</dl>

### Description

Imports source code (module) from another *.ez file (source code is parsed and 
executed, return value is returned as import return value). First and only 
parameter is filename or opened `File` object. Files are searched in directories 
listed in `import.getImportPaths()`. Imported modules are cached in `modules`.

### Example

```js
let file = File();
file.create("maths.ez");
file.write("return { multiply: (a, b) => a * b; };");
file.close();
let importedModule = import("maths.ez");
/* A new cache entry has been inserted into the modules:
{
    "maths": {
        "name": "maths",
        "filename": "maths.ez",
        "path": "C:\Easypt\library",
        "exports": {
            "multiply": <function>
        }
    },
    ...
}*/
file.remove();
console.write(importedModule.multiply(4, 6), " ");
console.write(modules.maths.name, " ");
console.write(modules.maths.filename, " ");
console.write(modules.maths.exports.multiply instanceOf Function);
```

**Expected output**

```
24 maths maths.ez true
```

### Properties

- [`getImportPaths`]({{site.baseurl}}\built-ins\import\getImportPaths\index)


