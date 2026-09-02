import sys

def parse_list_val(infos):
    list_val = []
    for info in infos:
        if info[:4] == "# - ":
            list_val.append(info.split('-')[1])
    return '{' + ','.join(list_val) + '}'

def parse_param_file(fname):
    raw_data = ''
    with open(fname,'r') as pfile:
        raw_data = pfile.read()
    params = {}
    sections = raw_data.split("# ----------------------------------------------------------------------")
    section_name = ""
    for section in sections[1:]:
        param_list = []
        parameters = section.split("\n\n")
        if len(parameters) > 1:
            for parameter in parameters:
                # print("P:",parameter)
                raw_info = parameter.split("\n")
                p_desc=""
                p_val=""
                p_name=""
                p_def=""
                for i,info in enumerate(filter(lambda i: len(i)>0, raw_info)):
                    if info.find("Range of possible")>0:
                        p_val = info.split(':')[1]
                    elif info.find("List of possible values")>0:
                        p_val = parse_list_val(raw_info[i:])
                    elif info[0] == '#':
                        if not info[:4] == "# - ":
                            p_desc += info[1:]
                    elif len(info)>0: # the parameter definition
                        p_name = info[:info.find('=')]
                        if info.find('='):
                            p_def = info.split('=')[1]
                    else:
                        print("Nothing:",info,"::")
                param_list.append({'desc': p_desc, 'domain': p_val, 'name': p_name, 'default value': p_def})
            params[section_name] = param_list # Empty list of parameters
        else:
            section_name = section.split('\n')[1][2:]
    return params


header = """
# Parameters description

RealPaver behavior can be controlled using some parameters. These parameters are defined by the class [`Params`](../api/classrealpaver_1_1_params.md) implemented by the file [`src/realpaver/Params.hpp`](../api/_params_8hpp.md) and [`src/realpaver/Params.cpp`](../api/_params_8cpp.md). Parameters can be handled more globally with environments using the class [`Env`](../api/classrealpaver_1_1_env.md) like for the CSP solver.

Parameters can also be defined in a text file and a default one is provided in `src/realpaver/ParamFile`.

Parameters are (key,value) pairs and three types of values are supported: integer, double and string. String values may be included in a restricted list of words like `YES` or `NO` as well as names of available strategies.

Several categories of parameters can be defined:

- general ones, that mainly apply to global algorithms like the CSP solver, or that are common to most of mechanisms
- specific ones, that are dedicated to some mechanisms like the branching or splitting strategies, the linear solver configuration, etc.

"""

footer = """
## Using the C++ API

Parameters are handled by the [`Params`](../api/classrealpaver_1_1_params.md) class. Below is an example of using this class:

```C++
#include <iostream>
#include "realpaver_api.hpp"

int main()
{
    // Create a Param instance with default values for pre-defined parameters
    Params prm;

    // Print the list of defined parameters with their values
    std::cout<<prm<<std::endl;

    // Change the integer parameter relating to the maximal depth of the search tree
    prm.setIntParam("DEPTH_LIMIT", 1000);
    // Change the double parameter relating to the time limit
    prm.setDblParam("TIME_LIMIT", 3.6e3);
    // Change the string parameter relating to the logging level
    prm.setStrParam("LOG_LEVEL", "MAIN");

    // Printing current value for double parameter relating to relative tolerance
    std::cout<<"Current relative tolerance: "<<prm.getDblParam("VAR_REL_TOL")<<std::endl;

    // Load parameters from a file
    prm.loadParam("my_params.txt");

    return 0;
}
```
"""


if __name__=='__main__':
    if len(sys.argv)!=2:
        print("The parameter files must be provided")
        exit(1)
    fname = sys.argv[1]

    params = parse_param_file(fname)

    param_doc_file = 'parameters.md'
    with open(param_doc_file,'w') as pdoc:
        pdoc.write(header)
        for section in params.keys():
            pdoc.write('## '+section+'\n')
            for p in params[section]:
                if len(p['name'])>0:
                    pdoc.write('- '+p['name']+':')
                    pdoc.write(p['desc']+'\n')
                    if p['domain'][0]=='{':
                        pdoc.write('\t- Domain:\n')
                        for val in p['domain'][1:len(p['domain'])-1].split(','):
                            pdoc.write('\t\t- '+val+'\n')
                    else:
                        pdoc.write('\t- Domain: '+p['domain']+'\n')
                    pdoc.write('\t- Default value: '+p['default value']+'\n')
                else:
                    pdoc.write('\n')
        pdoc.write(footer)
