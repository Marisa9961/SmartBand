# MIT License
#
# Copyright (c) 2023 Yule Duan
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# This code is part of the stm32cubemx-vscode-cmake project and is used to extract
# variables and lists from a Makefile.

# Function to extract a variable from the Makefile
function(extract_makefile_variable var_name out_var)
    string(REGEX MATCH "${var_name} = ([^\n]*)" match ${STM32CUBEMX_MAKEFILE})
    if (match)
        string(STRIP "${CMAKE_MATCH_1}" match)
        set(${out_var} ${match} PARENT_SCOPE)
    else()
        set(${out_var} "" PARENT_SCOPE)
    endif()
endfunction()

# Function to extract a list of variables from the Makefile. 
# Only keep items that start with `leading`, and remove the leading text if `remove_leading` is set.
function(extract_makefile_list var_name out_list leading remove_leading)
    extract_makefile_variable(${var_name} temp)
    # Split the list into items by spaces 
    string(REPLACE " " ";" temp "${temp}")
    # Filter out items that don't start with `leading`
    set(output "")
    foreach(item ${temp})
        if (item)
            if (item MATCHES "^${leading}")
                if (remove_leading)
                    string(REGEX REPLACE "^${leading}" "" item ${item})
                endif()
                list(APPEND output ${item})
            endif()
        endif()
    endforeach()
    set(${out_list} ${output} PARENT_SCOPE)
endfunction()
