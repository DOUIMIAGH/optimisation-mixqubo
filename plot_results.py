import argparse
from pathlib import Path

import pandas as pd
import matplotlib


def main():
    parser = argparse.ArgumentParser(description="Tracer la comparaison des méthodes mixQUBO.")
    parser.add_argument("csv", nargs="?", type=Path, default=Path(__file__).parent / "results.csv")
    parser.add_argument("--output", type=Path, default=Path(__file__).parent / "results/comparaison.png")
    parser.add_argument("--show", action="store_true", help="Afficher aussi le graphique dans une fenêtre.")
    args = parser.parse_args()

    if not args.show:
        matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    df = pd.read_csv(args.csv)
    grouped = df.groupby("algo")["best_f"].agg(["mean", "std"]).sort_values("mean")
    fig, ax = plt.subplots(figsize=(12, 6))
    ax.bar(grouped.index, grouped["mean"], yerr=grouped["std"], capsize=5)
    ax.set_ylabel("Moyenne de best_f (plus faible = meilleure solution)")
    ax.set_xlabel("Méthode")
    ax.set_title("Comparaison des méthodes sur les instances fournies")
    ax.tick_params(axis="x", labelrotation=35)
    for label in ax.get_xticklabels():
        label.set_horizontalalignment("right")
    ax.grid(axis="y", linestyle="--", alpha=0.7)
    fig.tight_layout()

    args.output.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(args.output, dpi=300)
    print(f"Graphique enregistré dans {args.output}")
    if args.show:
        plt.show()
    plt.close(fig)


if __name__ == "__main__":
    main()
