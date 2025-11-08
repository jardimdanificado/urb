#!/usr/bin/env bash
set -euo pipefail


# Default values
DONT_ASSEMBLE=""
JUST_ASSEMBLE=""
output=""
command=""
args=()

# Parse the first argument as command
if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <command> [options] [args]"
  exit 1
fi

command="$1"
shift

case "$command" in
  compile)
    while [[ $# -gt 0 ]]; do
      case "$1" in
        -o)
          shift
          output="${1:-}"
          if [[ -z "$output" ]]; then
            echo "Error: -o requires a filename" >&2
            exit 1
          fi
          ;;
        -S)
          DONT_ASSEMBLE=1
          ;;
        -c)
          JUST_ASSEMBLE=1
          ;;
        -*)
          echo "Unknown option for compile: $1" >&2
          ;;
        *)
          args+=("$USER_PWD/$1")
          ;;
      esac
      shift
    done

    extension=".urb"

    if [[ -n "$JUST_ASSEMBLE" ]]; then
        # Só assemble
        ./rap/scripts/assemble.sh "${args[@]}" > "$USER_PWD/$output"
    elif [[ -n "$DONT_ASSEMBLE" ]]; then
        # Só preprocessa
        ./rap/scripts/preprocess.sh "${args[@]}" > "$USER_PWD/$output$extension"
    else
        # Compila completo
        ./rap/scripts/preprocess.sh "${args[@]}" > "$USER_PWD/$output$extension"
        ./rap/scripts/assemble.sh "$USER_PWD/$output$extension" > "$USER_PWD/$output"
    fi

    ;;

  run)
    while [[ $# -gt 0 ]]; do
      case "$1" in
        -*)
          echo "Unknown option for run: $1" >&2
          ;;
        *)
          args+=("$USER_PWD/$1")
          ;;
      esac
      shift
    done
    ./rapper "${args[@]}"
    ;;

  help)
    cat <<EOF
Usage: $0 <command> [options] [args]

Commands:
  compile        Compile a source file
      -o FILE    Specify output file
      -S FILE    Prevent assembling
      -c FILE    Just assemble
  run            Run a program
  help           Show this help
  version        Show version info
EOF
    ;;

  version)
    echo "rap pre-alpha, based on urb 0.9.4a"
    ;;

  *)
    echo "Unknown command: $command" >&2
    echo "Try '$0 help' for usage."
    exit 1
    ;;
esac
