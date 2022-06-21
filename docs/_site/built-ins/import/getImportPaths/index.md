# import.getImportPaths

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Array*/` 
</dd>
</dl>

### Description

Returns list of directories in which `import`
searches for files.

### Example

```js
let importPaths = import.getImportPaths();
let l = importPaths.length();
let interpreterExecutableParentPath = importPaths[l - 1];
let interpreterExecutablePath = importPaths[l - 2];
let currentPathFromInterpreterLaunch = importPaths[l - 3];
console.write(interpreterExecutablePath.indexOf(interpreterExecutableParentPath));
```

**Expected output**

```
0
```

