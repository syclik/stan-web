{
  'targets': [
    {
      'target_name': 'linear_model',
      #'type': 'static_library',
      'sources': [ 'src/linear_model.cpp', ],
      'include_dirs': [
        '.',
        'stan/src',
        'stan/lib/boost_1.53.0',
        'stan/lib/eigen_3.1.3',
      ],
      'link_settings': {
        'libraries': ['-L../stan/bin', '-lstan'],
      },
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ],
      'cflags': [ '-fpermissive'],
      'conditions': [
        ['OS=="mac"', {
            'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            }
          }]
      ]
    }
  ]
}
