"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = void 0;
const vscode = require("vscode");
const path = require("path");
const os = require("os");
const child_process = require("child_process");
const lookpath_1 = require("lookpath");
class DocItem {
    constructor(label, processOutput) {
        var _a, _b, _c;
        const map = processOutput.split(/(?=[A-Z][a-z ]+:$)/gm).map(x => x.split(/(?<=[a-z]):(?=$)/gm)).reduce((a, c) => a.set(c[0], c[1].trim()), new Map());
        this.label = (label.match(/[_a-zA-Z][_a-zA-Z0-9]*$/g) || [label])[0];
        this.instanceOf = map.get('Instance of');
        this.parameters = map.get('Parameters');
        this.returnValue = map.get('Return value');
        this.description = (_a = map.get('Description')) === null || _a === void 0 ? void 0 : _a.replace(/\s+/g, ' ');
        this.example = map.get('Example');
        this.expectedOutput = map.get('Expected output');
        this.members = ((_b = map.get('Members')) === null || _b === void 0 ? void 0 : _b.split('\n').map(x => x.trim())) || [];
        this.properties = ((_c = map.get('Properties')) === null || _c === void 0 ? void 0 : _c.split('\n').map(x => x.trim())) || [];
    }
    static getDocFromProcess(source) {
        console.log(source);
        if (DocItem.sourceToDoc.has(source))
            return new Promise(resolve => resolve(DocItem.sourceToDoc.get(source)));
        return new Promise(resolve => {
            child_process.execFile('easypt', ['-docs', source], { encoding: 'utf16le' }, async (err, stdout) => {
                const processOutput = stdout.toString().replace(/\x1B\[[0-9]+m/g, '');
                if (processOutput.length == 0) {
                    DocItem.sourceToDoc.set(source, undefined);
                    resolve(undefined);
                }
                else {
                    const docItem = new DocItem(source, processOutput);
                    if (docItem.properties.indexOf('prototype') !== -1 && source.indexOf('prototype') == -1) {
                        const prototypeDoc = await DocItem.getDocFromProcess(`${source}.prototype`);
                        if (prototypeDoc !== undefined)
                            docItem.properties.push(...prototypeDoc.properties.filter(x => x !== 'prototype'));
                    }
                    DocItem.sourceToDoc.set(source, docItem);
                    resolve(docItem);
                }
            });
        });
    }
    toCompletionItem() {
        const result = new vscode.CompletionItem(this.label);
        result.insertText = (this.label.match(/[_a-zA-Z][_a-zA-Z0-9]*$/g) || [this.label])[0];
        result.detail = this.instanceOf;
        if (this.description !== undefined) {
            result.documentation = new vscode.MarkdownString(`${this.description}${this.example ? `\n#### Example:\n\`\`\`\n${this.example}\n\`\`\`` : ''}${this.expectedOutput ? `\n#### Expected output:\n${this.expectedOutput}` : ''}`);
        }
        if (result.detail === 'Function') {
            result.kind = vscode.CompletionItemKind.Function;
            if (this.parameters === undefined)
                result.detail += ' ()';
        }
        else if (result.detail === 'Class')
            result.kind = vscode.CompletionItemKind.Class;
        else
            result.kind = vscode.CompletionItemKind.Variable;
        if (this.parameters !== undefined)
            result.detail += ` (${this.parameters})`;
        if (this.returnValue !== undefined)
            result.detail += ` : ${this.returnValue}`;
        return result;
    }
    static getGlobalStackFromProcess() {
        if (DocItem.globalStack.length !== 0)
            return new Promise(resolve => resolve(DocItem.globalStack));
        return new Promise(resolve => {
            child_process.execFile('easypt', ['-execute', 'getStack().getOwnPropertyNames().forEach((x) => console.writeLine(x););'], { encoding: 'utf16le' }, (err, stdout) => {
                DocItem.globalStack = stdout.toString().replace(/\x1B\[[0-9]+m/g, '').split('\n').map(x => x.trim()).filter(x => x.length);
                resolve(DocItem.globalStack);
            });
        });
    }
}
DocItem.sourceToDoc = new Map();
DocItem.globalStack = [];
function notUndefined(x) {
    return x !== undefined;
}
async function getDefaultDocs(name) {
    const Object = await DocItem.getDocFromProcess(name);
    if (Object !== undefined) {
        return (await Promise.all(Object.members
            .map(member => DocItem.getDocFromProcess(`${name}.classPrototype.${member}`))))
            .filter(notUndefined)
            .map(x => {
            if (x.label.indexOf('.') === -1)
                x.label = `${name}.${x.label}`;
            return x;
        });
    }
    else
        return [];
}
function activate(context) {
    const config = vscode.workspace.getConfiguration('editor', { languageId: 'easypt' });
    config.update('quickSuggestions', true, vscode.ConfigurationTarget.Global, true);
    (async () => {
        const easyptPath = await lookpath_1.lookpath('easypt');
        if (easyptPath === undefined) {
            const result = await vscode.window.showInformationMessage('Easypt interpreter not found. Would you like to install Easypt interpreter?', 'Yes', 'No');
            if (result === 'Yes') {
                if (os.platform() === 'win32')
                    vscode.env.openExternal(vscode.Uri.parse('https://ci.appveyor.com/api/projects/antollo/Easypt2/artifacts/packages%2FEasypt-0.0.1-win32.exe?branch=master&job=Image%3A%20Visual%20Studio%202019'));
                else
                    vscode.env.openExternal(vscode.Uri.parse('https://ci.appveyor.com/api/projects/antollo/Easypt2/artifacts/packages%2FEasypt-0.0.1-Linux.sh?branch=master&job=Image%3A%20Ubuntu'));
            }
            else
                vscode.window.showInformationMessage('Some of the Easypt auto-completion features might not work.');
        }
        else
            vscode.window.showInformationMessage(`Easypt interpreter detected at: ${easyptPath}`);
    })();
    const runFile = vscode.commands.registerCommand('extension.runFile', () => {
        var _a;
        if (((_a = vscode.window.activeTextEditor) === null || _a === void 0 ? void 0 : _a.document) === undefined)
            vscode.window.showErrorMessage('There is no opened Easypt file.');
        else if (vscode.window.activeTextEditor.document.languageId !== 'easypt')
            vscode.window.showErrorMessage('There is no opened Easypt file.');
        else {
            let terminal = vscode.window.terminals.find(t => t.name === 'easypt');
            if (terminal === undefined)
                terminal = vscode.window.createTerminal({
                    name: 'easypt',
                    cwd: path.dirname(vscode.window.activeTextEditor.document.fileName)
                });
            terminal.show();
            terminal.sendText(`easypt -file "${vscode.window.activeTextEditor.document.fileName}"`);
        }
    });
    const runRepl = vscode.commands.registerCommand('extension.runRepl', async () => {
        let cwd = os.homedir();
        if (vscode.workspace.workspaceFolders !== undefined)
            cwd = vscode.workspace.workspaceFolders[0].uri.fsPath;
        const easyptPath = await lookpath_1.lookpath('easypt');
        if (easyptPath !== undefined) {
            const terminal = vscode.window.createTerminal({
                name: 'easypt repl',
                cwd,
                shellPath: easyptPath,
                shellArgs: '-repl'
            });
            terminal.show();
        }
        else
            vscode.window.showErrorMessage('Easypt interpreter not found.');
    });
    const provider1 = vscode.languages.registerCompletionItemProvider({ language: 'easypt' }, {
        async provideCompletionItems(_document, _position, _token, _context) {
            const doc = await DocItem.getGlobalStackFromProcess();
            const result = (await Promise.all(doc.map(name => DocItem.getDocFromProcess(name))))
                .map(x => x === null || x === void 0 ? void 0 : x.toCompletionItem())
                .filter(notUndefined);
            return result;
        }
    });
    const provider2 = vscode.languages.registerCompletionItemProvider({ language: 'easypt' }, {
        async provideCompletionItems(document, position) {
            const range = document.getWordRangeAtPosition(position, /(([_a-zA-Z][_a-zA-Z0-9]*)|("[^"\\]*(\\.[^"\\]*)*")|(\[[^\[\]]*\]))((\.[_a-zA-Z][_a-zA-Z0-9]*)|(\([^\)]*\)))*\.?/g);
            if (range !== undefined) {
                const text = document.getText(range);
                const source = text.substring(0, text.lastIndexOf('.'));
                const doc = await DocItem.getDocFromProcess(source);
                if (doc !== undefined) {
                    const result = (await Promise.all(doc.properties.map(property => DocItem.getDocFromProcess(`${source}.${property}`))
                        .concat(doc.members.map(member => DocItem.getDocFromProcess(`${source}.classPrototype.${member}`)))))
                        .map(x => x === null || x === void 0 ? void 0 : x.toCompletionItem())
                        .filter(notUndefined);
                    if (result.length !== 0) {
                        return result;
                    }
                }
                const result = [
                    ...(await getDefaultDocs('String')),
                    ...(await getDefaultDocs('Array')),
                    ...(await getDefaultDocs('Object'))
                ]
                    .map(x => x.toCompletionItem());
                return result;
            }
        }
    }, '.');
    context.subscriptions.push(runFile, runRepl, provider1, provider2);
}
exports.activate = activate;
//# sourceMappingURL=extension.js.map