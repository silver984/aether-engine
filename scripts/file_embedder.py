#!/usr/bin/env python3

import re
import sys
from pathlib import Path


def sanitize_name(name: str) -> str:
    name = re.sub(r"\W", "_", name)

    if name and name[0].isdigit():
        name = "_" + name

    return name


def format_bytes(data: bytes) -> str:
    lines = []
    current = "    "

    for byte in data:
        value = f"0x{byte:02X}, "

        if len(current) + len(value) > 140:
            lines.append(current.rstrip())
            current = "    "

        current += value

    if current.strip():
        lines.append(current.rstrip())

    return "\n".join(lines)


def main() -> None:
    if len(sys.argv) != 6:
        print(
            f"Usage: {sys.argv[0]} "
            "<input-file> "
            "<include-directory> "
            "<output-header-path> "
            "<output-impl-path> "
            "<namespace-suffix>"
        )
        sys.exit(1)

    input_path = Path(sys.argv[1])
    include_directory = Path(sys.argv[2])
    header_path = Path(sys.argv[3])
    implementation_path = Path(sys.argv[4])
    name = sanitize_name(sys.argv[5])

    data = input_path.read_bytes()

    try:
        include_path = header_path.relative_to(include_directory)
    except ValueError:
        print(
            f"Error: header path '{header_path}' is not inside "
            f"include directory '{include_directory}'"
        )
        sys.exit(1)

    include_path = include_path.as_posix()

    namespace = f"aether::_embedded_{name}"
    byte_array = format_bytes(data)

    header = f"""#pragma once

#include <cstddef>
#include <cstdint>

namespace {namespace} {{

extern size_t const SIZE_;
extern uint8_t const DATA_[];

}} // namespace {namespace}
"""

    implementation = f"""#include <{include_path}>

namespace {namespace} {{

size_t const SIZE_ = {len(data)};
uint8_t const DATA_[] = {{
{byte_array}
}};

}} // namespace {namespace}
"""

    header_path.parent.mkdir(parents=True, exist_ok=True)
    implementation_path.parent.mkdir(parents=True, exist_ok=True)

    header_path.write_text(header, encoding="utf-8")
    implementation_path.write_text(implementation, encoding="utf-8")


if __name__ == "__main__":
    main()