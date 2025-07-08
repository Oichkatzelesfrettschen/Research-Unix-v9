# License Audit Summary

This document summarizes the tools installed and the basic findings of the license audit.

## Installed tools
- `licensecheck` via apt
- `reuse` via apt
- `tree` via apt
- `spdx-licenses` and related node SPDX helpers via apt
- `scancode-toolkit` via pip
- `license-checker` via npm

## Findings
- Only one file references Carnegie Mellon University: `v9/sys/h.old/ilreg.h` at line 6.
- `licensecheck` scanned the repository and stored results in `licensecheck.log`.
- `reuse lint` reported the project is not compliant with the REUSE specification; results in `reuse.log`.
- `scancode-toolkit` produced SPDX output for a sample file in `scancode_partial.spdx`.
- `license-checker` examined Node dependencies and generated `license_checker.json`.

