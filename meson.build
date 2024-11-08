project('rubberband-python', 'c')

rubberband_dep = dependency('rubberband', default_options: ['jni=disabled'], static: true)
apple_dep = dependency('appleframeworks', modules : 'Accelerate', required: false)  # vDSP

py = import('python').find_installation(pure: false)
py.extension_module(
    'rubberband_python',
    'rubberband_python.c',
    dependencies: [rubberband_dep, apple_dep],
    install: true,
)
