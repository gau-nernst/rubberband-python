root_dir = 'rubberband_python'
py_sources = [
    '__init__.py',
]

py = import('python').find_installation(pure: false)
py.install_sources(py_sources, subdir: root_dir)

rubberband_dep = dependency('rubberband', default_options: ['jni=disabled'], static: true)
apple_dep = dependency('appleframeworks', modules : 'Accelerate', required: false)  # vDSP
py.extension_module(
    '_c',
    '_c.c',
    dependencies: [rubberband_dep, apple_dep],
    install: true,
    c_args: ['-DMODULE_NAME=_c'],
    subdir: root_dir,
)
