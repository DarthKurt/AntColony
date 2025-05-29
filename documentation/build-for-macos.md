# Build process for MacOS targets

## Mount SDK to workspace

1. Create an OSX Cross SDK package
2. Mount it to the `/sdks/osxcross` into workspace, for example:

```JSON
{
    "mounts": [
            // OSX SDKs can't be distibuted 
            "source=${localEnv:HOME}/SDKs/osxcross-osx-15.4/,target=/sdks/osxcross,type=bind,consistency=cached"
        ],
}

```