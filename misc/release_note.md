## New Features
- ALG library enhancement ([#49](https://github.com/mmcesim/mmcesim/issues/49), [PR #51](https://github.com/mmcesim/mmcesim/pull/51)):
  - Add OMPL[^1] algorithm to the library.
  - ALG dependency check.
  - Enhanced log and TeX report ([PR #55](https://github.com/mmcesim/mmcesim/pull/55)).
- Update Doxygen website configurations (including more graphs).
- Automatic version bump script ([#70](https://github.com/mmcesim/mmcesim/issues/70), [PR #75](https://github.com/mmcesim/mmcesim/pull/75)).

## Bug Fixes
- Rename configuration file to `mmcesim.cfg` ([#50](https://github.com/mmcesim/mmcesim/issues/50), [PR #53](https://github.com/mmcesim/mmcesim/pull/53)).
- Change Unix file location of `mmcesim.log` and `mmcesim.cfg` ([#71](https://github.com/mmcesim/mmcesim/issues/71), [PR #74](https://github.com/mmcesim/mmcesim/pull/74))
- Fix C++ (Armadillo) runtime error when simulating with `-O2` or `-O3` on macOS by patching `arma::randperm` ([#52](https://github.com/mmcesim/mmcesim/issues/52), [PR #54](https://github.com/mmcesim/mmcesim/pull/54)).
- Fix warnings due to Boost Library's use of deprecated functions ([#66](https://github.com/mmcesim/mmcesim/issues/66)).

## News
- The research paper entitled ‘Beam Pattern and Reflection Pattern Design for Channel Estimation in RIS-assisted mmWave MIMO Systems’ [^2] is online as an early access paper of *IEEE Transactions on Vehicular Technology* (Sep. 8, 2023).
- The research paper entitled ‘Beam Pattern and Reflection Pattern Design for Channel Estimation in RIS-assisted mmWave MIMO Systems’ [^2] has been accepted by *IEEE Transactions on Vehicular Technology* (May 21, 2023).
- The research paper entitled ‘OMPL-SBL Algorithm for Intelligent Reflecting Surface-Aided mmWave Channel Estimation’ [^1] is online as an early access paper of *IEEE Transactions on Vehicular Technology* (Jun. 27, 2023): https://ieeexplore.ieee.org/document/10164645 ([#69](https://github.com/mmcesim/mmcesim/discussions/69))

> [!NOTE]\
> x86 and x64 binaries are built automatically by GitHub Actions; The macOS arm binary is built manually on macOS with M1 chip.

Contributed by @Teddy-van-Jerry and @mmcesim-bot, for version 0.2.2 release ([#73](https://github.com/mmcesim/mmcesim/issues/73)).

[^1]: W. Zhao, Y. You, L. Zhang, X. You, and C. Zhang, “OMPL-SBL algorithm for intelligent reflecting surface-aided mmWave channel estimation”, *IEEE Trans. Veh. Technol.*, 2023, to be published. DOI: [10.1109/TVT.2023.3287400](https://doi.org/10.1109/TVT.2023.3287400).
[^2]: Y. You, W. Zhao, L. Zhang, X. You, and C. Zhang, “Beam pattern and reflection pattern design for channel estimation in RIS-assisted mmWave MIMO systems”, *IEEE Trans. Veh. Technol.*, 2023, to be published.
