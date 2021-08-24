function merge(left, right) {
    let array = []
    let leftIndex = 0
    let rightIndex = 0
    while (leftIndex < left.length && rightIndex < right.length)
        array.push(left[leftIndex] < right[rightIndex] ? left[leftIndex++] : right[rightIndex++])
    return array
        .concat(left.slice(leftIndex))
        .concat(right.slice(rightIndex))
}

%NeverOptimizeFunction(merge)

function mergeSort(array) {
    if (array.length <= 1)
        return array
    let pivot = Math.floor(array.length / 2)
    return merge(mergeSort(array.slice(0, pivot)), mergeSort(array.slice(pivot)))
}

%NeverOptimizeFunction(mergeSort)


const fs = require('fs')
const path = require('path')
const array = fs.readFileSync(path.resolve(__dirname, 'numbers.txt')).toString().split('\n').map(n => Number(n))


let iterations = 100
let mean = 0
let time


for (let i = 0; i < iterations; i++) {
    time = Date.now()
    mergeSort(array)
    mean = mean + Date.now() - time
    process.stdout.write(`\r--- ${Math.round(i / iterations * 100  + 1)}% ---`)
}

mean = mean / iterations;
process.stdout.write(`\nMean time: ${mean} ms\n`)