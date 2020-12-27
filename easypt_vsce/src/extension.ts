import * as vscode from 'vscode';
import * as path from 'path';
import * as os from 'os';
import *  as child_process from 'child_process';
import { lookpath } from 'lookpath';

class DocItem {
    constructor(label: string, processOutput: string) {
        const map = processOutput.split(/(?=[A-Z][a-z ]+:$)/gm).map(x => x.split(/(?<=[a-z]):(?=$)/gm)).reduce((a, c) => a.set(c[0], c[1].trim()), new Map<string, string>());
        this.label = (label.match(/[_a-zA-Z][_a-zA-Z0-9]*$/g) || [label])[0];
        this.instanceOf = map.get('Instance of');
        this.parameters = map.get('Parameters');
        this.returnValue = map.get('Return value');
        this.description = map.get('Description')?.replace(/\s+/g, ' ');
        this.example = map.get('Example');
        this.expectedOutput = map.get('Expected output');
        this.members = map.get('Members')?.split('\n').map(x => x.trim()) || [];
        this.properties = map.get('Properties')?.split('\n').map(x => x.trim()) || [];
    }
    static sourceToDoc: Map<string, DocItem | undefined> = new Map();
    static getDocFromProcess(source: string): Promise<DocItem | undefined> {
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
    toCompletionItem(): vscode.CompletionItem {
        const result: vscode.CompletionItem = new vscode.CompletionItem(this.label);
        result.insertText = (this.label.match(/[_a-zA-Z][_a-zA-Z0-9]*$/g) || [this.label])[0];
        result.detail = this.instanceOf;
        if (this.description !== undefined) {
            result.documentation = new vscode.MarkdownString(
                `${this.description}${this.example ? `\n#### Example:\n\`\`\`\n${this.example}\n\`\`\`` : ''}${this.expectedOutput ? `\n#### Expected output:\n${this.expectedOutput}` : ''}`
            );
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
    static globalStack: string[] = [];
    static getGlobalStackFromProcess(): Promise<string[]> {
        if (DocItem.globalStack.length !== 0)
            return new Promise(resolve => resolve(DocItem.globalStack));
        return new Promise(resolve => {
            child_process.execFile('easypt', ['-execute', 'getStack().getOwnPropertyNames().forEach((x) => console.writeLine(x););'], { encoding: 'utf16le' }, (err, stdout) => {
                DocItem.globalStack = stdout.toString().replace(/\x1B\[[0-9]+m/g, '').split('\n').map(x => x.trim()).filter(x => x.length);
                resolve(DocItem.globalStack);
            });
        });
    }
    label: string;
    instanceOf?: string;
    parameters?: string;
    returnValue?: string;
    description?: string;
    example?: string;
    expectedOutput?: string;
    members: string[];
    properties: string[];
}

function notUndefined<T>(x: T | undefined): x is T {
    return x !== undefined;
}

async function getDefaultDocs(name: string) {
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
    } else
        return [];
}

export function activate(context: vscode.ExtensionContext) {

    const config = vscode.workspace.getConfiguration('editor',  {languageId: 'easypt'});
    config.update('quickSuggestions', true, vscode.ConfigurationTarget.Global, true);

    (async () => {
        const easyptPath = await lookpath('easypt');
        if (easyptPath === undefined) {
            const result = await vscode.window.showInformationMessage('Easypt interpreter not found. Would you like to install Easypt interpreter?', 'Yes', 'No');
            if (result === 'Yes') {
                if (os.platform() === 'win32')
                    vscode.env.openExternal(vscode.Uri.parse('https://ci.appveyor.com/api/projects/antollo/Easypt2/artifacts/packages%2FEasypt-0.0.1-win32.exe?branch=master&job=Image%3A%20Visual%20Studio%202019'));
                else
                    vscode.env.openExternal(vscode.Uri.parse('https://ci.appveyor.com/api/projects/antollo/Easypt2/artifacts/packages%2FEasypt-0.0.1-Linux.sh?branch=master&job=Image%3A%20Ubuntu'));
            } else
                vscode.window.showInformationMessage('Some of the Easypt auto-completion features might not work.');
        } else
            vscode.window.showInformationMessage(`Easypt interpreter detected at: ${easyptPath}`);
    })();


    const runFile = vscode.commands.registerCommand('extension.runFile', () => {
        if (vscode.window.activeTextEditor?.document === undefined)
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
        let cwd: string = os.homedir();
        if (vscode.workspace.workspaceFolders !== undefined)
            cwd = vscode.workspace.workspaceFolders[0].uri.fsPath;
        const easyptPath = await lookpath('easypt');
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

        async provideCompletionItems(_document: vscode.TextDocument, _position: vscode.Position, _token: vscode.CancellationToken, _context: vscode.CompletionContext) {
            const doc = await DocItem.getGlobalStackFromProcess();
            const result = (
                await Promise.all(doc.map(name => DocItem.getDocFromProcess(name)))
            )
                .map(x => x?.toCompletionItem())
                .filter(notUndefined);
            return result;
        }
    });

    const provider2 = vscode.languages.registerCompletionItemProvider({ language: 'easypt' }, {
        async provideCompletionItems(document: vscode.TextDocument, position: vscode.Position) {

            const range = document.getWordRangeAtPosition(position, /(([_a-zA-Z][_a-zA-Z0-9]*)|("[^"\\]*(\\.[^"\\]*)*")|(\[[^\[\]]*\]))((\.[_a-zA-Z][_a-zA-Z0-9]*)|(\([^\)]*\)))*\.?/g);
            if (range !== undefined) {
                const text = document.getText(range);
                const source = text.substring(0, text.lastIndexOf('.'));
                const doc = await DocItem.getDocFromProcess(source);
                if (doc !== undefined) {
                    const result = (await Promise.all(
                        doc.properties.map(property => DocItem.getDocFromProcess(`${source}.${property}`))
                            .concat(doc.members.map(member => DocItem.getDocFromProcess(`${source}.classPrototype.${member}`)))
                    ))
                        .map(x => x?.toCompletionItem())
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
    },
        '.'
    );

    context.subscriptions.push(runFile, runRepl, provider1, provider2);
}