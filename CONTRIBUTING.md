# Contributing to mmCEsim

## Project Structure

The project is configured with CMake, and the role of each folder is listed below.
- `include`: C++ Header Files (`.h`)
  - `export`: Header Files Concerning ALG Export
  - `mmcesim`: Files Needed by mmCEsim Binary
    - `copy`: Contents Directly Copied to Form Generated C++/Python/Matlab Code
    - `tex`: $\rm\LaTeX$ Files for Report Generation
- `src`: C++ Source Files (`.cpp`)
  - `export`: Class Implementations Corresponding to Those in `include/export`
- `ext`: 3rd Party Resources
  - `astyle`: Format Generated C++ Code
  - `fmt`: C++ String Format (Like That in C++20)
  - `yaml-cpp`: Parse YAML Configuration (`.sim`)
- `test`: Test Files Including `CTest` Configuration
- `tools`: Accompanying Tools (`maintain` and `compose`)
- `doc`: Documentation (*Currently Empty*)
- `bin`: Generated Binary Executable
- `misc`: Miscellaneous Files

> **Note** This project is still under active development and the structure is subject to potential changes.

## Code Style
The C++ code style is based on LLVM,
with additional configuration listed in [`.clang-format`](.clang-format).
Please run [^1]
```
.misc/format
```
to format the code before committing.

[^1]: Appropriate permission is needed.
On a Unix (Linux and macOS) system, you may use `chmod +x ./misc/format`.

## Other Ways You can Contribute to mmCEsim
### Open Issues
You may freely [open an issue](https://github.com/mmcesim/mmcesim/issues) for bug reports.

### Join Discussions
You can also [join discussions](https://github.com/mmcesim/mmcesim/discussions)
if you have questions about the use of mmCEsim
or about the task-oriented mmWave channel estimation techniques.

### Cite Our Works
Please cite our work if you use mmCEsim in your research and/or software.
Citations are useful for the continued development and maintenance of this project.[^2]

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
[^2]: In `bibtex`, the `note` field may not be necessary.
It should only be used when the `doi` field does not show (for example IEEEtran style).
