
project('Realpaver','cpp',version: '1.0.0',
    default_options: ['cpp_std=c++14'])

## Main compiler and linker configs

cpp_comp = meson.get_compiler('cpp')

## dependencies

#includedir = get_option('includedir')
#libdir = get_option('libdir')

gurobi_dir = get_option('GUROBI_DIR')


#include_dirs = [includedir]

lp_lib_name = get_option('LP_LIB')
lp_dep = dependency('', required : false)
if lp_lib_name.to_upper() == 'CLP'
  lp_dep = dependency('clp', required : true)
elif lp_lib_name.to_upper() == 'GUROBI'
  #lp_dep = dependency('gurobi100', required: true, method: 'auto')
  lp_dep = cpp_comp.find_library('gurobi100', required: true, dirs: [gurobi_dir+'/lib'])
  #include_dirs += [gurobi_dir+'/include']
elif lp_lib_name.to_upper() == 'SOPLEX'
  lp_dep = cpp_comp.find_library('soplex', required : true)
elif lp_lib_name.to_upper() == 'HIGHS'
  lp_dep = dependency('highs', required : true)
endif

interval_lib_name = get_option('INTERVAL_LIB')
interval_dep = dependency('', required : false)
if interval_lib_name.to_upper() == 'GAOL'
  # Search first for apmathlib if present
  mathlib_dep = cpp_comp.find_library('ultim', required : false)
  # Else consider to use crlibm
  if not mathlib_dep.found()
    mathlib_dep = cpp_comp.find_library('crlibm', required : true)
  endif
  gaol_dep = cpp_comp.find_library('gaol', required : true)
endif
m_dep = cpp_comp.find_library('m', required : true)

if not lp_dep.found()
  error('Missing linear programming library (Clp or Gurobi)!')
endif

deps_lib = [lp_dep,mathlib_dep,gaol_dep,m_dep]

flex = find_program('flex', required: false)
bison = find_program('bison', required: false)



## Config
os_unix   = false
os_linux  = false
os_win32  = false
os_darwin = false

if host_machine.system() == 'darwin'
  os_darwin = true
elif host_machine.system() == 'windows'
  os_win32 = true
elif host_machine.system() == 'linux'
  os_linux = true
else
  warning('Unkown host system!')
endif
os_unix = not os_win32

arm64 = false
x86 = false
x86_64 = false
if host_machine.cpu() == 'aarch64'
  arm64 = true
elif host_machine.cpu() == 'x86'
  x86 = true
elif host_machine.cpu() == 'x86_64'
  x86_64 = true
endif

if not(arm64 or x86 or x86_64)
  error('Unsupported cpu arch: '+host_machine.cpu())
endif

conf_data = configuration_data()
conf_data.set('version', meson.project_version())
conf_data.set('project_name', meson.project_name())
conf_data.set('project_full_name', meson.project_name())
conf_data.set('bug_report', 'laurent.granvilliers@univ-nantes.fr')
conf_data.set('project_url','https://gitlab.univ-nantes.fr/granvilliers-l/realpaver')

if os_darwin
  conf_data.set('PLATFORM_OSX',1)
endif
conf_data.set('ARCH_X86',x86)
conf_data.set('ARCH_X86_64',x86_64)
conf_data.set('ARCH_ARM64',arm64)
# Configuring LP lib
if lp_lib_name == 'Clp'
  conf_data.set('LP_LIB_CLP',1)
elif lp_lib_name == 'Clp'
  conf_data.set('LP_LIB_GUROBI',1)
endif
# Configuring interval lib
if interval_lib_name == 'Gaol'
  conf_data.set('INTERVAL_LIB_GAOL',1)
elif interval_lib_name == 'Crlibm'
  conf_data.set('INTERVAL_LIB_CRLIBM',1)
endif

assert_opt = get_option('ASSERT')
conf_data.set('ASSERT_ON',assert_opt.to_int())
debug_opt = get_option('DEBUG')
conf_data.set('DEBUG_ON',debug_opt.to_int())
log_opt = get_option('LOG')
conf_data.set('LOG_ON',log_opt.to_int())

check_headers = [
  'dlfcn.h',
  'inttypes.h',
  'stdint.h',
  'stdio.h',
  'stdlib.h',
  'strings.h',
  'string.h',
  'sys/stat.h',
  'sys/types.h',
  'unistd.h'
]

foreach h : check_headers
  if cpp_comp.has_header(h)
    conf_data.set('HAVE_' + h.underscorify().to_upper(), 1)
  endif
endforeach

# Checking flex and bison to generate parser files
flex = find_program('flex', required: false)
bison = find_program('bison', required: false)

if not flex.found()
  warning('MESON_SKIP_TEST flex not found.')
endif

if not bison.found()
  warning('MESON_SKIP_TEST bison not found.')
endif

subdir('src')

subdir('check')

subdir('doc')

subdir('examples')

subdir('work')

subdir('ncsp')


## Generation of pkg-config file to ease dependency detection when using realpaver as a library
## In your meson project, just add: rp_lib = dependency('realpaver')
pkgconfig = find_program('pkg-config', required: false)
if pkgconfig.found()
  pkg = import('pkgconfig')
  pkg.generate(realpaver_lib)
endif