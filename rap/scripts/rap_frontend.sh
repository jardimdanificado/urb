#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<EOF
Usage: ${0##*/} <command> [options] [args...]

Commands (and short aliases):
  compile (-c)   Compile a source file. Supports option -o <output>.
  run     (-r)   Run a program.
  help    (-h)   Show this help message.
  version (-v)   Show version information.

Examples:
  ${0##*/} compile -o app.bin main.rap
  ${0##*/} run app.bin
EOF
}

version() {
  echo "${0##*/} version 1.0.0"
}

command=""
output=""
forward_args=()

# Detect command or alias
while [[ $# -gt 0 ]]; do
  case "$1" in
    compile|-c) command="compile"; shift; break ;;
    run|-r)     command="run"; shift; break ;;
    help|-h)    command="help"; shift; break ;;
    version|-v) command="version"; shift; break ;;
    *)          forward_args+=("$1"); shift ;;
  esac
done

# Default to help if no command provided
if [[ -z "${command:-}" ]]; then
  command="help"
fi

case "$command" in
  compile)
    # Parse options for 'compile'
    while getopts ":o:" opt; do
      case "$opt" in
        o) output="$OPTARG" ;;
        :) echo "Error: option -$OPTARG requires an argument" >&2; exit 2 ;;
        \?) forward_args+=("-$OPTARG") ;;
      esac
    done
    shift $((OPTIND - 1))

    # The next argument should be the input file
    input_file="${1:-}"
    [[ -z "$input_file" ]] && { echo "Error: missing input file for compile"; exit 1; }
    shift

    forward_args+=("$@")

    echo "Command: compile"
    echo "Input file: $input_file"
    echo "Output: ${output:-<none>}"
    echo "Forwarded args: ${forward_args[*]-}"

    ##############################################################
    # >>> PLACE YOUR COMPILE LOGIC HERE <<<
    # Example:
    # rapc "$input_file" -o "${output:-a.out}" "${forward_args[@]}"
    ##############################################################
    ;;

  run)
    # The next argument should be the program to run
    program="${1:-}"
    [[ -z "$program" ]] && { echo "Error: missing program name for run"; exit 1; }
    shift

    forward_args+=("$@")

    echo "Command: run"
    echo "Program: $program"
    echo "Forwarded args: ${forward_args[*]-}"

    ##############################################################
    # >>> PLACE YOUR RUN LOGIC HERE <<<
    # Example:
    # ./rapvm "$program" "${forward_args[@]}"
    ##############################################################
    ;;

  help)
    usage
    ;;

  version)
    version
    ;;

  *)
    echo "Unknown command: $command" >&2
    usage
    exit 1
    ;;
esac
