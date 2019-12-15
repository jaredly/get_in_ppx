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
        if (fs.existsSync(path.join(dir, 'esy.json'))) {
            checkRun('esy', [], dir);
            checkRun('esy', ['test'], dir);
        } else {
            checkRun('npm', ['i'], dir);
            checkRun('npm', ['run', 'build'], dir);
            checkRun('npm', ['test'], dir);
        }
    } catch (error) {
        return {succeeded: false, reason: error.message}
    }
    return {succeeded: true}
});

const succeeds = results.map(result => result.succeeded);
const failures = succeeds.reduce((count, success) => success ? count : count + 1, 0);

if (failures.length) {
    console.log(`${failures.length}/${results.length} failed`)
} else {
    console.log('All clear!')
}

process.exit(failures.length > 0 ? 1 : 0)
