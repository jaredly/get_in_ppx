#!/usr/bin/env node

const core = require('@actions/core');
const { context, GitHub } = require('@actions/github');
const { spawnSync } = require('child_process');
const semver = require('semver');

const TRIGGER = /^actions: release (major|minor|patch|premajor|preminor)$/m;

const packageName = require('../../package.json').name;

const getMostRecentVersion = () => {
    const result = spawnSync('npm', ['show', packageName, 'version'], {
        cwd: __dirname,
        encoding: 'utf8',
    });
    if (result.status !== 0) {
        throw new Error('Unable to get most recent version of ' + packageName);
    }
    return result.stdout.trim();
};

const [_, __, arg] = process.argv;

async function run() {
    const { GITHUB_TOKEN } = process.env;
    const { owner, repo } = context.repo;
    const client = new GitHub(GITHUB_TOKEN);

    const match = context.payload.issue.title.match(TRIGGER);

    if (!match) {
        core.setFailed('No trigger')
        return;
    }

    const collab = await client.repos.getCollaboratorPermissionLevel({
        owner,
        repo,
        username: context.payload.issue.user.login,
    });

    if (collab.data.permission !== 'admin') {
        console.log(
            'not allowed',
            collab.data.permission,
            context.payload.issue.user.login,
        );
        core.setFailed('Insufficient rights')
        return;
    }

    const releaseType = match[1];
    const nextVersion = semver.inc(getMostRecentVersion(), releaseType);

    core.setOutput('version', nextVersion);

    if (arg && arg === '--no-comment') {
        return;
    }

    await client.issues.createComment({
        owner,
        repo,
        issue_number: context.payload.issue.number,
        body: `🚀 Starting release, version ${nextVersion}.\n\nGo to the [actions tab](https://github.com/${owner}/${repo}/actions/) to cancel it if that's not what you want.`,
    });
}

run().catch(err => {
    console.error(err);
    core.setFailed('Unexpected error');
});
