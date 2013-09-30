#!/bin/sh

TEXT=$1
SCRIPT_DIR=$(dirname $0)

${SCRIPT_DIR}/TraX --hide /boot/develop/headers/be/ &
	 
# Add another panel
hey "TraX" \
	set "Value" \
	of  "View" "Add" \
	of  "View" "OptionsPanel" \
	of  "Window" 0 \
	to  1

# Set the first 'predicate' to "Extension"
hey "TraX" \
	set "Selection" \
	of  "View" "action" \
	of  "View" 3 \
	of  "Window" 0 \
	to  "Extension"

# Set the extension to 'h'
hey "TraX" \
	set "Value" \
	of  "View" "edit" \
	of  "View" 3 \
	of  "Window" 0 \
	to  "h"

# Set the 2nd 'predicate' to "Contains"
hey "TraX" \
	set "Selection" \
	of  "View" "action" \
	of  "View" 5 \
	of  "Window" 0 \
	to  "Contains"

# Colour the "Contains" view
hey "TraX" \
	set "Colour" \
	of  "View" 5 \
	of  "Window" 0 \
	to  "rgb_color(220, 200, 255)"

# Set the focus to the EditBox "Contains" view
hey "TraX" \
	set "Focus" \
	of  "View" "edit" \
	of  "View" 5 \
	of  "Window" 0 \
	to  "true"

test "${TEXT}" && {
	# Set the text to search for
	hey "TraX" \
		set "Value" \
		of  "View" "edit" \
		of  "View" 5 \
		of  "Window" 0 \
		to  "${TEXT}"
	
	# Start Search
	hey "TraX" \
		set "Value" \
		of "View" "go" \
		of "View" "OptionsPanel" \
		of "Window" 0 \
		to 1
}

# Show the window
hey "TraX" \
	set "Hidden" \
	of  "Window" 0 \
	to  "false"

