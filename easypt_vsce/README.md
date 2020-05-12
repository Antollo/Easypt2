# Easypt extension

[![vs marketplace](https://img.shields.io/vscode-marketplace/v/Antollo.easypt.svg?label=vs%20marketplace)](https://marketplace.visualstudio.com/items?itemName=Antollo.easypt)
[![downloads](https://img.shields.io/vscode-marketplace/d/Antollo.easypt.svg)](https://marketplace.visualstudio.com/items?itemName=Antollo.easypt)
[![rating](https://img.shields.io/vscode-marketplace/r/Antollo.easypt.svg)](https://marketplace.visualstudio.com/items?itemName=Antollo.easypt)

## Features

Highlight keywords, comments, strings and numeric literals in Easypt scripts (`*.ez` files). Provides basic snippets.

## Requirements

Manually add following `tasks.json` to your project's `.vscode` folder:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Run Easypt source",
            "type": "process",
            "command": "easypyt2",
            "args": [
                "-file",
                "${file}"
            ],
            "presentation": {
                "reveal": "always",
                "panel": "new"
            },
            "problemMatcher": [],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

Add easypt interpreter (`easypt2.exe` - Windows or `easypt2` - Linux) to PATH. You can get latest interpreter from [here](https://ci.appveyor.com/project/Antollo/easypt2).

Now you can run your code with `Ctrl+Shift+B`.

## Last changes

- `json` and `class` highlights

## For more information

* [Easypt on Github](https://github.com/Antollo/Easypt2)

**Enjoy!**
