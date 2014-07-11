# Copyright (c) 2013, Google Inc. All rights reserved.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

{
  'target_defaults': {
    'includes': [
      '../../build/third_party_common.gypi',
    ],
  },
  'variables': {
    'src': 'src',
  },
  'targets': [
    {
      'target_name': 'modp',
      'type': 'static_library',
      'sources': [
        '<(src)/modp_b64w.c',
        '<(src)modp_b64w_data.h',
        'include/modp/modp_b64w.h',
      ],
      'include_dirs': [
        'include/modp',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'include',
        ],
      },
    },
  ]
}
