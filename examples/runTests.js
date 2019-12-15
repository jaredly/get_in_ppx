#!/usr/bin/env node
const fs = require('fs');
const path = require('path');
const { spawnSync } = require('child_process');

const examples = fs
    .readdirSync(__dirname)
    .map(name => path.join(__dirname, name))
    .filter(name => fs.statSync(name).isDirectory());

const checkRun = (cmd, args, dir) => {
    const res = spawnSync(cmd, args, {cwd: dir, stdio: 'inherit'});
    if (res.status !== 0) {
        throw new Error(`${cmd} ${args.join(' ')}`)
    }
}

const results = examples.map(dir => {
    console.log(`>> [[${path.basename(dir)}]]`);
    try {
        // For this particular ppx, we're unlikely to have dune-based projects in the near future :)
        // But I'm including this for the benefit of future ppxs that I might use the same setup for.
        if (fs.existsSync(path.join(dir, 'esy.json'))) {
            checkRun('esy', [], dir);
            checkRun('esy', ['test'], dir);
        } else {
            checkRun('npm', ['-s', 'i'], dir);
            checkRun('npm', ['-s', 'run', 'clean'], dir);
            checkRun('npm', ['-s', 'run', 'build'], dir);
            checkRun('npm', ['-s', 'test'], dir);
        }
    } catch (error) {
        return {succeeded: false, reason: error.message}
    }
    return {succeeded: true}
});

const failed = results.filter(result => !result.succeeded);

// console.log(results)
if (failed.length) {
    console.log(`ERROR: ${failed.length}/${results.length} failed`)
} else {
    console.log('All clear!')
}

process.exit(failed.length > 0 ? 1 : 0)
