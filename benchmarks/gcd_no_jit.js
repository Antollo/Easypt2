function gcd(a, b) {
    while (a != b) {
        if (a < b)
            b = b - a
        else
            a = a - b
    }
    return a
}

%NeverOptimizeFunction(gcd)

let iterations = 100
let mean = 0
let time


for (let i = 0; i < iterations; i++) {
    time = Date.now()
    gcd(144, 1836311903)
    mean = mean + Date.now() - time
    process.stdout.write(`\r--- ${Math.round(i / iterations * 100  + 1)}% ---`)
}

mean = mean / iterations;
process.stdout.write(`\nMean time: ${mean} ms\n`)