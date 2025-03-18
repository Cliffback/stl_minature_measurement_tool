#!/bin/bash

# Parse command line options
auto_select=false
up_axis=""
set_axis=false

while getopts "yza" opt; do
  case $opt in
    y)
      set_axis=true
      up_axis="y"
      ;;
    z)
      set_axis=true
      up_axis="z"
      ;;
    a)
      auto_select=true
      ;;
    *)
      echo "Usage: $0 [-a] [-y|-z]"
      exit 1
      ;;
  esac
done

# List all STL files in the current directory
stl_files=(*.stl)
selected_files=()
base_files=()

if [ ${#stl_files[@]} -eq 0 ]; then
    echo "No STL files found in the current directory."
    exit 1
fi

echo "Previewing STL files. Select the ones you want to process (Press Enter for Yes, 'n' for No)."
echo "Files containing 'base' in the name will be treated as base automatically."

# Loop through each STL file
for file in "${stl_files[@]}"; do
    # Convert filename to lowercase and check if it contains 'base'
    lowercase_file=$(echo "$file" | tr '[:upper:]' '[:lower:]')
    if [[ "$lowercase_file" == *base* ]]; then
        base_files+=("$file")
        continue
    fi

    # If auto-select is enabled, automatically include the file
    if [ "$auto_select" = true ]; then
        selected_files+=("$file")
    else
        # Preview file and ask user if they want to include it
        qlmanage -p "$file" &> /dev/null  # Preview STL file

        read -p "Include $file? (Y/n): " choice
        choice=${choice:-y}  # Default to "y" if Enter is pressed
        if [[ "$choice" == "y" || "$choice" == "Y" ]]; then
            selected_files+=("$file")
        fi
    fi
done

# Check if any files were selected
if [ ${#selected_files[@]} -eq 0 ] && [ ${#base_files[@]} -eq 0 ]; then
    echo "No files selected. Exiting."
    exit 1
fi

# If axis is not set by -x or -z, ask for the axis determination
if [ "$set_axis" = false ]; then
  # Ask for axis determination (Enter defaults to "Yes")
  read -p "Was the model pointing up? (Y/n): " up_choice
  up_choice=${up_choice:-y}  # Default to "y" if Enter is pressed
  if [[ "$up_choice" == "y" || "$up_choice" == "Y" ]]; then
      axis="y"
  else
      read -p "Was the model pointing left? (Y/n): " left_choice
      left_choice=${left_choice:-y}  # Default to "y" if Enter is pressed
      if [[ "$left_choice" == "y" || "$left_choice" == "Y" ]]; then
          axis="z"
      else
          echo "Axis not determined. Exiting."
          exit 1
      fi
  fi
else
  axis="$up_axis"
  echo "Selected up axis: $axis"
fi

printf "\n"

# Run the base files with the two axes not used
if [ ${#base_files[@]} -gt 0 ]; then
    echo "Base dimensions:"
    if [[ "$axis" == "z" ]]; then
        ./stl_dimensions "${base_files[@]}" "x" "y" "s"
    elif [[ "$axis" == "y" ]]; then
        ./stl_dimensions "${base_files[@]}" "x" "z" "s"
    elif [[ "$axis" == "x" ]]; then
        ./stl_dimensions "${base_files[@]}" "y" "z" "s"
    fi
    printf "\n"
fi

if [ ${#selected_files[@]} -eq 0 ]; then
    exit 0
fi

# Run the selected files with the chosen axis in silent mode
echo "Model dimensions:"
./stl_dimensions "${selected_files[@]}" "$axis" s

