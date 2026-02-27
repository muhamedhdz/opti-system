import os
import re
import subprocess
import statistics
from pathlib import Path

import matplotlib.pyplot as plt

C_EXEC = "TP1.exe" if os.name == "nt" else "./TP1"
INST_DIR = Path("instances")

# a
B_VALUES_A = list(range(100, 1001, 100))
N_FIXED_A = 100

# b
N_VALUES_B = list(range(10, 101, 10))
B_FIXED_B = 500

RE_GREEDY = re.compile(r"EXEC_TIME_GREEDY:\s*([0-9\.eE\+\-]+)")
RE_LP = re.compile(r"EXEC_TIME_LP:\s*([0-9\.eE\+\-]+)")


def run_one(instance_file: Path):
    """Exécute TP1 une fois et renvoie (lp_time, greedy_time) ou (None,None)."""
    result = subprocess.run([C_EXEC, "-F", str(instance_file)],
                            capture_output=True, text=True)
    if result.returncode != 0:
        return None, None

    out = result.stdout or ""
    mg = RE_GREEDY.search(out)
    ml = RE_LP.search(out)
    if not mg or not ml:
        return None, None

    return float(ml.group(1)), float(mg.group(1))


def collect_times(instance_file: Path, iterations: int = 25):
    lp_times, g_times = [], []
    for _ in range(iterations):
        lp, g = run_one(instance_file)
        if lp is None or g is None:
            continue
        lp_times.append(lp)
        g_times.append(g)
    return lp_times, g_times


def median_or_fail(times, label: str):
    if not times:
        raise RuntimeError(f"Aucune mesure valide pour {label}")
    return statistics.median(times)


def main():
    if not Path(C_EXEC).exists():
        print(f"Erreur : '{C_EXEC}' introuvable. Compile TP1 d'abord.")
        return
    if not INST_DIR.exists():
        print("Erreur : dossier 'instances/' introuvable. Lance generate_instances.py.")
        return

    # a
    print("Benchmark (a): n=100 fixe, b variable")
    t_lp_a, t_g_a = [], []
    for b in B_VALUES_A:
        inst = INST_DIR / f"inst_a_n{N_FIXED_A}_b{b}.csv"
        lp_times, g_times = collect_times(inst, iterations=25)
        t_lp_a.append(median_or_fail(lp_times, f"{inst} (LP)"))
        t_g_a.append(median_or_fail(g_times, f"{inst} (Greedy)"))

    # b
    print("Benchmark (b): b=500 fixe, n variable")
    t_lp_b, t_g_b = [], []
    for n in N_VALUES_B:
        inst = INST_DIR / f"inst_b_n{n}_b{B_FIXED_B}.csv"
        lp_times, g_times = collect_times(inst, iterations=25)
        t_lp_b.append(median_or_fail(lp_times, f"{inst} (LP)"))
        t_g_b.append(median_or_fail(g_times, f"{inst} (Greedy)"))

    # Courbes
    plt.figure()
    plt.plot(B_VALUES_A, t_lp_a, marker="o", label="Relaxation Linéaire (médiane)")
    plt.plot(B_VALUES_A, t_g_a, marker="x", label="Glouton (médiane)")
    plt.title("TP1 - Complexité : n=100 fixe, b variable")
    plt.xlabel("Capacité (b)")
    plt.ylabel("Temps (secondes)")
    plt.ylim(bottom=0)
    plt.grid(True)
    plt.legend()

    plt.figure()
    plt.plot(N_VALUES_B, t_lp_b, marker="o", label="Relaxation Linéaire (médiane)")
    plt.plot(N_VALUES_B, t_g_b, marker="x", label="Glouton (médiane)")
    plt.title("TP1 - Complexité : b=500 fixe, n variable")
    plt.xlabel("Nombre d'objets (n)")
    plt.ylabel("Temps (secondes)")
    plt.ylim(bottom=0)
    plt.grid(True)
    plt.legend()

    plt.show()


if __name__ == "__main__":
    main()