
Basic plan:

- have tests
- have CI that runs on pull-request and runs the tests on all platforms,

Releases:
- you make a github release with a specific version
- CI sees that, makes a commit updating the package.json
- builds for each platform
- makes an npm package, publishes it with the tag `alpha`
- you then manually install from npm, do a gut-check, and use `dist-tag` to promote to `latest`
- the version that gets published should support both dune and npm.
