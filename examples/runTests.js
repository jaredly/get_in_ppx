#!/usr/bin/env node
const fs = require('fs');
const path = require('path');
const { spawnSync } = require('child_process');

const examples = fs
    .readdirSync(__dirname)
    .map(name => path.join(__dirname, name))
    .filter(name => fs.statSync(name).isDirectory());

const checkRun = (cmd, args, dir) => {
    // We need "shell: true" for windows to work
    const res = spawnSync(cmd, args, {shell: true, cwd: dir, stdio: 'inherit'});
    if (res.status !== 0) {
        console.log(res.stdout)
        console.log(res.stderr)
        throw new Error(`${cmd} ${args.join(' ')} - exited with status ${res.status}`)
    }
}
const root = path.join(__dirname, '..')
checkRun('esy', [], root)
checkRun('esy', ['cp-bin'], root)


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
