# dyn_calc

This program loads dynamic libraries as plugins from the 'plugins' directory, i.e. functionality expands for a program without compilation as libraries are added.  
Before compiling the program, start compiling plugins:   

$ make plugins 

This will compile some examples  
To compile the calculator itself, run:   

$ make  

To run the program:  

$ LD_LIBRARY_PATH=./plugins ./calc  
  
In order for the plugin to connect to the program, you must use the global structure called fun_info in the plugin code, an example from add.c:  
func_info_t fun_info = {  
    .func_name = "add",  
    .return_type=INT,  
    .number_of_args = 2  
};  
