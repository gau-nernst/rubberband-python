project('rubberband-python', 'c')

rubberband_dep = dependency('rubberband', static: true)
apple_dep = dependency('appleframeworks', modules : 'Accelerate')  # for vDSP in rubberband

py = import('python').find_installation(pure: false)
py.extension_module(
    'rubberband_python',
    'rubberband_python.c',
    dependencies: [rubberband_dep, apple_dep],
    install: true,
)
