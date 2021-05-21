//Forces the environment to be Node, to run with babel-tape-runner

try {
    Module.ENVIRONMENT = NODE;
} catch (e) {
    // do nothing
}