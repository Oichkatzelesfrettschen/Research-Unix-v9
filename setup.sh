#!/bin/bash
# Setup script for building Research Unix V9 on modern systems
# Installs required compilers and tooling for code navigation and analysis.

set -e

# Update package lists
sudo apt-get update

# Install clang and other build tools
sudo apt-get install -y \
  clang-14 \
  make \
  cscope \
  cloc \
  npm \
  python3-pip

# Configure clang as the default C/C++ compiler via update-alternatives
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-14 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-14 100

# Install Python utilities
sudo pip install --break-system-packages lizard tree_sitter

# Install tree-sitter CLI via npm
sudo npm install -g tree-sitter-cli

# Optional: verify installations
clang --version
cscope --version
cloc --version
lizard --version
tree-sitter --version
