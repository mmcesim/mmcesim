## New Features
- Single RIS channel estimation implementation ([#89](https://github.com/mmcesim/mmcesim/issues/89), [PR #96](https://github.com/mmcesim/mmcesim/pull/96))
  - Implement functions and macros concerning channel graph ([#63](https://github.com/mmcesim/mmcesim/issues/63), [PR #88](https://github.com/mmcesim/mmcesim/pull/88))
    - Multiple `RECOVER` for one cascaded channel ([#67](https://github.com/mmcesim/mmcesim/issues/67), [PR #87](https://github.com/mmcesim/mmcesim/pull/87))
    - Paths sorting according to number of jumps in channel graph ([#68](https://github.com/mmcesim/mmcesim/issues/68), [PR #86](https://github.com/mmcesim/mmcesim/pull/86))
    - Implement macros related to node size, beam, grid ([#90](https://github.com/mmcesim/mmcesim/issues/90), [PR #91](https://github.com/mmcesim/mmcesim/pull/91))
- Disable color output in CTest ([#93](https://github.com/mmcesim/mmcesim/issues/93))
- CI for macOS ARM ([#101](https://github.com/mmcesim/mmcesim/issues/101), [PR #102](https://github.com/mmcesim/mmcesim/pull/102))

## Bug Fixes
- Updated Beamforming Generation Out of Loop ([#98](https://github.com/mmcesim/mmcesim/issues/98), [PR #99](https://github.com/mmcesim/mmcesim/pull/99))
- Fully Show Simulation Subprocess Terminal Output ([#100](https://github.com/mmcesim/mmcesim/issues/100))
- Update Mac Trust App Shell Script ([#103](https://github.com/mmcesim/mmcesim/issues/103))

> [!CAUTION]
> **BREAKING CHANGE**:
> ALG function `ESTIMATE` now accept multiple parameters, not a single parameter anymore.
> Therefore, each parameter should be wrapped as `$ ... $` if there is space inside.

Contributed by @Teddy-van-Jerry and @mmcesim-bot, for version 0.3.0 release ([#95](https://github.com/mmcesim/mmcesim/issues/95)).
