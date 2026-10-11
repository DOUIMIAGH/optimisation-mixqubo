#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/cpp/build}"
INST_DIR="$ROOT_DIR/instances"
OUT="${OUT:-$ROOT_DIR/results/latest.csv}"
RUNS="${RUNS:-30}"
TIME_BUDGET_MS="${TIME_BUDGET_MS:-31563}"
INSTANCE_IDS="${INSTANCE_IDS:-0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15}"

if [[ ! "$RUNS" =~ ^[1-9][0-9]*$ || ! "$TIME_BUDGET_MS" =~ ^[1-9][0-9]*$ ]]; then
  printf 'RUNS et TIME_BUDGET_MS doivent être des entiers strictement positifs.\n' >&2
  exit 1
fi

ALGOS=(
  main_student hill_climbing simulated_annealing
  continuous_random continuous_local_search continuous_simulated_annealing
  mixed_random mixed_alternating mixed_simulated_annealing
)

read -r -a IDS <<< "$INSTANCE_IDS"
for id in "${IDS[@]}"; do
  if [[ ! "$id" =~ ^([0-9]|1[0-5])$ || ! -f "$INST_DIR/mxqubo_v0_${id}.json" ]]; then
    printf 'Instance invalide : %s (valeurs attendues : 0 à 15).\n' "$id" >&2
    exit 1
  fi
done
if [[ ${#IDS[@]} -eq 0 ]]; then
  printf 'INSTANCE_IDS doit contenir au moins une instance.\n' >&2
  exit 1
fi
for algo in "${ALGOS[@]}"; do
  if [[ ! -x "$BUILD_DIR/src/$algo" ]]; then
    printf 'Exécutable absent : %s. Compiler le projet avant de lancer les expériences.\n' "$algo" >&2
    exit 1
  fi
done

mkdir -p -- "$(dirname -- "$OUT")"
# Protéger le CSV fourni, y compris lorsqu’il est désigné par un chemin alternatif.
if [[ "$OUT" -ef "$ROOT_DIR/results.csv" ]]; then
  printf 'Le fichier results.csv contient la campagne initiale. Choisir un autre chemin OUT.\n' >&2
  exit 1
fi
printf 'algo,instance,run,best_f,nb_eval,time_ms\n' > "$OUT"

for algo in "${ALGOS[@]}"; do
  for id in "${IDS[@]}"; do
    inst="mxqubo_v0_${id}.json"
    for ((run=1; run<=RUNS; run++)); do
      printf '%s — %s — essai %s/%s\n' "$algo" "$inst" "$run" "$RUNS"
      output="$("$BUILD_DIR/src/$algo" "$INST_DIR/$inst" "$TIME_BUDGET_MS")"
      best_f="$(awk -F= '$1 == "best_f" {print $2}' <<< "$output")"
      nb_eval="$(awk -F= '$1 == "nb_eval" {print $2}' <<< "$output")"
      time_ms="$(awk -F= '$1 == "time_ms" {print $2}' <<< "$output")"
      if [[ ! "$best_f" =~ ^[-+]?[0-9]+([.][0-9]+)?([eE][-+]?[0-9]+)?$ ||
            ! "$nb_eval" =~ ^[1-9][0-9]*$ ||
            ! "$time_ms" =~ ^[0-9]+([.][0-9]+)?([eE][-+]?[0-9]+)?$ ]]; then
        printf 'Résultat invalide pour %s sur %s ; arrêt de la campagne.\n' "$algo" "$inst" >&2
        exit 1
      fi
      printf '%s,%s,%s,%s,%s,%s\n' "$algo" "$inst" "$run" "$best_f" "$nb_eval" "$time_ms" >> "$OUT"
    done
  done
done
printf 'Résultats enregistrés dans %s\n' "$OUT"
