#!/usr/bin/env python3
"""Select Wokwi scenarios for CI based on event mode and changed files."""

from __future__ import annotations

import argparse
import json
import re
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


REPO_ROOT = Path(__file__).resolve().parent.parent
EXAMPLES_DIR = REPO_ROOT / "examples"
TEST_DIR = REPO_ROOT / "test"

DEFAULT_SMOKE_SCENARIOS = ("Basic", "SubMenu", "Widgets", "MenuTimeout")

WIDE_CHANGE_PATHS = {
    "platformio.ini",
    "diagram.json",
    "wokwi.toml",
    ".scripts/prepare_example.py",
    ".scripts/prepare_workflow.py",
    ".scripts/select_wokwi_matrix.py",
    ".github/workflows/wokwi_ci.yml",
}


@dataclass(frozen=True)
class Scenario:
    name: str
    example_path: str


def discover_scenarios() -> list[Scenario]:
    scenarios: list[Scenario] = []
    for ino_path in sorted(EXAMPLES_DIR.rglob("*.ino")):
        name = ino_path.stem
        test_path = TEST_DIR / f"{name}.test.yml"
        if test_path.exists():
            rel_path = ino_path.relative_to(REPO_ROOT).as_posix()
            scenarios.append(Scenario(name=name, example_path=rel_path))
    return scenarios


def parse_smoke_scenarios(raw: str) -> list[str]:
    return [item.strip() for item in raw.split(",") if item.strip()]


def normalize_path(raw_path: str) -> str:
    cleaned = raw_path.strip().replace("\\", "/")
    if cleaned.startswith("./"):
        cleaned = cleaned[2:]
    return cleaned


def load_changed_files(args: argparse.Namespace) -> list[str]:
    files: list[str] = []
    for item in args.changed_file or []:
        cleaned = normalize_path(item)
        if cleaned:
            files.append(cleaned)

    if args.changed_files_file:
        path = Path(args.changed_files_file)
        if not path.exists():
            raise SystemExit(f"Changed files manifest not found: {path}")
        for line in path.read_text().splitlines():
            cleaned = normalize_path(line)
            if cleaned:
                files.append(cleaned)

    deduped: list[str] = []
    seen: set[str] = set()
    for path in files:
        if path not in seen:
            seen.add(path)
            deduped.append(path)
    return deduped


def select_by_changes(
    scenarios: list[Scenario],
    changed_files: Iterable[str],
    smoke_scenarios: list[str],
) -> list[Scenario]:
    selected_names: set[str] = set()
    require_smoke = False

    for path in changed_files:
        test_match = re.match(r"^test/([^/]+)\.test\.yml$", path)
        if test_match:
            selected_names.add(test_match.group(1))
            continue

        if path.startswith("examples/") and path.endswith(".ino"):
            selected_names.add(Path(path).stem)
            continue

        if path.startswith("src/") or path in WIDE_CHANGE_PATHS:
            require_smoke = True

    if require_smoke:
        selected_names.update(smoke_scenarios)

    return [scenario for scenario in scenarios if scenario.name in selected_names]


def select_scenarios(
    mode: str,
    scenarios: list[Scenario],
    changed_files: list[str],
    smoke_scenarios: list[str],
) -> list[Scenario]:
    if mode == "skip":
        return []
    if mode == "full":
        return list(scenarios)
    if mode == "smoke":
        smoke_set = set(smoke_scenarios)
        return [scenario for scenario in scenarios if scenario.name in smoke_set]
    if mode == "changed":
        return select_by_changes(scenarios, changed_files, smoke_scenarios)
    raise SystemExit(f"Unsupported mode: {mode}")


def write_github_output(path: str, result: dict[str, str]) -> None:
    with Path(path).open("a") as output_file:
        for key, value in result.items():
            output_file.write(f"{key}={value}\n")


def main() -> int:
    parser = argparse.ArgumentParser(description="Select Wokwi CI matrix entries.")
    parser.add_argument(
        "--mode", choices=["changed", "smoke", "full", "skip"], required=True
    )
    parser.add_argument("--smoke-scenarios", default=",".join(DEFAULT_SMOKE_SCENARIOS))
    parser.add_argument(
        "--changed-file", action="append", help="Changed file path (can repeat)."
    )
    parser.add_argument(
        "--changed-files-file", help="Path to a file containing changed file paths."
    )
    parser.add_argument(
        "--github-output", help="Optional path to GITHUB_OUTPUT for workflow outputs."
    )
    args = parser.parse_args()

    scenarios = discover_scenarios()
    changed_files = load_changed_files(args)

    selected: list[Scenario]
    mode = args.mode
    if not scenarios:
        mode = "skip"
        selected = []
    else:
        smoke_scenarios = parse_smoke_scenarios(args.smoke_scenarios)
        selected = select_scenarios(mode, scenarios, changed_files, smoke_scenarios)

    matrix = [scenario.example_path for scenario in selected]
    names = [scenario.name for scenario in selected]
    payload = {
        "mode": mode,
        "count": len(selected),
        "names": names,
        "matrix": matrix,
        "changed_files": changed_files,
    }
    print(json.dumps(payload))

    if args.github_output:
        write_github_output(
            args.github_output,
            {
                "mode": mode,
                "count": str(len(selected)),
                "names": ",".join(names),
                "matrix": json.dumps(matrix),
                "should_run": "true" if selected else "false",
            },
        )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
