# Portal JSON parser

Rewritten from `geant-config-generator/tests/geant4/Bremsstrahlung/parser.py`
(old `gts.BaseParser` + PyROOT, which merged repeated-seed job groups) for
the module-level `metadata()`/`parse()` API that
`ci-workflows/validation/export.py` actually calls. ROOT is read with
`uproot` (no PyROOT on the CI runner, matching `LowEFrag`/`TestEm12-DPK`);
there is no job-group merge any more — one macro means one run, so the
`1/len(jobs)` histogram rescaling in the original `read_root` is dropped
(equivalent to `len(jobs) == 1`).

`metadata()` reads the ROOT output path straight from the macro's own
`/analysis/setFileName` command instead of reconstructing it from
`MATERIAL`/`PHYSLIST`/`ENERGY` fields the way the original parser did (those
fields came from `config.json`'s own templating variables in the old system,
which don't exist here) — simpler and can't drift out of sync with what the
macro actually writes.

All 52 macros already present in `geant-validation-tests/src/Bremsstrahlung`
are self-contained and are used as-is (`<Material>_<PhysicsList>_EnergyID_<N>_MeV.mac`,
Aluminum/Lead × emstandard_opt0/opt4/GoudsmitSaunderson/Livermore/Penelope ×
1-5 MeV); no macros needed to be generated for this test.

For each of the 8 `Scorer_<angle>_deg` histograms, bin content/errors are
normalized by bin width and solid angle exactly as in the original
`read_root`. No asymmetric error option is set on these histograms in
`HistoManager.cc`, so (as with the original's `GetBinErrorLow`/`Up` on the
same histogram) the statistical error is symmetric — read once via
`uproot`'s `errors()`. The trimming to the filled bin range reproduces the
original's `ROOT::FindFirstBinAbove(0)`/`FindLastBinAbove(0)` plus its
`range(first, last)` loop, which — in the original too — **excludes the
last filled bin** (an inherited quirk, not something introduced by this
port).
