#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
exec docker run --rm --platform=linux/amd64 -v "$PWD":/src -w /src debian:bookworm-slim \
  bash -c "apt-get update -qq && apt-get install -y -qq g++ make libhidapi-dev libsystemd-dev libncurses-dev libncursesw5-dev >/dev/null && touch mk_deps_run.tmp mk_deps.tmp mk_chdeps.tmp mk_deps_mem.tmp && make SHELL=/bin/bash release NOPREPARE=1"
