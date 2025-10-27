
CC=/usr/bin/gcc
CXX=/usr/bin/g++
CONFIG=Debug
TARGET=all

all: config build

config:
	cmake --no-warn-unused-cli \
		-DCMAKE_BUILD_TYPE=${CONFIG} \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
		-DCMAKE_C_COMPILER=${CC} \
		-DCMAKE_CXX_COMPILER=${CXX} \
		-S${PWD} \
		-B${PWD}/build \
		-G Ninja

build:
	cmake --build ${PWD}/build --config ${CONFIG} --target ${TARGET}

install:
	cmake --install ${PWD}/build --config ${CONFIG}

lint:
	@find src include examples tests -name '*.c' -or -name '*.h' -or -name '*.cpp' -or -name '*.hpp' | xargs clang-format --dry-run --Werror --sort-includes

format:
	@find src include examples tests -name '*.c' -or -name '*.h' -or -name '*.cpp' -or -name '*.hpp' | xargs clang-format -i --Werror --sort-includes

.PHONY: config build install lint format
