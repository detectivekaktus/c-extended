#!/usr/bin/env bash
CC=gcc
CFLAGS="-Wall -Wextra -Werror -std=c99 -pedantic -ggdb3"

SOURCES=("gear.c")
OBJECTS=("gear.o")

BUILD_DIR=bin
TARGET=libgear

TESTS=("arrays" "io" "string" "map")

build() {
  mkdir -p $BUILD_DIR
  if [ "$1" = "fpic" ]; then
    for ((i = 0; i < "${#SOURCES[@]}"; i++)); do
      echo "Compiling fPIC "${SOURCES[i]}"..."
      $CC $CFLAGS -fPIC -c "${SOURCES[i]}" -o $BUILD_DIR/"${OBJECTS[i]}"
      if [ $? -ne 0 ]; then
        exit 1
      fi
    done
  elif [ -z "$1" ]; then
    for ((i = 0; i < "${#SOURCES[@]}"; i++)); do
      echo "Compiling "${SOURCES[i]}"..."
      $CC $CFLAGS -c "${SOURCES[i]}" -o $BUILD_DIR/"${OBJECTS[i]}"
      if [ $? -ne 0 ]; then
        exit 1
      fi
    done
  else
    echo "Unknown arg for build $1"
    exit 1
  fi 
}

static() {
  build
  echo "Creating archive from object files..."
  ar rcs $TARGET.a $BUILD_DIR/$OBJECTS
}

dynamic() {
  build fpic
  echo "Creating shared library from object files..."
  $CC $CFLAGS -shared -o $TARGET.so $BUILD_DIR/$OBJECTS
}

test() {
  build
  mkdir -p $BUILD_DIR/test
  echo "Compiling tests..."
  for ((i = 0; i < "${#TESTS[@]}"; i++)); do
    $CC $CFLAGS -c test/"${TESTS[i]}".c -o $BUILD_DIR/test/"${TESTS[i]}".o
    if [ $? -ne 0 ]; then
      exit 1
    fi
    $CC $CFLAGS -o $BUILD_DIR/test/"${TESTS[i]}" $BUILD_DIR/$OBJECTS $BUILD_DIR/test/"${TESTS[i]}".o
    if [ $? -ne 0 ]; then
      exit 1
    fi
  done

  echo "Running tests..."
  for ((i = 0; i < "${#TESTS[@]}"; i++)); do
    $BUILD_DIR/test/"${TESTS[i]}"
    if [ $? -ne 0 ]; then
      exit 1
    fi
  done
}

clean() {
  rm -r $BUILD_DIR
  rm -f $TARGET.a
  rm -f $TARGET.so
}

case $1 in
  static)
    static
    ;;
  dynamic)
    dynamic
    ;;
  test)
    test 
    ;;
  clean)
    clean 
    ;;
  *)
    if [ -z "$1" ]; then
      static 
    else
      echo "Unknown command ${1}"
    fi
    ;;
esac
