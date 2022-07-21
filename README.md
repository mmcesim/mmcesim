# mmCEsim [![Build mmCEsim CLI](https://github.com/mmcesim/mmcesim/actions/workflows/build.yml/badge.svg)](https://github.com/mmcesim/mmcesim/actions/workflows/build.yml)
![Code Size](https://img.shields.io/github/languages/code-size/mmcesim/mmcesim)
![Total Lines](https://img.shields.io/tokei/lines/github/mmcesim/mmcesim)
[![License](https://img.shields.io/github/license/mmcesim/mmcesim)](LICENSE)
![Release](https://img.shields.io/github/v/release/mmcesim/mmcesim?include_prereleases)
![Download](https://img.shields.io/github/downloads/mmcesim/mmcesim/total)
<!-- ![Twitter](https://img.shields.io/twitter/follow/mmcesim?style=social) -->

CLI Application for Millimeter Wave Channel Estimation Simulation

> This application is currently under active development.

## Usage
```
$ ./mmcesim -h
mmCEsim 0.1.0    (C) Wuqiong Zhao
Millimeter Wave Channel Estimation Simulation
=============================================

Usage: ./mmcesim <command> <input> [options]

Commands:
  sim [simulate]         run simulation
  dbg [debug]            debug simulation settings
  exp [export]           export code
  fmt [format]           format .sim configuration file
  (Leave empty)          generic use

Allowed options:

Generic options:
  -v [ --version ]       print version string
  -h [ --help ]          produce help message
  --gui                  open the GUI app

Configuration:
  -o [ --output ] arg    output file name
```

## Build
| TinyTeX-mmCEsim | MinGW-mmCEsim |
| :-: | :-: |
| Portable LaTeX Engine for Report Export | Portable C++ Compiler Backend on Windows |
| [mmcesim/tinytex-mmcesim](https://github.com/mmcesim/tinytex-mmcesim) | [mmcesim/mingw-mmcesim](https://github.com/mmcesim/mingw-mmcesim) |
| [![Build TinyTeX-mmCEsim](https://github.com/mmcesim/tinytex-mmcesim/actions/workflows/build.yml/badge.svg)](https://github.com/mmcesim/tinytex-mmcesim/actions/workflows/build.yml) | [![Build MinGW-mmCEsim](https://github.com/mmcesim/mingw-mmcesim/actions/workflows/build.yml/badge.svg)](https://github.com/mmcesim/mingw-mmcesim/actions/workflows/build.yml) |
