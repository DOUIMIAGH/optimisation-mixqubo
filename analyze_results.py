import argparse
from pathlib import Path

import pandas as pd


def main():
    parser = argparse.ArgumentParser(description="Comparer les résultats des méthodes mixQUBO.")
    parser.add_argument("csv", nargs="?", type=Path, default=Path(__file__).parent / "results.csv")
    args = parser.parse_args()
    df = pd.read_csv(args.csv)

    grouped = df.groupby(["algo", "instance"])["best_f"].agg(["mean", "std", "min", "max"])
    print("\n=== Résultats par méthode et instance ===")
    print(grouped.to_string())

    global_stats = df.groupby("algo")["best_f"].mean().sort_values()
    print("\n=== Moyennes globales sur les instances fournies ===")
    print(global_stats.to_string())


if __name__ == "__main__":
    main()
