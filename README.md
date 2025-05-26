# GLPP / OpenGL++

Helper Classes that wrap OpenGL functionality.

## Building

```sh
git clone https://github.com/automas-dev/glpp.git
cd glpp
./post-clone.sh
make build
```

### Installing

```sh
make install
```

To install the release version, set the `CONFIG` var before building.

```sh
make config CONFIG=Release
make build CONFIG=Release
make install CONFIG=Release
```

## Deprecation Notice

The following have been deprecated and will be removed or replaced in a future
release.

| What will be removed      | Why                                                 |
| ------------------------- | --------------------------------------------------- |
| `Shader::defaultShader()` | This shader will always outlive the OpenGL context. |

## CMake Options

| Option              | Description                   | Default |
| ------------------- | ----------------------------- | ------- |
| GLPP_BUILD_DOCS     | Builds the glpp documentation | Off[^1] |
| GLPP_BUILD_EXAMPLES | Builds the glpp examples      | Off[^1] |
| GLPP_BUILD_TESTS    | Builds the glpp tests         | Off[^1] |

[^1] When includes in another project. If this is the master project default
will be On.

## CMake Submodule

```sh
git submodule add https://github.com/automas-dev/glpp.git
git submodule update --init --recursive
```

```cmake
add_subdirectory(glpp EXCLUDE_FROM_ALL)
```

## CMake Fetch Content
### Installing

```sh
cd build
make install
```

```cmake
include(FetchContent)
FetchContent_Declare(
    glpp
    GIT_REPOSITORY https://github.com/automas-dev/glpp.git
    GIT_TAG        0.8.1
)
FetchContent_MakeAvailable(glpp)
```

## CMake External Project

```cmake
include(ExternalProject)
ExternalProject_Add(
    glpp
    GIT_REPOSITORY  https://github.com/automas-dev/glpp.git
    GIT_TAG         0.8.1
    UPDATE_COMMAND  ""
    INSTALL_COMMAND ""
)
```

## License

This project uses the [MIT](LICENSE) License.
