# QPR2 shader state identifiers

These are the public metadata for the three v126 literal material classes.
Each state hash is SHA-256 over the exact technical state key used by
`tools/extract_qpr2_literal_programs.py`: control and stage selector,
final selector, then the active color and alpha input/output words, encoded
as little-endian 32-bit integers. This document publishes only fingerprints,
numeric class IDs, counts, and generalized descriptors.

| State hash (SHA-256) | Class ID | Active stages | Observed final variants |
| --- | ---: | ---: | ---: |
| `7d1599e1dc0ba85f48471d0661729bb3cd4df012b93b068ec55e148d507e1d7a` | 1 | 8 | 1 |
| `11458e542c8664c1fea8d61df35c63922071e0093b99f61671198ad71ea39bc0` | 2 | 6 | 3 |
| `8df78d1449c6ed59b4508c7efdfb3486f25d70d1d7f685762200aa52cba80db2` | 3 | 4 | 1 |

These fingerprints identify the reviewed v126 shapes. They cannot
reconstruct the game’s shader instructions. To build the local
specializations, capture states from your own game and follow
[PUBLISHING.md](../PUBLISHING.md).
