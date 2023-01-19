# mmCEsim [![Build mmCEsim CLI](https://github.com/mmcesim/mmcesim/actions/workflows/build.yml/badge.svg)](https://github.com/mmcesim/mmcesim/actions/workflows/build.yml)
![Code Size](https://img.shields.io/github/languages/code-size/mmcesim/mmcesim)
![Total Lines](https://img.shields.io/tokei/lines/github/mmcesim/mmcesim)
[![License](https://img.shields.io/github/license/mmcesim/mmcesim)](LICENSE)
[![Release](https://img.shields.io/github/v/release/mmcesim/mmcesim?include_prereleases)](https://github.com/mmcesim/mmcesim/releases/latest)
[![Download](https://img.shields.io/github/downloads/mmcesim/mmcesim/total)](https://github.com/mmcesim/mmcesim/releases/latest)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.6912824.svg)](https://doi.org/10.5281/zenodo.6912824)
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fmmcesim%2Fmmcesim.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Fmmcesim%2Fmmcesim?ref=badge_shield)
<!-- ![Twitter](https://img.shields.io/twitter/follow/mmcesim?style=social) -->

CLI & GUI Application for Millimeter Wave Channel Estimation Simulation

> **Note** This application is currently under active development.
> The task-oriented method will be published after the patent is accepted.

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
mmCEsim 0.2.0  (C) 2022-2023 Wuqiong Zhao
Millimeter Wave Channel Estimation Simulation
=============================================

Usage: ./mmcesim <command> <input> [options]

Commands:
  sim [ simulate ]       run simulation
  dbg [ debug ]          debug simulation settings
  exp [ export ]         export code
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
  -V [ --verbose ]       print additional information
  --no-error-compile     do not raise error if simulation compiling fails
  --no-term-color        disable colorful terminal contents
```

## Contributing
You are welcome to contribute to mmCEsim.
More information is available in [CONTRIBUTING](CONTRIBUTING.md).
Development documentation generated by Doxygen is available at https://dev.mmcesim.org.

## Build

### Release
You may download the latest built binary in [GitHub Releases](https://github.com/mmcesim/mmcesim/releases).
All these releases are **statically** linked, so no dependency library is required.

| System | Architecture | File Extension |
| :-: | :-: | :-: |
| Linux | [x86](https://github.com/mmcesim/mmcesim/releases/latest/download/mmCEsim-CLI-Linux-x86.tar.gz) | `tar.gz` |
| macOS | [x64](https://github.com/mmcesim/mmcesim/releases/latest/download/mmCEsim-CLI-macOS-x64.zip), [arm](https://github.com/mmcesim/mmcesim/releases/latest/download/mmCEsim-CLI-macOS-arm.zip) | `zip` |
| Windows | [x86](https://github.com/mmcesim/mmcesim/releases/latest/download/mmCEsim-CLI-Windows-x86.zip) | `zip` |

### Accompanying Build Support

| TinyTeX-mmCEsim | MinGW-mmCEsim |
| :-: | :-: |
| Portable LaTeX Engine for Report Export | Portable C++ Compiler Backend on Windows |
| [mmcesim/tinytex-mmcesim](https://github.com/mmcesim/tinytex-mmcesim) | [mmcesim/mingw-mmcesim](https://github.com/mmcesim/mingw-mmcesim) |
| [![Build TinyTeX-mmCEsim](https://github.com/mmcesim/tinytex-mmcesim/actions/workflows/build.yml/badge.svg)](https://github.com/mmcesim/tinytex-mmcesim/actions/workflows/build.yml) | [![Build MinGW-mmCEsim](https://github.com/mmcesim/mingw-mmcesim/actions/workflows/build.yml/badge.svg)](https://github.com/mmcesim/mingw-mmcesim/actions/workflows/build.yml) |

## License
mmCEsim is distributed by an [MIT License](LICENSE).


[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fmmcesim%2Fmmcesim.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Fmmcesim%2Fmmcesim?ref=badge_large)