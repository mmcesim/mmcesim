## New Features
- Single RIS channel estimation implementation ([#89](https://github.com/mmcesim/mmcesim/issues/89), [PR #96](https://github.com/mmcesim/mmcesim/pull/96))
  - Implement functions and macros concerning channel graph ([#63](https://github.com/mmcesim/mmcesim/issues/63), [PR #88](https://github.com/mmcesim/mmcesim/pull/88))
    - Multiple `RECOVER` for one cascaded channel ([#67](https://github.com/mmcesim/mmcesim/issues/67), [PR #87](https://github.com/mmcesim/mmcesim/pull/87))
    - Paths sorting according to number of jumps in channel graph ([#68](https://github.com/mmcesim/mmcesim/issues/68), [PR #86](https://github.com/mmcesim/mmcesim/pull/86))
    - Implement macros related to node size, beam, grid ([#90](https://github.com/mmcesim/mmcesim/issues/90), [PR #91](https://github.com/mmcesim/mmcesim/pull/91))
- Disable color output in CTest ([#93](https://github.com/mmcesim/mmcesim/issues/93))

> [!CAUTION]
> **BREAKING CHANGE**:
> ALG function `ESTIMATE` now accept multiple parameters, not a single parameter anymore.
> Therefore, each parameter should be wrapped as `$ ... $` if there is space inside.

> [!NOTE]
> x86 and x64 binaries are built automatically by GitHub Actions; The macOS arm binary is built manually on macOS with M1 chip.

Contributed by @Teddy-van-Jerry and @mmcesim-bot, for version 0.3.0 release ([#95](https://github.com/mmcesim/mmcesim/issues/95)).
