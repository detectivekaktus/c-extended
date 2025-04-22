#!/usr/bin/env bash
CC=gcc
CFLAGS=("-Wall -Wextra -Werror -std=c99 -pedantic -ggdb3")

SOURCES=("mygear.c")
OBJECTS=("mygear.o")

BUILD_DIR=bin/
TARGET=libmygear

build() {
  mkdir -p $BUILD_DIR
  if [ -z $1 ]; then
    for ((i = 0; i < "${#SOURCES[@]}"; i++)); do
      $CC $CFLAGS -c "${SOURCES[i]}" -o $BUILD_DIR/"${OBJECTS[i]}"
    done
  else
    for ((i = 0; i < "${#SOURCES[@]}"; i++)); do
      $CC $CFLAGS -fPIC -c "${SOURCES[i]}" -o $BUILD_DIR/"${OBJECTS[i]}"
    done
  fi 
}

static() {
  build
  ar rcs $TARGET.a $BUILD_DIR/$OBJECTS
}

dynamic() {
  build
  $CC $CFLAGS -shared -o $TARGET.so $BUILD_DIR/$OBJECTS
}

case $1 in
  static)
    static
    ;;
  dynamic)
    dynamic
    ;;
  *)
    if [ "$1" = "" ]; then
      static 
    else
      echo "Unknown command ${1}"
    fi
    ;;
esac
