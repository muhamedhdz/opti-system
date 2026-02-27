import random
from pathlib import Path
from typing import List, Tuple

Item = Tuple[int, int]  # (c, a)

OUTPUT_DIR = Path("instances")
SEED = 42

# Expérience (a)
N_A = 100
B_VALUES_A = list(range(100, 1001, 100))  # 100..1000

# Expérience (b)
B_FIXED_B = 500
N_VALUES_B = list(range(10, 101, 10))     # 10..100


def write_instance_csv(filepath: Path, n: int, b: int, items: List[Item]) -> None:
    filepath.parent.mkdir(parents=True, exist_ok=True)
    with filepath.open("w", encoding="utf-8") as f:
        f.write(f"{n},{b}\n")
        for (c, a) in items[:n]:
            f.write(f"{c},{a}\n")


def gen_items(n: int, rng: random.Random) -> List[Item]:
    return [(rng.randint(1, 100), rng.randint(1, 100)) for _ in range(n)]  # (c,a)


def sum_weights(items: List[Item]) -> int:
    return sum(a for _, a in items)


def main() -> None:
    OUTPUT_DIR.mkdir(exist_ok=True)
    print(f"Dossier '{OUTPUT_DIR}/' prêt.")

    # (a) n=100 fixe, b variable (mêmes objets)
    print("Génération (a) : n=100 fixe, b variable...")
    rng_a = random.Random(SEED)
    max_b = max(B_VALUES_A)

    while True:
        items_a = gen_items(N_A, rng_a)
        if sum_weights(items_a) >= max_b:
            break

    for b in B_VALUES_A:
        filename = OUTPUT_DIR / f"inst_a_n{N_A}_b{b}.csv"
        write_instance_csv(filename, N_A, b, items_a)
        print(f" -> {filename}")

    # (b) b=500 fixe, n variable (préfixes)
    print("\nGénération (b) : b=500 fixe, n variable...")
    rng_b = random.Random(SEED + 1)

    while True:
        first_10 = gen_items(10, rng_b)
        if sum_weights(first_10) >= B_FIXED_B:
            break

    items_b = first_10 + gen_items(90, rng_b)

    for n in N_VALUES_B:
        filename = OUTPUT_DIR / f"inst_b_n{n}_b{B_FIXED_B}.csv"
        write_instance_csv(filename, n, B_FIXED_B, items_b)
        print(f" -> {filename}")

    print("\nOK: 20 instances générées.")


if __name__ == "__main__":
    main()