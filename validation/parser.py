#!/usr/bin/env python

# Bremsstrahlung portal parser: metadata() reads the macro, parse() the 8
# "Scorer_<angle>_deg" angular photon-energy spectra written to the ROOT
# file named by the macro's own /analysis/setFileName.
#
# Ported from geant-config-generator/tests/geant4/Bremsstrahlung/parser.py
# (gts.BaseParser + PyROOT, merging repeated-seed jobs) to the module-level
# metadata()/parse() API expected by ci-workflows/validation/export.py,
# reading ROOT with uproot (no PyROOT on the runner, no job-group merge:
# one macro now means one run). The physics-list naming and theta-binning
# logic are otherwise unchanged from the original.
import math
import os

from geantval import getJSON, one_command, single_run

NAMES = {"Titanium": "Ti", "Gold": "Au", "Lead": "Pb", "Graphite": "C",
         "Tantalum": "Ta", "Beryllium": "Be", "Aluminum": "Al", "Copper": "Cu"}

PLISTS = {"Penelope": "empenelope", "Livermore": "emlivermore",
          "GoudsmitSaunderson": "emstandardGS",
          "emstandard_opt0": "emstandard_opt0", "emstandard_opt4": "emstandard_opt4"}

HISTOGRAMS = ["Scorer_0_deg", "Scorer_1_deg", "Scorer_2_deg", "Scorer_4_deg",
              "Scorer_10_deg", "Scorer_30_deg", "Scorer_60_deg", "Scorer_90_deg"]


def metadata(commands):
    single_run(commands)
    filename = one_command(commands, "/analysis/setFileName")
    energy, unit = one_command(commands, "/gun/energy").split()
    if unit != "MeV":
        raise ValueError("Expected a MeV beam energy, got: " + unit)
    return {"TEST": "Bremsstrahlung",
            "MATERIAL": one_command(commands, "/testem/det/setMaterial"),
            "PHYSLIST": one_command(commands, "/testem/phys/addPhysics"),
            "ENERGY": float(energy),
            "OUTPUT": filename if filename.endswith(".root") else filename + ".root"}


def theta_window(theta):
    if theta == 0.:
        return 0., 0.5
    if theta == 1.:
        return 0.75, 1.25
    if theta == 2.:
        return 1.75, 2.25
    return theta - 0.5, theta + 0.5


def read_spectrum(hist, theta):
    theta_min, theta_max = theta_window(theta)
    denom = 2. * math.pi * abs(math.cos(math.radians(theta_max)) - math.cos(math.radians(theta_min)))

    edges = hist.axis().edges(flow=False).tolist()
    values = hist.values(flow=False).tolist()
    # No asymmetric error option is set on these histograms, so (as with the
    # original's GetBinErrorLow/Up on the same histogram) up == low.
    errors = hist.errors(flow=False).tolist()

    # ROOT's FindFirstBinAbove(0)/FindLastBinAbove(0), 1-indexed, and the
    # original's `range(first, last)` (excluding the last filled bin, as the
    # original parser does) translate to this half-open slice of the
    # 0-indexed uproot arrays.
    first = next(i for i, v in enumerate(values) if v > 0)
    last = len(values) - 1 - next(i for i, v in enumerate(reversed(values)) if v > 0)

    binlow, binhigh, binContent, binErrors = [], [], [], []
    for i in range(first, last):
        width = edges[i + 1] - edges[i]
        binlow.append(edges[i])
        binhigh.append(edges[i + 1])
        binContent.append(values[i] / (width * denom))
        binErrors.append(errors[i] / (width * denom))
    return binlow, binhigh, binContent, binErrors


def parse(job):
    import uproot

    filename = os.path.join(job["path"], job["OUTPUT"])
    with uproot.open(filename) as root:
        for name in HISTOGRAMS:
            theta = float(name.split("_")[1])
            binlow, binhigh, binContent, binErrors = read_spectrum(root[name], theta)
            yield getJSON(job, "histogram",
                          mctool_name="GEANT4",
                          mctool_model=PLISTS[job["PHYSLIST"]],
                          observableName="fluence",
                          targetName=NAMES[job["MATERIAL"]],
                          beamParticle="e-",
                          beamEnergies=[job["ENERGY"]],
                          secondaryParticle="None",
                          title="fluence",
                          xAxisName="energy (MeV)",
                          yAxisName="dS/dE (sr^{-1})",
                          binContent=binContent,
                          binEdgeLow=binlow,
                          binEdgeHigh=binhigh,
                          yStatErrorsPlus=binErrors,
                          yStatErrorsMinus=binErrors,
                          parameters=[{"names": "theta", "values": str(theta)}])
