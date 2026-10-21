# Bremsstrahlung

**Bremsstrahlung photon yield from thick targets**, electrons of 1-5 MeV on
thick Aluminum and Lead targets, across several EM physics lists
(emstandard_opt0/opt4, Goudsmit-Saunderson, Livermore, Penelope).
History: S. Guatelli.

---

## 🔬 What this test does

A mono-energetic e- beam (selected by `EnergyID`, 1-5 MeV) impinges on a
thick target (Aluminum or Lead) and the bremsstrahlung photon spectrum
produced is scored, for each of the five supported EM physics
configurations.

## 📚 Reference

- Source: `geant-validation-tests/src/Bremsstrahlung`.
- History file: tagged `Bremsstrahlung-V10-07-*` (S. Guatelli, Nov. 2021),
  migrated to `G4AnalysisManager` and `G4RunManagerFactory`/Geant4 V11.

## Import notes

- `main.cc` is `bremsstrahlung.cc` (note: on case-insensitive filesystems
  `Bremsstrahlung.cc` in the original repository is a symlink to
  `bremsstrahlung.cc` and the two collide; the real content was retrieved
  via `git show` and renamed to `main.cc`). `include/`, `src/` copied as-is.
- `macro/` contains all 52 concrete `<Material>_<PhysicsList>_EnergyID_<N>_MeV.mac`
  combinations found directly in the source tree (Aluminum/Lead ×
  emstandard_opt0/opt4, GoudsmitSaunderson, Livermore, Penelope × 1-5 MeV).
- `validation/parser.py` was **rewritten** for the `metadata()`/`parse()`
  module API that `ci-workflows/validation/export.py` actually calls (the
  first version, copied unmodified from
  `geant-config-generator/tests/geant4/Bremsstrahlung/parser.py`, used the
  old `gts.BaseParser` class API and would have failed CI the same way
  `fanoCavity` initially did). See `validation/README.md` for details. The
  vendored `gts` package is no longer needed and was removed.
  Note: the README originally shipped alongside the C++ source in
  `geant-validation-tests/src/Bremsstrahlung/README` is actually a
  mismatched copy of the `ElectronScattering` README (titled
  "electronScattering") — not reproduced here; this file instead summarizes
  the test from `params.conf`, `parser.py` and `History`.
