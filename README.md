# mmCEsim [![Build mmCEsim CLI](https://github.com/mmcesim/mmcesim/actions/workflows/build.yml/badge.svg)](https://github.com/mmcesim/mmcesim/actions/workflows/build.yml)
![Code Size](https://img.shields.io/github/languages/code-size/mmcesim/mmcesim)
![Total Lines](https://img.shields.io/tokei/lines/github/mmcesim/mmcesim)
[![License](https://img.shields.io/github/license/mmcesim/mmcesim)](LICENSE)
![Release](https://img.shields.io/github/v/release/mmcesim/mmcesim?include_prereleases)
![Download](https://img.shields.io/github/downloads/mmcesim/mmcesim/total)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.6912824.svg)](https://doi.org/10.5281/zenodo.6912824)
<!-- ![Twitter](https://img.shields.io/twitter/follow/mmcesim?style=social) -->

CLI & GUI Application for Millimeter Wave Channel Estimation Simulation

> **Note** This application is currently under active development.

## Citation
Find mmCEsim useful to your research?
You may consider citing it[^1]:

> [1] W. Zhao, “mmCEsim: Tasked-oriented millimeter wave channel estimation,” *Zenodo*, Jul. 2022, doi: [10.5281/zenodo.6912824](https://doi.org/10.5281/zenodo.6912824).
```bib
@article{zhao2022mmcesim,
  title   = {{mmCEsim}: Tasked-Oriented Millimeter Wave Channel Estimation Simulation},
  author  = {Zhao, Wuqiong},
  journal = {Zenodo},
  year    = {2022},
  month   = jul,
  doi     = {10.5281/zenodo.6912824},
  note    = {doi: \href{https://doi.org/10.5281/zenodo.6912824}{10.5281/zenodo.6912824}}
}
```
[^1]: In `bibtex`, the `note` field may not be necessary. It should only be used when the `doi` field does not show (for example IEEEtran style).

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
  config                 configure mmCEsim options
  (Leave empty)          generic use

Allowed options:

Generic options:
  -v [ --version ]       print version string
  -h [ --help ]          produce help message
  --gui                  open the GUI app

Configuration:
  -o [ --output ] arg    output file name
  -s [ --style ] arg     style options (C++ only, with astyle)
  -l [ --lang ] arg      export language or simulation backend
  --value arg            value for configuration option
  -f [ --force ]         force writing mode
  --no-error-compile     Do not raise error if simulation compiling fails
```

## Contributing
You are welcome to contribute to mmCEsim.
More information is available in [CONTRIBUTING](CONTRIBUTING.md).

## Build
| TinyTeX-mmCEsim | MinGW-mmCEsim |
| :-: | :-: |
| Portable LaTeX Engine for Report Export | Portable C++ Compiler Backend on Windows |
| [mmcesim/tinytex-mmcesim](https://github.com/mmcesim/tinytex-mmcesim) | [mmcesim/mingw-mmcesim](https://github.com/mmcesim/mingw-mmcesim) |
| [![Build TinyTeX-mmCEsim](https://github.com/mmcesim/tinytex-mmcesim/actions/workflows/build.yml/badge.svg)](https://github.com/mmcesim/tinytex-mmcesim/actions/workflows/build.yml) | [![Build MinGW-mmCEsim](https://github.com/mmcesim/mingw-mmcesim/actions/workflows/build.yml/badge.svg)](https://github.com/mmcesim/mingw-mmcesim/actions/workflows/build.yml) |
